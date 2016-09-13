#pragma once
#include<iostream>
#include<assert.h>
#include "heap.hpp"
#include<vector>
using namespace std;





template<typename T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>*_pleft;
	HuffmanTreeNode<T>*_pright;
	HuffmanTreeNode<T>*_parent;
	HuffmanTreeNode(const T&weight)
		:_weight(weight)
		, _pleft(NULL)
		, _pright(NULL)
		, _parent(NULL)
	{}
};


//比较器
template<typename T>
class Compare_less
{
public:
	bool operator()(HuffmanTreeNode<T>*left, HuffmanTreeNode<T>*right)
	{
		return left->_weight < right->_weight;
	}
};

template<typename T>
class Compare_more
{
public:
	bool operator()(HuffmanTreeNode<T>*left, HuffmanTreeNode<T>*right)
	{
		return left->_weight > right->_weight;
	}
};

template<typename T>
class HuffmanTree
{
public:
	HuffmanTree() :_root(NULL){}
	HuffmanTree(const T*weight, int size,T invaild)
	{
		CreateTree(weight, size,invaild);
	}

	//水平打印
	void PrintLevel()
	{
		_PrintLevel(_root);
	}
	//释放
	~HuffmanTree()
	{
		Destroy(_root);
	}
	HuffmanTreeNode<T>*GetTop()
	{
		return _root;
	}
private:
	void CreateTree(const T*weight, int size,T invaild)
	{
		assert(weight);
		if (size == 0)
			return;
		if (size == 1&&weight[0]!=invaild)
		{
			_root = new HuffmanTreeNode<T>(weight[0]);
			return;
		}
		heap<HuffmanTreeNode<T>*, Compare_less<T> >coll;
		for (int i = 0; i < size; ++i)
		if (weight[i]!=invaild)
			coll.Insert(new HuffmanTreeNode<T>(weight[i]));
		HuffmanTreeNode<T>*parent = NULL;
		while (coll.Size()>1)//注意size要大于1，因为要取两个元素
		{
			HuffmanTreeNode<T>*left = coll.Top();
			coll.Remove();
			HuffmanTreeNode<T>*right = coll.Top();
			coll.Remove();
			parent = new HuffmanTreeNode<T>(left->_weight + right->_weight);
			parent->_pleft = left;
			parent->_pright = right;
			left->_parent = parent;
			right->_parent = parent;
			coll.Insert(parent);
		}
		_root = parent;
	}

	void _PrintLevel(HuffmanTreeNode<T>*root)
	{
		if (NULL == root)
			return;
		queue<HuffmanTreeNode<T>*>coll;
		coll.push(root);
		while (!coll.empty())
		{
			HuffmanTreeNode<T>*tmp = coll.front();
			cout << tmp->_weight << " ";
			if (tmp->_pleft)
				coll.push(tmp->_pleft);
			if (tmp->_pright)
				coll.push(tmp->_pright);
			coll.pop();
		}
	}

	void Destroy(HuffmanTreeNode<T>*&root)
	{
		if (root)
		{
			Destroy(root->_pleft);
			Destroy(root->_pright);
			delete root;
			root = NULL;
		}
	}
private:
	HuffmanTreeNode<T>*_root;
};
