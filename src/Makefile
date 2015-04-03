CC = clang
BOOST_HEADERS = /Users/regisdupont/Documents/Code/boost_1_56_0
BOOST_LIBS = /Users/regisdupont/Documents/Code/boost_1_56_0/bin.v2/libs/date_time/build/darwin-4.2.1/release/link-static/threading-multi
BOOST_LIBS = /Users/regisdupont/Documents/Code/boost_1_56_0/bin.v2/libs/program_options/build/darwin-4.2.1/release/link-static/threading-multi

LFLAGS += -L $(BOOST_LIBS)
LFLAGS += -lboost_date_time
LFLAGS += -lboost_program_options
LFLAGS += -lstdc++

SRC_DIR = ../src
HDR_DIR = ../include
BUILD_DIR = ../build
BIN_DIR = ../bin

INCLUDES += -I $(BOOST_HEADERS)
INCLUDES += -I $(HDR_DIR)

CPPFLAGS = --std=c++11 -Wall -stdlib=libc++ -g-MMD $(INCLUDES)
#CPPFLAGS = --std=c++11 -Wall -stdlib=libc++ -g -MMD $(INCLUDES)

TARGET = $(BIN_DIR)/GenPDETest

CPPFILES += ASROptionTests.cpp
CPPFILES += ASRRawTests.cpp
CPPFILES += ASRTests.cpp
CPPFILES += AVContext.cpp
CPPFILES += AVDiscretizationPolicy.cpp
CPPFILES += AVInterpolatorTests.cpp
CPPFILES += AVReference.cpp
CPPFILES += AuxiliaryVariable.cpp
CPPFILES += CEValues.cpp
CPPFILES += CEValuesFunctionsTests.cpp
CPPFILES += CEValuesStored.cpp
CPPFILES += CEValuesStoredTests.cpp
CPPFILES += CubicSplineInterpolator.cpp
CPPFILES += CubicSplineInterpolatorTests.cpp
CPPFILES += DateTests.cpp
CPPFILES += DateTimeTests.cpp
CPPFILES += FwdStartingCall.cpp
CPPFILES += GenPDEParser.cpp
CPPFILES += GenPDEParserTests.cpp
CPPFILES += GpDate.cpp
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

EXES += GenPDETest
EXES += GenPDEPricer

SRCFILES = $(addprefix $(SRC_DIR)/, $(CPPFILES))

OBJFILES = $(addprefix $(BUILD_DIR)/, $(CPPFILES:.cpp=.o))

DEPSFILES = $(addprefix $(BUILD_DIR)/, $(CPPFILES:.cpp=.d))

all: $(addprefix $(BIN_DIR)/, $(EXES))

-include $(DEPSFILES)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BIN_DIR)/GenPDETest: $(SRC_DIR)/GenPDETest_main.cpp $(OBJFILES) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(LFLAGS) $(OBJFILES) $< -o $@

$(BIN_DIR)/GenPDEPricer: $(SRC_DIR)/GenPDEPricer_main.cpp $(OBJFILES) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(LFLAGS) $(OBJFILES) $< -o $@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

cleanobj:
	rm -fr $(BUILD_DIR)/*

cleanbin:
	rm -fr $(BIN_DIR)/*

clean: cleanobj cleanbin


