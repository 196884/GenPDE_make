/*
 * TestSuite.cpp
 *
 *  Created on: Oct 31, 2009
 *      Author: dupontr
 */
#include <iostream>

#include "Framework.h"
#include "TestSuite.h"

using namespace std;

/////////////////
// TestResults //
/////////////////

TestResults::TestResults()
:mNbSuccesses(0)
{}

void TestResults::pushFailure(const std::string& failure_info)
{
	mFailures.push_back(failure_info);
}

void TestResults::pushException(const std::string& exception_info)
{
	mExceptions.push_back(exception_info);
}

bool TestResults::isSuccess() const
{
	return mFailures.empty() && mExceptions.empty();
}

void TestResults::notifySuccess()
{
	++mNbSuccesses;
}

void TestResults::printAll() const
{
	cout << "  " << mNbSuccesses << " success(es)" << endl;
	if ( ! mFailures.empty() )
	{
		const size_t nbFailures(mFailures.size());
		cout << "  " << mFailures.size() << " failure(s):" << endl;
		for (size_t i=0; i<nbFailures; ++i)
			cout << "    Failure #" << i+1 << "/" << nbFailures << ": " << mFailures[i] << endl;
	}
	if ( ! mExceptions.empty() )
	{
		const size_t nbExceptions(mExceptions.size());
		cout << "  " << nbExceptions << " exception(s):" << endl;
		for (size_t i=0; i<nbExceptions; ++i)
			cout << "    Exception #" << i+1 << "/" << nbExceptions << ": " << mExceptions[i] << endl;
	}
}


//////////////
// UnitTest //
//////////////

UnitTest::UnitTest(
	const std::string& name,
	const std::string& file,
	size_t             line,
	UnitTestFn         function)
	:mName(name)
	,mFile(file)
	,mLine(line)
	,mFunction(function)
{}


///////////////
// TestSuite //
///////////////

bool TestSuite::doRegister(const boost::shared_ptr<UnitTest>& unit_test)
{
	return instance().doRegisterPriv(unit_test);
}

bool TestSuite::doRegisterPriv(const boost::shared_ptr<UnitTest>& unit_test)
{
	const std::string name(unit_test->mName);
	std::map<std::string, boost::shared_ptr<UnitTest> >::const_iterator it(mUnitTests.find(name));
	if ( it != mUnitTests.end() )
	{
		Exception::raise("TestSuite::doRegisterPriv", "trying to register a test which name already exists");
		return false;
	}
	mUnitTests[name] = unit_test;
	return true;
}

void TestSuite::runTest(const std::string& name)
{
	instance().runTestPriv(name);
}

void TestSuite::runPriv(const boost::shared_ptr<UnitTest>& unit_test)
{
	TestResults& results(mResults[unit_test->mName]);
	try
	{
		(*(unit_test->mFunction))(results);
	} catch ( Exception e )
	{
		results.pushException(e.toString());
	}
}

void TestSuite::runTestPriv(const std::string& name)
{
	std::map<std::string, boost::shared_ptr<UnitTest> >::iterator test(mUnitTests.find(name));
	if ( test == mUnitTests.end() )
		Exception::raise("TestSuite::runTestPriv", "could not find the requested test");
	runPriv(test->second);
}

void TestSuite::printAllTestNames()
{
    instance().printAllTestNamesPriv();
}

void TestSuite::runAll()
{
	instance().runAllPriv();
}

void TestSuite::printAllResults()
{
	instance().printAllResultsPriv();
}

void TestSuite::printRecapWithFailures()
{
    instance().printRecapWithFailuresPriv();
}

void TestSuite::printAllTestNamesPriv()
{
    cout << "Available tests:" << endl;
	std::map<std::string, boost::shared_ptr<UnitTest> >::iterator       itCurr(mUnitTests.begin());
	std::map<std::string, boost::shared_ptr<UnitTest> >::const_iterator itEnd(mUnitTests.end());
	for (; itCurr != itEnd; ++itCurr)
        cout << "* " << itCurr->first << endl; 
}

void TestSuite::runAllPriv()
{
    cout << "*** Test run starting ***" << endl << endl;
	std::map<std::string, boost::shared_ptr<UnitTest> >::iterator       itCurr(mUnitTests.begin());
	std::map<std::string, boost::shared_ptr<UnitTest> >::const_iterator itEnd(mUnitTests.end());
	for (; itCurr != itEnd; ++itCurr)
	{
		const string& name(itCurr->first);
		cout << name << " running " << endl;
		runPriv(itCurr->second);
		cout << name << " finished" << endl << endl;
	}
    cout << "*** Test run finished ***" << endl << endl;
}

void TestSuite::printAllResultsPriv()
{
    cout << "*** Printing all results ***" << endl;
	std::map<std::string, TestResults>::iterator       itCurr(mResults.begin());
	std::map<std::string, TestResults>::const_iterator itEnd(mResults.end());
	for (; itCurr != itEnd; ++itCurr)
	{
		const string& name(itCurr->first);
		boost::shared_ptr<UnitTest> test(mUnitTests[name]);
		cout << name;
		cout << " (file '" << test->mFile << "', l. " << test->mLine << ")" << endl;
		itCurr->second.printAll();
		cout << endl;
	}
	cout << "*** All results printed ***" << endl;
}

void TestSuite::printRecapWithFailuresPriv()
{
    size_t nbRun     = 0;
    size_t nbSuccess = 0;
    size_t nbFailure = 0;
    cout << "*** Printing all failures ***" << endl;
	std::map<std::string, TestResults>::iterator       itCurr(mResults.begin());
	std::map<std::string, TestResults>::const_iterator itEnd(mResults.end());
	for (; itCurr != itEnd; ++itCurr)
	{
        ++nbRun;
        if( itCurr->second.isSuccess() )
        {
            ++nbSuccess;
        } else {
            ++nbFailure;
            const string& name(itCurr->first);
            boost::shared_ptr<UnitTest> test(mUnitTests[name]);
            cout << name;
            cout << " (file '" << test->mFile << "', l. " << test->mLine << ")" << endl;
            itCurr->second.printAll();
            cout << endl;
        }
	}
	cout << "*** All failures printed ***" << endl << endl;
    cout << "*** Test run recap ***" << endl;
    cout << nbRun << " test(s) run" << endl;
    cout << nbSuccess << " success(es)" << endl;
    cout << nbFailure << " failure(es)" << endl;
    cout << endl;
}
