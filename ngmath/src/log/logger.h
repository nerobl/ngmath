#pragma once
#include "formatter.h"
namespace ngmath {

	class logger : public ngmobj {
	private:
		static logger* s_Instance;
		formatter m_Formatter;
		log_size_t m_Index = 0;
		//The last byte of the buffer is always '/0'
		char m_Buffer[NGM_LOGGER_BUFFER_SIZE];
		static void _set_instance(logger* instance) { s_Instance = instance; }

		logger& operator=(logger&& other) noexcept;
		logger(logger&& other) noexcept;
	public:
		logger();
		~logger() {}

		logger& operator<<(const char* str);
		logger& operator<<(char c);

		template<typename ... T>
		void push(const char* mssg, const T& ... args) {
			log_size_t prevIndex = m_Index;
			m_Formatter.format(mssg, args...);
		}
		void print() const;
		void clear();
		void out(const char* str);

		static logger* GetInstance() { return s_Instance; }
		static bool IsInitiated() { return s_Instance; }
		friend struct obj_id<logger>;
		friend class memory;
	};

	void log_debug(const char* mssg);
	template<typename ... T> void log_debug(const char* mssg, const T& ... args) {

		logger* log = logger::GetInstance();
		if (!log) {
			throw NGM_UNINITATED_LOGGER;
			return;
		}

		log->push(mssg, args...);
		log->operator<<('\n');
		log->print();
		log->clear();


	}
	inline void log_error(const char* mssg) { log_debug(mssg); }


	void run_logger_test();


}