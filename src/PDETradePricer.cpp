#include "ChoiceEnums.h"
#include "PDETradePricer.h"
#include "CEValues.h"
#include "TradeLeg.h"
#include "PricingInstruction.h"
#include "PayoutExpression.h"
#include "PDEPricingModelInterface.h"
#include "TradeRepresentation.h"

void PDETradePricer::pricerInit(PricerUid uid, const TLPtr &trade_leg)
{
    CEVConstPtr tlValue(trade_leg->evalCE(mPDEModel));
    CEVPtr      pricer(mPDEModel->addPricer(uid, tlValue->getVarMemoryLayout()));
    pricer->evalUnaryOperation(GenPDE::Id, tlValue);
}

void PDETradePricer::timeStepToNextDate() 
{ 
    mPDEModel->timeStepToNextDate();
}

void PDETradePricer::pricerAdd(
    PricerUid       uid,
    const TLPtr&    leg_0,
    const TLPtr&    leg_1
)
{
    CEVConstPtr     leg0Val(leg_0->evalCE(mPDEModel));
    CEVConstPtr     leg1Val(leg_1->evalCE(mPDEModel));
    VarDependencies varDeps(leg0Val->getVarMemoryLayout(), leg1Val->getVarMemoryLayout());
    CEVPtr          pricer(mPDEModel->addPricer(uid, varDeps));
    pricer->evalBinaryOperation(GenPDE::Add, leg0Val, leg1Val);
}

void PDETradePricer::pricerMult(
    PricerUid       uid,
    const PEPtr&    pe,
    const TLPtr&    leg
)
{
    CEVConstPtr     peVal(pe->evalCE(mPDEModel));
    CEVConstPtr     legVal(leg->evalCE(mPDEModel));
    VarDependencies varDeps(legVal->getVarMemoryLayout(), peVal->getVarMemoryLayout());
    CEVPtr          pricer(mPDEModel->addPricer(uid, varDeps));
    pricer->evalBinaryOperation(GenPDE::Mul, legVal, peVal);
}
    
void PDETradePricer::pricerChoice(
    PricerUid       uid,
    Choice::Chooser chooser,
    const TLPtr&    leg_0,
    const TLPtr&    leg_1
)
{
    CEVConstPtr     leg0Val(leg_0->evalCE(mPDEModel));
    CEVConstPtr     leg1Val(leg_1->evalCE(mPDEModel));
    VarDependencies varDeps(leg0Val->getVarMemoryLayout(), leg1Val->getVarMemoryLayout());
    CEVPtr          pricer(mPDEModel->addPricer(uid, varDeps));
    switch (chooser)
    {
    case Choice::Chooser_Us:
        pricer->evalBinaryOperation(GenPDE::Min, leg0Val, leg1Val);
        break;
    case Choice::Chooser_Client:
        pricer->evalBinaryOperation(GenPDE::Max, leg0Val, leg1Val);
        break;
    default:
        Exception::raise("PDESolverTradePricer::pricerChoice", "Unhandled chooser");
    }
}

void PDETradePricer::removeAVDependency(PricerUid uid, GenPDE::VariableUID av_uid)
{
    CEVPtr                 oldPricer(mPDEModel->getPricer(uid));
    const VarMemoryLayout& oldDeps(oldPricer->getVarMemoryLayout());
    if( av_uid != oldDeps.getInnerAV() )
        Exception::raise("PDESolverTradePricer::removeAVDependency", "Can only remove innermost AV");
    boost::shared_ptr<const AuxiliaryVariable> avDetails = mTradeRepresentation->getAuxiliaryVariables()->getAuxiliaryVariable(av_uid);
    CEVConstPtr     avValues(avDetails->getDefinition()->evalCE(mPDEModel));
    VarDependencies newDeps(oldDeps.removeAVDependency(av_uid), avValues->getVarMemoryLayout());
    
    PricerUid      tmpUid;
    CEVPtr         tmpPricer(mPDEModel->addTempPricer(newDeps, tmpUid));
    CEValues::rmAVDependency(
        tmpPricer,
        oldPricer,
        avValues,
        mPDEModel->auxiliaryVariableCE(av_uid)
    );
    mPDEModel->renamePricer(tmpUid, uid);
}

void PDETradePricer::pricerRemove(PricerUid uid)
{
    mPDEModel->removePricer(uid);
}

double PDETradePricer::price()
{
    boost::shared_ptr<const DatedPricingInstructions> datedPricingInstructions(mTradeRepresentation->getDatedPricingInstructions());
    std::vector<GenPDE::Date>                         tradeDates(datedPricingInstructions->getInstructionDates());
    mPDEModel->setupForTrade(tradeDates, *mTradeRepresentation->getAuxiliaryVariables());
    GenPDE::Date                                      currentDate(mPDEModel->getCurrentDate());
    std::vector<PIPtr> instructions(datedPricingInstructions->getPricingInstructions(currentDate));
    for(const PIPtr& instruction : instructions )
        instruction->apply(mPDEModel, mTradeRepresentation);
    while( mPDEModel->timeStepToNextDate() )
    {
        instructions = datedPricingInstructions->getPricingInstructions(mPDEModel->getCurrentDate());
        for(const PIPtr& instruction : instructions)
            instruction->apply(mPDEModel, mTradeRepresentation);
    }
    
    boost::shared_ptr<const TradeLeg>                 mainTradeLeg(mTradeRepresentation->getMainTradeLeg());
    CEVConstPtr mainLegValue(mainTradeLeg->evalCE(mPDEModel));
    return *(mainLegValue->getDataPtr());
}
