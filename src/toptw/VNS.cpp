//
//  Created by Qian Hu on 20/9/2016.
//  Copyright © 2016 Naiquh Corporation. All rights reserved.
//

#include "VNS.hpp"

#include <iomanip>
#include <random>
#include <fstream>

VNS::VNS(const ProblemToptw &_inst, const AlgoParameter &_param)
        : inst(_inst), param(_param), result(inst),
          ort(inst), rm(inst) {
//    std::srand(AlgoParameter::seed);  // ensure the random sequence for each run of vns
//    AlgoParameter::rnd = std::default_random_engine(AlgoParameter::seed);

    maxNonImprovingIterations = 2000;
}

void VNS::run() {
    timer.on();

    SolutionToptw sol(inst);
    construct(sol);
    printLog("construct  ", sol);
    localSearch(sol);

    nonImproving = 0;
    while (nonImproving <= maxNonImprovingIterations && timer.elapsed() < param.time_limit) {
        sol = result.best;
        perturb(sol);
        localSearch(sol);
        nonImproving++;
    }

    result.time = timer.elapsed();
    writeSolution();
    writeCsv();
    huq::println_tab(inst.instname, inst.m, AlgoParameter::seed, result.best.m_profit, result.time);
    std::cout << result.best << std::endl;
}

void VNS::localSearch(SolutionToptw &sol) {
    double start = timer.elapsed();
    int R = 7;
    std::vector<int> choice(R);
    for (int i = 0; i < R; ++i)
        choice[i] = i;
    std::shuffle(choice.begin() + 2, choice.end(), AlgoParameter::rnd);

    if (AlgoParameter::debug) sol.check(inst);
    int _profit = sol.m_profit;
    for (int i = 0; i < R; ++i) {
        std::string prefix;
        switch (choice[i]) {
            case 0:
                ort.relocate_0(sol);
                prefix = "relocate0";
                break;
            case 1:
                ort.exchange_0(sol);
                prefix = "exchange0";
                break;
            case 2:
                ort.relocate_1(sol);
                prefix = "relocate1";
                break;
            case 3:
                ort.exchange_1(sol);
                prefix = "exchange1";
                break;
            case 4:
                ort.relocate_2(sol);
                prefix = "relocate2";
                break;
            case 5:
                ort.exchange_2(sol);
                prefix = "exchange2";
                break;
            case 6:
                ort.two_opt_2(sol);
                prefix = "2opt_star";
                break;
        }
        if (_profit < sol.m_profit) {
            printLog(prefix, sol);
            _profit = sol.m_profit;
//            std::shuffle(choice.begin(), choice.end(), AlgoParameter::rnd);
            i = -1;
        }
    }
    if (AlgoParameter::debug) sol.check(inst);
    result.timeLocalSearch += (timer.elapsed() - start);
}

void VNS::perturb(SolutionToptw &sol) {
    double start = timer.elapsed();
//    int R= 4;
    int R = 2;
    std::vector<int> choice(R);
    for (int i = 0; i < R; ++i)
        choice[i] = i;
    std::shuffle(choice.begin(), choice.end(), AlgoParameter::rnd);

    SolutionToptw last(sol);
    for (int i = 0; i < R; ++i) {
        int T = 10 + nonImproving + std::rand() % (result.lb / 100);
        double removalRate = 0.2 + 0.4 * nonImproving / maxNonImprovingIterations;
        switch (choice[i]) {
            case 0:
                rm.eliminate(sol, T);
                // ort.relocate_0(sol);
                break;
            case 1:
                rm.randomRemove(sol, removalRate);
                break;
//            case 2:
//                rm.Relocate_2(sol, T);
//                break;
//            case 4:
//                rm.Exchange_2(sol, T);
//                break;
        }
    }
    if (AlgoParameter::debug) sol.check(inst);
    result.timePerturb += (timer.elapsed() - start);
}

void VNS::construct(SolutionToptw &sol) {
    sol.resize(inst.m);
    sol.clear_route(inst);
    ort.relocate_0(sol);
}

void VNS::printLog(const std::string &prefix, const SolutionToptw &sol) {
    if (result.lb < sol.m_profit) {
        if (AlgoParameter::debug) {
            huq::println_tab(prefix, sol.m_profit, inst.benchmark, nonImproving, timer.elapsed());
        }
        result.best = sol;
        result.lb = result.best.m_profit;
        nonImproving = 0;
        if (AlgoParameter::debug) sol.check(inst);
    }
}

void VNS::writeSolution() {
    std::string fname =
            param.path_result_sol + "/sol_" + inst.instname + "_" + std::to_string(inst.m) + ".txt";
    std::ofstream out(fname, std::ofstream::out);
    out << "instance\t" << inst.instname << std::endl;
    out << "m = " << inst.m << std::endl << std::endl;
    out << "algo\tvns" << std::endl;
    out << "seed\t" << AlgoParameter::seed << std::endl;
    out << "timelim\t" << param.time_limit << std::endl << std::endl;
    out << "profit\t" << result.best.m_profit << std::endl;
    out << "time\t" << timer.elapsed() << std::endl;
    out << "timeLocalSearch \t" << result.timeLocalSearch << std::endl;
    out << "timePerturb \t" << result.timePerturb << std::endl;
    out << std::endl;
    out << result.best << std::endl;
    out.close();
}

void VNS::writeCsv() {
    std::ofstream out(param.path_result_csv, std::ofstream::app);
    out << param.authorId << "," << param.authorName << "," << param.problem_name << "," << inst.instname << ","
        << inst.N << "," << inst.m << "," << inst.benchmark << "," << param.algo_name << "," << result.best.m_profit
        << "," << timer.elapsed()
        << std::endl;
    out.close();
}
