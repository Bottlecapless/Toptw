//
//  AlgoParameter.cpp
//
//  Created by Qian Hu on 24/6/2017.
//  Copyright © 2017 Naiquh Corporation. All rights reserved.
//

#include "AlgoParameter.hpp"

#include <utility>

bool AlgoParameter::debug = false;
double AlgoParameter::tolerance = 1.0e-7;
double AlgoParameter::optimalityGap = 1.0e-7;
long AlgoParameter::maxValueLong = std::numeric_limits<long>::max();
int AlgoParameter::maxValueInt = std::numeric_limits<int>::max();
int AlgoParameter::seed = 3;
std::default_random_engine  AlgoParameter::rnd = std::default_random_engine(AlgoParameter::seed);

void AlgoParameter::resetRandomness(int _seed) {
    AlgoParameter::seed = _seed;
    std::srand(seed); // ensure the same random sequence for each run
    AlgoParameter::rnd = std::default_random_engine(seed);
}

std::ostream &operator<<(std::ostream &out, const AlgoParameter &param) {
    out << "problem: \t" << param.problem_name << std::endl;
    out << "algo_name: \t" << param.algo_name << std::endl;
    out << "time_limit: \t" << param.time_limit << std::endl;
    out << "test_prefix: \t" << param.test_prefix << std::endl;
    out << "test_extension: \t" << param.test_extension << std::endl;
    out << "path_data: \t" << param.path_data << std::endl;
    out << "path_result_sol: \t" << param.path_result_sol << std::endl;
    out << "path_result_csv: \t" << param.path_result_csv << std::endl;
    return out;
}




