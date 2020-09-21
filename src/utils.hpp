//
//  utils.hpp
//  Jacobian
//
//  Created by David Freifeld
//

#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <immintrin.h>

// A zoo of activation functions.
float sigmoid(float x);
float sigmoid_deriv(float x);
float linear(float x);
float linear_deriv(float x);
float step(float x);
float step_deriv(float x);
float bipolar(float x);
float bipolar_deriv(float x);
float mytanh(float x);
float tanh_deriv(float x);
float lecun_tanh(float x);
float lecun_tanh_deriv(float x);
float cloglog(float x);
float cloglog_deriv(float x);
float softplus(float x);
float softplus_deriv(float x);
float inverse_logit(float x);
float inverse_logit_deriv(float x);
float hard_tanh(float x);
float hard_tanh_deriv(float x);
float bipolar_sigmoid(float x);
float bipolar_sigmoid_deriv(float x);
float leaky_relu(float x);
float leaky_relu_deriv(float x);
std::function<float(float)> rectifier(float (*activation)(float));

Eigen::MatrixXf avx_product(Eigen::MatrixXf a, Eigen::MatrixXf b);
Eigen::MatrixXf avx_exp(Eigen::MatrixXf m);
Eigen::MatrixXf avx_cdiv(Eigen::MatrixXf m, float denom);
Eigen::MatrixXf avx_log(Eigen::MatrixXf m);
Eigen::MatrixXf avx_cpow(Eigen::MatrixXf m, float exponent);
Eigen::MatrixXf avx_tanh(Eigen::MatrixXf m);
Eigen::MatrixXf avx_cosh(Eigen::MatrixXf m);

inline float scan(char **p);
Eigen::MatrixXf strassen_mul(Eigen::MatrixXf a, Eigen::MatrixXf b);

#endif /* MODULE_H */
