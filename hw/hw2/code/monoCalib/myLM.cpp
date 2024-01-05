/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "myLM.h"
#include "opencv2/core/core_c.h"
#include <iostream>
// #include "calib3d_c_api.h"

/************************************************************************************\
       Some backward compatibility stuff, to be moved to legacy or compat module
\************************************************************************************/
using namespace std;
using cv::Ptr;

////////////////// Levenberg-Marquardt engine (the old variant) ////////////////////////

linLevMarq::linLevMarq()
{
    lambdaLg10 = 0;
    state = DONE;
    criteria = cvTermCriteria(0, 0, 0);
    iters = 0;
    completeSymmFlag = false;
    errNorm = prevErrNorm = DBL_MAX;
    solveMethod = cv::DECOMP_SVD;
}

linLevMarq::linLevMarq(int nparams, int nerrs, CvTermCriteria criteria0, bool _completeSymmFlag)
{
    init(nparams, nerrs, criteria0, _completeSymmFlag);
}

void linLevMarq::clear()
{
    //    mask.release();
    prevParam.release();
    param.release();
    J.release();
    err.release();
    JtJ.release();
    JtJN.release();
    JtErr.release();
    JtJV.release();
    JtJW.release();
}

linLevMarq::~linLevMarq()
{
    clear();
}

void linLevMarq::init(int nparams, int nerrs, CvTermCriteria criteria0, bool _completeSymmFlag)
{
    if (!param || param->rows != nparams || nerrs != (err ? err->rows : 0))
        clear();
    // mask.reset(cvCreateMat( nparams, 1, CV_8U ));
    // cvSet(mask, cvScalarAll(1));
    prevParam.reset(cvCreateMat(nparams, 1, CV_64F));
    param.reset(cvCreateMat(nparams, 1, CV_64F));
    // JtJ是J的转置与J的乘积
    JtJ.reset(cvCreateMat(nparams, nparams, CV_64F));
    // JtErr应该是J的转置乘上f,也就是误差项
    JtErr.reset(cvCreateMat(nparams, 1, CV_64F));
    if (nerrs > 0)
    {
        // J为Jacobian矩阵，显然应该是误差约束性个数*优化变量个数
        J.reset(cvCreateMat(nerrs, nparams, CV_64F));
        // err就是误差项，也就是f
        err.reset(cvCreateMat(nerrs, 1, CV_64F));
    }
    errNorm = prevErrNorm = DBL_MAX;
    lambdaLg10 = -3;
    criteria = criteria0;
    if (criteria.type & CV_TERMCRIT_ITER)
        criteria.max_iter = MIN(MAX(criteria.max_iter, 1), 1000);
    else
        criteria.max_iter = 30;
    if (criteria.type & CV_TERMCRIT_EPS)
        criteria.epsilon = MAX(criteria.epsilon, 0);
    else
        criteria.epsilon = DBL_EPSILON;
    state = STARTED;
    iters = 0;
    completeSymmFlag = _completeSymmFlag;
    solveMethod = cv::DECOMP_SVD;
}

bool linLevMarq::update(const CvMat *&_param, CvMat *&matJ, CvMat *&_err)
{
    matJ = _err = 0;

    assert(!err.empty());
    if (state == DONE)
    {
        _param = param;
        return false;
    }

    if (state == STARTED)
    {
        _param = param;
        cvZero(J);
        cvZero(err);
        matJ = J;
        _err = err;
        state = CALC_J;
        return true;
    }

    // 如果当前状态为CALC_J，意思是计算Jacobian矩阵。在这里检测到这个状态的时候，实际上
    // J已经算完了。
    if (state == CALC_J)
    {
        cvMulTransposed(J, JtJ, 1); // 计算J的转置byJ
        // 计算J的转置by Err (就是f),结果放到JtErr里面，就是我们公式里面Jtf
        cvGEMM(J, err, 1, 0, 0, JtErr, CV_GEMM_A_T);
        cvCopy(param, prevParam);
        step();
        if (iters == 0)
            prevErrNorm = cvNorm(err, 0, CV_L2);
        _param = param;
        cvZero(err);
        _err = err;
        state = CHECK_ERR;
        return true;
    }

    assert(state == CHECK_ERR);
    errNorm = cvNorm(err, 0, CV_L2);

    lambdaLg10 = MAX(lambdaLg10 - 1, -16);
    // 算法收敛判断条件，如果迭代次数足够多或者前后两次参数变化已经很小，则停止迭代
    if (++iters >= criteria.max_iter ||
        cvNorm(param, prevParam, CV_RELATIVE_L2) < criteria.epsilon)
    {
        _param = param;
        state = DONE;
        return true;
    }

    prevErrNorm = errNorm;
    _param = param;
    cvZero(J);
    matJ = J;
    _err = err;
    state = CALC_J;
    return true;
}

bool linLevMarq::updateAlt(const CvMat *&_param, CvMat *&_JtJ, CvMat *&_JtErr, double *&_errNorm)
{
    CV_Assert(!err);
    if (state == DONE)
    {
        _param = param;
        return false;
    }

    if (state == STARTED)
    {
        _param = param;
        cvZero(JtJ);
        cvZero(JtErr);
        errNorm = 0;
        _JtJ = JtJ;
        _JtErr = JtErr;
        _errNorm = &errNorm;
        state = CALC_J;
        return true;
    }

    if (state == CALC_J)
    {
        cvCopy(param, prevParam);
        step();
        _param = param;
        prevErrNorm = errNorm;
        errNorm = 0;
        _errNorm = &errNorm;
        state = CHECK_ERR;
        return true;
    }

    assert(state == CHECK_ERR);
    if (errNorm > prevErrNorm)
    {
        if (++lambdaLg10 <= 16)
        {
            step();
            _param = param;
            errNorm = 0;
            _errNorm = &errNorm;
            state = CHECK_ERR;
            return true;
        }
    }

    lambdaLg10 = MAX(lambdaLg10 - 1, -16);
    if (++iters >= criteria.max_iter ||
        cvNorm(param, prevParam, CV_RELATIVE_L2) < criteria.epsilon)
    {
        _param = param;
        _JtJ = JtJ;
        _JtErr = JtErr;
        state = DONE;
        return false;
    }

    prevErrNorm = errNorm;
    cvZero(JtJ);
    cvZero(JtErr);
    _param = param;
    _JtJ = JtJ;
    _JtErr = JtErr;
    state = CALC_J;
    return true;
}

namespace
{
    static void subMatrix(const cv::Mat &src, cv::Mat &dst, const std::vector<uchar> &cols,
                          const std::vector<uchar> &rows)
    {
        int nonzeros_cols = cv::countNonZero(cols);
        cv::Mat tmp(src.rows, nonzeros_cols, CV_64FC1);

        for (int i = 0, j = 0; i < (int)cols.size(); i++)
        {
            if (cols[i])
            {
                src.col(i).copyTo(tmp.col(j++));
            }
        }

        int nonzeros_rows = cv::countNonZero(rows);
        dst.create(nonzeros_rows, nonzeros_cols, CV_64FC1);
        for (int i = 0, j = 0; i < (int)rows.size(); i++)
        {
            if (rows[i])
            {
                tmp.row(i).copyTo(dst.row(j++));
            }
        }
    }
}

void linLevMarq::step()
{
    using namespace cv;
    const double LOG10 = log(10.);
    double lambda = exp(lambdaLg10 * LOG10);
    int nparams = param->rows; // 待更新的参数个数

    Mat _JtJ = cvarrToMat(JtJ);

    if (!JtJN || JtJN->rows != nparams)
    {
        // prevent re-allocation in every step
        JtJN.reset(cvCreateMat(nparams, nparams, CV_64F));
        JtJV.reset(cvCreateMat(nparams, 1, CV_64F));
        JtJW.reset(cvCreateMat(nparams, 1, CV_64F));
    }

    Mat _JtJN = cvarrToMat(JtJN);
    Mat _JtErr = cvarrToMat(JtJV);
    Mat_<double> nonzero_param = cvarrToMat(JtJW);

    cv::Mat src = cvarrToMat(JtErr);
    src.copyTo(_JtErr);

    /*subMatrix(cvarrToMat(JtErr), _JtErr, std::vector<uchar>(1, 1), _mask);
    subMatrix(_JtJ, _JtJN, _mask, _mask);*/
    _JtJ.copyTo(_JtJN);

    if (!err)
        completeSymm(_JtJN, completeSymmFlag);

    // （JtJ + muI）x=-Jtf
    //_JtJN 就是JtJ + muI,lambda就是mu
    //_JtErr就是Jtf
    _JtJN.diag() *= 1. + lambda;
    solve(_JtJN, _JtErr, nonzero_param, solveMethod);

    // 注意这里更新的是减法，也就是LM公式里面有个负号，体现在这里面了
    for (int i = 0; i < nparams; i++)
        param->data.db[i] = prevParam->data.db[i] - nonzero_param(i);
}
