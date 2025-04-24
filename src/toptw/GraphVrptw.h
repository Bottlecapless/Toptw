//
// Created by Qian Hu on 2021/8/7.
//

#ifndef OPTIMIZER_CPP_GRAPHVRPTW_H
#define OPTIMIZER_CPP_GRAPHVRPTW_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>

#include "huq.h"
#include "AlgoParameter.hpp"

class Vertex {
public:
    Vertex() = default;

    Vertex(int _id, double _x, double _y, int _tw_start, int _tw_end, int _s)
            : id(_id), x(_x), y(_y), service(_s), tw_start(_tw_start), tw_end(_tw_end) {

    }

    ~Vertex() = default;

    int get_tw_start() const { return tw_start; }

    int get_tw_end() const { return tw_end; }

public:
    int id{};
    double x{};   // position - x coordinate
    double y{};   // position - y corrdinate
    int service{};    // service time at the vertex
private:
    int tw_start{};   // time window - start
    int tw_end{};     // time window - end
};

///////////////////////////  class GraphVrptw //////////////////////////

class GraphVrptw : public std::vector<Vertex> {
public:
    std::vector <std::vector<double>> d; // distance matrix
    std::vector <std::vector<double>> t; // travel time matrix
    std::vector<double> tw_start; // improved time window start
    std::vector<double> tw_end; // improved time window end

    std::vector <std::vector<bool>> infeasible; // indicate infeasible arcs

public:
    void prepare() {
        // compute distance matrix
        int n = size() - 1;
        d.resize(n + 1, std::vector<double>(n + 1, 0));
        t.resize(n + 1, std::vector<double>(n + 1, 0));
        for (int i = 0; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                d[i][j] = d[j][i] = computeDistance(i, j);
                t[i][j] = at(i).service + d[i][j];
                t[j][i] = at(j).service + d[i][j];
            }
        }

        tw_start.resize(n + 1);
        tw_end.resize(n + 1);
        for (int i = 0; i <= n; ++i) {
            Vertex vertex = at(i);
            tw_start[i] = vertex.get_tw_start();
            tw_end[i] = vertex.get_tw_end();
        }
        identifyInfeasibleArcs();
    }

private:
    double computeDistance(int i, int j) const {
        double xDiff = at(i).x - at(j).x;
        double yDiff = at(i).y - at(j).y;
        return std::sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    void identifyInfeasibleArcs() {
        int n = size() - 1;
        infeasible = std::vector < std::vector < bool >> (n + 1, std::vector<bool>(n + 1, false));

        // check arcs (0,i) and (i,0)
        for (int i = 1; i <= n; ++i) {
            if (huq::greater(d[0][i], tw_end[i]))
                infeasible[0][i] = true;
            if (huq::greater(tw_start[i] + d[i][0], tw_end[0]))
                infeasible[i][0] = true;
        }

        // check arcs (i, j) and (j, i); requiring triangle inequalities
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                auto d0ij = std::max(tw_start[i], d[0][i]) + t[i][j];
                auto d0ji = std::max(tw_start[j], d[0][j]) + t[j][i];
                infeasible[i][j] = infeasible[0][i] || infeasible[j][0]
                                   || huq::greater(d0ij, tw_end[j])
                                   || huq::greater(std::max(d0ij, tw_start[j]) + t[j][0], tw_end[0]);
                infeasible[j][i] = infeasible[0][j] || infeasible[i][0]
                                   || huq::greater(d0ji, tw_end[i])
                                   || huq::greater(std::max(d0ji, tw_start[i]) + t[i][0], tw_end[0]);
            }
        }
    }
};


////////////////////  class Route  /////////////////////////////

class RouteTw : public std::vector<int> {
public:
    std::vector<double> et;  // earliest arrival times
    std::vector<double> lt;  // latest arrival times
    int profit; // route profit

private:
    std::vector<bool> visited;
    double waiting_time;
public:
    RouteTw() : et(2, 0), lt(2, 0), profit(0), waiting_time(0) {
        resize(2, 0);
    }

    ~RouteTw() = default;

    inline bool served(int u) const {
        return visited[u];
    }

    inline bool emptyRoute() const {
        return size() < 3;
    }

    inline int numServed() const {
        return size() - 2;
    }

    void reset(const GraphVrptw &graph) {
        resize(2);
        profit = 0;
        front() = back() = 0;
        et.resize(2);
        et[0] = et[1] = graph.tw_start[0];
        lt.resize(2);
        lt[0] = lt[1] = graph.tw_end[0];
        visited.resize(graph.size());
        std::fill(visited.begin(), visited.end(), 0);
    }

    int accumulate(const std::vector<int> &p) const {
        int sum = 0;
        for (int x: *this) {
            sum += p[x];
        }
        return sum;
    }

    double accumulate(const std::vector<double> &p) const {
        double sum = 0;
        for (int x: *this) {
            sum += p[x];
        }
        return sum;
    }

    bool check(const GraphVrptw &graph) const {
        if (size() < 2 || front() != 0 || back() != 0) {
            std::cerr << "Route::check() > route must start and end at depot 0: " << huq::join(*this) << std::endl;
            return false;
        }
        int n = graph.size() - 1;


        // check et and lt
        std::vector<int> counter(n + 1, 0);
        for (int i = 1; i < size(); i++) {
            int x = at(i);
            counter[x]++;
            if (huq::greater(et[i], graph.tw_end[x])) {
                std::cerr << "Route::check() > time window violation @ location " << x << std::endl;
                return false;
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (counter[i] > 1) {
                std::cerr << "Route::check() > infeasible route " << huq::join(*this) << std::endl;
                return false;
            }
            if (counter[i] == 1 && !visited[i]) {
                std::cerr << "Route::check() > visited error " << i << "  " << visited[i] << "  " << huq::join(*this) << std::endl;
            }
            if (counter[i] <= 0 && visited[i]) {
                std::cerr << "Route::check() > visited error " << i << "  " << visited[i] << "  " << huq::join(*this) << std::endl;
            }
            assert((counter[i] == 0 && !visited[i]) || (counter[i] == 1 && visited[i]));
        }

        // check time window
        double t_arr = 0;
        for (int i = 1; i < size(); ++i) {
            int x = at(i - 1), y = at(i);
            t_arr += graph.t[x][y];
            t_arr = std::max(t_arr, graph.tw_start[y]);
            if (huq::greater(t_arr, graph.tw_end[y])) {
                std::cerr << "Route::check() > time window violation @ location " << y << std::endl;
                return false;
            }
        }
        return true;
    }

    void updateEarliestArrivalTime(const GraphVrptw &graph, int pos) {
        double t_arr = et[pos - 1];
        for (int i = pos; i < size(); ++i) {
            int x = at(i - 1), y = at(i);
            t_arr = std::max(t_arr, graph.tw_start[x]) + graph.t[x][y];
            if (i > pos && huq::equal(et[i], t_arr)) break;
            else et[i] = t_arr;
        }
    }

    void updateLatestArrivalTime(const GraphVrptw &graph, int pos) {
        double t_arr = lt[pos + 1];
        for (int i = pos; i >= 0; --i) {
            int x = at(i), y = at(i + 1);
            t_arr = std::min(graph.tw_end[x], t_arr - graph.t[x][y]);
            if (i < pos && huq::equal(lt[i], t_arr)) break;
            else lt[i] = t_arr;
        }
    }

    void update(const GraphVrptw &graph) {
        updateArrivals(graph);
        std::fill(visited.begin(), visited.end(), false);
        for (int i: *this) {
            if (i > 0) visited[i] = true;
        }
    }

    void updateArrivals(const GraphVrptw &graph) {
        int n = size();
        et.resize(n);
        lt.resize(n);

        et.front() = 0;
        for (int i = 1; i < n; ++i) {
            int x = at(i - 1), y = at(i);
            et[i] = std::max(et[i - 1], graph.tw_start[x]) + graph.t[x][y];
        }

        lt.back() = graph.tw_end[0];
        for (int i = n - 2; i >= 0; --i) {
            int x = at(i), y = at(i + 1);
            lt[i] = std::min(graph.tw_end[x], lt[i + 1] - graph.t[x][y]);
        }
    }

    bool canInsert(const GraphVrptw &inst, int pos, int u) {
        if (pos < 1 || pos > size() - 1 || visited[u]) return false;

        int x = at(pos - 1), y = at(pos); // x u y
        auto et_x = et[pos - 1];
        auto lt_y = lt[pos];

        auto et_u = std::max(et_x, inst.tw_start[x]) + inst.t[x][u];
        auto et_y = std::max(et_u, inst.tw_start[u]) + inst.t[u][y];
        auto lt_u = std::min(inst.tw_end[u], lt_y - inst.t[u][y]);
        auto lt_x = std::min(inst.tw_end[x], lt_u - inst.t[x][u]);
        if (huq::greater(et_u, lt_u) || huq::greater(et_y, lt_y) ||
            huq::greater(et_x, lt_x)) {
            return false;
        }
        return true;
    }

    bool canReplace(const GraphVrptw &inst, int pos, int u) {
        if (pos < 1 || pos >= size() - 1 || visited[u]) return false;

        int x = at(pos - 1), y = at(pos + 1);
        auto et_x = et[pos - 1];
        auto lt_y = lt[pos + 1]; // bug fixed: lt_y = et[pos]
        auto et_u = std::max(et_x, inst.tw_start[x]) + inst.t[x][u];
        auto et_y = std::max(et_u, inst.tw_start[u]) + inst.t[u][y];
        auto lt_u = std::min(inst.tw_end[u], lt_y - inst.t[u][y]);
        auto lt_x = std::min(inst.tw_end[x], lt_u - inst.t[x][u]);

        if (huq::greater(et_u, lt_u) || huq::greater(et_y, lt_y) ||
            huq::greater(et_x, lt_x)) {
            return false;
        }
        return false;
    }

    void add(const GraphVrptw &inst, int pos, int u) {
        visited[u] = true;
        insert(begin() + pos, u);
        et.insert(et.begin() + pos, 0);
        lt.insert(lt.begin() + pos, 0);
        updateEarliestArrivalTime(inst, pos);
        updateLatestArrivalTime(inst, pos);
        if (AlgoParameter::debug) check(inst);
    }

    void remove(const GraphVrptw &inst, int pos) {
        visited[at(pos)] = false;
        erase(begin() + pos);
        et.erase(et.begin() + pos);
        lt.erase(lt.begin() + pos);
        updateEarliestArrivalTime(inst, pos);
        updateLatestArrivalTime(inst, pos - 1);
        if (AlgoParameter::debug) {
            check(inst);
        }
    }

    void exchange(const GraphVrptw &inst, int pos1, int pos2) {
        int x1 = at(pos1), x2 = at(pos2);
        at(pos1) = x2;
        at(pos2) = x1;
        int a = std::min(pos1, pos2);
        int b = std::max(pos1, pos2);
        updateEarliestArrivalTime(inst, a);
        updateLatestArrivalTime(inst, b);
        if (AlgoParameter::debug) {
            check(inst);
        }
    }
};


#endif //OPTIMIZER_CPP_GRAPHVRPTW_H
