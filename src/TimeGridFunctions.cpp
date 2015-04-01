//
//  TimeGridFunctions.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/25/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <math.h>

#include "Framework.h"

#include "TimeGridFunctions.h"

void GenPDE::buildTimeGrid(
    const std::vector<Date>& dates,
    double                   max_timestep_length,
    size_t                   nb_rannacher_steps,
    double                   max_rannacher_step_length,
    std::vector<double>&     time_grid,
    std::vector<size_t>&     date_indices
)
{
    max_timestep_length       /= GenPDE::NbDaysPerYear;
    max_rannacher_step_length /= GenPDE::NbDaysPerYear;
    time_grid.resize(1);
    date_indices.resize(1);
    time_grid[0]    = 0;
    date_indices[0] = 0;
    double lastNode = 0;
    Date lastDate   = dates[0];
    for(size_t currDateIdx=1; currDateIdx<dates.size(); ++currDateIdx)
    {
        Date currDate   = dates[currDateIdx];
        long dateDiffDays = GenPDE::dateDifferenceInDays(currDate, lastDate);
        if( dateDiffDays <= 0 )
            Exception::raise("GenPDE::buildTimeGrid", "dates must be strictly increasing");
        double node     = ((double) GenPDE::dateDifferenceInDays(currDate, dates[0])) / GenPDE::NbDaysPerYear;
        double nodeDiff = node - lastNode;
        if( nb_rannacher_steps * max_rannacher_step_length >= nodeDiff )
        {
            // Adding only Rannacher steps
            double stepLength = nodeDiff / nb_rannacher_steps;
            for(size_t i=1; i<nb_rannacher_steps; ++i)
                time_grid.push_back(time_grid.back() + stepLength);
        } else {
            size_t nbStepsAux = ceil(( node - lastNode - nb_rannacher_steps * max_rannacher_step_length ) / max_timestep_length);
            double stepLength = (node - lastNode - nb_rannacher_steps * max_rannacher_step_length) / nbStepsAux;
            if( nb_rannacher_steps == 0 )
                --nbStepsAux;
            for(size_t i=0; i<nbStepsAux; ++i)
                time_grid.push_back(time_grid.back() + stepLength);
            for(size_t i=1; i<nb_rannacher_steps; ++i)
                time_grid.push_back(time_grid.back() + max_rannacher_step_length);
        }
        
        date_indices.push_back(time_grid.size());
        time_grid.push_back(node);
        lastDate      = currDate;
        lastNode      = node;
    }
}
