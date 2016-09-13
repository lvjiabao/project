#ifndef __BIGDATA_H__
#define __BIGDATA_H__
#include<string>
#include<iostream>
#include<stdio.h>
using namespace std;
typedef long long INT64;
#define UN_INIT 0xcccccccccccccccc
#define MAX_INT64 0x7fffffffffffffff
#define MIN_INT64 0x8000000000000000


//RSA加密算法
//素数*素数=秘钥
class BigData
{
public:
	BigData(INT64 value);
	BigData(const char*pData="");
	friend ostream&operator<<(ostream&os, BigData&num);
	friend BigData operator+(BigData&left, BigData&right);
	friend BigData operator-(BigData&left, BigData&right);
	friend BigData operator*(BigData&left, BigData&right);
	friend BigData operator/(BigData&left, BigData&right);

	

	string Add(string left, string right);
	string Sub(string left, string right);
	string Mul(string left, string right);
	string Div(string left, string right);
private:
	bool IsLeftBig(const char*pList, int lSize, const char*pRight, int rSize);
	char SubLoop(char*&pLeft, int&lSize,char*pRight, int rSize);
	bool IsINT64Overflow();
private:
	INT64 _value;
	string _pData;
};
#endif