#ifndef TIME_GRID_FUNCTIONS_H
#define TIME_GRID_FUNCTIONS_H

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

#endif // TIME_GRID_FUNCTIONS_H
