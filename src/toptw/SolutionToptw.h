

#ifndef BASE_SOLUTION_TOPTW_H_
#define BASE_SOLUTION_TOPTW_H_

#include <utility>
#include <map>
#include <vector>
#include "ProblemToptw.hpp"
#include "GraphVrptw.h"

class SolutionToptw : public std::vector<RouteTw> {
public:
    SolutionToptw(const ProblemToptw &inst);

    ~SolutionToptw() = default;;

    bool diff_route(const SolutionToptw &other);
    int difference(const SolutionToptw &other);
    void clear_route(const ProblemToptw &inst);
    void setRoute(int k, RouteTw &route);
    void calculateProfit(const ProblemToptw &inst, bool flag = false);
    bool check(const ProblemToptw &inst) const;
    void update_location_visit();
    void update_location_visit(int k);
    int numServed() const;

public:
    int m_profit;
    std::vector<int> m_location_vid;  // each location is visited by which vehicles

};

bool operator<(const RouteTw &r1, const RouteTw &r2);
bool operator<(const SolutionToptw &s1, const SolutionToptw &s2);
std::ostream &operator<<(std::ostream &out, const RouteTw &route);
std::ostream &operator<<(std::ostream &out, const SolutionToptw &s);

#endif /* BASE_SOLUTION_TOPTW_H_ */
