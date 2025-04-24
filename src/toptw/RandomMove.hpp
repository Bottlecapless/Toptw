//
//  random_move.hpp
//  Created by Qian Hu on 2018/9/16.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef random_move_toptw_hpp
#define random_move_toptw_hpp

#include "ProblemToptw.hpp"
#include "SolutionToptw.h"
#include "OperatorRoute.hpp"

class RandomMove{
public:
    RandomMove(const ProblemToptw & _inst);
    ~RandomMove()= default;
    
    void eliminate(SolutionToptw & sol, int T);
    void randomRemove(SolutionToptw & sol, double removalRate);
  //  void randomRelocate(SolutionToptw & sol, int T); // random relocate allowing deteriorating moves
//    void randomExchange(SolutionToptw & sol, int T);

public:
    const ProblemToptw & inst;
    OperatorRoute opr;
};

#endif /* random_move_toptw_hpp */
