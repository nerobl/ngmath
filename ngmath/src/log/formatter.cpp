#include "ngmpch.h"
#include "formatter.h"

namespace ngmath {
	log_size_t formatter::_get_argument_index_from_digit_buffer()
	{
		log_size_t res = 0;
		log_size_t order = 1;
		log_size_t index = 0;
		bool gotFirst = false;
		while (index < NGM_FORMAT_ANUM_BUFFER_SIZE) {
			if (m_ArgDigitsBuffer[index] != nullDigit) {
				res += order * (log_size_t)(m_ArgDigitsBuffer[index] - '0');
				order *= 10;
			}
			++index;
		}

		return res;
	}
	log_size_t formatter::_get_argument_index(const char* mssg, log_size_t& index)
	{
		memset(&m_ArgDigitsBuffer[0], (int)nullDigit, NGM_FORMAT_ANUM_BUFFER_SIZE);
		log_size_t anumbuffindex = NGM_FORMAT_ANUM_BUFFER_SIZE - 1;
		log_size_t maxsize = strlen(mssg);
		while (index < maxsize) {
			if (mssg[index] == '}')
				return _get_argument_index_from_digit_buffer();
			else if (mssg[index] > '9' || mssg[index] < 0) {
				throw NGM_FORMATTER_ERROR;
				return 0;
			}

			m_ArgDigitsBuffer[anumbuffindex] = mssg[index];
			--anumbuffindex;
			++index;
		}

		throw NGM_FORMATTER_ERROR;
		return 0;
	}
	bool formatter::_check_format_overflow_err(log_size_t size)
	{
		if (m_FormatIndex + size >= NGM_FORMAT_MAP_SIZE) {
			throw NGM_FORMATTER_BUFFER_OVERFLOW;
			return true;
		}

		return false;
	}
	bool formatter::_check_log_overflow_err(log_size_t size)
	{
		if (m_LogIndex + size >= NGM_LOGGER_BUFFER_SIZE - 1) {
			throw NGM_LOGGER_BUFFER_OVERFLOW;
			return true;
		}

		return false;
	}
	void formatter::_make_formatter_log(const char* mssg)
	{
		log_size_t mssglen = strlen(mssg);
		const char* cur = mssg;
		log_size_t lastIndex = 0;
		log_size_t index = 0;
		log_size_t aindex = 0;
		log_size_t aorder = 1;
		log_size_t secsize = 0;
		bool getanum = false;
		while (index < mssglen) {
			if (mssg[index] == '{') {
				if (getanum) { throw NGM_FORMATTER_ERROR; return; }
				_copy_to_log_buffer(&mssg[lastIndex], index - lastIndex);
				getanum = true;
			}
			else if (mssg[index] == '}') {
				if (!getanum) { throw NGM_FORMATTER_ERROR; return; }
				_copy_formatter_arg_to_buffer(aindex);
				getanum = false;
				lastIndex = index + 1;
			}
			else if (getanum) {
				aindex = _get_argument_index(mssg, index);
				continue;
			}
			++index;
		}

		_copy_to_log_buffer(&mssg[lastIndex], mssglen - lastIndex);
		//memcpy(&m_FormatMap[0], &m_LogBuffer[m_LastIndex], m_LogIndex - m_LastIndex);
		//m_FormatMap[m_LogIndex - m_LastIndex + 1] = '\0';
	}
	void formatter::_copy_data_to_format_map(const void* data, log_size_t size)
	{
		memcpy(&m_FormatMap[m_FormatIndex], data, size);
		m_FormatIndex += size;
	}
	void formatter::_copy_formatter_arg_to_buffer(log_size_t index)
	{
		log_size_t size = 0;
		size_t _index = 0;
		const char* where = &m_FormatMap[0];
		while (_index <= index) {
			where += size;
			size = (log_size_t)(*where);
			if (size == 0) { 
				_copy_to_log_buffer(errorStr, sizeof(errorStr));
				return;
			}
			++_index;
		}

		_copy_to_log_buffer((where + sizeof(log_size_t)), size - sizeof(log_size_t));

	}
	void formatter::_copy_to_log_buffer(const char* mssg, log_size_t size)
	{
		if (m_LogIndex + size >= NGM_LOGGER_BUFFER_SIZE - 1) {
			throw NGM_LOGGER_BUFFER_OVERFLOW;
			return;
		}
		memcpy(&m_LogBuffer[m_LogIndex], mssg, size);
		m_LogIndex += size;
		m_LogBuffer[m_LogIndex] = '\0';
	}
	void formatter::_make_ready()
	{
		m_LastIndex = m_LogIndex;
		memset(&m_FormatMap[0], 0, NGM_FORMAT_MAP_SIZE);
		m_FormatIndex = 0;
	}

	void formatter::_reset_temp_buffer()
	{
		memset(&m_TempBuffer[0], 0, NGM_FORMAT_TEMP_BUFFER_SIZE);
	}

	bool formatter::_copy_arg_to_format_map()
	{
		log_size_t len = strlen(&m_TempBuffer[0]);
		log_size_t size = len + sizeof(log_size_t);
		if (_check_format_overflow_err(size))
			return false;
		_copy_data_to_format_map(&size, sizeof(log_size_t));
		_copy_data_to_format_map(&m_TempBuffer[0], len);
		return true;
	}
	formatter::formatter(char* buffer, log_size_t& index) : m_LogBuffer(buffer), m_LogIndex(index){
		memset(&m_FormatMap[0], 0, NGM_FORMAT_MAP_SIZE);
		memset(&m_ArgDigitsBuffer[0], (int)nullDigit, NGM_FORMAT_ANUM_BUFFER_SIZE);
	}
}
