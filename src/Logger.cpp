// Project includes
#include "../include/Logger.h"

// C++ includes
#include <iostream>

namespace util{

// Initial value of static object
Logger* Logger::m_logger = NULL;

// Singleton instance function
Logger* Logger::get_instance( void )
{
		if(m_logger == NULL)
		{
			m_logger = new Logger();
		}

		return m_logger;
}

// Destructor
Logger::~Logger( void )
{
		delete m_logger;
}

// Log message based on severity level
void Logger::log( LOG_LEVEL level, std::string msg ) const 
{
		// Only execute log if input level is greater than or equal to max level
		if( level >= static_cast<LOG_LEVEL>( max_debug ) )
		{
				switch (level) 
				{
						case LOG_LEVEL::DEBUG:
						{
								std::cout << "Debug Message: " << msg << std::endl;
						} break;
						case LOG_LEVEL::ERROR:
						{
								std::cout << "***ERROR*** Message: " << msg << std::endl;
						} break;
						case LOG_LEVEL::NONE:
						default:
						{
								// Do nothing
						} break;
				}
		}
}

// Set max log level
void Logger::set_max_log_level(LOG_LEVEL level)
{
		max_debug = static_cast<int>(level);
}

// Helper function outside class to wrap singleton call
void LOG(Logger::LOG_LEVEL level, std::string msg)
{
		Logger::get_instance()->log(level, msg);
}

} // End of namespace util