#include <cstring>

#include "bytevector.h"
#include "utils.h"

ByteVector::ByteVector()
    : m_len(0), m_a(nullptr)
{
}

ByteVector::ByteVector(const char* str)
    : m_len(strlen(str)), m_a(new uint8_t[m_len])
{
    memcpy(m_a, str, m_len * sizeof(uint8_t));
}

ByteVector::ByteVector(int len, const uint8_t* array)
    : m_len(len), m_a(new uint8_t[len])
{
    if (array)
        memcpy(m_a, array, m_len * sizeof(uint8_t));
    else
        memset(m_a, 0, m_len * sizeof(uint8_t));
}

ByteVector::ByteVector(const ByteVector& array)
    : m_len(array.m_len), m_a(new uint8_t[m_len])
{
    memcpy(m_a, array.m_a, m_len * sizeof(uint8_t));
}

ByteVector::ByteVector(ByteVector&& array)
    : m_len(array.m_len), m_a(array.m_a)
{
    array.m_a = nullptr;
}

ByteVector::~ByteVector()
{
    if (m_a)
        delete[] m_a;
}

ByteVector& ByteVector::operator=(const ByteVector& array)
{
    m_len = array.m_len;
    memcpy(m_a, array.m_a, m_len * sizeof(uint8_t));
    return *this;
}

ByteVector& ByteVector::operator=(ByteVector&& array)
{
    if (this != &array)
    {
        delete[] m_a;

        m_len = array.m_len;
        m_a = array.m_a;
        array.m_a = nullptr;
    }
    return *this;
}

ByteVector ByteVector::padding(int len, uint8_t value) const
{
    if (m_len >= len)
        return *this;
    ByteVector array(len);
    memcpy(array.m_a, m_a, m_len * sizeof(uint8_t));
    memset(array.m_a + m_len, value, (len - m_len) * sizeof(uint8_t));
    return array;
}

std::string ByteVector::to_plain_text() const
{
    std::string str = "";
    for (int i = 0; i < m_len; i++)
        str += char(m_a[i]);
    return str;
}

std::string ByteVector::to_hex_string() const
{
    std::string str = "";
    for (int i = 0; i < m_len; i++)
        str += Utils::byte2hex(m_a[i]);
    return str;
}

void ByteVector::save_to_file(const char* fileName) const
{
    FILE* file = fopen(fileName, "wb");

    const int BUF_SIZE = 1 << 20;
    for (int i = 0; i < m_len; i += BUF_SIZE)
        fwrite(m_a + i, sizeof(uint8_t), std::min(BUF_SIZE, m_len - i), file);

    fclose(file);
}

bool ByteVector::operator==(const ByteVector& array) const
{
    if (m_len != array.m_len)
        return false;
    for (int i = 0; i < m_len; i++)
        if (m_a[i] != array[i])
            return false;
    return true;
}

ByteVector ByteVector::error_array()
{
    ByteVector array;
    array.m_len = -1;
    return array;
}

ByteVector ByteVector::from_file(const char* fileName)
{
    FILE* file = fopen(fileName, "rb");
    if (file == NULL)
        return ByteVector();

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const int BUF_SIZE = 1 << 20;
    ByteVector array(size);
    for (int i = 0; i < size; i += BUF_SIZE)
        fread(array.m_a + i, sizeof(uint8_t), BUF_SIZE, file);
    fclose(file);

    return array;
}

ByteVector ByteVector::from_hex_string(const char* str)
{
    int len = strlen(str) / 2;
    ByteVector array(len);
    for (int i = 0; i < len; i++)
        array[i] = Utils::hex2byte(str[i * 2], str[i * 2 + 1]);
    return array;
}

void ByteVector::copy(const ByteVector& a, int sa, const ByteVector& b, int sb, int len)
{
    memcpy(a.m_a + sa, b.m_a + sb, len * sizeof(uint8_t));
}
