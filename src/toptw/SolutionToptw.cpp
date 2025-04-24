
#include "SolutionToptw.h"
#include <iostream>
#include <cassert>


SolutionToptw::SolutionToptw(const ProblemToptw &inst) : m_profit(0), m_location_vid(inst.N + 1, -1) {
    resize(inst.m);
    for (auto &route: *this) {
        route.reset(inst.graph);
    }
}

bool SolutionToptw::diff_route(const SolutionToptw &other) {
    for (int i = 1; i <= m_location_vid.size(); ++i) {
        if (m_location_vid[i] != other.m_location_vid[i]) return true;
    }
    return false;
}

int SolutionToptw::difference(const SolutionToptw &other) {
    int diff = 0;
    for (int i = 1; i < m_location_vid.size(); ++i) {
        if (m_location_vid[i] != other.m_location_vid[i]) diff++;
    }
    return diff;
}


void SolutionToptw::clear_route(const ProblemToptw &inst) {
    m_profit = 0;
    for (auto &route: *this)
        route.reset(inst.graph);
    std::fill(m_location_vid.begin(), m_location_vid.end(), -1);
}

void SolutionToptw::setRoute(int k, RouteTw &route) {
    m_profit -= at(k).profit;
    for (int i: at(k)) {
        if (i > 0) m_location_vid[i] = -1;
    }
    at(k) = route;
    m_profit += route.profit;
    update_location_visit(k);
}

void SolutionToptw::calculateProfit(const ProblemToptw &p, bool flag) {
    m_profit = 0;
    for (auto &route: *this) {
        route.profit = route.accumulate(p.p);
        m_profit += route.profit;
    }
    std::sort(begin(), end()); // sort routes by profit descending
}

void SolutionToptw::update_location_visit() {
    std::fill(m_location_vid.begin(), m_location_vid.end(), -1);
    for (int k = 0; k < size(); ++k) {
        for (int i: at(k)) {
            if (i > 0) m_location_vid[i] = k;
        }
    }
}

void SolutionToptw::update_location_visit(int k) {
    for (int i: at(k)) {
        if (i > 0) m_location_vid[i] = k;
    }
}

bool SolutionToptw::check(const ProblemToptw &inst) const {
    if (!AlgoParameter::debug) return true;

    // check route
    for (const auto &route: *this) {
        route.check(inst.graph);
    }

    // check m_locaiton_vid
    for (int k = 0; k < inst.m; ++k) {
        for (int i: at(k)) {
            if (i > 0 && m_location_vid[i] != k) {
                std::cerr << "sol::check() > m_location_vid error \r\n";
                return false;
            }
        }
    }

    // check profit
    int profitSol = 0;
    for (auto &route: *this) {
        for (int i: route) {
            if (i > 0) profitSol += inst.p[i];
        }
        int profitRoute = route.accumulate(inst.p);
        assert(profitRoute == route.profit);
    }
    assert(m_profit == profitSol);

    return true;
}

int SolutionToptw::numServed() const {
    int n = 0;
    for (auto &route: *this) {
        n += route.numServed();
    }
    return n;
}

bool operator<(const RouteTw &r1, const RouteTw &r2) {
    return r1.profit > r2.profit;
}

bool operator<(const SolutionToptw &s1, const SolutionToptw &s2) {
    return s1.m_profit < s2.m_profit;
}

std::ostream &operator<<(std::ostream &out, const RouteTw &route) {
    out << "route=" << huq::join(route) << ", et=" << huq::join(route.et) << ", lt=" << huq::join(route.lt)
        << ", profit=" << route.profit;
    return out;
}

std::ostream &operator<<(std::ostream &out, const SolutionToptw &sol) {
    out << "# profit " << sol.m_profit << std::endl;
    int k = 0;
    for (const auto &route: sol) {
        out << "# route " << (k++) << " >  " << route.profit << "  " << route.numServed() << "  " << huq::join(route)
            << std::endl;
    }
    out << "# m_location_vid = " << huq::join(sol.m_location_vid);
    return out;
}
