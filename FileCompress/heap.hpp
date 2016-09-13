#include<iostream>
#include<assert.h>
#include<vector>
using namespace std;

template<typename T>
class Less
{
public:
	bool operator()(const T&left, const T&right)
	{
		return left < right;
	}
};

template<typename T>
class Greater
{
public:
	bool operator()(const T&left, const T&right)
	{
		return left > right;
	}
};

template<typename T, class Compare = Less<T> >
class heap
{
public:
	heap()
	{}
	heap(T*arr, size_t size)
	{
		for (size_t i = 0; i < size; i++)
			s.push_back(arr[i]);
		size_t root = size / 2 - 1;//sizeÎª´óÐ¡
		for (int i = root; i >= 0; --i)
			_AdjustDown(i);
	}

	void Insert(const T&data)
	{
		s.push_back(data);
		size_t child = s.size() - 1;
		_AdjustUp(child);
	}

	void Remove()
	{
		assert(s.size());
		s[0] = s[s.size() - 1];
		s.pop_back();
		if (s.size() > 1)
			_AdjustDown(0);
	}

	size_t Size()
	{
		return s.size();
	}

	T Top()
	{
		return s.front();
	}

private:
	void _AdjustUp(size_t child)
	{
		if (child < 1)
			return;
		size_t parent = (child - 1) / 2;
		while (child != 0)
		{
			if (Compare()(s[child], s[parent]))
			{
				swap(s[parent], s[child]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
	void _AdjustDown(size_t root)
	{
		size_t size = s.size();
		size_t child = 2 * root + 1;
		while (child < size)
		{
			if (child + 1 < size&&Compare()(s[child + 1], s[child]))
			{
				child = child + 1;
			}
			if (!Compare()(s[root], s[child]))
			{
				swap(s[root], s[child]);
				root = child;
				child = 2 * root + 1;
			}
			else
				break;
		}
	}

private:
	vector<T>s;
};


