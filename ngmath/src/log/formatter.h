#pragma once
#include "../core.h"
#include "../numerical/numerical_defs.h"
namespace ngmath {

	template<typename T> struct type_formatter;

	template<signed_integral_data_type T> struct signed_integral_type_formatter {
		static constexpr const char* str = "%i";
	};
	template<unsigned_integral_data_type T> struct unsigned_integral_type_formatter {
		static constexpr const char* str = "%u";
	};
	template<floating_point_data_type T> struct floating_point_type_formatter {
		static constexpr const char* str = "%f";
	};

	template<size_t_data_type T> struct size_t_type_formatter {
		static constexpr const char* str = "%zu";
	};

	class formatter : public ngmobj {
	private:
		static constexpr const char* errorStr = "{ERR}";
		static constexpr char nullDigit = '#';
		static constexpr char argBegin = '{';
		static constexpr char argEnd = '}';
		char m_ArgDigitsBuffer[NGM_FORMAT_ANUM_BUFFER_SIZE];
		char m_TempBuffer[NGM_FORMAT_TEMP_BUFFER_SIZE];
		char m_FormatMap[NGM_FORMAT_MAP_SIZE];
		log_size_t m_FormatIndex = 0;
		char* m_LogBuffer = nullptr;
		log_size_t m_LastIndex = 0;
		log_size_t& m_LogIndex;

		log_size_t _get_argument_index_from_digit_buffer();
		log_size_t _get_argument_index(const char* mssg, log_size_t& index);
		bool _check_format_overflow_err(log_size_t size);
		bool _check_log_overflow_err(log_size_t size);
		void _make_formatter_log(const char* mssg);
		void _copy_data_to_format_map(const void* data, log_size_t size);
		void _copy_formatter_arg_to_buffer(log_size_t index);
		void _copy_to_log_buffer(const char* mssg, log_size_t size);
		void _make_ready();
		void _reset_temp_buffer();
		bool _copy_arg_to_format_map();
		template<typename  T> inline bool _map_arg(const T& arg) { 
			_reset_temp_buffer();
			if constexpr (is_size_t_data_type_v<T>)
				snprintf(&m_TempBuffer[0], NGM_FORMAT_TEMP_BUFFER_SIZE, size_t_type_formatter<T>::str, arg);
			else if constexpr (is_signed_integral_data_type_v<T>)
				snprintf(&m_TempBuffer[0], NGM_FORMAT_TEMP_BUFFER_SIZE, signed_integral_type_formatter<T>::str, arg);
			else if constexpr (is_unsigned_integral_data_type_v<T>)
				snprintf(&m_TempBuffer[0], NGM_FORMAT_TEMP_BUFFER_SIZE, unsigned_integral_type_formatter<T>::str, arg);
			else if constexpr (is_floating_point_data_type_v<T>)
				snprintf(&m_TempBuffer[0], NGM_FORMAT_TEMP_BUFFER_SIZE, floating_point_type_formatter<T>::str, arg);
			else
				type_formatter<T>::fmt(&m_TempBuffer[0], arg); //snprintf(&m_TempBuffer[0], NGM_FORMAT_TEMP_BUFFER_SIZE, type_formatter<T>::str);
			return _copy_arg_to_format_map();
		}
	public:
		formatter(char* buffer, log_size_t& index);
		template<typename ... T>
		void format(const char* mssg, const T& ... args) {
			_make_ready();
			bool err = (!_map_arg(args) || ...);
			if (err)
				return;
			_make_formatter_log(mssg);
		}
	};

	
}