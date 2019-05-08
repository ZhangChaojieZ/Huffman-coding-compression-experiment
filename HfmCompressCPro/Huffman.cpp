#include<iostream>
#include<stdlib.h>
#include"Huffman.h"

using namespace std;

void InitHuffmanTree(HuffmanTree &pHT, int m)//��ʼ��Huffman��,mΪ���н����
{
	pHT = new HTNode[m];

	for (int i = 0; i < m; i++)
	{
		pHT[i].weight = 0;
		pHT[i].parent = -1;
		pHT[i].lchild = -1;
		pHT[i].rchild = -1;
	}

}

int Select(HuffmanTree pHT, int nSize)//������СȨֵ
{
	int minValue = 0x7FFFFFFF;//��Сֵ
	int min = 0;//��СԪ�����

	//Ѱ����СԪ�����
	for (int i = 0; i < nSize; i++)
	{
		if (pHT[i].parent == -1 && pHT[i].weight < minValue)//�ҳ�ʣ�����е���Сֵ
		{
			minValue = pHT[i].weight;
			min = i;
		}
	}

	return min;
}

int HufffmanTree(HuffmanTree &pHT, int *w, int n)//����Huffman ��
{
	int min1;//��СԪ�����
	int min2;//��СԪ�����
	int m;

	m = 2 * n - 1;//Huffman���������

	InitHuffmanTree(pHT, m);

	for (int i = 0; i < n; i++)
	{
		pHT[i].weight=w[i];//Ȩֵ��ʼ��
	}

	for (int i = n; i < m; i++)
	{
		min1=Select(pHT, i);
		pHT[min1].parent = i;
		pHT[i].lchild = min1;//��СֵΪ����
		min2 = Select(pHT, i);
		pHT[min2].parent = i;
		pHT[i].rchild = min2;//��СֵΪ�Һ���
		pHT[i].weight = pHT[min1].weight + pHT[min2].weight;
	}

	return 0;
}

void HuffmanCoding(HuffmanTree pHT, HuffmanCode &pHC,int n)//����
{
	int start;
	char *Code;//�洢����
	int temp_c=0,temp_p=0;//�ֱ�Ϊchild��parent���

	Code = new char[n];//Code = (char *)malloc(n * sizeof(char));//�����ڴ�ռ�
	Code[n-1] = '\0';

	for (int i = 0; i < 256; i++)
	{
		start = n-1;
		temp_c = i;//���ӽ�����
		temp_p = pHT[temp_c].parent;//��������

		while (temp_p >= 0)//�ӵ�һֱ������㣬�����ĸ����Ϊ-1
		{
			--start;
			if (pHT[temp_p].lchild == temp_c)//Ϊ������Ϊ0����֮Ϊ1
				Code[start] = '0';
			else
				Code[start] = '1';
			temp_c = temp_p;
			temp_p = pHT[temp_c].parent;	
		}
		pHC[i] = new char[n - start];// (char *)malloc((256 - start) * sizeof(char));//�����ڴ�ռ�
		strcpy(pHC[i], &Code[start]);//�Ѵ洢������ַ���ֵ
	}

	delete Code;
}

void TestHuffmanCode(int root, HuffmanTree pHT, HuffmanCode pHC)//����Huffman������
{
	if (pHT[root].lchild == -1 && pHT[root].rchild == -1)//-1�ǳ�ʼ�����ӽڵ�ı�־
	{
		printf("0x%02X %s\n", root, pHC[root]);
	}
	if (pHT[root].lchild!=-1)
	{
		TestHuffmanCode(pHT[root].lchild, pHT, pHC);
	}
	if (pHT[root].rchild!=-1)
	{
		TestHuffmanCode(pHT[root].rchild, pHT, pHC);
	}
}

void TestHuffmanTree(HuffmanTree pHT)///����Huffman��
{
	for (int i = 0; i < 511; i++)
	{
		printf("pHT[%d]\t%d\t%d\t%d\t%d\n", i, pHT[i].weight, pHT[i].parent,pHT[i].lchild,pHT[i].rchild);
	}
}