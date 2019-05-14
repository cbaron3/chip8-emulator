#ifndef LOGGER_H
#define LOGGER_H

// C++ includes
#include <string>
#include <iomanip>

/*!
 *  \addtogroup util
 *  @{
 */

//! project utility for logging errors
namespace util
{

/**
 * @brief Logger class used to log messages at different severity levels
 */
class Logger
{
public:

	/**
	 * @brief enum class describing basic logging levels
	 */
	enum class LOG_LEVEL{DEBUG, ERROR, NONE};

	/**
	 * @brief Get the singleton instance object
	 * 
	 * @return Logger* singleton object
	 */
	static Logger* get_instance();

	/**
	 * @brief Logging function
	 * 
	 * @param level logging level you want to restrict based on
	 * @param msg string message you want to write to output
	 */
	void log(LOG_LEVEL level, std::string msg) const;

	/**
	 * @brief Set the max log level of the logger
	 * 
	 * @param level new maximum log level
	 */
	void set_max_log_level(LOG_LEVEL level);

	/**
	 * @brief Destroy the Logger object
	 */
	~Logger( void );
	
protected:
private:
	/** Hiding constructor to enforce singleton */
	Logger(){}
	
	/** Max debug level */
	int max_debug = 0;

	/** Singleton object */
	static Logger* m_logger;
};

/**
 * @brief Helper function to encapsulate singleton logging
 * 
 * @param level level to log at
 * @param msg string message
 */
void LOG(Logger::LOG_LEVEL level, std::string msg);

} // End of namespace util

/*! @} End of Doxygen Groups*/

// Useful alias
using LOGTYPE = util::Logger::LOG_LEVEL;

#endif // LOGGER_H