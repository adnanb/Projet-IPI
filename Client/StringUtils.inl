inline NetstringBuilder::NetstringBuilder()
{

}

inline std::string NetstringBuilder::str()
{
    return m_outStream.str();
}

inline NetstringBuilder::operator std::string()
{
    return m_outStream.str();
}

template <typename T>
inline NetstringBuilder& NetstringBuilder::operator <<(const T& value)
{
    int size = std::string(value).length();
    size_t pos = m_outStream.str().find(':');
    m_outStream << size << ':' << value << ',';
    if(pos == std::string::npos)
        pos = -1;

    std::string temp = m_outStream.str().substr(pos+1, std::string::npos);
    size = temp.length();
    m_outStream.seekp(0);
    m_outStream << size << ':' << temp;

    return *this;
}

inline QVector<QString> StringExtract(char* message)
{
    QString in(message);
    QVector<QString> vect;

    while(in != "")
    {
        int pos = in.indexOf(":");
        int l = in.left(pos).toInt();
        in = in.mid(pos+1);
        vect.push_back(in.left(l));
        in = in.mid(l+1);
    }

    return vect;
}
