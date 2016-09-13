//��Ҫһ��256�Ľṹ������������ascii����Ϣ
//��Ҫ����һϵ�еĲ���������Ϊ����huffmantree�Ľڵ��ǽṹ���������͵�,��Ҫ����ڵ��ıȽ�
//��Ҫ�Խڵ��ʼ����countʲô�ģ���Ϊ����Ҫ++��

//ѹ��
/*
1.�����ļ������count�ļ���
2.����count����Ŀ������һ��huffmantree
3.����huffmantree,�õ���ֵ�Ķ����Ʊ���
	���һ������GetCode,��������ͷ��㣬�õݹ鵽Ҷ�ڵ㣬Ȼ�����������жϣ�������������������
4.д��ͷ������Ϣ����һ��string�У����ļ���׺�����ַ���Ϣ���������Լ�����
	дһ������GetFileName��������string��substr�õ��ļ���
5.д������Ʊ�����Ϣ
	ѭ����ȡ���ݣ�Ȼ���õ�ȡ����ÿ���ֽڵĶ����Ʊ��룬����ѭ��������pos����һ��Ϊ0��ch����������Ʊ���
	�𲽸�ֵ����8λ�Ժ��д�뵽pWriteBuff�У�pWriteBuff���˾�д�뵽�ļ��У�Ҫע�����һ�ε�ch����û����8λ
	Ҫ��ǰ�ƶ��㹻��λ����˳���ûд���ļ���pWriteBuffд��ȥ
*/

//��ѹ
/*
1.���ļ�����ȡ�ļ������ַ���Ϣ������
	дһ������GetLine(),�õ�ÿ�е���Ϣ��Ҫע�⣬����\n������EOF�ͽ���
2.���ַ���Ϣд�뵽�ṹ��������ȥ
3.����huffmantree
4.д�������ļ�
	����ѭ����ȡѹ���ļ���PReadBuff�У���ȡԭ�ַ��ķ���������һ��ָ��ֵΪ���ĸ���Ȼ���ж����pReadBuff[i]�е���һλΪ1�������ߣ����������ߣ���Ҷ�ڵ�ͰѸýڵ��_chд�뵽pWriteBuff��ȥ��
	����pos�����pos==-1��ʹpWriteBuff��idx++��д��pWriteBuff��д�뵽�ļ���ȥ�����Ҫע�⽫û����pWriteBuffд���ļ�
	��λ�ȡ���һ���ֽ�����Ч��ǰ��λ���ݣ�����һ��count--��ÿ���ַ�д�������--��
*/

//���⣺д������ĸ����ѹ��������
//��������ļ�ʱ:"rb","wb",��ʾ�򿪶������ļ�������ǡ�r������w�������ı��ļ����ı��ļ�������������EOF

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
	unsigned char _ch;         //�ַ�
	long long  count;			   //���ִ���
	string _strcode;		   //����

	//�Ƚ�����ʱ���õ�
	bool operator<(const Fileinfo&fileinfo)
	{
		return count < fileinfo.count;
	}
	//HuffmanTree�й��츸�ڵ���õ�
	Fileinfo operator+(const Fileinfo&fileinfo)
	{
		Fileinfo ret(*this);
		ret.count += fileinfo.count;
		return  ret;
	}

	//invaild��ʱ���õ�,�ڵ�Ϊ�վͲ�����huffmantree�Ĺ���
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


	//ѹ��
	void CompressFile(const string&filename)
	{
		//��ȡÿ���ַ��ĸ���
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

		//����huffmantree
		Fileinfo invaild;//����invaild��Ϊ�˱���յĽڵ㱻��ӵ�����
		HuffmanTree<Fileinfo>hp(fileinfo,256,invaild);
		
		//���ascii����
		GetHuffmanCode(hp.GetTop());

		//��дͷ����Ϣ
		string CompressFileName = GetFilename(filename);
		string CompressFileSuffix = GetFileSuffix(filename);
		string HeadMessage;//ͷ����Ϣ
		int CountOfLine = 0;//��¼����
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
		//�ѱ���д�뵽�ļ�����ȥ


		rewind(Fout);//ʹ�ļ�ָ��ص��ļ�ͷ
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

	//��ѹ
	void UnCompressFile(const string&filename)
	{
		FILE*Fout = fopen(filename.c_str(), "rb");
		assert(Fout);

		string namemes;
		ReadLine(Fout, namemes);

		string nummes;
		ReadLine(Fout, nummes);

		int numline = atoi(nummes.c_str());

		//����Ϣ����������ȥ
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
				//proot->_weight._strcode+='0';�����ǲ��еģ��ı�������ϵĽڵ�
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
		return filename.substr(pos + 1, filename.find_last_of('.')-pos-1);//�ڶ��������ǳ���
	}
private:
	Fileinfo fileinfo[256];
};