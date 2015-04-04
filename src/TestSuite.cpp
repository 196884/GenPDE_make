#include <iostream>

#include "Framework.h"
#include "TestSuite.h"

using namespace std;

/////////////////
// TestResults //
/////////////////

TestResults::TestResults()
:m_nbSuccesses(0)
{}

void TestResults::pushFailure(const std::string& failure_info)
{
	m_failures.push_back(failure_info);
}

void TestResults::pushException(const std::string& exception_info)
{
	m_exceptions.push_back(exception_info);
}

bool TestResults::isSuccess() const
{
	return m_failures.empty() && m_exceptions.empty();
}

void TestResults::notifySuccess()
{
	++m_nbSuccesses;
}

void TestResults::printAll() const
{
	cout << "  " << m_nbSuccesses << " success(es)" << endl;
	if ( ! m_failures.empty() )
	{
		const size_t nbFailures(m_failures.size());
		cout << "  " << m_failures.size() << " failure(s):" << endl;
		for (size_t i=0; i<nbFailures; ++i)
			cout << "    Failure #" << i+1 << "/" << nbFailures << ": " << m_failures[i] << endl;
	}
	if ( ! m_exceptions.empty() )
	{
		const size_t nbExceptions(m_exceptions.size());
		cout << "  " << nbExceptions << " exception(s):" << endl;
		for (size_t i=0; i<nbExceptions; ++i)
			cout << "    Exception #" << i+1 << "/" << nbExceptions << ": " << m_exceptions[i] << endl;
	}
}


//////////////
// UnitTest //
//////////////

UnitTest::UnitTest(
	const std::string& name,
	const std::string& file,
	size_t             line,
	UnitTestFn         function,
    bool               autorun
)
	:m_name(name)
	,m_file(file)
	,m_line(line)
	,m_function(function)
    ,m_autorun(autorun)
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
	const std::string name(unit_test->m_name);
	std::map<std::string, boost::shared_ptr<UnitTest> >::const_iterator it(m_unitTests.find(name));
	if ( it != m_unitTests.end() )
	{
		Exception::raise("TestSuite::doRegisterPriv", "trying to register a test which name already exists");
		return false;
	}
	m_unitTests[name] = unit_test;
	return true;
}

void TestSuite::runTest(const std::string& name)
{
	instance().runTestPriv(name);
}

void TestSuite::runPriv(UnitTest& unit_test)
{
	TestResults& results(m_results[unit_test.m_name]);
	try
	{
		(*(unit_test.m_function))(results);
	} catch ( Exception e )
	{
		results.pushException(e.toString());
	}
}

void TestSuite::runTestPriv(const std::string& name)
{
	std::map<std::string, boost::shared_ptr<UnitTest> >::iterator test(m_unitTests.find(name));
	if ( test == m_unitTests.end() )
		Exception::raise("TestSuite::runTestPriv", "could not find the requested test");
	runPriv(*test->second);
}

void TestSuite::printAllTestNames()
{
    instance().printAllTestNamesPriv();
}

void TestSuite::runAll(bool autorun_only)
{
	instance().runAllPriv(autorun_only);
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
	std::map<std::string, boost::shared_ptr<UnitTest> >::iterator       itCurr(m_unitTests.begin());
	std::map<std::string, boost::shared_ptr<UnitTest> >::const_iterator itEnd(m_unitTests.end());
	for (; itCurr != itEnd; ++itCurr)
        cout << "* " << itCurr->first << endl; 
}

void TestSuite::runAllPriv(bool autorun_only)
{
    cout << "*** Test run starting ***" << endl << endl;
	std::map<std::string, boost::shared_ptr<UnitTest> >::iterator       itCurr(m_unitTests.begin());
	std::map<std::string, boost::shared_ptr<UnitTest> >::const_iterator itEnd(m_unitTests.end());
	for (; itCurr != itEnd; ++itCurr)
	{
        UnitTest& test = *itCurr->second;
        if( test.m_autorun || !autorun_only )
        {
            const string& name(itCurr->first);
            cout << name << " running " << endl;
            runPriv(test);
            cout << name << " finished" << endl << endl;
        }
	}
    cout << "*** Test run finished ***" << endl << endl;
}

void TestSuite::printAllResultsPriv()
{
    cout << "*** Printing all results ***" << endl;
	std::map<std::string, TestResults>::iterator       itCurr(m_results.begin());
	std::map<std::string, TestResults>::const_iterator itEnd(m_results.end());
	for (; itCurr != itEnd; ++itCurr)
	{
		const string& name(itCurr->first);
		boost::shared_ptr<UnitTest> test(m_unitTests[name]);
		cout << name;
		cout << " (file '" << test->m_file << "', l. " << test->m_line << ")" << endl;
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
	std::map<std::string, TestResults>::iterator       itCurr(m_results.begin());
	std::map<std::string, TestResults>::const_iterator itEnd(m_results.end());
	for (; itCurr != itEnd; ++itCurr)
	{
        ++nbRun;
        if( itCurr->second.isSuccess() )
        {
            ++nbSuccess;
        } else {
            ++nbFailure;
            const string& name(itCurr->first);
            boost::shared_ptr<UnitTest> test(m_unitTests[name]);
            cout << name;
            cout << " (file '" << test->m_file << "', l. " << test->m_line << ")" << endl;
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
