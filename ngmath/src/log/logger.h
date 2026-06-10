#pragma once
#include "../core.h"
namespace ngmath {

	class logger : public ngmobj {
	private:
		static logger* s_Instance;
		size_t m_Index = 0;
		char m_Buffer[NGM_LOGGER_BUFFER_SIZE];
		static void _set_instance(logger* instance) { s_Instance = instance; }
	public:
		logger();
		~logger() {}

		logger& operator<<(const char* str);
		void print() const;
		void clear();
		void out(const char* str);

		static logger* GetInstance() { return s_Instance; }
		static bool IsInitiated() { return s_Instance; }
		friend class memory;
	};

	void log_debug(const char* mssg);



}