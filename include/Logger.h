#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

class Logger
{
public:
    void log( const char* msg )
    {
        std::cerr << m_linePrefix << msg << std::endl;
    }

    std::ostream& getStream( bool with_prefix = true )
    {
        if( with_prefix )
            std::cerr << m_linePrefix;
        return std::cerr;
    }

    static Logger* instance()
    {
        static Logger* s_logger = NULL;
        if( !s_logger )
        {
            s_logger = new Logger();
        }

        return s_logger;
    }

private:
    Logger()
    :m_linePrefix( "GenPDE::Logger - " )
    {}

    ~Logger();

    Logger( const Logger& );

private:
    std::string m_linePrefix;
};

#endif //  LOGGER_H

