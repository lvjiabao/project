//#include <string>
//#include <assert.h>
//#include "HuffmanTree.hpp"
//
//struct FileInfo
//{
//	FileInfo(size_t count = 0)
//	: _count(count)
//	, _ch(0)
//	{}
//
//	unsigned char _ch;     // 字符
//	size_t _count;         // 字符出现的次数
//	std::string _strCode;  // 字符的编码
//
//	FileInfo operator+(const FileInfo& fileInfo)
//	{
//		FileInfo ret(*this);
//		ret._count += fileInfo._count;
//		return ret;
//	}
//
//	bool operator<(const FileInfo& fileInfo)
//	{
//		return _count < fileInfo._count;
//	}
//
//	bool operator!=(const FileInfo& fileInfo)const
//	{
//		return _count != fileInfo._count;
//	}
//};
//
//class FileCompress
//{
//public:
//	FileCompress()
//	{
//		for (int idx = 0; idx < 256; ++idx)
//		{
//			_fileInfo[idx]._ch = idx;
//			_fileInfo[idx]._count = 0;
//		}
//	}
//
//	void CompressFile(const std::string& fileName)
//	{
//		//1、读取文件获取每个字符出现的次数
//		FILE* fOut = fopen(fileName.c_str(), "r");
//		assert(fOut);
//
//		unsigned char *pReadBuff = new unsigned char[1024];
//		while (true)
//		{
//			size_t readSize = fread(pReadBuff, 1, 1024, fOut);
//			if (0 == readSize)
//				break;
//
//			for (size_t idx = 0; idx < readSize; ++idx)
//			{
//				_fileInfo[pReadBuff[idx]]._count++;
//			}
//		}
//
//		//2、 获取每个字符的huffman编码
//		FileInfo invalid;
//		invalid._count = 0;
//
//		HuffmanTree<FileInfo> ht(_fileInfo, 256, invalid);
//		// 获取huffman编码
//		_GenetateHuffmanCode(ht.GetRoot());
//
//
//		// 对文件进行重新编码
//
//		// 3、写编码的信息
//		std::string compressFileName = GetFileName(fileName);
//		//compressFileName
//
//		// 压缩编码信息
//		std::string strCodeInfo;
//		char count[32];
//		size_t lineCount = 0;
//		for (int idx = 0; idx < 256; ++idx)
//		{
//			//\n,20
//			if (_fileInfo[idx]._count)
//			{
//				strCodeInfo += _fileInfo[idx]._ch;
//				strCodeInfo += ',';
//				itoa(_fileInfo[idx]._count, count, 10);
//				strCodeInfo += count;
//				strCodeInfo += '\n';
//				lineCount++;
//			}
//		}
//
//		std::string strHeadCotent;
//		strHeadCotent += GetFilePostFix(fileName);
//		strHeadCotent += '\n';
//		itoa(lineCount, count, 10);
//		strHeadCotent += count;
//		strHeadCotent += '\n';
//		strHeadCotent += strCodeInfo;
//
//		compressFileName += ".hzp";
//		FILE* fIn = fopen(compressFileName.c_str(), "w");
//		assert(fIn);
//		fwrite(strHeadCotent.c_str(), 1, strHeadCotent.length(), fIn);
//
//		//fseek(fOut, 0, SEEK_SET);
//		rewind(fOut);
//		int pos = 0;
//		char ch = 0;
//		char* pWriteBuff = new char[1024];
//		int wrIdx = 0;
//		while (true)
//		{
//			int readSize = fread(pReadBuff, 1, 1024, fOut);
//			if (0 == readSize)
//			{
//				break;
//			}
//
//			// 1024
//			for (int idx = 0; idx < readSize; ++idx)
//			{
//				std::string code = _fileInfo[pReadBuff[idx]]._strCode;
//				for (int idx = 0; idx < code.size(); ++idx)
//				{
//					ch <<= 1;
//					pos++;
//					if (code[idx] == '1')
//					{
//						ch |= 1;
//					}
//
//					if (pos == 8)
//					{
//						pWriteBuff[wrIdx++] = ch;
//						if (wrIdx == 1024)
//						{
//							// 写入文件
//							fwrite(pWriteBuff, 1, 1024, fIn);
//							wrIdx = 0;
//						}
//						ch = 0;
//						pos = 0;
//					}
//				}
//			}
//		}
//
//		if (pos < 8)
//		{
//			ch <<= (8 - pos);
//			pWriteBuff[wrIdx] = ch;
//			// 写文件
//			fwrite(pWriteBuff, 1, wrIdx, fIn);
//		}
//
//		delete[] pReadBuff;
//		delete[] pWriteBuff;
//
//		fclose(fOut);
//		fclose(fIn);
//	}
//
//	void _GenetateHuffmanCode(HuffmanTreeNode<FileInfo>* pRoot)
//	{
//		if (pRoot)
//		{
//			_GenetateHuffmanCode(pRoot->_pLeft);
//			_GenetateHuffmanCode(pRoot->_pRight);
//
//			if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight)
//			{
//				HuffmanTreeNode<FileInfo>* pParent = pRoot->_pParent;
//				std::string& code = _fileInfo[pRoot->_weight._ch]._strCode;
//				while (pParent)
//				{
//					if (pParent->_pLeft == pRoot)
//					{
//						code += '0';
//					}
//
//					if (pParent->_pRight == pRoot)
//					{
//						code += '1';
//					}
//
//					pParent = pParent->_pParent;
//					pRoot = pRoot->_pParent;
//				}
//
//				std::reverse(code.begin(), code.end());
//			}
//		}
//	}
//
//	void UnCompressFile(const std::string& FilePath)
//	{
//		FILE* fOut = fopen(FilePath.c_str(), "r");
//		assert(fOut);
//
//		std::string strPostFix;
//		ReadLine(fOut, strPostFix);
//
//		std::string strLineCount;
//		ReadLine(fOut, strLineCount);
//
//		int lineCount = atoi(strLineCount.c_str());
//
//		// 读取编码信息
//		std::string strCodeInfo;
//		while (lineCount)
//		{
//			ReadLine(fOut, strCodeInfo);//
//			if ("" != strCodeInfo)
//			{
//				lineCount--;
//				unsigned char ch = strCodeInfo[0];
//				_fileInfo[ch]._ch = ch;
//				strCodeInfo = strLineCount.substr(2);
//				_fileInfo[ch]._count = atoi(strCodeInfo.c_str());
//				strCodeInfo = "";
//			}
//			else
//			{
//				strCodeInfo += '\n';
//				lineCount--;
//			}
//		}
//
//		// 获取每个字符的编码信息
//		HuffmanTree<FileInfo> ht(_fileInfo, 256, FileInfo(0));
//		HuffmanTreeNode<FileInfo>* pCur = ht.GetRoot();
//		//解码
//		char* pReadBuff = new char[1024];
//		char* pWriteBuff = new char[1024];
//		size_t writeSize = 0;
//		int pos = 7;
//		while (true)
//		{
//			size_t readSize = fread(pReadBuff, 1, 1024, fOut);
//			if (0 == readSize)
//				break;
//			int idx = 0;
//			while (idx < readSize)
//			{
//				if (pReadBuff[idx] & 1 << pos--)
//				{
//					pCur = pCur->_pRight;
//
//				}
//				else
//				{
//					pCur = pCur->_pLeft;
//				}
//
//				if (NULL == pCur->_pLeft && NULL == pCur->_pRight)
//				{
//					pWriteBuff[writeSize++] = pCur->_weight._ch;
//					if (1024 == writeSize)
//					{
//						// 写文件
//						writeSize = 0;
//					}
//
//					pCur = ht.GetRoot();
//				}
//
//				if (0 == pos)
//				{
//					++idx;
//					pos = 7;
//				}
//			}
//		}
//	}
//
//	//Test.txt
//	//F:\Dir\Test.txt 11 - 6
//	std::string GetFileName(const std::string& strFilePath)
//	{
//		size_t pos = strFilePath.find_last_of('\\');
//		return strFilePath.substr(pos + 1, strFilePath.find_last_of('.') - pos - 1);
//	}
//
//	std::string GetFilePostFix(const std::string& strFilePath)
//	{
//		size_t pos = strFilePath.find_last_of('.');
//		return strFilePath.substr(pos, strFilePath.length() - pos);
//	}
//
//	//\n,20
//	bool ReadLine(FILE* fOut, std::string& codeInfo)
//	{
//		assert(fOut);
//		char ch = fgetc(fOut);
//		if (EOF == ch)
//			return false;
//
//		//
//		while ('\n' != ch)
//		{
//			codeInfo += ch;
//
//			ch = fgetc(fOut);
//			if (EOF == ch)
//				return false;
//		}
//
//		return true;
//	}
//
//private:
//	FileInfo _fileInfo[256];
//};
//
//void Test()
//{
//	FileCompress file;
//	file.CompressFile("Test.txt");
//}