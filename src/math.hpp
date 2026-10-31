#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <utility>
#include <iostream>
#include <vector>

template <typename integral_type>
std::pair<double, double> mean_and_dev(std::vector<integral_type>& vec){
    double mean = 0, deviation = 0;
    for (integral_type d: vec){
        mean += d;
    }
    mean /= vec.size();
    for (integral_type d: vec){
        deviation += std::pow((d - mean), 2);
    }
    deviation /= vec.size() - 1;
    deviation = std::sqrt(deviation);

    return {mean, deviation};
} 

#endif

