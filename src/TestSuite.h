/*
 * TestSuite.h
 *
 *  Created on: Oct 31, 2009
 *      Author: dupontr
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <boost/shared_ptr.hpp>

/// \brief A class designed to store the result(s) of the run of a single test
class TestResults
{
public:
	/// Default constructor
	TestResults();

	/// Adding a failure
	///
	/// \param	failure_info		a string describing the failure
	void pushFailure(const std::string& failure_info);

	/// Adding an exception
	///
	/// \param	exception_info		a string describing the exception
	void pushException(const std::string& exception_info);

	/// Notifying a test success
	void notifySuccess();

	/// Checking if the test is a success
	///
	/// \result						<tt>true</tt> iff the test is a success (i.e., no failures and no exceptions)
	bool isSuccess() const;

	/// Printing (on std::out) all results
	void printAll() const;

private:
	/// The test failures
	std::vector<std::string> mFailures;

	/// The test exceptions
	std::vector<std::string> mExceptions;

	/// The number of test successes
	size_t                   mNbSuccesses;
};

/// The type definition for a single test
typedef void (*UnitTestFn)(TestResults&);

/// \brief Information about a unit test definition (name, file, line
/// in the file, and test function)
struct UnitTest
{
	/// Constructor
	UnitTest(
		const std::string& name,
		const std::string& file,
		size_t             line,
		UnitTestFn         function);

	std::string mName;
	std::string mFile;
	size_t      mLine;
	UnitTestFn  mFunction;
};

/// \brief The <c>TestSuite</c> class is a singleton factory, in which
/// individual tests are registered.
///
/// Methods on the class can then be called to run tests and access results.
class TestSuite
{
public:
	static bool doRegister(const boost::shared_ptr<UnitTest>& unit_test);

	/// Running a single test
	static void runTest(const std::string& name);
    
    /// Prints the names of all available tests
    static void printAllTestNames();

	/// Running all registered tests
	static void runAll();

	/// Outputting all test results
	static void printAllResults();
    
    /// Outputting the number of tests run, #of successes and detailed failures:
    static void printRecapWithFailures();

private:
	/// The method to access the singleton instance
	/// \result					the singleton instance
	static TestSuite& instance()
	{
		static TestSuite result;
		return result;
	}

	bool doRegisterPriv(const boost::shared_ptr<UnitTest>& unit_test);

	/// Running a single test
	void runPriv(const boost::shared_ptr<UnitTest>& unit_test);

	void runTestPriv(const std::string& name);

	/// Running all registered tests
	void runAllPriv();
    
    void printAllTestNamesPriv();

	/// Printing all test results
	void printAllResultsPriv();
    
    void printRecapWithFailuresPriv();

private:
	/// The currently registered unit tests
	std::map<std::string, boost::shared_ptr<UnitTest> >    mUnitTests;

	/// The results
	std::map<std::string, TestResults> mResults;
};

////////////
// MACROS //
////////////

#define REGISTER_TEST(name) \
	void TEST_##name(TestResults& _test_results); \
	const bool _is_TEST_##name##_registered(TestSuite::doRegister(boost::shared_ptr<UnitTest>(new UnitTest(#name, __FILE__, __LINE__, &TEST_##name))));\
	void TEST_##name(TestResults& _test_results)

#define TEST(totest) \
	if ( totest )    \
	_test_results.notifySuccess(); \
    else {\
    std::ostringstream o; \
    o << "boolean test failure at l. " << __LINE__; \
    _test_results.pushFailure(o.str()); \
    }

#define TEST_EQ(a, b) \
	if ( (a) == (b) ) \
	_test_results.notifySuccess(); \
	else {\
	std::ostringstream o; \
	o << "equality test failure at l. " << __LINE__; \
	o << " ( '" << (a) << "' is not equal to '" << (b) << "' )"; \
	_test_results.pushFailure(o.str()); \
	}

#define TEST_EQ_DBL(a, b, c) \
	if ( Double::isEqual((a), (b), (c)) ) \
	_test_results.notifySuccess(); \
	else {\
	std::ostringstream o; \
	o << "equality test failure at l. " << __LINE__; \
	o << " ( '" << (a) << "' is not equal to '" << (b) << "' )"; \
	_test_results.pushFailure(o.str()); \
	}

#endif /* TESTSUITE_H_ */
