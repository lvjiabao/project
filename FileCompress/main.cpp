#include "FileCompress.h"


void testcompress()
{
	FileCompress file1;
	file1.CompressFile("12.txt");
}
void testuncompress()
{
	FileCompress file2;
	file2.UnCompressFile("12.hzp");
}

int main()
{
	testcompress();
	testuncompress();
	return 0;
}