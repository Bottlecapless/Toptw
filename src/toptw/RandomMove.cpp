//
//  random_move.cpp
//  gtop_vns
//
//  Created by Qian Hu on 2018/9/16.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "RandomMove.hpp"
#include "AlgoParameter.hpp"
#include <random>

RandomMove::RandomMove(const ProblemToptw &_inst) : inst(_inst), opr(inst) {

}


void RandomMove::eliminate(SolutionToptw &sol, int T) {
    for (int k = 0; k < sol.size(); ++k) {
        auto &r = sol[k];
        for (int pos = 1; pos + 1 < r.size(); ++pos) {
            int u = r[pos];
            if (inst.p[u] <= T) {
                r.remove(inst.graph, pos);
                sol[k].profit -= inst.p[u];
                sol.m_profit -= inst.p[u];
                sol.m_location_vid[u] = -1;
                --pos;
            }
        }
    }
    if (AlgoParameter::debug) sol.check(inst);
}

void RandomMove::randomRemove(SolutionToptw &sol, double removalRate) {
    for (int k = 0; k < sol.size(); ++k) {
        auto &r = sol[k];
        for (int pos = 1; pos + 1 < r.size(); ++pos) {
            int u = r[pos];
            double p = 1.0 * (rand() % 100);
            if (p < removalRate) {
                r.remove(inst.graph, pos);
                sol[k].profit -= inst.p[u];
                sol.m_profit -= inst.p[u];
                sol.m_location_vid[u] = -1;
                --pos;
            }
        }
    }
    if (AlgoParameter::debug) sol.check(inst);
}


