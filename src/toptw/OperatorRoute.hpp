//
//  AlgoRouting.hpp
//
//  Created by Qian Hu on 20/9/2016.
//  Copyright © 2016 Naiquh Corporation. All rights reserved.
//

#ifndef AlgoRouting_hpp
#define AlgoRouting_hpp

#include <iostream>
#include "ProblemToptw.hpp"
#include "SolutionToptw.h"
#include "AlgoParameter.hpp"

class OperatorRoute {
public:
    OperatorRoute(const ProblemToptw &_inst);
    ~OperatorRoute() = default;;

    void relocate_0(SolutionToptw &sol);
    void exchange_0(SolutionToptw &sol);

    void relocate_1(SolutionToptw &sol);
    void exchange_1(SolutionToptw &sol);
//    void Two_opt_1(SolutionToptw &sol);

    void two_opt_2(SolutionToptw &sol);
    void relocate_2(SolutionToptw &sol);
    void exchange_2(SolutionToptw &sol);

//    void k_exchange(SolutionToptw &sol);
//    void k_opt(SolutionToptw &sol);


public:
    const ProblemToptw &inst;
    const GraphVrptw &graph;

private:
    std::vector<int> sortedVertices;
};

#endif /* AlgoRouting_hpp */
