#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"Huffman.h"
#include"Compress.h"

using namespace std;


#define OK 1
#define ERROR 0

const int SIZE = 256;


char str2byte(const char *pBinStr)//字符串转码，将Huffman编码中的八个字符转换成一个
{
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;//左移一位
		if (pBinStr[i] == '1')
			b = b | 0x01;
	}
	return b;
}

int Encode(const char *pFilename, const HuffmanCode pHC, char *pBuffer, const int nSize)//压缩编码
{
	//打开文件
	FILE *in = fopen(pFilename, "rb");

	//开辟缓冲区
	//pBuffer = (char *)malloc(nSize * sizeof(char));////pBuffer = new char[nSize];//
	//pBuffer[nSize] = { '\0' };
	
	//if (!pBuffer)
	//{
	//	cerr << "开辟缓冲区失败！EnCode!" << endl;
	//	return 0;//返回0表示失败
	//}

	char cd[SIZE] = { 0 };//工作区
	int pos = 0;//缓冲区指针
	int ch;// char ch;//用char ch出错，可能会有所不同,这里与最初读文件时一致
	
	//扫描文件，根据Huffman编码表对其进行压缩，压缩结果暂存到缓冲区中
	while ((ch = fgetc(in)) != EOF)
	{
		//cout << ch << endl;
		strcat(cd, pHC[ch]);//从HC复制编码到cd
		//压缩编码
		while (strlen(cd) >= 8)
		{
			//截取字符串左边的8个字符，编码成字节
			pBuffer[pos++] = str2byte(cd);//出现写入文件异常
			//cout << "hhhh" << endl;
			//字符串整体左移8字节
			for (int i = 0; i < SIZE - 8; i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	//关闭文件流
	fclose(in);
	in = NULL;

	if (strlen(cd) > 0)
	{
		pBuffer[pos++] = str2byte(cd);
	}
	return OK;
}

int InitHead(const char *pFilename, HEAD &sHead)//初始化文件头
{
	strcpy(sHead.type, "HUF");//文件类型
	sHead.length = 0;//原文件长度
	for (int i = 0; i < SIZE; i++)
	{
		sHead.weight[i] = 0;		//权值
	}

	//以二进制流形式打开文件
	FILE *in = fopen(pFilename, "rb");

	//扫描文件，获得权重
	int ch;// char ch;//用int ch可能会不一样，这里和开始读文件时保持一致
	while ((ch = fgetc(in)) != EOF)
	{
		sHead.weight[ch]++;
		sHead.length++;
	}

	//关闭文件
	fclose(in);
	in = NULL;
	return OK;

}

int WriteFile(const char *pFilename, const HEAD sHead, const char *pBuffer, const int nSize)//将文件头信息，压缩后的编码写入文件，即得到压缩后的新文件
{
	//生成文件名
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf");

	//以二进制流形式打开文件
	FILE *out = fopen(filename, "wb");

	//写文件头
	fwrite(&sHead, sizeof(HEAD), 1, out);
	//写压缩后的编码
	fwrite(pBuffer, sizeof(char), nSize, out);
	//关闭文件，释放文件指针
	fclose(out);
	out = NULL;

	cout << "生成压缩文件：" << filename << endl;
	int len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}

int Compress(const char *pFilename)//压缩文件
{
	HuffmanTree pHT;
	char** pHC = new char*[256];//分配内存空间//HuffamnCode pHC = NULL;
	
	//打开并扫描文件
	cout << "正在读取文件……" << endl;
	int weight[256] = { 0 };//打开文件，获取权重
	int ch;// char ch;//

	FILE *in = fopen(pFilename, "rb");
	if (in == NULL)
	{
		cerr << "文件打开失败！" << endl;
		return ERROR;
	}
	while ((ch = getc(in)) != EOF)
	{
		weight[ch]++;//统计256中字符出现的次数
	}

	cout << "文件读取完毕！\n" << endl << endl;

	//关闭文件
	fclose(in);
	in = NULL;

	//测试得到的权重
	/*
	cout << "显示256种字节出现的次数：" << endl;
	cout << "Byte\t" << "Weight" << endl;
	for (int i = 0; i < 256; i++)
	{
		printf("0x%02X    %d\n", i, weight[i]);
	}*/

	//构造Huffman树
	HufffmanTree(pHT, weight, 256);
	
	//测试Huffman树
	//TestHuffmanTree(pHT);
	
	//进行Huffman编码
	HuffmanCoding(pHT, pHC, 256);
	
	//测试Huffman编码
	//cout << "Byte\t" << "huffmanCode" << endl;
	//TestHuffmanCode(510, pHT, pHC);
	
	//计算编码缓冲区大小
	int nSize = 0;
	for (int i = 0; i < 256; i++)
	{
		nSize += weight[i] * strlen(pHC[i]);
	}
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;

	//对原文件压缩编码
	char *pBuffer = NULL;
	pBuffer = new char[nSize];//pBuffer = (char *)malloc(nSize * sizeof(char)); //
	memset(pBuffer, 0, (nSize) * sizeof(char));
	//pBuffer = { '\0' };
	Encode(pFilename, pHC, pBuffer, nSize);
	if (!pBuffer)
	{
		cout << "开辟缓冲区失败！Compress!" << endl;
		return ERROR;
	}

	HEAD sHead;
	InitHead(pFilename, sHead);
	cout << "目标文件大小：" << sHead.length << "字节" << endl;
	int afterlen = WriteFile(pFilename, sHead, pBuffer, nSize);
	cout << "压缩文件大小：" << afterlen << "字节  \n其中文件头sHead大小：" << sizeof(sHead) << "字节" << endl;
	cout << "压缩比率：" << (double)afterlen * 100 / sHead.length << "%" << endl;

	delete pHT;
	//delete pHC;
	delete pBuffer;

	return OK;
}