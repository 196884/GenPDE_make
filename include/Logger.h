#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

class Logger
{
public:
    enum Level
    {
        None    = 0,
        Error   = 1,
        Warning = 2,
        Info    = 3,
        Debug   = 4
    };

public:
    void setLevel( Level level )
    {
        m_level = level;
    }

    void log( Level level, const char* msg )
    {
        if( None == level )
            return;

        if( level <= m_level )
        {
            std::cerr << m_linePrefix;
            if( m_addTimestamp )
            {
                boost::posix_time::ptime mst = boost::posix_time::microsec_clock::local_time();
                std::cerr << boost::posix_time::to_iso_extended_string( mst ) << " - ";
            }

            switch( level )
            {
                case Info:
                    std::cerr << "INFO:    ";
                    break;
                case Error:
                    std::cerr << "ERROR:   ";
                    break;
                case Warning:
                    std::cerr << "WARNING: ";
                    break;
                case Debug:
                    std::cerr << "DEBUG:   ";
                    break;
                default:
                    break;
            }
            std::cerr << msg << std::endl;
        }
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

    const std::string& getPrefix() const
    {
        return m_linePrefix;
    }

private:
    Logger()
    :m_level(        None                )
    ,m_addTimestamp( true                )
    ,m_linePrefix(   "GenPDE::Logger - " )
    {}

    ~Logger();

    Logger( const Logger& );

private:
    Level       m_level;
    bool        m_addTimestamp;
    std::string m_linePrefix;
};

#endif //  LOGGER_H
