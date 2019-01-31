#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <array>

// Singleton logger class with varying levels of message types to be logged
namespace util{
	class Logger{
	public:
		enum class LOG_LEVEL{DEBUG, ERROR, NONE};
		static Logger* getInstance();
		void log(LOG_LEVEL level, std::string msg) const;
		void set_max_log_level(LOG_LEVEL level);
	protected:
	private:
		Logger(){}
		int max_debug = 0;
		static Logger* logger_;
	};

	void LOG(Logger::LOG_LEVEL level, std::string msg);
}

typedef util::Logger::LOG_LEVEL LOGTYPE;

#endif // LOGGER_H