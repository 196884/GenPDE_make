#include <cstddef>
#include "PDEParameters1D.h"

PDEParameters1D::PDEParameters1D()
:m_maxTimestepLength()
,m_nbRannacherSteps()
,m_maxRannacherStepLength()
,m_spaceGridSize()
,m_spaceGridNbStdDevs()
{}

PDEParameters1D::PDEParameters1D(
    double max_timestep_length,
    double nb_rannacher_steps,
    double max_rannacher_step_length,
    size_t space_grid_size,
    double space_grid_nb_std_devs
)
:m_maxTimestepLength(      max_timestep_length       )
,m_nbRannacherSteps(       nb_rannacher_steps        )
,m_maxRannacherStepLength( max_rannacher_step_length )
,m_spaceGridSize(          space_grid_size           )
,m_spaceGridNbStdDevs(     space_grid_nb_std_devs    )
{}

PDEParameters1D::~PDEParameters1D()
{}

double PDEParameters1D::getMaxTimestepLength()      const
{
    return m_maxTimestepLength;
}

double PDEParameters1D::getNbRannacherSteps()       const
{
    return m_nbRannacherSteps;
}

double PDEParameters1D::getMaxRannacherStepLength() const
{
    return m_maxRannacherStepLength;
}

size_t PDEParameters1D::getSpaceGridSize()          const
{
    return m_spaceGridSize;
}

double PDEParameters1D::getSpaceGridNbStdDevs()     const
{
    return m_spaceGridNbStdDevs;
}



