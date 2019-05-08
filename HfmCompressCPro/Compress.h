#pragma once
#include<iostream>
#include"Huffman.h"

using namespace std;

struct HEAD
{
	char type[4];//�ļ�����
	int length; //Դ�ļ�����
	int weight[256];//Ȩֵ��ֵ
};

char str2byte(const char *pBinstr);//�ַ���ת�룬��Huffman�����еİ˸��ַ�ת����һ��

int Encode(const char *pFilename, const HuffmanCode pHC, char *pBuffer, const int nSize);//ѹ������

int InitHead(const char *pFilename, HEAD &sHead);//��ʼ���ļ�ͷ

int WriteFile(const char *pFilename, const HEAD sHead, const char *pBuffer, const int nSize);//���ļ�ͷ��Ϣ��ѹ����ı���д���ļ������õ�ѹ��������ļ�

int Compress(const char *pFilename);//ѹ���ļ�