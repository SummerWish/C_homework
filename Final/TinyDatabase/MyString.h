//
//  MyString.h
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__MyString__
#define ___352978__MyString__

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <vector>

class MyString
{

private:
    /*
     内部字符串，MyString是对该字符串的进一步封装
     */
    char* _str;

public:
    
    /*
     空构造函数
     
     eg:
     MyString s;
     */
    MyString();
    
    /*
     析构函数
     */
    ~MyString();
    
    /*
     从整数构造
     
     eg:
     MyString s(10);
     */
    MyString(int n);
    
    /*
     从浮点数构造
     
     eg:
     MyString s(0.0);
     */
    MyString(float n);
    
    /*
     从char*构造
     
     eg:
     MyString s("str");
     */
    MyString(const char* str);
    
    /*
     从char*构造以length为长度的字符串
     
     eg:
     MyString s("str", 1);
     */
    MyString(const char* str, int length);
    
    /*
     从MyString构造
     
     eg:
     MyString a;
     MyString b(a);
     */
    MyString(const MyString& str);
    
    /*
     从vector<char>构造
     */
    MyString(std::vector<char> v);
    
    /*
     eg:
     MyString a = "str";
     */
    MyString& operator=(const char *str);
    
    /*
     eg:
     MyString a, b;
     a = b;
     */
    MyString& operator=(const MyString& str);
    
    /*
     字符串连接
     
     eg:
     MyString a;
     a = a + 'h';
     */
    MyString& concat(char ch) const;
    
    /*
     字符串连接
     
     eg:
     MyString a;
     a = a + "hello";
     */
    MyString& concat(const char *str) const;
    
    /*
     字符串连接
     
     eg:
     MyString a, b, c;
     c = a + b;
     */
    MyString& concat(const MyString& str) const;
    
    /*
     字符串连接
     
     eg:
     MyString a;
     a = a + 'h';
     */
    MyString& operator+(char ch) const;
    
    /*
     字符串连接
     
     eg:
     MyString a;
     a = a + "hello";
     */
    MyString& operator+(const char *str) const;
    
    /*
     字符串连接
     
     eg:
     MyString a, b, c;
     c = a + b;
     */
    MyString& operator+(const MyString& str) const;
    
    /*
     允许通过[]方式访问或修改字符串的字符
     */
	char& operator[](int pos);
    char operator[](int pos) const;
	
    /*
     字符串比较
     */
    int operator<(const MyString& str) const;
    int operator<=(const MyString& str) const;
	int operator>(const MyString& str) const;
    int operator>=(const MyString& str) const;
	int operator==(const MyString& str) const;
    int operator!=(const MyString& str) const;
    int operator<(const char *str) const;
    int operator<=(const char *str) const;
	int operator>(const char *str) const;
    int operator>=(const char *str) const;
	int operator==(const char *str) const;
    int operator!=(const char *str) const;
    
    /*
     返回字符串长度
     */
    int length() const;
    
    /*
     返回字符串中从start开始长度为len的子串
     若len为-1，则返回字符串从start开始直到结束的子串
     */
    MyString& substring(int start, int len) const;
    /*
     返回字符串中从start开始长度为len的子串
     不检查边界, 不支持len<0
     */
    MyString& _substring(int start, int len) const;
    
    /*
     查找子串，若未找到返回-1
     */
    int indexOf(const char *search) const;
    int indexOf(const MyString search) const;
    
    /*
     分割字符串
     */
    std::vector<MyString>& split(const char *delim) const;
    
    /*
     转为大写
     */
    MyString& toUpper() const;
    
    /*
     转为小写
     */
    MyString& toLower() const;
    
    /*
     是否是数字
     */
    bool isDigit() const;
    
    /*
     转换为整数
     */
    int toInteger() const;
    
    /*
     转换为浮点数
     */
    float toFloat() const;
    
    /*
     转换为char*
     */
    char *toCString() const;
    
    /*
     重写 <<
     */
	friend std::ostream& operator<<(std::ostream& s, const MyString& str);
    
    /*
     重写 >>
     注：最大允许读取4096字节
     */
	friend std::istream& operator>>(std::istream& s, MyString& str);
    
};

int my_memcmp(const void *s1, const void *s2, int n);
char *my_strcat(char *dest, const char *src);
char *my_strcpy(char *dest, const char *src);
char *my_strchr(const char *s, char c);
int my_strlen(const char *str);
int my_strcmp(const char *p1, const char *p2);
char *my_strstr(const char *s1, const char *s2);

/*
 highly optimized string split
 */
std::vector<MyString>& my_split(const char *str, const char *delim);

#endif /* defined(___352978__MyString__) */
