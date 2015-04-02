#include <iostream>
#include <string>

#include <boost/program_options.hpp>

void usage(boost::program_options::options_description& desc)
{
    std::cout << desc << std::endl;
}

int main(int argc, char* argv[])
{
    std::string payoutFile;
    std::string modelFile;

    namespace po = boost::program_options;
    po::options_description desc( "GenPDEPricer options" );
    desc.add_options()
        ("help,h", "Display help")
        ("payout,p", po::value<std::string>(&payoutFile)->required(), "specify payout file")
        ("model,m",  po::value<std::string>(&modelFile )->required(), "specify model file" )
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

        std::cout << "GenPDEPricer" << std::endl;
        std::cout << "* Payout: " << payoutFile << std::endl;
        std::cout << "* Model:  " << modelFile  << std::endl;
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
