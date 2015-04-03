#ifndef TRIDIAGONAL_ROUTINES_H
#define TRIDIAGONAL_ROUTINES_H

/// \brief A collection of tridiagonal matrix handling routines
/// (matrix-vector multiplication, and tridiagonal system inversion)
///
/// Tridiagonal operators of size n are coded as flat arrays of size
/// 3*n, where t[3 * i], t[3 * i + 1] and t[3 * i + 2] are the coefficients
/// of the i-th line of the operator (respectively lower-diagonal, diagonal
/// and upper-diagonal). Note that the very first and last elements
/// of the array do not correspond to actual coefficients of the operator.
class Tridiagonal
{
public:
    /// Multiplication of a vector by a tridiagonal matrix
    /// Sets y so that t.x = y, where n is the size of the system
    static void apply(
    	const double* t,
    	const double* x,
    	double*       y,
    	size_t        n)
    {
    	*y++ = t[1] * x[0] + t[2] * x[1];
    	t   += 3;
    	for (size_t i=n-2; i>0; --i)
    	{
    	    *y++ = t[0] * x[0] + t[1] * x[1] + t[2] * x[2];
    	    ++x;
    	    t += 3;
    	}
    	*y = t[0] * x[0] + t[1] * x[1];
    }

    /// Multiplication of a vector by a tridiagonal matrix
    static void apply(
    	const double* t,
    	const double* x,
    	double*       y,
    	size_t        n,
    	size_t        p)
    {
    	if ( p == 1 )
    		return apply(t, x, y, n);
    	++t;
    	double*       yAux(y);
    	const double* xAux(x);
    	double        tAux(*t++);
    	for (size_t i=p; i>0; --i)
    		*yAux++  = tAux * *xAux++;
    	yAux = y;
    	tAux = *t++;
    	for (size_t i=p; i>0; --i)
    		*yAux++ += tAux * *xAux++;

    	for (size_t k=n-2; k>0; --k)
    	{
    	    y    = yAux;
    	    xAux = x;
    	    tAux = *t++;
    	    for (size_t i=p; i>0; --i)
    	    	*yAux++  = tAux * *xAux++;

    	    x    = xAux;
    	    yAux = y;
    	    tAux = *t++;
    	    for (size_t i=p; i>0; --i)
    	    	*yAux++ += tAux * *xAux++;

    	    yAux = y;
    	    tAux = *t++;
    	    for (size_t i=p; i>0; --i)
    	    	*yAux++ += tAux * *xAux++;
    	}

    	y    = yAux;
    	xAux = x;
    	tAux = *t++;
    	for (size_t i=p; i>0; --i)
    		*yAux++  = tAux * *xAux++;

    	yAux = y;
    	tAux = *t++;
    	for (size_t i=p; i>0; --i)
    		*yAux++ += tAux * *xAux++;
    }

    /// Decompose and solve a tridiagonal system
    static void decomposeAndSolve(
    	double*       t,
    	double*       x,
    	size_t        n)
    {
    	t    += 1;
    	*t    = 1.0 / *t;
    	t[1] *= t[0];
    	*x   *= *t++;
    	for (size_t i=n-1; i>0; --i)
    	{
    		// t points to the super-diagonal element of the last line
    		// (which has index (n-1-i))
    		// x also points to the last line
    		t[2]  = 1.0 / (t[2] - t[0] * t[1]);
    		t[3] *= t[2];
    		x[1]  = (x[1] - x[0] * t[1]) * t[2];
    		t    += 3;
    		++x;
    	}
    	for (size_t i=n-1; i>0; --i)
    	{
    		t -= 3;
    		--x;
    		x[0] -= t[0] * x[1];
    	}
    }

    /// Decompose and solve a tridiagonal system
    static void decomposeAndSolve(
    	double*       t,
    	double*       x,
    	size_t        n,
    	size_t        p)
    {
    	if ( p == 1 )
    		return decomposeAndSolve(t, x, n);
    	t    += 1;
    	*t    = 1.0 / *t;
    	t[1] *= t[0];
    	const double a(*t++);
    	double* xAux(x);
    	for (size_t k=p; k>0; --k)
    		*xAux++ *= a;
    	for (size_t i=n-1; i>0; --i)
    	{
    		// t points to the super-diagonal element of the last line
    		// (which has index (n-1-i))
    		// x also points to the last line
    		t[2]  = 1.0 / (t[2] - t[0] * t[1]);
    		t[3] *= t[2];
    		const double b(*(++t));
    		const double c(*(++t));
    		for (size_t k=p; k>0; --k)
    		{
    			*xAux = (*xAux - *x++ * b) * c;
    			++xAux;
    		}
    		++t;
    	}
    	--xAux;
    	--x;
    	for (size_t i=n-1; i>0; --i)
    	{
    		t -= 3;
    		const double d(t[0]);
    		for (size_t k=p; k>0; --k)
    			*x-- -= d * *xAux--;
    	}
    }

    /// Decompose and solve a tridiagonal system
    static void solveFromDecomposed(
    	double*       t,
    	double*       x,
    	size_t        n)
    {
    	++t;
    	*x   *= *t++;
    	++t;
    	for (size_t i=n-1; i>0; --i)
    	{
    		// t points to the super-diagonal element of the last line
    		// (which has index (n-1-i))
    		// x also points to the last line
    		x[1]  = (x[1] - x[0] * t[0]) * t[1];
    		t    += 3;
    		++x;
    	}
    	--t;
    	for (size_t i=n-1; i>0; --i)
    	{
    		t -= 3;
    		--x;
    		x[0] -= t[0] * x[1];
    	}
    }

    /// Decompose and solve a tridiagonal system
    static void solveFromDecomposed(
    	double*       t,
    	double*       x,
    	size_t        n,
    	size_t        p)
    {
    	if ( p == 1 )
    		return solveFromDecomposed(t, x, n);
    	++t;
    	const double a(*t++);
    	double* xAux(x);
    	for (size_t k=p; k>0; --k)
    		*xAux++ *= a;
    	++t;
    	for (size_t i=n-1; i>0; --i)
    	{
    		// t points to the super-diagonal element of the last line
    		// (which has index (n-1-i))
    		// x also points to the last line
    		const double b(*t++);
    		const double c(*t++);
    		for (size_t k=p; k>0; --k)
    		{
    			*xAux = (*xAux - *x++ * b) * c;
    			++xAux;
    		}
    		++t;
    	}
    	--t;
    	--xAux;
    	--x;
    	for (size_t i=n-1; i>0; --i)
    	{
    		t -= 3;
    		const double d(t[0]);
    		for (size_t k=p; k>0; --k)
    			*x-- -= d * *xAux--;
    	}
    }
};

#endif // TRIDIAGONAL_ROUTINES_H
