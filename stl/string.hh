#ifndef MINISTL_STRING_HH
#define MINISTL_STRING_HH

class String
{
public:
    String(const char *cstr = 0);
    String(const String &str);
    String(String &&str);
    String &operator=(const String &str);
    String &operator=(String &&str);
    ~String();
    char *get_c_str() const { return m_data; }

private:
    char *m_data;
};

#include <cstring>
#include <iostream>
#include "utils.hh"

inline String::String(const char *cstr)
{
    debug("String: default constructor");
    if (cstr)
    {
        m_data = new char[strlen(cstr) + 1];
        strcpy(m_data, cstr);
    }
    else
    {
        m_data = new char[1];
        *m_data = '\0';
    }
}

inline String::~String()
{
    debug("String:  destructor");
    delete[] m_data;
}

inline String &String::operator=(const String &str)
{
    debug("String:  = const String &");
    if (this == &str)
        return *this;

    delete[] m_data;
    m_data = new char[strlen(str.m_data) + 1];
    strcpy(m_data, str.m_data);
    return *this;
}

inline String &String::operator=(String &&str)
{
    debug("String:  = String &&");
    if (this == &str)
        return *this;

    m_data = str.m_data;
    str.m_data = nullptr;
    return *this;
}

inline String::String(const String &str)
{
    debug("String: copy constructor");
    m_data = new char[strlen(str.m_data) + 1];
    strcpy(m_data, str.m_data);
}

inline String::String(String &&str)
{
    debug("String: move constructor");

    m_data = str.m_data;
    str.m_data = nullptr;
}

#include <iostream>
using namespace std;

ostream &operator<<(ostream &os, const String &str)
{
    os << str.get_c_str();
    return os;
}

#endif