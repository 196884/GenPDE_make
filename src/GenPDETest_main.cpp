#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

void usage(boost::program_options::options_description& desc)
{
    std::cout << desc << std::endl;
}

int main(int argc, const char * argv[]) {
    int autoOnly = 1;

    namespace po = boost::program_options;
    po::options_description desc( "GenPDEPricer options" );
    desc.add_options()
        ("help,h", "Display help")

        ("auto,a", po::value<int>(&autoOnly)->default_value(1),
         "autorun only (0/1, if 1, excludes manual tests)")

        ("test,t", po::value<std::string>(),
         "specifies a single test to run")
    ;

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if( vm.count("help") )
        {
            usage(desc);
            return 0;
        }
        po::notify(vm);

        if( vm.count("test" ) )
        {
            TestSuite::runTest(vm["test"].as<std::string>());
        } else {
            TestSuite::runAll(1 == vm["auto"].as<int>());
        }
        TestSuite::printRecapWithFailures();
    }
    catch(std::exception& e)
    {
        std::cerr << "Unhandled exception in GenPDEPricer: "
                  << e.what()
                  << std::endl
                  << std::endl
                  << desc
                  << std::endl
                  << "Exiting."
                  << std::endl;
        return 1;
    }
    return 0;
}
    
