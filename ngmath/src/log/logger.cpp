#include "ngmpch.h"
#include "logger.h"

namespace ngmath {

    logger* logger::s_Instance = nullptr;

    logger& logger::operator=(logger&& other) noexcept
    {
        if (this != &other) {
            m_Index = other.m_Index;
            memcpy(&m_Buffer[0], &other.m_Buffer[0], NGM_LOGGER_BUFFER_SIZE);
            s_Instance = this;
        }
        return *this;
    }

    logger::logger(logger&& other) noexcept : m_Formatter(&m_Buffer[0], m_Index)
    {
        m_Index = other.m_Index;
        memcpy(&m_Buffer[0], &other.m_Buffer[0], NGM_LOGGER_BUFFER_SIZE);
        s_Instance = this;
    }

    logger::logger() : m_Formatter(&m_Buffer[0], m_Index)
    {
        memset(&m_Buffer[0], 0, NGM_LOGGER_BUFFER_SIZE);
    }

    logger& logger::operator<<(const char* str)
    {
        size_t size = strlen(str);
        if (m_Index + size >= NGM_LOGGER_BUFFER_SIZE - 1) {
            throw NGM_LOGGER_BUFFER_OVERFLOW;
            return *this;
        }
        memcpy(&m_Buffer[m_Index], str, size);
        m_Index += size;
        m_Buffer[m_Index] = '\0';
        return *this;
    }

    logger& logger::operator<<(char c)
    {
        if (m_Index + 1 >= NGM_LOGGER_BUFFER_SIZE - 1) {
            throw NGM_LOGGER_BUFFER_OVERFLOW;
            return *this;
        }
        m_Buffer[m_Index] = c;
        ++m_Index;
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
        if (!logger::IsInitiated()) {
            throw NGM_UNINITATED_LOGGER;
            return;
        }

        logger::GetInstance()->out(mssg);

    }

    void run_logger_test()
    {
        int val = 20;
        double second = 2.1;
        log_debug("Testing formatting: '{0}', second: {1}, first again: {0}", val, second);
        log_debug("Repeat: Testing formatting: '{0}', second: {1}, first again: {0}", val, second);
        log_debug("With index error: Testing formatting: '{0}', second: {2}, first again: {0}", val, second);
    }
}