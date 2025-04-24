//
//  AlgoParameter.hpp
//
//  Created by Qian Hu on 24/6/2017.
//  Copyright © 2017 Naiquh Corporation. All rights reserved.
//

#ifndef algo_parameter_hpp
#define algo_parameter_hpp

#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <utility>

class AlgoParameter {
public:
    AlgoParameter() : time_limit(-1) {
        debug = false;
        resetRandomness();
    }

    AlgoParameter(std::string _authorId, std::string _authorName, std::string _prob, std::string _algo,
                  int _tlim, std::string _prefix, std::string _ext, std::string _data,
                  int _seed = 3, bool _debug = false)
            : authorId(std::move(_authorId)), authorName(std::move(_authorName)),
              problem_name(std::move(_prob)),
              algo_name(std::move(_algo)),
              time_limit(_tlim),
              test_prefix(std::move(_prefix)),
              test_extension(std::move(_ext)),
              path_data(std::move(_data)) {
        resetRandomness(_seed);
        AlgoParameter::debug = _debug;
    }

    ~AlgoParameter() = default;

    inline std::string csv_name() const {
        return "result_" + algo_name + ".csv";
    }

    static void resetRandomness(int _seed = 3);
public:
    std::string authorId;
    std::string authorName;
    std::string problem_name;
    std::string algo_name;
    int time_limit;
    std::string test_prefix;
    std::string test_extension;
    std::string path_data;
    std::string path_result_sol;
    std::string path_result_csv;

    static bool debug;
    static double tolerance;
    static int maxValueInt;
    static long maxValueLong;
    static double optimalityGap;
    static int seed;
    static std::default_random_engine rnd;

};

std::ostream &operator<<(std::ostream &out, const AlgoParameter &param);


#endif /* algo_parameter_hpp */
