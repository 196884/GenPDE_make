#ifndef PDE_PARAMETERS_1D_H
#define PDE_PARAMETERS_1D_H

class PDEParameters1D
{
public:
    PDEParameters1D();

    PDEParameters1D(
        double max_timestep_length,
        double nb_rannacher_steps,
        double max_rannacher_step_length,
        size_t space_grid_size,
        double space_grid_nb_std_devs
    );

    virtual ~PDEParameters1D();

    double getMaxTimestepLength()      const;
    double getNbRannacherSteps()       const;
    double getMaxRannacherStepLength() const;
    size_t getSpaceGridSize()          const;
    double getSpaceGridNbStdDevs()     const;

protected:
    double m_maxTimestepLength;
    double m_nbRannacherSteps;
    double m_maxRannacherStepLength;
    size_t m_spaceGridSize;
    double m_spaceGridNbStdDevs;
};

#endif // PDE_PARAMETERS_1D_H
