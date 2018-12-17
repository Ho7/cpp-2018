#pragma once

#include <cstdint>
#include <ostream>

class String {
public:
    String(const char*);
    String(std::size_t, char);
    virtual ~String();

    void append(const char*);
    void prepend(const char*);
    friend std::ostream& operator<<(std::ostream&, const String&);

private:
    std::size_t m_length;
    char* m_data;
};
