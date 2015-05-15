#include <vector>
#include <iostream>

#include "GpThreadPool.h"
#include "GpParallelizer.h"

#include "Framework.h"
#include "TestSuite.h"

void process( int* data, int a, int b, int n )
{
    std::cout << "process(" << data << ", " << a << ", " << b << ", " << n << ")" << std::endl;
    for( size_t i = 0; i < n; ++i )
        data[i] = a * data[i] + b;
}

REGISTER_TEST( ThreadPool1 )
{
    int n = 64;
    std::vector< int > data( 256, 0 );
    {
        GenPDE::ThreadPool* threadPool = new GenPDE::ThreadPool( 4 );
        GenPDE::Parallelizer parallelizer( *threadPool );
        //GenPDE::ThreadPool threadPool( 4 );
        int * data0 = &data[  0];
        int * data1 = &data[ 64];
        int * data2 = &data[128];
        int * data3 = &data[192];
        //threadPool->enqueue( [ data0, n ] { process( data0, 0, 1, n ); } );
        parallelizer.addTask( [ data0, n ] { process( data0, 0, 1, n ); } );
        parallelizer.addTask( [ data1, n ] { process( data1, 0, 1, n ); } );
        parallelizer.addTask( [ data2, n ] { process( data2, 0, 1, n ); } );
        parallelizer.addTask( [ data3, n ] { process( data3, 0, 1, n ); } );
        parallelizer.addTask( [ data0, n ] { process( data0, 1, 1, n ); } );
        parallelizer.addTask( [ data1, n ] { process( data1, 1, 2, n ); } );
        parallelizer.addTask( [ data2, n ] { process( data2, 1, 3, n ); } );
        parallelizer.addTask( [ data3, n ] { process( data3, 1, 4, n ); } );
        parallelizer.addTask( [ data0, n ] { process( data0, 1, 0, n ); } );
        parallelizer.addTask( [ data1, n ] { process( data1, 2, 0, n ); } );
        parallelizer.addTask( [ data2, n ] { process( data2, 3, 0, n ); } );
        parallelizer.addTask( [ data3, n ] { process( data3, 4, 0, n ); } );
        parallelizer.run();
    }
    for( int i = 0; i < 256; ++i )
        std::cout << i << " -> " << data[i] << std::endl;
}
