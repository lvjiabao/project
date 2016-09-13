#include "BigData.h"

BigData::BigData(INT64 value):_value(value)
{
	char pData[20] = { 0 };
	if (_value > 0)
	{
		pData[0] = '+';
		sprintf(pData+1, "%lld", value);
		_pData = pData;
	}
	else
	{
		sprintf(pData, "%lld", value);
		_pData = pData;
	}
}


BigData::BigData(const char*pData)
{
	if (NULL == pData)
	{
		_value = UN_INIT;
		return;
	}
	char*pTemp = (char*)pData;
	char cSymbol = pData[0];
	if ('-'==cSymbol)
	{
		pTemp++;
	}
	else if ('+'==cSymbol)
	{
		pTemp++;
	}
	else
	{
		cSymbol = '+';
	}
	_pData.resize(strlen(pData)+1);
	int count = 1;
	_pData[0] = cSymbol;
	while ('0' == *pTemp)
		++pTemp;
	_value = 0;
	while (*pTemp >= '0'&&*pTemp <= '9')
	{
		_value = 10 * _value + *pTemp - '0';
		_pData[count++] = *pTemp;
		pTemp++;
	}
	if ('-' == _pData[0])
		_value = 0 - _value;
	_pData.resize(count);
}

bool BigData::IsINT64Overflow()
{
	char*StrMax = "+9223372036854775807";
	char*StrMin = "-9223372036854775808";
	int lenmax = strlen(StrMax);
	int lenpData = strlen(_pData.c_str());
	if (lenpData>lenmax)
		return true;
	else if (lenpData == lenmax)
	{
		if ((_pData[0] == '-'&&_pData > StrMin) || ((_pData[0] == '+'&&_pData > StrMax)))
			return true;
	}
	return false;
}

ostream&operator<<(ostream&_cout, BigData&num)
{
	if (!num.IsINT64Overflow())
	{
		cout << num._value;
	}
	else
	{
		if (num._pData[0] == '+')
		{
			char*pt = (char*)num._pData.c_str() + 1;
			cout << pt;
		}
		else
			cout << num._pData;
	}
	return _cout;
}

string BigData::Add(string left,string right)
{
	int lenl = left.size();
	int lenr = right.size();
	if (lenl < lenr)//默认left更长
	{
		swap(left, right);
		swap(lenl, lenr);
	}
	string ret;
	int step = 0;
	ret.resize(lenl + 1);
	ret[0] = left[0];
	for (int i = 0; i <lenl - 1; i++)////
	{
		if (i <lenr - 1)////
			ret[lenl - i] = left[lenl - 1 - i] + right[lenr - 1 - i] - '0' + step;
		else
			ret[lenl - i] = left[lenl - 1 - i] + step;
		step = 0;
		if (ret[lenl - i] > '9')
		{
			ret[lenl - i] -= 10;
			step = 1;
		}
	}
	ret[1] = step + '0';
	return ret;
}


BigData operator+(BigData&left, BigData&right)//函数代码太长，如何分配
{
	if (left._pData[0] != right._pData[0])        //不同号
	{
		if (!left.IsINT64Overflow() && !right.IsINT64Overflow())
			return BigData(left._value + right._value);//////
		else
		{
			BigData pl = left;
			BigData pr = right;
			if (pl._pData[0] == '-')
			{
				pl._pData[0] = '+';
				return pr - pl;
			}
			else
			{
				pr._pData[0] = '+';
				return pl - pr;
			}
		}
	}
	else
	{
		if (!left.IsINT64Overflow() && !right.IsINT64Overflow() && 
			((left._pData[0] == '+'&&left._value <= MAX_INT64 - right._value) ||
			(left._pData[0] == '-'&&left._value >= (INT64)MIN_INT64 - right._value)))
		{
				return BigData(left._value + right._value);//////
		}
	}
	return BigData(left.Add(left._pData, right._pData).c_str());//调用构造函数，消去符号位后面的零
}

string BigData::Sub(string left, string right)
{
	int lenl = left.size();
	int lenr = right.size();
	string ret;
	char cSymbol = '+';
	if (lenr > lenl||(lenr==lenl&&left<right))
	{
		if (left[0] == '-')
			cSymbol = '+';
		else
			cSymbol = '-';
		swap(left, right);
		swap(lenl, lenr);
	}
	else
	{
		if (left[0] == '-')
			cSymbol = '-';
	}
	ret.resize(lenl);
	ret[0] = cSymbol; 
	int borrow = 0;
	for (int i = 1; i < lenl; i++)
	{
		if (i<lenr)
			ret[lenl - i] = left[lenl - i] - right[lenr - i] + '0' + borrow;
		else
			ret[lenl - i] = left[lenl - i] + borrow;
		borrow = 0;
		if (ret[lenl - i] < '0')
		{
			borrow = -1;
			ret[lenl - i] += 10;
		}
	}
	return ret;
}


BigData operator-(BigData&left, BigData&right)
{
	if ((!left.IsINT64Overflow()&&!right.IsINT64Overflow()) &&
		((left._pData[0] == right._pData[0] ||
		(left._pData[0] != right._pData[0] && left._pData[0] == '+'&&MAX_INT64 + right._value >= left._value) ||
		(left._pData[0] != right._pData[0] && left._pData[0] == '-' && (INT64)MIN_INT64 + right._value <= left._value))))
		return BigData(left._value - right._value);
	else
	{
		if (left._pData[0] != right._pData[0])
		{
			BigData temp = right;
			temp._pData[0] = left._pData[0];
			return BigData(left + temp);
		}
	}
	return BigData(left.Sub(left._pData, right._pData).c_str());
}

string BigData::Mul(string left, string right)
{
	int lenl = left.size();
	int lenr = right.size();
	if (lenl > lenr)
	{
		swap(left, right);
		swap(lenl, lenr);
	}
	string ret;
	int lenret = lenl + lenr - 1;
	ret.resize(lenret);
	if (left[0] != right[0])
		ret[0] = '-';
	else
		ret[0] = '+';
	int step = 0, offset = 0, temp = 0;
	for (int i = 1; i < lenl; i++)
	{
		for (int j = 1; j < lenr; j++)
		{
			temp = (left[lenl - i] - '0')*(right[lenr - j] - '0') + step;
			ret[lenret - j - offset] += temp;
			step = 0;
			if (ret[lenret - j - offset]>9)
			{
				step = ret[lenret - j - offset] / 10;
				ret[lenret - j - offset] %= 10;
			}
		}		
		ret[lenret - offset-lenr] += step;
		offset++;
	}
	for (int i = 1; i < lenret; i++)
		ret[i] += '0';
	return ret;
}

BigData operator*(BigData&left, BigData&right)
{
	if ((!left.IsINT64Overflow() && left._value == 0) || (!right.IsINT64Overflow() && right._value == 0))
		return BigData((INT64)0);
	if ((!left.IsINT64Overflow() && !right.IsINT64Overflow())&&
		((left._pData[0] == right._pData[0] && ((left._pData[0] == '-'&&MAX_INT64 / left._value <= right._value) || (left._pData[0] == '-'&&MAX_INT64 / left._value <= right._value))) ||
		(left._pData[0] != right._pData[0] && (((INT64)MIN_INT64 / left._value <= right._value&&left._pData[0] == '+')) || ((INT64)MIN_INT64 / left._value >= right._value&&left._pData[0] == '-'))))
	{
		return BigData(left._value*right._value);
	}
	return BigData(left.Mul(left._pData, right._pData).c_str());
}


bool BigData::IsLeftBig(const char*pList, int lSize, const char*pRight, int rSize)//比较左值和右值的大小
{
	if (lSize < rSize || (lSize == rSize&&strncmp(pList, pRight, lSize) < 0))
		return false;
	return true;
}
char BigData::SubLoop(char*&pLeft, int&lSize,char*pRight, int rSize)//引用，
{
	char count = '0';
	int borrow = 0;
	while (IsLeftBig(pLeft, lSize, pRight, rSize))
	{
		for (int i = 1; i <= lSize; i++)
		{
			if (rSize>=i)
				pLeft[lSize - i] = pLeft[lSize - i] - pRight[rSize - i] + '0' + borrow;
			else
				pLeft[lSize - i] += borrow;
			borrow = 0;
			if (pLeft[lSize - i] < '0')
			{
				borrow = -1;
				pLeft[lSize - i] += 10;
			}
		}
		while (*pLeft == '0'&& lSize--)/////问题点，
		{
			pLeft++;
		}
		count++;
	}
	return count;
}

string BigData::Div(string left, string right)
{
	string ret;
	char cSymbol = '+';
	if (left[0] != right[0])
		cSymbol = '-';
	ret.append(1, cSymbol);
	int lenleftnum = left.size() - 1;
	int lenrigntnum = right.size() - 1;
	char*ptleft = (char*)left.c_str() + 1;
	char*ptright = (char*)right.c_str() + 1;
	int lenleftcompare = lenrigntnum;
	while ((int)strlen(ptleft) >= lenrigntnum)
	{
		if ((int)strlen(ptleft) == lenrigntnum&&strncmp(ptleft, ptright, lenrigntnum) < 0)
			break;
		char num = SubLoop(ptleft, lenleftcompare, ptright, lenrigntnum);
		ret.append(1, num);
		lenleftcompare++;////如论如何都是要加的
	}
	//俩数相除，商的位数是一定的，不同的是第一位是否为0
	while (lenleftnum - lenrigntnum >= (int)ret.size() - 1)
	{
		ret.append(1, '0');
	}
	return ret;
}


BigData operator/(BigData&left, BigData&right)
{
	//除数为0
	if (!right.IsINT64Overflow() && right._value == 0)
	{
		cout << "除数不能为0\n";
		exit(EXIT_FAILURE);
	}
	//被除数小于除数&&被除数为0
	if ((!left.IsINT64Overflow() && left._value == 0) || 
		((left._pData.size()<right._pData.size()) || 
		(left._pData.size() == right._pData.size() && strcmp(left._pData.c_str() + 1, left._pData.c_str() + 1)<0)))
	{
		return BigData((INT64)0);
	}
	//被除数大于除数&&俩都在范围内
	if (!left.IsINT64Overflow() && !right.IsINT64Overflow())
		return BigData(left._value / right._value);
	return BigData(left.Div(left._pData, right._pData).c_str());
}




