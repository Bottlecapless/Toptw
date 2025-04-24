//
//  result.hpp
//  BinPacking
//
//  Created by Qian Hu on 24/6/2017.
//  Copyright © 2017 Naiquh Corporation. All rights reserved.
//

#ifndef result_toptw_hpp
#define result_toptw_hpp

#include <string>
#include <vector>
#include <ctime>
#include <ostream>
#include "SolutionToptw.h"
#include "ProblemToptw.hpp"

class ResultToptw {
public:
    ResultToptw(const ProblemToptw &inst) : instname(inst.instname), lb(-1), best(inst),
                                            time(0), timeLocalSearch(0), timePerturb(0) {}

    ~ResultToptw() = default;

public:
    std::string instname;

    int lb;
    SolutionToptw best;

    double time;
    double timeLocalSearch;
    double timePerturb;


};

#endif /* result_toptw_hpp */
