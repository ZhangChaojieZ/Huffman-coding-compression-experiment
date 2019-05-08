#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"Huffman.h"
#include"Compress.h"

using namespace std;


#define OK 1
#define ERROR 0

const int SIZE = 256;


char str2byte(const char *pBinStr)//�ַ���ת�룬��Huffman�����еİ˸��ַ�ת����һ��
{
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;//����һλ
		if (pBinStr[i] == '1')
			b = b | 0x01;
	}
	return b;
}

int Encode(const char *pFilename, const HuffmanCode pHC, char *pBuffer, const int nSize)//ѹ������
{
	//���ļ�
	FILE *in = fopen(pFilename, "rb");

	//���ٻ�����
	//pBuffer = (char *)malloc(nSize * sizeof(char));////pBuffer = new char[nSize];//
	//pBuffer[nSize] = { '\0' };
	
	//if (!pBuffer)
	//{
	//	cerr << "���ٻ�����ʧ�ܣ�EnCode!" << endl;
	//	return 0;//����0��ʾʧ��
	//}

	char cd[SIZE] = { 0 };//������
	int pos = 0;//������ָ��
	int ch;// char ch;//��char ch�������ܻ�������ͬ,������������ļ�ʱһ��
	
	//ɨ���ļ�������Huffman�����������ѹ����ѹ������ݴ浽��������
	while ((ch = fgetc(in)) != EOF)
	{
		//cout << ch << endl;
		strcat(cd, pHC[ch]);//��HC���Ʊ��뵽cd
		//ѹ������
		while (strlen(cd) >= 8)
		{
			//��ȡ�ַ�����ߵ�8���ַ���������ֽ�
			pBuffer[pos++] = str2byte(cd);//����д���ļ��쳣
			//cout << "hhhh" << endl;
			//�ַ�����������8�ֽ�
			for (int i = 0; i < SIZE - 8; i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	//�ر��ļ���
	fclose(in);
	in = NULL;

	if (strlen(cd) > 0)
	{
		pBuffer[pos++] = str2byte(cd);
	}
	return OK;
}

int InitHead(const char *pFilename, HEAD &sHead)//��ʼ���ļ�ͷ
{
	strcpy(sHead.type, "HUF");//�ļ�����
	sHead.length = 0;//ԭ�ļ�����
	for (int i = 0; i < SIZE; i++)
	{
		sHead.weight[i] = 0;		//Ȩֵ
	}

	//�Զ���������ʽ���ļ�
	FILE *in = fopen(pFilename, "rb");

	//ɨ���ļ������Ȩ��
	int ch;// char ch;//��int ch���ܻ᲻һ��������Ϳ�ʼ���ļ�ʱ����һ��
	while ((ch = fgetc(in)) != EOF)
	{
		sHead.weight[ch]++;
		sHead.length++;
	}

	//�ر��ļ�
	fclose(in);
	in = NULL;
	return OK;

}

int WriteFile(const char *pFilename, const HEAD sHead, const char *pBuffer, const int nSize)//���ļ�ͷ��Ϣ��ѹ����ı���д���ļ������õ�ѹ��������ļ�
{
	//�����ļ���
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf");

	//�Զ���������ʽ���ļ�
	FILE *out = fopen(filename, "wb");

	//д�ļ�ͷ
	fwrite(&sHead, sizeof(HEAD), 1, out);
	//дѹ����ı���
	fwrite(pBuffer, sizeof(char), nSize, out);
	//�ر��ļ����ͷ��ļ�ָ��
	fclose(out);
	out = NULL;

	cout << "����ѹ���ļ���" << filename << endl;
	int len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}

int Compress(const char *pFilename)//ѹ���ļ�
{
	HuffmanTree pHT;
	char** pHC = new char*[256];//�����ڴ�ռ�//HuffamnCode pHC = NULL;
	
	//�򿪲�ɨ���ļ�
	cout << "���ڶ�ȡ�ļ�����" << endl;
	int weight[256] = { 0 };//���ļ�����ȡȨ��
	int ch;// char ch;//

	FILE *in = fopen(pFilename, "rb");
	if (in == NULL)
	{
		cerr << "�ļ���ʧ�ܣ�" << endl;
		return ERROR;
	}
	while ((ch = getc(in)) != EOF)
	{
		weight[ch]++;//ͳ��256���ַ����ֵĴ���
	}

	cout << "�ļ���ȡ��ϣ�\n" << endl << endl;

	//�ر��ļ�
	fclose(in);
	in = NULL;

	//���Եõ���Ȩ��
	/*
	cout << "��ʾ256���ֽڳ��ֵĴ�����" << endl;
	cout << "Byte\t" << "Weight" << endl;
	for (int i = 0; i < 256; i++)
	{
		printf("0x%02X    %d\n", i, weight[i]);
	}*/

	//����Huffman��
	HufffmanTree(pHT, weight, 256);
	
	//����Huffman��
	//TestHuffmanTree(pHT);
	
	//����Huffman����
	HuffmanCoding(pHT, pHC, 256);
	
	//����Huffman����
	//cout << "Byte\t" << "huffmanCode" << endl;
	//TestHuffmanCode(510, pHT, pHC);
	
	//������뻺������С
	int nSize = 0;
	for (int i = 0; i < 256; i++)
	{
		nSize += weight[i] * strlen(pHC[i]);
	}
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;

	//��ԭ�ļ�ѹ������
	char *pBuffer = NULL;
	pBuffer = new char[nSize];//pBuffer = (char *)malloc(nSize * sizeof(char)); //
	memset(pBuffer, 0, (nSize) * sizeof(char));
	//pBuffer = { '\0' };
	Encode(pFilename, pHC, pBuffer, nSize);
	if (!pBuffer)
	{
		cout << "���ٻ�����ʧ�ܣ�Compress!" << endl;
		return ERROR;
	}

	HEAD sHead;
	InitHead(pFilename, sHead);
	cout << "Ŀ���ļ���С��" << sHead.length << "�ֽ�" << endl;
	int afterlen = WriteFile(pFilename, sHead, pBuffer, nSize);
	cout << "ѹ���ļ���С��" << afterlen << "�ֽ�  \n�����ļ�ͷsHead��С��" << sizeof(sHead) << "�ֽ�" << endl;
	cout << "ѹ�����ʣ�" << (double)afterlen * 100 / sHead.length << "%" << endl;

	delete pHT;
	//delete pHC;
	delete pBuffer;

	return OK;
}