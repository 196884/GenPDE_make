/*
 * IteratorsTests.cpp
 *
 *  Created on: Dec 17, 2009
 *      Author: dupontr
 */

#include "Framework.h"
#include "TestSuite.h"

#include <vector>

using namespace std;

// Memory mapping example for the matrix:
//
//
//       j ->
//        0   1   2   3   4
//
// i  0   0   3   6   9  12
// |  1   1   4   7  10  13
// v  2   2   5   8  11  14
namespace {
	class MyMatrix
	{
	public:
		/// Constructor
		MyMatrix(size_t nb_rows, size_t nb_columns);

		/// Accessor
		double& operator()(size_t i, size_t j);
		double  operator()(size_t i, size_t j) const;

		/// The iterator class (used for rows or columns)
		class iterator
		{
		public:
			iterator(double* data, size_t stride)
			:mPosition(data)
			,mStride(stride)
			{}

			double operator*() { return *mPosition; }

			iterator operator++() { mPosition += mStride; return *this; }

			bool operator!=(const iterator& it) const { return mPosition != it.mPosition; }

		private:
			double* mPosition;
			size_t  mStride;
		};

		iterator beginRow(size_t index_row)
		{
			return iterator(mDataBegin + index_row, mNbRows);
		}

		iterator endRow(size_t index_row)
		{
			return iterator(mDataBegin + index_row + mNbRows * mNbColumns, mNbRows);
		}

		iterator beginColumn(size_t index_column)
		{
			return iterator(mDataBegin + index_column * mNbRows, 1);
		}

		iterator endColumn(size_t index_column)
		{
			return iterator(mDataBegin + (index_column + 1) * mNbRows, 1);
		}

		iterator begin()
		{
			return iterator(mDataBegin, 1);
		}

		iterator end()
		{
			return iterator(mDataBegin + mNbRows * mNbColumns, 1);
		}

	private:
		size_t         mNbRows;
		size_t         mNbColumns;
		vector<double> mData;
		double* const  mDataBegin;
	};

	MyMatrix::MyMatrix(size_t nb_rows, size_t nb_columns)
	:mNbRows(nb_rows)
	,mNbColumns(nb_columns)
	,mData(nb_rows * nb_columns)
	,mDataBegin(&mData[0])
	{}

	double& MyMatrix::operator()(size_t i, size_t j)
	{
		return mData[i + j * mNbRows];
	}

	double MyMatrix::operator()(size_t i, size_t j) const
	{
		return mData[i + j * mNbRows];
	}

}

REGISTER_TEST(IteratorsTest1)
{
	MyMatrix m(3, 5);
	for(size_t i=0; i<3; ++i)
		for(size_t j=0; j<5; ++j)
			m(i, j) = 100 + 10 * i + j;
	TEST( m(0, 4) == 104 );
	size_t index = 0;
	MyMatrix::iterator itBegin = m.begin();
	MyMatrix::iterator itEnd   = m.end();
	TEST( itBegin != itEnd );
	for(MyMatrix::iterator it = m.begin(); it != m.end(); ++it)
	{
		size_t i = index % 3;
		size_t j = index / 3;
		TEST( m(i, j) == *it );
		++index;
	}
	TEST( index == 3 * 5 );

	index = 0;
	for(MyMatrix::iterator it = m.beginRow(2); it != m.endRow(2); ++it)
		TEST( *it == m(2, index++) );
	TEST( index == 5 );

	index = 0;
	for(MyMatrix::iterator it = m.beginColumn(1); it != m.endColumn(1); ++it)
		TEST( *it == m(index++, 1) );
	TEST( index == 3 );
}

