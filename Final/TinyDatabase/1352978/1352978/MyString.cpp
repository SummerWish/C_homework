//
//  MyString.cpp
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "MyString.h"

/*
 将src追加到dest末尾
 返回追加后的dest字符串
 */
char *my_strcat(char *dest, const char *src)
{
    char *ret = dest;
    
    // move to end
    while (*dest != '\0') {
        dest++;
    }
    
    // copy
    while ((*dest++ = *src++) != '\0') /* empty */;
    
    return ret;
}

/*
 将src复制到dest
 返回复制后的dest字符串
 */
char *my_strcpy(char *dest, const char *src)
{
    char *ret = dest;
    
    while ((*dest++ = *src++) != '\0') /* empty */;
    
    return ret;
}

/*
 查找字符c在字符串s中位置，并返回其地址
 */
char *my_strchr(const char *s, char c)
{
    while (*s != c) {
        if (*s++ == '\0') {
            return 0;
        }
    }
    
    return (char*)s;
}

/*
 计算并返回str长度
 */
int my_strlen(const char *str)
{
    int len = 0;
    
    while (*str != '\0') {
        str++;
        len++;
    }
    
    return len;
}

/*
 比较字符串s1, s2
 返回比较结果
 */
int my_strcmp(const char *p1, const char *p2)
{
    unsigned char *pc1 = (unsigned char *)p1;
    unsigned char *pc2 = (unsigned char *)p2;
    unsigned char c1, c2;
    
    do {
        c1 = *pc1++;
        c2 = *pc2++;
    } while (c1 == c2 && c1 != '\0');
    
    return (c1 - c2);
}

/*
 查找并返回s2字符串在s1的起始地址
 */
char *my_strstr(const char *s1, const char *s2)
{
    int n = my_strlen(s2);
    
    while (*s1 != '\0') {
        if (!memcmp(s1++, s2, n)) {
            return (char *)s1 - 1;
        }
    }
    
    return 0;
}

/*
 空构造函数
 
 eg:
 MyString s;
 */
MyString::MyString()
{
    _str = new char[1];
    *_str = '\0';
}

/*
 析构函数
 */
MyString::~MyString()
{
    delete[] _str;
}

/*
 从整数构造
 
 eg:
 MyString s(10);
 */
MyString::MyString(int n)
{
    char tmp[12];
    sprintf(tmp, "%d", n);
    
    _str = new char[my_strlen(tmp) + 1];
    my_strcpy(_str, tmp);
}

/*
 从浮点数构造
 
 eg:
 MyString s(0.0);
 */
MyString::MyString(float n)
{
    char tmp[12];
    sprintf(tmp, "%12f", n);
    
    _str = new char[my_strlen(tmp) + 1];
    my_strcpy(_str, tmp);
}

/*
 从char*构造
 
 eg:
 MyString s("str");
 */
MyString::MyString(const char *str)
{
    _str = new char[my_strlen(str) + 1];
    my_strcpy(_str, str);
}

/*
 从MyString构造
 
 eg:
 MyString a;
 MyString b(a);
 */
MyString::MyString(const MyString& str)
{
    _str = new char[str.length() + 1];
    my_strcpy(_str, str._str);
}

/*
 从vector<char>构造
 */
MyString::MyString(std::vector<char> v)
{
    int len = (int)v.size();
    _str = new char[len + 1];
    
    for (int i = 0; i < len; i++) {
        _str[i] = v[i];
    }
    _str[len] = '\0';
}

/*
 eg:
 MyString a = "str";
 */
MyString& MyString::operator=(const char *str)
{
    delete[] _str;
    
    _str = new char[my_strlen(str) + 1];
    my_strcpy(_str, str);
    
    return *this;
}

/*
 eg:
 MyString a, b;
 a = b;
 */
MyString& MyString::operator=(const MyString& str)
{
    // 忽略自己对自己赋值
    if (&str == this) {
		return *this;
	}
    
    delete[] _str;
    
    _str = new char[str.length() + 1];
    my_strcpy(_str, str._str);
    
    return *this;
}

/*
 字符串连接
 
 eg:
 MyString a;
 a = a + 'h';
 */
MyString& MyString::concat(char ch) const
{
    MyString *newstr = new MyString();
    
    int len = length();
    
    delete[] newstr->_str;
    newstr->_str = new char[len + 2];
    
    my_strcpy(newstr->_str, _str);
    newstr->_str[len] = ch;
    newstr->_str[len + 1] = '\0';
    
    return *newstr;
}

/*
 字符串连接
 
 eg:
 MyString a;
 a = a + "hello";
 */
MyString& MyString::concat(const char *str) const
{
    MyString *newstr = new MyString();
    
    delete[] newstr->_str;
    newstr->_str = new char[length() + my_strlen(str) + 1];
    
    my_strcpy(newstr->_str, _str);
    my_strcat(newstr->_str, str);
    
    return *newstr;
}

/*
 字符串连接
 
 eg:
 MyString a, b, c;
 c = a + b;
 */
MyString& MyString::concat(const MyString& str) const
{
    MyString *newstr = new MyString();
    
    delete[] newstr->_str;
    newstr->_str = new char[length() + str.length() + 1];
    
    my_strcpy(newstr->_str, _str);
    my_strcat(newstr->_str, str._str);
    
    return *newstr;
}


/*
 字符串连接
 
 eg:
 MyString a;
 a = a + 'h';
 */
MyString& MyString::operator+(char ch) const
{
    return concat(ch);
}

/*
 字符串连接
 
 eg:
 MyString a;
 a = a + "hello";
 */
MyString& MyString::operator+(const char *str) const
{
    return concat(str);
}

/*
 字符串连接
 
 eg:
 MyString a, b, c;
 c = a + b;
 */
MyString& MyString::operator+(const MyString& str) const
{
    return concat(str);
}

/*
 允许通过[]方式访问或修改字符串的字符
 */
char& MyString::operator[](int pos)
{
    // 超出索引长度
    if (pos >= length()) {
        return _str[length()];
    }
    
    // 负索引
    if (pos < 0) {
        return _str[0];
    }
    
    return _str[pos];
}
char MyString::operator[](int pos) const
{
    // 超出索引长度
    if (pos >= length()) {
        return _str[length()];
    }
    
    // 负索引
    if (pos < 0) {
        return _str[0];
    }
    
    return _str[pos];
}

/*
 字符串比较
 */
int MyString::operator<(const MyString& str) const
{
    return (my_strcmp(_str, str._str) < 0);
}
int MyString::operator<=(const MyString& str) const
{
    return (my_strcmp(_str, str._str) <= 0);
}
int MyString::operator>(const MyString& str) const
{
    return (my_strcmp(_str, str._str) > 0);
}
int MyString::operator>=(const MyString& str) const
{
    return (my_strcmp(_str, str._str) >= 0);
}
int MyString::operator==(const MyString& str) const
{
    return (my_strcmp(_str, str._str) == 0);
}
int MyString::operator!=(const MyString& str) const
{
    return (my_strcmp(_str, str._str) != 0);
}
int MyString::operator<(const char *str) const
{
    return (my_strcmp(_str, str) < 0);
}
int MyString::operator<=(const char *str) const
{
    return (my_strcmp(_str, str) <= 0);
}
int MyString::operator>(const char *str) const
{
    return (my_strcmp(_str, str) > 0);
}
int MyString::operator>=(const char *str) const
{
    return (my_strcmp(_str, str) >= 0);
}
int MyString::operator==(const char *str) const
{
    return (my_strcmp(_str, str) == 0);
}
int MyString::operator!=(const char *str) const
{
    return (my_strcmp(_str, str) != 0);
}

/*
 返回字符串长度
 */
int MyString::length() const
{
    return my_strlen(_str);
}

/*
 返回字符串中从start开始长度为len的子串
 若len为-1，则返回字符串从start开始直到结束的子串
 */
MyString& MyString::substring(int start, int len) const
{
    int max_len = length();
    
    // 无效参数
    if (start < 0 || len < -1) {
        return *new MyString(_str);
    }
    
    MyString *newstr = new MyString();
    
    // 返回空字符串
    if (len == 0 || start >= max_len) {
        return *newstr;
    }
    
    // 需要截取到结束
    if (len == -1 || start + len > max_len) {
        len = max_len - start;
    }
    
    // 截取子串
    delete[] newstr->_str;
    newstr->_str = new char[len + 1];
    newstr->_str[len] = '\0';
    
    for (int i = 0; i < len; ++i) {
        newstr->_str[i] = _str[start + i];
    }
    
    return *newstr;
}

/*
 查找子串，若未找到则返回-1
 */
int MyString::indexOf(const char *search) const
{
    char *pos = my_strstr(_str, search);
    
    if (pos == 0) {
        return -1;
    } else {
        return (int)(pos - _str);
    }
}

int MyString::indexOf(const MyString search) const
{
    return indexOf(search._str);
}

/*
 分割字符串
 */
std::vector<MyString>& MyString::split(const char *delim) const
{
    MyString clone(_str);
    std::vector<MyString> *ret = new std::vector<MyString>;
    int pos;
    
    while ((pos = clone.indexOf(delim)) != -1) {
        ret->push_back(clone.substring(0, pos));
        clone = clone.substring(pos + 1, -1);
    }
    
    ret->push_back(clone);
    
    return *ret;
}

/*
 是否是数字
 */
bool MyString::isDigit() const
{
    return isdigit(_str[0]);
}

/*
 转换为整数
 */
int MyString::toInteger() const
{
    return atoi(_str);
}

/*
 转换为浮点数
 */
float MyString::toFloat() const
{
    return atof(_str);
}

/*
 转为大写
 */
MyString& MyString::toUpper() const
{
    MyString *newstr = new MyString(_str);
    
    char *s = newstr->_str;
    
    for (; *s; s++) {
        if (('a' <= *s) && (*s <= 'z'))
            *s = 'A' + (*s - 'a');
    }
    
    return *newstr;
}

/*
 转为小写
 */
MyString& MyString::toLower() const
{
    MyString *newstr = new MyString(_str);
    
    char *s = newstr->_str;
    
    for (; *s; s++) {
        if (('A' <= *s) && (*s <= 'Z'))
            *s = 'a' + (*s - 'A');
    }
    
    return *newstr;
}

/*
 转换为char*
 */
char *MyString::toCString() const
{
    char *_s = new char[length() + 1];
    my_strcpy(_s, _str);
    
    return _s;
}

/*
 重写 <<
 */
std::ostream& operator<<(std::ostream& s, const MyString& str)
{
    return s << str._str;
}

/*
 重写 >>
 注：最大允许读取4096字节
 */
std::istream& operator>>(std::istream& s, MyString& str)
{
    char tmp[4096];
    s >> tmp;
    
    // 截取有效长度并复制
    delete[] str._str;
    str._str = new char[my_strlen(tmp) + 1];
    my_strcpy(str._str, tmp);
    
    return s;
}

