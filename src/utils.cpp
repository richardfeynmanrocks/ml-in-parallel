//
//  utils.cpp
//  Jacobian
//
//  Created by David Freifeld
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <eigen3/Eigen/Dense>

inline float sgn(float val) {
    return (0.0f < val) - (val < 0.0f);
}

double fexp(double val)
{  
    long tmp = static_cast<long>(1512775 * val + 1072632447) << 32;
    return *reinterpret_cast<double*>(&tmp);
}

float ftanh(float val)
{
    return sgn(val) * (1 - 2/(fexp(2*abs(val))+1));
}

float fcosh(float val)
{
    return (fexp(val) + fexp(-val)) * 0.5;
}

// A bunch of hardcoded activation functions. Avoids much of the slowness of custom functions.
// Although the std::function makes it not the fastest way, the functionality is worth it.
// Yes, these functions may be a frustrating to read but they're just equations and I want to conserve space.

float sigmoid(float x) {return 1.0/(1+fexp(-x));}
float sigmoid_deriv(float x) {return 1.0/(1+fexp(-x)) * (1 - 1.0/(1+fexp(-x)));}

float linear(float x) {return x;}
float linear_deriv(float x) {return 1;}

float lecun_tanh(float x) {return 1.7159 * ftanh((2.0/3) * x);}
float lecun_tanh_deriv(float x) {return 1.14393 * pow(1.0/fcosh(2.0/3 * x),2);}

float inverse_logit(float x) {return (fexp(x)/(fexp(x)+1));}
float inverse_logit_deriv(float x) {return (fexp(x)/pow(fexp(x)+1, 2));}

float softplus(float x) {return log(1+fexp(x));}
float softplus_deriv(float x) {return fexp(x)/(fexp(x)+1);}

float cloglog(float x) {return 1-fexp(-fexp(x));}
float cloglog_deriv(float x) {return fexp(x-fexp(x));}

float step(float x)
{
    if (x > 0) return 1;
    else return 0;
}
float step_deriv(float x) {return 0;}

float bipolar(float x)
{
    if (x > 0) return 1;
    else if (x == 0) return 0;
    else return -1;
}
float bipolar_deriv(float x) {return 0;}

float bipolar_sigmoid(float x) {return (1-fexp(-x))/(1+fexp(-x));}
float bipolar_sigmoid_deriv(float x) {return (2*fexp(x))/(pow(fexp(x)+1,2));}

float hard_tanh(float x) {return fmax(-1, fmin(1,x));}
float hard_tanh_deriv(float x)
{
    if (-1 < x && x < 1) return 1;
    else return 0;
}

float leaky_relu(float x)
{
    if (x > 0) return x;
    else return 0.01 * x;
}

float leaky_relu_deriv(float x)
{
    if (x > 0) return 1;
    else return 0.01;
}

std::function<float(float)> rectifier(float (*activation)(float))
{
    auto rectified = [activation](float x) -> float
    { 
        if (x > 0) return (*activation)(x);
        else return 0; 
    };
    return rectified;
}
