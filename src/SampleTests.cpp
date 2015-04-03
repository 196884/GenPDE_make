/*
 * basicTests.cpp
 *
 *  Created on: Oct 31, 2009
 *      Author: dupontr
 */

#include "Framework.h"
#include "TestSuite.h"

// Test examples/tests

namespace {
	void AATest(TestResults& _test_results);
	const bool is_mOwnTest_registered(TestSuite::doRegister(boost::shared_ptr<UnitTest>(new UnitTest("AATest", "basicTests.cpp", 13, &AATest))));
	void AATest(TestResults& _test_results)
	{
		_test_results.notifySuccess();
		_test_results.notifySuccess();
		_test_results.notifySuccess();
		_test_results.notifySuccess();
		_test_results.notifySuccess();
		_test_results.pushFailure("This is the first failure");
		_test_results.pushFailure("This is the second failure");
		_test_results.notifySuccess();
		Exception::raise("myOwnTest", "Ouch!!! an unexpected (!!!) exception :)");
		return;
	}
}


REGISTER_TEST(SecondTest)
{
	_test_results.notifySuccess();
}

REGISTER_TEST(Test3)
{
	int a(5);
	int b(2);
	int c(3);
	TEST( a == b + c )
	TEST( a == b )
	TEST( a == a )
	TEST_EQ(a, b+c)
	TEST_EQ(b+c, c+b)
	TEST_EQ(a+b, c)
	double x(1.01);
	double z(1.02);
	TEST_EQ_DBL(x, z, 0.1)
	TEST_EQ_DBL(x, z, 0.01)
	TEST_EQ_DBL(x, z, 0.0100001)
	TEST_EQ_DBL(x, z, 0.001)
}

