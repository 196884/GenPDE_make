#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"

#include "GenPDEParser.h"
#include "PDETradePricer.h"

void usage(boost::program_options::options_description& desc)
{
    std::cout << desc << std::endl;
}

int main(int argc, char* argv[])
{
    std::string payoutFile;
    std::string modelFile;
    std::string moFixingsFile;
    std::string choiceFixingsFile;
    std::string loggingLevel( "None" );

    namespace po = boost::program_options;
    po::options_description desc( "GenPDEPricer options" );
    desc.add_options()
        ("help,h", "Display help")
        ("payout,p",  po::value<std::string>(&payoutFile)->required(), "specify payout file"               )
        ("model,m",   po::value<std::string>(&modelFile )->required(), "specify model file"                )
        ("fixings,f", po::value<std::string>(&moFixingsFile),          "specify market observable fixings" )
        ("choices,c", po::value<std::string>(&choiceFixingsFile),      "specify choice fixings" )
        ("timing,t", "display timing information" )
        ("logging,l", po::value<std::string>(&loggingLevel)->required(), "logging level (None, Error, Warning, Info, Debug), None by default" )
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

        if( "Error" == loggingLevel )
            Logger::instance()->setLevel( Logger::Error );
        else if( "Warning" == loggingLevel )
            Logger::instance()->setLevel( Logger::Warning );
        else if( "Info" == loggingLevel )
            Logger::instance()->setLevel( Logger::Info );
        else if( "Debug" == loggingLevel )
            Logger::instance()->setLevel( Logger::Debug );
        else
            Logger::instance()->setLevel( Logger::None );

        boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
        boost::shared_ptr<const TradeRepresentation> trade(GenPDEParser::parseTradeRepresentation(payoutFile, true));
        boost::shared_ptr<PDEPricingModelInterface>  model(GenPDEParser::parsePDEModel(modelFile, true));
        boost::shared_ptr<const MOFixingsIfc> moFixings( new MOFixingsStore() );
        if( vm.count("fixings") > 0 )
        {
            moFixings = GenPDEParser::parseMOFixings(moFixingsFile, true);
        } else {
            moFixings = boost::shared_ptr<const MOFixingsStore>( new MOFixingsStore() );
        }
        boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
        boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer( model, trade, moFixings ));
        boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
        if( vm.count("timing") )
            std::cout << "Total pricing time: "
                      << (mst2 - mst0).total_microseconds()
                      << "us (parsing: "
                      << (mst1 - mst0).total_microseconds()
                      << "us, pricer: "
                      << (mst2 - mst1).total_microseconds()
                      << "us)"
                      << std::endl;

        std::cout << "Price: " << pricer->price() << std::endl;
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
