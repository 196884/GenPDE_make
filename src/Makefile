CC = clang
BOOST_HEADERS = /Users/regisdupont/Documents/Code/boost_1_56_0
BOOST_LIBS = /Users/regisdupont/Documents/Code/boost_1_56_0/bin.v2/libs/date_time/build/darwin-4.2.1/release/link-static/threading-multi

CPPFLAGS = --std=c++11 -Wall -stdlib=libc++ -MMD

LFLAGS += -L $(BOOST_LIBS)
LFLAGS += -lboost_date_time
LFLAGS += -lstdc++

BUILD_DIR = ../build
SRC_DIR = ../src
BIN_DIR = ../bin

INCLUDES += -I $(BOOST_HEADERS)

TARGET = $(BIN_DIR)/GenPDETest

CPPFILES += ASROptionTests.cpp
CPPFILES += ASRRawTests.cpp
CPPFILES += ASRTests.cpp
CPPFILES += AVInterpolatorTests.cpp
CPPFILES += AVReference.cpp
CPPFILES += CEValues.cpp
CPPFILES += CEValuesFunctionsTests.cpp
CPPFILES += CEValuesStored.cpp
CPPFILES += CEValuesStoredTests.cpp
CPPFILES += CubicSplineInterpolator.cpp
CPPFILES += CubicSplineInterpolatorTests.cpp
CPPFILES += Date.cpp
CPPFILES += DateTests.cpp
CPPFILES += DateTimeTests.cpp
CPPFILES += FwdStartingCall.cpp
CPPFILES += GenPDEParser.cpp
CPPFILES += GenPDEParserTests.cpp
CPPFILES += IteratorsTests.cpp
CPPFILES += MOContext1DFixed.cpp
CPPFILES += MOContextTests.cpp
CPPFILES += MOReference.cpp
CPPFILES += MarkovFunctionalDistribution.cpp
CPPFILES += MarkovFunctionalTests.cpp
CPPFILES += PDECallPricingTests.cpp
CPPFILES += PDEPricerSolutionStorage.cpp
CPPFILES += PDEPricingModelBase.cpp
CPPFILES += PDEPricingModelBlackScholes.cpp
CPPFILES += PDESolver1D.cpp
CPPFILES += PDETradePricer.cpp
CPPFILES += PDETradePricer_AsianCall_Tests.cpp
CPPFILES += PDETradePricer_Call_Tests.cpp
CPPFILES += PDETradePricer_DigitalCall_Tests.cpp
CPPFILES += PEBinaryOperation.cpp
CPPFILES += PEBinaryOperationTests.cpp
CPPFILES += PEComparisonOperation.cpp
CPPFILES += PEConditional.cpp
CPPFILES += PEConstant.cpp
CPPFILES += PEOperators.cpp
CPPFILES += PEOperatorsTests.cpp
CPPFILES += PEUnaryOperation.cpp
CPPFILES += PIPricerAdd.cpp
CPPFILES += PIPricerChoice.cpp
CPPFILES += PIPricerCond.cpp
CPPFILES += PIPricerInit.cpp
CPPFILES += PIPricerMult.cpp
CPPFILES += PIPricerRemove.cpp
CPPFILES += PIPricerRmAVDeps.cpp
CPPFILES += PayoutExpression.cpp
CPPFILES += PayoutExpressionTests.cpp
CPPFILES += PricingInstruction.cpp
CPPFILES += PricingInstructionTests.cpp
CPPFILES += RngTests.cpp
CPPFILES += TLPayment.cpp
CPPFILES += TLPricerRef.cpp
CPPFILES += TestSuite.cpp
CPPFILES += TestSuite_main.cpp
CPPFILES += TimeGridFunctions.cpp
CPPFILES += TimeGridFunctionsTests.cpp
CPPFILES += TradeFixings.cpp
CPPFILES += TradeLeg.cpp
CPPFILES += TradeLegTests.cpp
CPPFILES += TradeRepresentationTests.cpp
CPPFILES += TridiagonalRoutinesTests.cpp
CPPFILES += VariableDependencies.cpp
CPPFILES += VariableDependenciesTests.cpp
CPPFILES += basicTests.cpp

SRCFILES = $(addprefix $(SRC_DIR)/, $(CPPFILES))

OBJFILES = $(addprefix $(BUILD_DIR)/, $(CPPFILES:.cpp=.o))

DEPSFILES = $(addprefix $(BUILD_DIR)/, $(CPPFILES:.cpp=.d))

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

GenPDETest: $(OBJFILES) $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(LFLAGS) $(OBJFILES) -o $(BIN_DIR)/GenPDETest

cleanobj:
	rm -fr $(BUILD_DIR)/*

cleanbin:
	rm -fr $(BIN_DIR)/*

clean: cleanobj cleanbin

$(BUILD_DIR)/ASROptionTests.o: $(SRC_DIR)/ASROptionTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/ASROptionTests.cpp -o $(BUILD_DIR)/ASROptionTests.o

$(BUILD_DIR)/ASRRawTests.o: $(SRC_DIR)/ASRRawTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/ASRRawTests.cpp -o $(BUILD_DIR)/ASRRawTests.o

$(BUILD_DIR)/ASRTests.o: $(SRC_DIR)/ASRTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/ASRTests.cpp -o $(BUILD_DIR)/ASRTests.o

$(BUILD_DIR)/AVInterpolatorTests.o: $(SRC_DIR)/AVInterpolatorTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/AVInterpolatorTests.cpp -o $(BUILD_DIR)/AVInterpolatorTests.o

$(BUILD_DIR)/AVReference.o: $(SRC_DIR)/AVReference.cpp AVReference.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/AVReference.cpp -o $(BUILD_DIR)/AVReference.o

$(BUILD_DIR)/CEValues.o: $(SRC_DIR)/CEValues.cpp CEValues.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/CEValues.cpp -o $(BUILD_DIR)/CEValues.o

$(BUILD_DIR)/CEValuesFunctionsTests.o: $(SRC_DIR)/CEValuesFunctionsTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/CEValuesFunctionsTests.cpp -o $(BUILD_DIR)/CEValuesFunctionsTests.o

$(BUILD_DIR)/CEValuesStored.o: $(SRC_DIR)/CEValuesStored.cpp CEValuesStored.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/CEValuesStored.cpp -o $(BUILD_DIR)/CEValuesStored.o

$(BUILD_DIR)/CEValuesStoredTests.o: $(SRC_DIR)/CEValuesStoredTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/CEValuesStoredTests.cpp -o $(BUILD_DIR)/CEValuesStoredTests.o

$(BUILD_DIR)/CubicSplineInterpolator.o: $(SRC_DIR)/CubicSplineInterpolator.cpp CubicSplineInterpolator.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/CubicSplineInterpolator.cpp -o $(BUILD_DIR)/CubicSplineInterpolator.o

$(BUILD_DIR)/CubicSplineInterpolatorTests.o: $(SRC_DIR)/CubicSplineInterpolatorTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/CubicSplineInterpolatorTests.cpp -o $(BUILD_DIR)/CubicSplineInterpolatorTests.o

$(BUILD_DIR)/Date.o: $(SRC_DIR)/Date.cpp Date.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/Date.cpp -o $(BUILD_DIR)/Date.o

$(BUILD_DIR)/DateTests.o: $(SRC_DIR)/DateTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/DateTests.cpp -o $(BUILD_DIR)/DateTests.o

$(BUILD_DIR)/DateTimeTests.o: $(SRC_DIR)/DateTimeTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/DateTimeTests.cpp -o $(BUILD_DIR)/DateTimeTests.o

$(BUILD_DIR)/FwdStartingCall.o: $(SRC_DIR)/FwdStartingCall.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/FwdStartingCall.cpp -o $(BUILD_DIR)/FwdStartingCall.o

$(BUILD_DIR)/GenPDEParser.o: $(SRC_DIR)/GenPDEParser.cpp GenPDEParser.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/GenPDEParser.cpp -o $(BUILD_DIR)/GenPDEParser.o

$(BUILD_DIR)/GenPDEParserTests.o: $(SRC_DIR)/GenPDEParserTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/GenPDEParserTests.cpp -o $(BUILD_DIR)/GenPDEParserTests.o

$(BUILD_DIR)/IteratorsTests.o: $(SRC_DIR)/IteratorsTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/IteratorsTests.cpp -o $(BUILD_DIR)/IteratorsTests.o

$(BUILD_DIR)/MOContext1DFixed.o: $(SRC_DIR)/MOContext1DFixed.cpp MOContext1DFixed.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/MOContext1DFixed.cpp -o $(BUILD_DIR)/MOContext1DFixed.o

$(BUILD_DIR)/MOContextTests.o: $(SRC_DIR)/MOContextTests.cpp MOContextTests.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/MOContextTests.cpp -o $(BUILD_DIR)/MOContextTests.o

$(BUILD_DIR)/MOReference.o: $(SRC_DIR)/MOReference.cpp MOReference.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/MOReference.cpp -o $(BUILD_DIR)/MOReference.o

$(BUILD_DIR)/MarkovFunctionalDistribution.o: $(SRC_DIR)/MarkovFunctionalDistribution.cpp MarkovFunctionalDistribution.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/MarkovFunctionalDistribution.cpp -o $(BUILD_DIR)/MarkovFunctionalDistribution.o

$(BUILD_DIR)/MarkovFunctionalTests.o: $(SRC_DIR)/MarkovFunctionalTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/MarkovFunctionalTests.cpp -o $(BUILD_DIR)/MarkovFunctionalTests.o

$(BUILD_DIR)/PDECallPricingTests.o: $(SRC_DIR)/PDECallPricingTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDECallPricingTests.cpp -o $(BUILD_DIR)/PDECallPricingTests.o

$(BUILD_DIR)/PDEPricerSolutionStorage.o: $(SRC_DIR)/PDEPricerSolutionStorage.cpp PDEPricerSolutionStorage.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDEPricerSolutionStorage.cpp -o $(BUILD_DIR)/PDEPricerSolutionStorage.o

$(BUILD_DIR)/PDEPricingModelBase.o: $(SRC_DIR)/PDEPricingModelBase.cpp PDEPricingModelBase.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDEPricingModelBase.cpp -o $(BUILD_DIR)/PDEPricingModelBase.o

$(BUILD_DIR)/PDEPricingModelBlackScholes.o: $(SRC_DIR)/PDEPricingModelBlackScholes.cpp PDEPricingModelBlackScholes.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDEPricingModelBlackScholes.cpp -o $(BUILD_DIR)/PDEPricingModelBlackScholes.o

$(BUILD_DIR)/PDESolver1D.o: $(SRC_DIR)/PDESolver1D.cpp PDESolver1D.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDESolver1D.cpp -o $(BUILD_DIR)/PDESolver1D.o

$(BUILD_DIR)/PDETradePricer.o: $(SRC_DIR)/PDETradePricer.cpp PDETradePricer.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDETradePricer.cpp -o $(BUILD_DIR)/PDETradePricer.o

$(BUILD_DIR)/PDETradePricer_AsianCall_Tests.o: $(SRC_DIR)/PDETradePricer_AsianCall_Tests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDETradePricer_AsianCall_Tests.cpp -o $(BUILD_DIR)/PDETradePricer_AsianCall_Tests.o

$(BUILD_DIR)/PDETradePricer_Call_Tests.o: $(SRC_DIR)/PDETradePricer_Call_Tests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDETradePricer_Call_Tests.cpp -o $(BUILD_DIR)/PDETradePricer_Call_Tests.o

$(BUILD_DIR)/PDETradePricer_DigitalCall_Tests.o: $(SRC_DIR)/PDETradePricer_DigitalCall_Tests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PDETradePricer_DigitalCall_Tests.cpp -o $(BUILD_DIR)/PDETradePricer_DigitalCall_Tests.o

$(BUILD_DIR)/PEBinaryOperation.o: $(SRC_DIR)/PEBinaryOperation.cpp PEBinaryOperation.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEBinaryOperation.cpp -o $(BUILD_DIR)/PEBinaryOperation.o

$(BUILD_DIR)/PEBinaryOperationTests.o: $(SRC_DIR)/PEBinaryOperationTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEBinaryOperationTests.cpp -o $(BUILD_DIR)/PEBinaryOperationTests.o

$(BUILD_DIR)/PEComparisonOperation.o: $(SRC_DIR)/PEComparisonOperation.cpp PEComparisonOperation.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEComparisonOperation.cpp -o $(BUILD_DIR)/PEComparisonOperation.o

$(BUILD_DIR)/PEConditional.o: $(SRC_DIR)/PEConditional.cpp PEConditional.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEConditional.cpp -o $(BUILD_DIR)/PEConditional.o

$(BUILD_DIR)/PEConstant.o: $(SRC_DIR)/PEConstant.cpp PEConstant.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEConstant.cpp -o $(BUILD_DIR)/PEConstant.o

$(BUILD_DIR)/PEOperators.o: $(SRC_DIR)/PEOperators.cpp PEOperators.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEOperators.cpp -o $(BUILD_DIR)/PEOperators.o

$(BUILD_DIR)/PEOperatorsTests.o: $(SRC_DIR)/PEOperatorsTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEOperatorsTests.cpp -o $(BUILD_DIR)/PEOperatorsTests.o

$(BUILD_DIR)/PEUnaryOperation.o: $(SRC_DIR)/PEUnaryOperation.cpp PEUnaryOperation.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PEUnaryOperation.cpp -o $(BUILD_DIR)/PEUnaryOperation.o

$(BUILD_DIR)/PIPricerAdd.o: $(SRC_DIR)/PIPricerAdd.cpp PIPricerAdd.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerAdd.cpp -o $(BUILD_DIR)/PIPricerAdd.o

$(BUILD_DIR)/PIPricerChoice.o: $(SRC_DIR)/PIPricerChoice.cpp PIPricerChoice.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerChoice.cpp -o $(BUILD_DIR)/PIPricerChoice.o

$(BUILD_DIR)/PIPricerCond.o: $(SRC_DIR)/PIPricerCond.cpp PIPricerCond.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerCond.cpp -o $(BUILD_DIR)/PIPricerCond.o

$(BUILD_DIR)/PIPricerInit.o: $(SRC_DIR)/PIPricerInit.cpp PIPricerInit.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerInit.cpp -o $(BUILD_DIR)/PIPricerInit.o

$(BUILD_DIR)/PIPricerMult.o: $(SRC_DIR)/PIPricerMult.cpp PIPricerMult.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerMult.cpp -o $(BUILD_DIR)/PIPricerMult.o

$(BUILD_DIR)/PIPricerRemove.o: $(SRC_DIR)/PIPricerRemove.cpp PIPricerRemove.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerRemove.cpp -o $(BUILD_DIR)/PIPricerRemove.o

$(BUILD_DIR)/PIPricerRmAVDeps.o: $(SRC_DIR)/PIPricerRmAVDeps.cpp PIPricerRmAVDeps.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PIPricerRmAVDeps.cpp -o $(BUILD_DIR)/PIPricerRmAVDeps.o

$(BUILD_DIR)/PayoutExpression.o: $(SRC_DIR)/PayoutExpression.cpp PayoutExpression.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PayoutExpression.cpp -o $(BUILD_DIR)/PayoutExpression.o

$(BUILD_DIR)/PayoutExpressionTests.o: $(SRC_DIR)/PayoutExpressionTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PayoutExpressionTests.cpp -o $(BUILD_DIR)/PayoutExpressionTests.o

$(BUILD_DIR)/PricingInstruction.o: $(SRC_DIR)/PricingInstruction.cpp PricingInstruction.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PricingInstruction.cpp -o $(BUILD_DIR)/PricingInstruction.o

$(BUILD_DIR)/PricingInstructionTests.o: $(SRC_DIR)/PricingInstructionTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/PricingInstructionTests.cpp -o $(BUILD_DIR)/PricingInstructionTests.o

$(BUILD_DIR)/RngTests.o: $(SRC_DIR)/RngTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/RngTests.cpp -o $(BUILD_DIR)/RngTests.o

$(BUILD_DIR)/TLPayment.o: $(SRC_DIR)/TLPayment.cpp TLPayment.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TLPayment.cpp -o $(BUILD_DIR)/TLPayment.o

$(BUILD_DIR)/TLPricerRef.o: $(SRC_DIR)/TLPricerRef.cpp TLPricerRef.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TLPricerRef.cpp -o $(BUILD_DIR)/TLPricerRef.o

$(BUILD_DIR)/TestSuite.o: $(SRC_DIR)/TestSuite.cpp TestSuite.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TestSuite.cpp -o $(BUILD_DIR)/TestSuite.o

$(BUILD_DIR)/TestSuite_main.o: $(SRC_DIR)/TestSuite_main.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TestSuite_main.cpp -o $(BUILD_DIR)/TestSuite_main.o

$(BUILD_DIR)/TimeGridFunctions.o: $(SRC_DIR)/TimeGridFunctions.cpp TimeGridFunctions.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TimeGridFunctions.cpp -o $(BUILD_DIR)/TimeGridFunctions.o

$(BUILD_DIR)/TimeGridFunctionsTests.o: $(SRC_DIR)/TimeGridFunctionsTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TimeGridFunctionsTests.cpp -o $(BUILD_DIR)/TimeGridFunctionsTests.o

$(BUILD_DIR)/TradeFixings.o: $(SRC_DIR)/TradeFixings.cpp TradeFixings.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TradeFixings.cpp -o $(BUILD_DIR)/TradeFixings.o

$(BUILD_DIR)/TradeLeg.o: $(SRC_DIR)/TradeLeg.cpp TradeLeg.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TradeLeg.cpp -o $(BUILD_DIR)/TradeLeg.o

$(BUILD_DIR)/TradeLegTests.o: $(SRC_DIR)/TradeLegTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TradeLegTests.cpp -o $(BUILD_DIR)/TradeLegTests.o

$(BUILD_DIR)/TradeRepresentationTests.o: $(SRC_DIR)/TradeRepresentationTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TradeRepresentationTests.cpp -o $(BUILD_DIR)/TradeRepresentationTests.o

$(BUILD_DIR)/TridiagonalRoutinesTests.o: $(SRC_DIR)/TridiagonalRoutinesTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/TridiagonalRoutinesTests.cpp -o $(BUILD_DIR)/TridiagonalRoutinesTests.o

$(BUILD_DIR)/VariableDependencies.o: $(SRC_DIR)/VariableDependencies.cpp VariableDependencies.h
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/VariableDependencies.cpp -o $(BUILD_DIR)/VariableDependencies.o

$(BUILD_DIR)/VariableDependenciesTests.o: $(SRC_DIR)/VariableDependenciesTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/VariableDependenciesTests.cpp -o $(BUILD_DIR)/VariableDependenciesTests.o

$(BUILD_DIR)/basicTests.o: $(SRC_DIR)/basicTests.cpp
	$(CC) $(CPPFLAGS) -c $(SRC_DIR)/basicTests.cpp -o $(BUILD_DIR)/basicTests.o

-include $(DEPSFILES)

