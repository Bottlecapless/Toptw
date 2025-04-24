//
//  Created by Qian Hu on 20/9/2016.
//  Copyright © 2016 Naiquh Corporation. All rights reserved.
//

#ifndef Vns_hpp
#define Vns_hpp


#include "ProblemToptw.hpp"
#include "GraphVrptw.h"
#include "AlgoParameter.hpp"
#include "SolutionToptw.h"
#include "OperatorRoute.hpp"
#include "RandomMove.hpp"
#include "ResultToptw.h"
#include "huq.h"
#include <algorithm>
#include <cmath>

class VNS {
public:
    VNS(const ProblemToptw &_inst, const AlgoParameter & _param);

    ~VNS() = default;;


    void run();
    void localSearch(SolutionToptw &sol);
    void vnd(SolutionToptw &sol);
    void perturb(SolutionToptw &sol);
    void construct(SolutionToptw &sol);

private:
    void printLog(const std::string &prefix, const SolutionToptw &sol);
    void writeSolution();
    void writeCsv();

public:
    const ProblemToptw &inst;
    const AlgoParameter & param;
    ResultToptw result;
    OperatorRoute ort;
    RandomMove rm;

private:
    int nonImproving;
    int maxNonImprovingIterations;
    huq::Timer timer;
};


#endif /* Vns_hpp */
