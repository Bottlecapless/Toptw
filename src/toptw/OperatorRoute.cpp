//
//  AlgoRouting.cpp
//
//  Created by Qian Hu on 20/9/2016.
//  Copyright © 2016 Naiquh Corporation. All rights reserved.
//

#include "OperatorRoute.hpp"
#include <iostream>
#include "AlgoParameter.hpp"


OperatorRoute::OperatorRoute(const ProblemToptw &_inst) : inst(_inst), graph(inst.graph) {

    // sort vertices by unit profit per distance decreasing
    std::vector<double> length(inst.N + 1, inst.graph.tw_end[0]);
    for (int i = 1; i <= inst.N; ++i) {
        sortedVertices.push_back(i);

//        for (int h = 0; h <= inst.N; ++h) {
//            if (h == i || inst.graph.infeasible[h][i]) continue;
//            for (int j = 0; j <= inst.N; ++j) {
//                if (j == h || j == i || inst.graph.infeasible[i][j]) continue;
//                double len = inst.graph.t[h][i] + inst.graph.t[i][j] ;
//                length[i] = std::min(length[i], len);
//            }
//        }

        double tij = inst.graph.tw_end[0];
        double tji = inst.graph.tw_end[0];
        for (int j = 0; j <= inst.N; ++j) {
            if (j == i) continue;
            if (!inst.graph.infeasible[i][j]) tij = std::min(tij, inst.graph.t[i][j]);
            if (!inst.graph.infeasible[j][i]) tji = std::min(tji, inst.graph.t[j][i]);
        }
        length[i] = tij + tji;
    }
    std::sort(sortedVertices.begin(), sortedVertices.end(), [&](int i, int j) {
//        return inst.p[i] > inst.p[j];
        return 1.0 * inst.p[i] / length[i] > 1.0 * inst.p[j] / length[j];
    });
}

void OperatorRoute::relocate_0(SolutionToptw &sol) {
//    std::vector<int> unvisited = sol.sortUnvisited(inst);
    for (int u: sortedVertices) {
        for (int k = 0; k < sol.size(); ++k) {
            auto &route = sol[k];
            if (sol.m_location_vid[u] == -1) {
                for (int pos = 1; pos < route.size(); ++pos) {
                    int x = route[pos - 1], y = route[pos];
                    if (graph.infeasible[x][u] || graph.infeasible[u][y]) continue;
                    if (route.canInsert(graph, pos, u)) {
                        route.add(graph, pos, u);
                        route.profit += inst.p[u];
                        sol.m_profit += inst.p[u];
                        sol.m_location_vid[u] = k;
                        break;
                    }
                }
            }
        }
    }
//    std::cout << "relocate_0 \r\n";
    if (AlgoParameter::debug) sol.check(inst);
}

void OperatorRoute::exchange_0(SolutionToptw &sol) {
    // todo: replace a visited node by an unvisited node
    if (AlgoParameter::debug) sol.check(inst);
}

void OperatorRoute::relocate_1(SolutionToptw &sol) {
    // todo
    if (AlgoParameter::debug) sol.check(inst);
}

void OperatorRoute::exchange_1(SolutionToptw &sol) {
    for (auto &route: sol) {
        for (int i = 1; i + 1 < route.size(); ++i) {
            int x1 = route[i - 1], u = route[i], y1 = route[i + 1];
            for (int j = i + 2; j + 1 < route.size(); ++j) {
                int x2 = route[j - 1], w = route[j], y2 = route[j + 1];
                if (graph.infeasible[x1][w] || graph.infeasible[w][y1] || graph.infeasible[x2][u] ||
                    graph.infeasible[u][y2])
                    continue;
                auto increment = graph.d[x1][w] + graph.d[w][y1] + graph.d[x2][u] + graph.d[u][y2] - graph.d[x1][u] -
                                 graph.d[u][y1] - graph.d[x2][w] - graph.d[w][y2];
                if (increment + AlgoParameter::tolerance < 0 && route.canReplace(graph, i, w) && route.canReplace(graph, j, u)) {
                    route.exchange(graph, i, j);
                    u = route[i];
                }
            }
        }
    }
//    std::cout << "exchange_1\r\n";
    if (AlgoParameter::debug) sol.check(inst);
}


void OperatorRoute::two_opt_2(SolutionToptw &sol) {
    // todo: two_opt_star

    if (AlgoParameter::debug) sol.check(inst);
}


void OperatorRoute::relocate_2(SolutionToptw &sol) {
    // todo:
    if (AlgoParameter::debug) sol.check(inst);
}

void OperatorRoute::exchange_2(SolutionToptw &sol) {
    // todo:
    if (AlgoParameter::debug) sol.check(inst);
}


