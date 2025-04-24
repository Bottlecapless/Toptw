// Created by Qian Hu on 2021/9/15 下午3:03.
// Copyright (c) 2021 NJU OR Lab.  All rights reserved.

#ifndef OPTIMIZER_CPP_HUQ_H
#define OPTIMIZER_CPP_HUQ_H

#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include "json.hpp"

namespace huq {

    ////////////////////////// Timer //////////////////////////////////////////

    class Timer {
    private:
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> start;
    public:
        Timer() {
            start = std::chrono::steady_clock::now(); // steady_clock is used to measure intervals
        }

        void on() {
            start = std::chrono::steady_clock::now();
        }

        double elapsed() const {
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            return elapsed_seconds.count();
        }

        std::string getCurrentDateTime() {
            auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::stringstream ss;
            ss << std::put_time(std::localtime(&t), "%F %T");
            return ss.str();
        }
    };

    ///////////////////////// Short println ///////////////////////////////////

    template<typename... Args>
    void println_tab(Args... args) {
        // ((std::cout << args << ", ") , ...) << std::endl;
        (..., (std::cout << args << "\t")) << std::endl;
    }

    template<typename... Args>
    void println(Args... args) {
        // ((std::cout << args << ", ") , ...) << std::endl;
        (..., (std::cout << args << ", ")) << std::endl;
    }

    /**
     * join a vector of element, return a string， i.e. [1, 2, 3]
     *
     * @tparam A  iterator of a vector
     * @param begin begin of vector
     * @param end end of vector
     * @param split split symbols
     * @return a string
     */
    template<class A>
    std::string join(const A &begin, const A &end, const std::string &split) {
        std::ostringstream oss;
        oss << "[";
        for (A it = begin; it != end; it++) {
            oss << *it;
            if (std::distance(it, end) > 1) oss << split;
        }
        oss << "]";
        return oss.str();
    }

    template<class A>
    std::string join(const std::vector<A> &a) {
        return join(a.begin(), a.end(), ",");
    }

    ////////////////////////// Comparing floating numbers /////////////////////

    /**
    * Test whether float value x is actually an integer
    * @tparam T - to_decide: float, double
    * @param x - float value
    * @return true if it is integral, and false otherwise
    */
    template<class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    integral(const T &x, double epsilon = 1.0e-10) {
        return std::fabs(x - int(x + 0.1)) <= epsilon;
    }

    template<typename T>
    bool fequal(const T &x, const T &y, double epsilon = 1.0e-10) {
        return std::fabs(x - y) <= epsilon;
    }

    template<class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp) {
        // the machine epsilon has to be scaled to the magnitude of the values used
        // and multiplied by the desired precision in ULPs (units in the last place)
        return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
               // unless the result is subnormal
               || std::fabs(x - y) < std::numeric_limits<T>::min();
    }

    template<typename T>
    bool greater(const T &a, const T &b, double epsilon = 1.0e-8) {
        if (std::is_floating_point_v<T>) return a > b + epsilon;
        else return a > b;
    }

    template<typename T>
    bool less(const T &a, const T &b, double epsilon = 1.0e-8) {
        if (std::is_floating_point_v<T>) return a + epsilon < b;
        else return a < b;
    }

    template<typename T>
    bool equal(const T &a, const T &b, double epsilon = 1.0e-8) {
        if (std::is_floating_point_v<T>) return std::abs(a - b) <= epsilon;
        else return a == b;
    }

}

#endif //OPTIMIZER_CPP_HUQ_H
