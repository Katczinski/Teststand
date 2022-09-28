#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <sstream>

enum ChecksumType {
    ChecksumIso3309,
    ChecksumItuV41
};

static const uint16_t crc_tbl[16] = {
    0x0000, 0x1081, 0x2102, 0x3183,
    0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b,
    0xc60c, 0xd68d, 0xe70e, 0xf78f
};

class ByteArray
{
private:
    typedef char                                        Data;
    typedef std::allocator<Data>                        Allocator;
    typedef Data                                        value_type;
    typedef Allocator                                   allocator_type;
    typedef size_t                                      size_type;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;

    Data*                                               _data;
    size_t                                              _size;
    size_t                                              _capacity;
    Allocator                                           _alloc;
public:
    ByteArray(const Allocator& alloc = allocator_type());
    ByteArray(size_type size, const value_type& val = value_type(), const allocator_type& alloc = allocator_type());
    ByteArray(const char *data, int size = -1);
    ByteArray(const ByteArray& other);

    ~ByteArray();

    size_type       size() const;
    size_type       length() const;
    Data*           data() const;
    const Data*     constData() const;
    reference       at(size_t index);
    const_reference at(size_t index) const;
    const_reference operator[](size_t index) const;
    reference       operator[](size_t index);
    ByteArray&      operator=(const ByteArray& other);
    ByteArray&      setNum(unsigned long long n, int base = 10);
    ByteArray&      setNum(long long n, int base = 10);
    ByteArray&      setNum(double n);
    ByteArray&      setNum(short n, int base = 10);
    ByteArray&      setNum(unsigned short n, int base = 10);
    ByteArray&      setNum(int n, int base = 10);
    ByteArray&      setNum(unsigned int n, int base = 10);
    ByteArray&      setNum(float n, char f, int prec = 6);
    ByteArray&      append(const std::string& str);
    ByteArray&      append(const char *str);
    ByteArray&      append(const ByteArray& ba);
    ByteArray&      append(char ch);
    ByteArray&      append(const char *str, int len);
    ByteArray       chopped(int len) const;
    ByteArray       toHex() const;
    ByteArray       toHex(char separator) const;
    ByteArray       toUpper();
    ByteArray       left(size_t len)  const;
    ByteArray       right(size_t len) const;
    bool            isEmpty() const;
    void            resize(size_t size);
    void            clear();
    void            chop(int n);
    void            reserve(size_t asize);
    friend std::ostream&   operator<<(std::ostream& out, const ByteArray &ba);
private:
    void            reallocData(size_t newCapacity);
    char            toHexLower(unsigned int value) const noexcept;
    static char*    qulltoa2(char *p, unsigned long long n, int base);
};

bool operator==(const ByteArray &a1, const ByteArray &a2) noexcept;

#endif // BYTEARRAY_H
