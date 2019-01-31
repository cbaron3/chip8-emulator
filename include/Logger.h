#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <array>

// Singleton logger class with varying levels of message types to be logged
namespace util{

	const int kmax_debug = 0;

	class Logger{
	public:
		enum class LOG_LEVEL{DEBUG, ERROR};
		static Logger* getInstance();
		void log(LOG_LEVEL level, std::string msg) const;
	protected:
	private:
		Logger(){}
		static Logger* logger_;
	};

	void LOG(Logger::LOG_LEVEL level, std::string msg);
}

typedef util::Logger::LOG_LEVEL LOGTYPE;

#endif // LOGGER_H