#include <vector>
#include <iostream>
#include <ctime>

#include "Framework.h"
#include "TestSuite.h"

#include "TridiagonalRoutines.h"

using namespace std;

REGISTER_TEST(TridiagonalApply1)
{
	double t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0};
	size_t n(4);
	double x[] = {-3, 2, 7, -1};
	double y[] = {0, 0, 0, 0};
	Tridiagonal::apply(t, x, y, n);
	TEST_EQ(y[0],  1)
	TEST_EQ(y[1], 34)
	TEST_EQ(y[2], 53)
	TEST_EQ(y[3], 53)
}

REGISTER_TEST(TridiagonalApply2)
{
	double t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0};
	size_t n(4);
	size_t p(5);
	double x[] = {-3, 3, 0, 6, -6, 2, -2, 0, -4, 4, 7, -7, 0, -14, 14, -1, 1, 0, 2, -2};
	double y[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	Tridiagonal::apply(t, x, y, n, p);
	TEST_EQ(y[0],  1)
	TEST_EQ(y[1], -1)
	TEST_EQ(y[2],  0)
	TEST_EQ(y[3], -2)
	TEST_EQ(y[4],  2)
	TEST_EQ(y[5], 34)
	TEST_EQ(y[6], -34)
	TEST_EQ(y[7],  0)
	TEST_EQ(y[8], -68)
	TEST_EQ(y[9], 68)
	TEST_EQ(y[10], 53);
	TEST_EQ(y[11], -53);
	TEST_EQ(y[12], 0);
	TEST_EQ(y[13], -106);
	TEST_EQ(y[14], 106);
	TEST_EQ(y[15], 53);
	TEST_EQ(y[16], -53);
	TEST_EQ(y[17], 0);
	TEST_EQ(y[18], -106);
	TEST_EQ(y[19], 106);
}

REGISTER_TEST(TridiagonalDecomposeAndSolve1)
{
	double t[] = {0, 3.1, 0.4, -0.7, 2.9, 0.1, 0.9, 3.5, -0.4, -0.3, 3.0, 1.1, -0.8, 3.3, 0};
	size_t n(5);
	double x[] = {1, 2, 3, 4, 5};
	double y[] = {0, 0, 0, 0, 0};
	double z[] = {3, 2, 1, 0, 5};
	double u[] = {0, 0, 0, 0, 0};
	Tridiagonal::apply(t, x, y, n);
	Tridiagonal::apply(t, z, u, n);
	Tridiagonal::decomposeAndSolve(t, y, n);
	TEST_EQ_DBL(x[0], y[0], 1e-6);
	TEST_EQ_DBL(x[1], y[1], 1e-6);
	TEST_EQ_DBL(x[2], y[2], 1e-6);
	TEST_EQ_DBL(x[3], y[3], 1e-6);
	TEST_EQ_DBL(x[4], y[4], 1e-6);
	Tridiagonal::solveFromDecomposed(t, u, n);
	TEST_EQ_DBL(z[0], u[0], 1e-6);
	TEST_EQ_DBL(z[1], u[1], 1e-6);
	TEST_EQ_DBL(z[2], u[2], 1e-6);
	TEST_EQ_DBL(z[3], u[3], 1e-6);
	TEST_EQ_DBL(z[4], u[4], 1e-6);
}

REGISTER_TEST(TridiagonalDecomposeAndSolve2)
{
	double t[] = {0, 3.1, 0.4, -0.7, 2.9, 0.1, 0.9, 3.5, -0.4, -0.3, 3.0, 1.1, -0.8, 3.3, 0};
	size_t n(5);
	double x[] = {1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7};
	double y[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	size_t k(3);
	Tridiagonal::apply(t, x, y, n, k);
	Tridiagonal::decomposeAndSolve(t, y, n, k);
	for (size_t i=0; i<15; ++i)
		TEST_EQ_DBL(x[i], y[i], 1e-6);
}

REGISTER_TEST(TridiagonalDecomposeAndSolve3)
{
	double t[] = {0, 10.2, 1.2, -0.7, 9.8, 1.1, 0.5, 12.7, 2.1, -1.3, 11.1, -1.6, 1.9, 13.1, -1.2, 1.7, 9.9, 0};
	size_t n(6);
	double x[] = {1, 2, 3, 4, 2, 7, 3, 9, 4, 1, 6, 3, 2, 6, 9, 3, 5, 6, 7, 3, 9, 0, 3, 2, 5, 7, 8, 9, 4, 2};
	double z[] = {3, 6, 3, 1, 2, 4, 6, 4, 2, 0, 0, 7};
	size_t px(5);
	size_t pz(2);
	vector<double> y(30);
	double* yp(&(y[0]));
	vector<double> u(12);
	double* up(&(u[0]));
	Tridiagonal::apply(t, x, yp, n, px);
	Tridiagonal::apply(t, z, up, n, pz);
	Tridiagonal::decomposeAndSolve(t, yp, n, px);
	Tridiagonal::solveFromDecomposed(t, up, n, pz);
	for (size_t i=0; i<30; ++i)
		TEST_EQ_DBL(x[i], y[i], 1e-6);
	for (size_t i=0; i<12; ++i)
		TEST_EQ_DBL(z[i], u[i], 1e-6);
}

REGISTER_TEST(TridiagonalDecomposeAndSolve4)
{
	const size_t n(21);
	vector<double> t(3 * n);
	t[0] = 0;
	t[1] = 0.99875;
	t[2] = 0.00125;
	for (size_t i=1; i<n-1; ++i)
	{
		t[3*i] = -0.001875;
		t[3*i+1] = 1.0025;
		t[3*i+2] = -0.000625;
	}
	t[3*n-3] = -0.00125;
	t[3*n-2] = 1.00125;
	t[3*n-1] = 0;
	double x[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8.32178e-05, 0.132993, 0.283849, 0.454792, 0.648495, 0.86799, 1.11671, 1.39855, 1.71791, 2.07979, 2.48983};
	vector<double> tBak(t);
	vector<double> y(x, x+n);
	vector<double> z(n);
	Tridiagonal::decomposeAndSolve(&(t[0]), x, n, 1);
	Tridiagonal::apply(&(tBak[0]), x, &(z[0]), n, 1);
	for (size_t i=0; i<n; ++i)
		TEST_EQ_DBL(z[i], y[i], 1e-10);
}

// The following test is mostly intended to try and evaluate the performance of
// *non-vectorized* versions of 'decomposeAndSolve'
REGISTER_TEST(TridiagonalPerformance1)
{
	// 500  -> 1042886
	// 1000 -> 2265434
	size_t n(1000);
	size_t reps(1);
	vector<double> t(3 * n);
	vector<double> tb(3 * n);
	vector<double> x(n);
	vector<double> y(n);
	double* tp(&(t[0]));
	double* tbp(&(tb[0]));
	double* xp(&(x[0]));
	double* yp(&(y[0]));
	for (size_t i=0; i<n; ++i)
	{
		tb[3 * i    ] =  1.1;
		tb[3 * i + 1] = 12.7;
		tb[3 * i + 2] = -1.3;
	}
	clock_t start(clock());
	for (size_t i=0; i<reps; ++i)
	{
		std::fill(x.begin(), x.end(), 3.14);
		std::copy(tb.begin(), tb.end(), t.begin());
		Tridiagonal::decomposeAndSolve(tp, xp, n);
	}
	clock_t end(clock());
	clock_t delay(end - start);
	delay++;
	//cout << "Timing: " << delay << endl;
	Tridiagonal::apply(tbp, xp, yp, n);
	for (size_t i=0; i<n; ++i)
		TEST_EQ_DBL(y[i], 3.14, 1e-6);
}
