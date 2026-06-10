#include "ngmpch.h"
#include "logger.h"

namespace ngmath {

    logger* logger::s_Instance = nullptr;

    logger::logger()
    {
        memset(&m_Buffer[0], 0, NGM_LOGGER_BUFFER_SIZE);
    }

    logger& logger::operator<<(const char* str)
    {
        size_t size = strlen(str);
        if (m_Index + size >= NGM_LOGGER_BUFFER_SIZE) {
            throw NGM_LOGGER_BUFFER_OVERFLOW;
            return *this;
        }
        memcpy(&m_Buffer[m_Index], str, size);
        m_Index += size;
        m_Buffer[m_Index] = '\0';
        return *this;
    }

    void logger::print() const
    {
        printf(&m_Buffer[0]);
    }
    void logger::clear()
    {
        memset(&m_Buffer[0], 0, NGM_LOGGER_BUFFER_SIZE);
        m_Index = 0;
    }
    void logger::out(const char* str)
    {
        operator<<(str);
        operator<<("\n");
        print();
        clear();
    }
    void log_debug(const char* mssg)
    {
        if (!logger::IsInitiated) {
            throw NGM_UNINITATED_LOGGER;
            return;
        }

        logger::GetInstance()->out(mssg);

    }
}