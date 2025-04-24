//
//  problem.hpp
//  gtop_f6
//
//  Created by Qian Hu on 29/6/2017.
//  Copyright © 2017 Naiquh Corporation. All rights reserved.
//

#ifndef problem_toptw_hpp
#define problem_toptw_hpp

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "GraphVrptw.h"

class ProblemToptw {
public:
    ProblemToptw() : N(-1), m(-1), benchmark(0) {}

    ~ProblemToptw() = default;

    void loadInstance(const std::string &filename, int numVehicles = 4) {
        m = numVehicles;
        std::ifstream fin(filename);
        if (!fin.is_open()) {
            std::cerr << "File open failed! " << filename << std::endl;
            std::exit(-1);
        }
        std::string tmp;
        fin >> tmp >> tmp >> N >> tmp;
        fin >> tmp >> tmp;
        // std::cout << "n=" << N << std::endl;
        int n = N + 1;

        double x = 0.0, y = 0.0;
        double service = 0.0;
        double profit = 0.0;
        int tw_start = 0, tw_end = 0;
        int a = 0;
        for (int i = 0; i < n; ++i) {
            fin >> tmp >> x >> y >> service >> profit >> tmp >> a;
            for (int j = 0; j < a; ++j) fin >> tmp;
            fin >> tw_start >> tw_end;
            graph.emplace_back(i, x, y, tw_start, tw_end, int(service));
            p.emplace_back(int(profit));
        }
        fin.close();
    }

    void prepare() {
        graph.prepare();
    }

    inline void setNumOfVehicles(int numVehicles, int bks = -1) {
        m = numVehicles;
        benchmark = bks;
    }

    inline int dist(int i, int j) const {
        return graph.d[i][j];
    }

public:
    std::string instname;
    int N; // number of vertices in graph: 0 depot, 1 ... N-1 customers
    int m; // number of buses
    int benchmark; // best known solution

    GraphVrptw graph; // transportation network graph
    std::vector<int> p; // profit vector

};

#endif /* problem_toptw_hpp */
