//需要一个256的结构体数组来保存ascii的信息
//需要重载一系列的操作符，因为构成huffmantree的节点是结构体数组类型的,需要满足节点间的比较
//需要对节点初始化，count什么的，因为它是要++的

//压缩
/*
1.遍历文件，完成count的计数
2.根据count的数目，构建一棵huffmantree
3.根据huffmantree,拿到各值的二进制编码
	设计一个函数GetCode,传入树的头结点，用递归到叶节点，然后依次往上判断，是左子树还是右子树
4.写入头部的信息（在一个string中）：文件后缀名，字符信息的行数，以及个数
	写一个函数GetFileName（），用string的substr拿到文件名
5.写入二进制编码信息
	循环读取数据，然后拿到取到的每个字节的二进制编码，设置循环，设置pos，将一个为0的ch依据其二进制编码
	逐步赋值，到8位以后就写入到pWriteBuff中，pWriteBuff满了就写入到文件中，要注意最后一次的ch可能没满足8位
	要向前移动足够的位数，顺便把没写入文件的pWriteBuff写进去
*/

//解压
/*
1.打开文件，读取文件名，字符信息的行数
	写一个函数GetLine(),拿到每行的信息，要注意，跳过\n，遇到EOF就结束
2.将字符信息写入到结构体数组中去
3.构建huffmantree
4.写解码后的文件
	设置循环读取压缩文件到PReadBuff中，获取原字符的方法（设置一个指针值为树的根，然后判断如果pReadBuff[i]中的下一位为1就向右走，否则向左走，到叶节点就把该节点的_ch写入到pWriteBuff中去）
	设置pos，如果pos==-1就使pWriteBuff的idx++，写满pWriteBuff就写入到文件中去，最后要注意将没满的pWriteBuff写入文件
	如何获取最后一个字节中有效的前几位数据（设置一个count--，每个字符写入过，就--）
*/

//问题：写得了字母，解压不了文字
//结决：打开文件时:"rb","wb",表示打开二进制文件，如果是“r”，“w”则是文本文件，文本文件编码后可能生成EOF

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<assert.h>
#include<stdlib.h>
#include "HuffmanTree.hpp"
using namespace std;

struct Fileinfo
{
	Fileinfo(size_t size=0)
	:count(size)
	,_ch(0)
	{}
	unsigned char _ch;         //字符
	long long  count;			   //出现次数
	string _strcode;		   //编码

	//比较器的时候用到
	bool operator<(const Fileinfo&fileinfo)
	{
		return count < fileinfo.count;
	}
	//HuffmanTree中构造父节点会用到
	Fileinfo operator+(const Fileinfo&fileinfo)
	{
		Fileinfo ret(*this);
		ret.count += fileinfo.count;
		return  ret;
	}

	//invaild的时候用到,节点为空就不参与huffmantree的构成
	bool operator!=(const Fileinfo&fileinfo)const
	{
		return fileinfo.count != count;
	}
};

class FileCompress
{
public:
	FileCompress()
	{
		for (int i = 0; i < 256; ++i)
		{
			fileinfo[i].count = 0;
			fileinfo[i]._ch = i;
			fileinfo[i]._strcode = "";
		}
	}


	//压缩
	void CompressFile(const string&filename)
	{
		//读取每个字符的个数
		FILE*Fout=fopen(filename.c_str(), "rb");
		assert(Fout);
		unsigned char* pReadBuff = new unsigned char[1024];
		while (1)
		{
			int readsize = fread(pReadBuff, 1, 1024, Fout);
			if (readsize == 0)
				break;
			for (int i = 0; i < readsize; i++)
			{
				fileinfo[pReadBuff[i]].count++;
			}
		}

		//构建huffmantree
		Fileinfo invaild;//传入invaild是为了避免空的节点被添加到树中
		HuffmanTree<Fileinfo>hp(fileinfo,256,invaild);
		
		//获得ascii编码
		GetHuffmanCode(hp.GetTop());

		//先写头部信息
		string CompressFileName = GetFilename(filename);
		string CompressFileSuffix = GetFileSuffix(filename);
		string HeadMessage;//头部信息
		int CountOfLine = 0;//记录行数
		string LineMessage;
		char count[32];
		for (int i = 0; i < 256; ++i)
		{
			if (fileinfo[i].count)
			{
				LineMessage += fileinfo[i]._ch;
				LineMessage += ',';
				itoa(fileinfo[i].count, count, 10);
				LineMessage += count;
				LineMessage += '\n';
				CountOfLine++;
			}
		}
		char*ret=itoa(CountOfLine, count, 10);
		string countline(ret);
		countline += '\n';
		countline += LineMessage;
		CompressFileSuffix += '\n';
		CompressFileSuffix += countline;


		CompressFileName += ".hzp";
		FILE*Fin = fopen(CompressFileName.c_str(), "wb");
		assert(Fin);
		fwrite(CompressFileSuffix.c_str(), 1, CompressFileSuffix.size(), Fin);
		//把编码写入到文件里面去


		rewind(Fout);//使文件指针回到文件头
		int pos = 0;
		int ch = 0;
		int Widx = 0;
		unsigned char*pWriteBuff = new unsigned char[1024];
		while (1)
		{
			size_t readsize = fread(pReadBuff, 1, 1024, Fout);
			if (0 == readsize)
				break;
			string code;
			for (int i = 0; i < readsize; i++)
			{
				code = fileinfo[pReadBuff[i]]._strcode;
				for (int j = 0; j < code.size(); ++j)
				{
					ch <<= 1;
					pos++;
					if (code[j] == '1')
						ch |= 1;
					if (pos == 8)
					{
						pWriteBuff[Widx++] = ch;
						if (Widx == 1024)
						{
							fwrite(pWriteBuff, 1, 1024, Fin);
							Widx = 0;
						}
						pos = 0;
						ch = 0;
					}
				}
			}
		}
		if (Widx)
			{
				if (pos < 8)
					ch <<= (8 - pos);
				pWriteBuff[Widx++] = ch;
				fwrite(pWriteBuff, 1, Widx, Fin);
			}
		delete[]pReadBuff;
		delete[]pWriteBuff;
		fclose(Fout);
		fclose(Fin);
	}

	//解压
	void UnCompressFile(const string&filename)
	{
		FILE*Fout = fopen(filename.c_str(), "rb");
		assert(Fout);

		string namemes;
		ReadLine(Fout, namemes);

		string nummes;
		ReadLine(Fout, nummes);

		int numline = atoi(nummes.c_str());

		//将信息读到数组中去
		string charandnum;
		while (numline)
		{
			ReadLine(Fout, charandnum);
			if ("" != charandnum)
			{
				unsigned char ch=charandnum[0];
				fileinfo[ch]._ch = ch;
				string numofchar = charandnum.substr(2);
				fileinfo[ch].count = atoi(numofchar.c_str());
				charandnum = "";
				numline--;
			}
			else
			{
				charandnum += '\n';
			}
		}

		string UnCompressName = GetFilename(filename);
		UnCompressName += '1';
		UnCompressName += namemes;
		Fileinfo invaild;
		HuffmanTree<Fileinfo>hp(fileinfo, 256, invaild);
		HuffmanTreeNode<Fileinfo>*pcur = hp.GetTop();
		unsigned char *pReadBuff = new unsigned char[1024];
		unsigned char *pWriteBuff = new unsigned char[1024];
		FILE*Fin = fopen(UnCompressName.c_str(), "wb");
		int pos = 7;
		int widx=0;
		int ch = 0;

		while (1)
		{
			int readsize = fread(pReadBuff, 1, 1024, Fout);
			if (0 == readsize)
				break;
			int i = 0;
			while (i < readsize)
			{
				if (NULL == pcur || (NULL == pcur->_pleft&&NULL == pcur->_pright))
					break;
				if (pReadBuff[i] & (1 << pos--))
				{
					pcur = pcur->_pright;
				}
				else
					pcur = pcur->_pleft;
				
				if (NULL == pcur->_pleft&&NULL == pcur->_pright&&pcur->_weight.count)
				{
					pWriteBuff[widx++] = pcur->_weight._ch;
					pcur->_weight.count--;
					if (widx == 1024)
					{
						fwrite(pWriteBuff, 1, 1024, Fin);
						widx = 0;
					}
					pcur = hp.GetTop();

				}
				if (-1 == pos)
				{
					i++;
					pos = 7;
				}
			}
			ch = 1;	
		}
		if (widx)
		{
			fwrite(pWriteBuff, 1, widx, Fin);
		}
		delete[]pWriteBuff;
		delete[]pReadBuff;
		fclose(Fout);
		fclose(Fin);
		
	}

	bool ReadLine(FILE*Fout, string&line)////////////////////////////////
	{
		char ch = fgetc(Fout);
		if (EOF == ch)
			return false;

		while (ch!='\n')
		{
			line += ch;
			if (EOF == ch)
				return false;
			ch = fgetc(Fout);
		}
		return true;
	}

	void GetHuffmanCode(HuffmanTreeNode<Fileinfo>*proot)//////////////////////
	{
		if (NULL == proot)
			return;
		GetHuffmanCode(proot->_pleft);
		GetHuffmanCode(proot->_pright);
		if (proot->_pleft == NULL&&proot->_pright == NULL)
		{
			HuffmanTreeNode<Fileinfo>*parent = proot->_parent;
			string& code = fileinfo[proot->_weight._ch]._strcode;
			while (parent)
			{
				//proot->_weight._strcode+='0';这样是不行的，改变的是树上的节点
				if (proot == parent->_pleft)
					code += '0';
				if (proot == parent->_pright)
					code += '1';
				proot = parent;
				parent = parent->_parent;
			}
			reverse(code.begin(), code.end());
		}
	}
private:
	string GetFileSuffix(const string&filename)
	{
		size_t pos=filename.find_last_of('.');
		return filename.substr(pos);
	}
	string GetFilename(const string&filename)
	{
		size_t pos = filename.find_last_of('\\');
		return filename.substr(pos + 1, filename.find_last_of('.')-pos-1);//第二个参数是长度
	}
private:
	Fileinfo fileinfo[256];
};