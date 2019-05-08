#pragma once
#include<iostream>
#include"Huffman.h"

using namespace std;

struct HEAD
{
	char type[4];//文件类型
	int length; //源文件长度
	int weight[256];//权值数值
};

char str2byte(const char *pBinstr);//字符串转码，将Huffman编码中的八个字符转换成一个

int Encode(const char *pFilename, const HuffmanCode pHC, char *pBuffer, const int nSize);//压缩编码

int InitHead(const char *pFilename, HEAD &sHead);//初始化文件头

int WriteFile(const char *pFilename, const HEAD sHead, const char *pBuffer, const int nSize);//将文件头信息，压缩后的编码写入文件，即得到压缩后的新文件

int Compress(const char *pFilename);//压缩文件