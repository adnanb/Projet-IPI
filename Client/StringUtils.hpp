#pragma once

#include <sstream>

class NetstringBuilder
{
public:
    NetstringBuilder();

    inline std::string str();

    template <typename T>
    inline NetstringBuilder& operator <<(const T& value);

    operator std::string();

private:
    std::ostringstream m_outStream;
};

inline QVector<QString> StringExtract(char *message);

#include "StringUtils.inl"
