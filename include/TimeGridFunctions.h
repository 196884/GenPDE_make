//
//  TimeGridFunctions.h
//  GenPDE
//
//  Created by Regis Dupont on 8/25/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TIMEGRIDFUNCTIONS_H_
#define TIMEGRIDFUNCTIONS_H_

#include <vector>

#include "GpDate.h"

namespace GenPDE
{
    void buildTimeGrid(
        const std::vector<Date>& dates,
        double                   max_timestep_length,
        size_t                   nb_rannacher_steps,
        double                   max_rannacher_step_length,
        std::vector<double>&     time_grid,
        std::vector<size_t>&     date_indices
    );
}

#endif // TIMEGRIDFUNCTIONS_H_
