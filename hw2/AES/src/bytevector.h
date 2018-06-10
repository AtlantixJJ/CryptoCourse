#ifndef _BYTE_ARRAY_H
#define _BYTE_ARRAY_H

#include <string>

#include "utils.h"

class ByteVector
{
public:
    ByteVector();
    ByteVector(const ByteVector& array);
    ByteVector(ByteVector&& array);
    ByteVector(const char* str);
    ByteVector(int len, const uint8_t* array = nullptr);
    virtual ~ByteVector();

    bool operator==(const ByteVector& array) const;
    uint8_t& operator[](int i) { return m_a[i]; }
    const uint8_t& operator[](int i) const { return m_a[i]; }
    ByteVector& operator=(const ByteVector& array);
    ByteVector& operator=(ByteVector&& array);

    void clip(int size) {
        if (size < m_len) m_len = size;
    }
    int length() const { return m_len; }
    bool is_error() const { return m_len < 0; }
    ByteVector padding(int len, uint8_t value = 0) const;
    std::string to_plain_text() const;
    std::string to_hex_string() const;
    void save_to_file(const char* fileName) const;

    static ByteVector error_array();

    static ByteVector from_file(const char* fileName);
    static ByteVector from_hex_string(const char* str);
    
    static void copy(const ByteVector& a, int sa, const ByteVector& b, int sb, int len);

private:
    int m_len;
    uint8_t* m_a;
};

#endif // _BYTE_ARRAY_H
