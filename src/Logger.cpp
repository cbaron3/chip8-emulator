#include <iostream>
#include "../include/Logger.h"

namespace util{
	Logger* Logger::logger_ = NULL;

	Logger* Logger::getInstance(){
		if(logger_ == NULL){
			logger_ = new Logger();
		}

		return logger_;
	}

	void Logger::log(LOG_LEVEL level, std::string msg) const {
		if(level >= static_cast<LOG_LEVEL>(max_debug)){
			switch (level) {
			    case LOG_LEVEL::DEBUG:
			      std::cout << "Debug Message: " << msg << std::endl;
			      break;
			    case LOG_LEVEL::ERROR:
			      std::cout << "***ERROR*** Message: " << msg << std::endl;
			      break;
			    case LOG_LEVEL::NONE:
			    default:
			      break;
			}
		}
	}

	void Logger::set_max_log_level(LOG_LEVEL level){
		max_debug = static_cast<int>(level);
	}

	void LOG(Logger::LOG_LEVEL level, std::string msg){
		Logger::getInstance()->log(level, msg);
	}
}