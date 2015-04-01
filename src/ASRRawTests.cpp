//
//  ASRRawTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 8/31/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "GpDate.h"
#include "AuxiliaryVariable.h"
#include "PDEPricingModelBlackScholes.h"
#include "PDETradePricer.h"
#include "AVInterpolatorNaturalCubic.h"
#include "AVInterpolatorLinear.h"

using std::vector;
using std::cerr;
using std::endl;
using namespace GenPDE;

REGISTER_TEST(ASRRaw1)
{
    // Payout:
    // S_T * (1 / S_0 - 1 / A_T)
    // where A_T is the (daily) average
    //
    // Note that we actually use the running sum as the AV
    //
    // THIS IS A BAREBONE VERSION, TO CHECK HOW MUCH WE CAN SAVE ON FUNCTION
    // EVALUATION.
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    
	const double spot(100.0);
	const double rate(0.01);
	const double volatility(0.25);
	const size_t nbSpaceNodes(500);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    const double avStdDevMultiple(4);
    const size_t nbPlanes(200);
    const size_t nbDaysTotal(181);
    const size_t nbDaysERStart(91);
    
    const double logSpot(Double::log(spot));
    
    Date d0 = dateFromString("20010101");
    Date firstERDate(GenPDE::dateAddDays(d0, nbDaysERStart));
    vector<Date> dates(nbDaysTotal+1);
    for(long i=0; i<=nbDaysTotal; ++i)
        dates[i] = dateAddDays(d0, i);
    
    // Discretization of the AVs:
    vector< vector<double> > avValues(nbDaysTotal-1);
    for(size_t i=0; i<nbDaysTotal-1; ++i)
    {
        double avTenor = dateDifferenceInDays(dates[i+1], dates[0]) / NbDaysPerYear;
        double stdDev  = volatility * Double::sqrt(avTenor);
        double avLower = logSpot - avStdDevMultiple * stdDev;
        double avStep  = 2 * avStdDevMultiple * stdDev / (nbPlanes - 1);
        vector<double>& values(avValues[i]);
        values.resize(nbPlanes);
        for(size_t j=0; j<nbPlanes; ++j)
        {
            values[j] = ((double) (i+1)) * Double::exp(avLower + j * avStep);
        }
    }
    boost::shared_ptr<AVContext> avc;
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        spot,
        rate,
        volatility,
        3,
        nbRannacher,
        0.3,
        nbSpaceNodes,
        stdDevMultiple
    ));
    boost::shared_ptr<TradeFixings> noFixings(new TradeFixings());
    model->setupForTrade(dates, avc, noFixings);
    boost::shared_ptr<const CEValues> moValues(model->marketObservableCE(1));
    const double* const spaceGridPtrBegin(moValues->getDataPtr());
    boost::shared_ptr<PDETradePricer> solverTradePricer(new PDETradePricer(model));
    
    VarDependencies avDepsPlanes(GenPDE::VT_AuxiliaryVariable, 1, nbPlanes);
    double* solPtr(model->addPricer(1, avDepsPlanes)->getDataPtr());
    const double* spaceGridPtr   = spaceGridPtrBegin;
    const double* avValuesPtr    = &avValues[nbDaysTotal-2][0];
    for(size_t i=0; i<nbSpaceNodes; ++i)
    {
        double s = *spaceGridPtr++;
        for(size_t j=0; j<nbPlanes; ++j)
            *solPtr++ = s * (0.01 - ((double) nbDaysTotal) / (s + *avValuesPtr++));
        avValuesPtr -= nbPlanes;
    }
    
    vector<double> auxData(nbPlanes * nbSpaceNodes);
    for(size_t i=dates.size()-2; i>1; --i)
    {
        model->timeStepToNextDate();
        
        // We evaluate the form of the new AV in auxData:
        double* auxDataPtr = &auxData[0];
        spaceGridPtr   = spaceGridPtrBegin;
        avValuesPtr    = &avValues[i-2][0];
        for(size_t j=0; j<nbSpaceNodes; ++j)
        {
            double s = *spaceGridPtr++;
            for(size_t k=0; k<nbPlanes; ++k)
                *auxDataPtr++ = s + *avValuesPtr++;
            avValuesPtr -= nbPlanes;
        }
        
        PricerUid tmpUid;
        boost::shared_ptr<CEValues> newSol(model->addTempPricer(avDepsPlanes, tmpUid));
        double*       newSolPtr(newSol->getDataPtr());
        const double* oldSolPtr(model->getPricer(1)->getDataPtr());
        const double* avPtr = &auxData[0];
        //AVInterpolatorNaturalCubic        interp(&avValues[i-1][0], nbPlanes, GenPDE::ExtrapolationType_Linear);
        //AVInterpolatorLinearUniform<GenPDE::Log>       interp(&avValues[i-1][0], nbPlanes, GenPDE::ExtrapolationType_Linear);
        AVInterpolatorNaturalCubicUniform<GenPDE::Log> interp(&avValues[i-1][0], nbPlanes, GenPDE::ExtrapolationType_Linear);
        for(size_t ii=0; ii<nbSpaceNodes; ++ii)
        {
            interp.updateY(oldSolPtr);
            oldSolPtr += nbPlanes;
            for(size_t j=0; j<nbPlanes; ++j)
                *newSolPtr++ = interp.interpolate(*avPtr++);
        }
        model->renamePricer(tmpUid, 1);
        if( dates[i] >= firstERDate )
        {
            double* solPtr     = model->getPricer(1)->getDataPtr();
            const double* sPtr = spaceGridPtrBegin;
            avPtr              = &avValues[i-1][0];
            for(size_t j=0; j<nbSpaceNodes; ++j)
            {
                double s = *sPtr++;
                for(size_t k=0; k<nbPlanes; ++k)
                {
                    *solPtr = Double::max(*solPtr, s * (0.01 - ((double) (i+1)) / (s + *avPtr++)));
                    ++solPtr;
                }
                avPtr -= nbPlanes;
            }
        }
    }
    model->timeStepToNextDate();
    // We need to remove the very last one:
    VarDependencies noAVDeps;
    PricerUid tmpUid;
    boost::shared_ptr<CEValues> newSol(model->addTempPricer(noAVDeps, tmpUid));
    double*       newSolPtr(newSol->getDataPtr());
    const double* oldSolPtr(model->getPricer(1)->getDataPtr());
    spaceGridPtr = spaceGridPtrBegin;
    //AVInterpolatorNaturalCubic        interp(&avValues[0][0], nbPlanes, GenPDE::ExtrapolationType_Linear);
    //AVInterpolatorLinearUniform<GenPDE::Log>       interp(&avValues[0][0], nbPlanes, GenPDE::ExtrapolationType_Linear);
    AVInterpolatorNaturalCubicUniform<GenPDE::Log> interp(&avValues[0][0], nbPlanes, GenPDE::ExtrapolationType_Linear);
    for(size_t i=0; i<nbSpaceNodes; ++i)
    {
        interp.updateY(oldSolPtr);
        *newSolPtr++ = interp.interpolate(*spaceGridPtr++);
        oldSolPtr += nbPlanes;
    }
    model->renamePricer(tmpUid, 1);
    if( dates[1] >= firstERDate )
    {
        double* solPtr = model->getPricer(1)->getDataPtr();
        const double* sPtr = spaceGridPtrBegin;
        for(size_t j=0; j<nbSpaceNodes; ++j)
        {
            *solPtr = Double::max(*solPtr, 0.01 * *sPtr++ - 1);
            ++solPtr;
        }
    }
    // Final timestepping:
    model->timeStepToNextDate();
    double v = *(model->getPricer(1)->getDataPtr());
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    
    cerr << "Time: " << (mst2 - mst1).total_microseconds() << endl;
    cerr << "Value: " << v << endl;
    TEST_EQ_DBL(v, 0.0247, 1e-3); // low precision, b/c we don't want to spend ages here!
}
