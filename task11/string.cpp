#include "string.hpp"
#include <cstring>

String::String(const char* string)
    : m_length { std::strlen(string) }
    , m_data { new char[m_length + 1] }
{
    std::strcpy(m_data, string);
}

String::String(std::size_t size, char character)
    : m_length { size }
    , m_data { new char[m_length + 1] }
{
    for (auto i = 0; i < m_length - 1; ++i)
        m_data[i] = character;
    m_data[m_length - 1] = '\0';
}

String::~String()
{
    if (m_data)
        delete[] m_data;
}

void String::append(const char* string)
{
    auto size = std::strlen(string);
    auto data = new char[m_length + size + 1];
    std::strcpy(data, m_data);
    std::strcpy(data + m_length, string);

    delete[] m_data;
    m_data = data;
    m_length += size + 1;
}

void String::prepend(const char* string)
{
    auto size = std::strlen(string);
    auto data = new char[m_length + size + 1];
    std::strcpy(data, string);
    std::strcpy(data + m_length, m_data);

    delete[] m_data;
    m_data = data;
    m_length += size + 1;
}

std::ostream& operator<<(std::ostream& outputStream, const String& string)
{
    outputStream << string.m_data;
    return outputStream;
}
