#include<iostream>
#include<stdlib.h>
#include"Huffman.h"

using namespace std;

void InitHuffmanTree(HuffmanTree &pHT, int m)//初始化Huffman树,m为所有结点数
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

int Select(HuffmanTree pHT, int nSize)//查找最小权值
{
	int minValue = 0x7FFFFFFF;//最小值
	int min = 0;//最小元素序号

	//寻找最小元素序号
	for (int i = 0; i < nSize; i++)
	{
		if (pHT[i].parent == -1 && pHT[i].weight < minValue)//找出剩余结点中的最小值
		{
			minValue = pHT[i].weight;
			min = i;
		}
	}

	return min;
}

int HufffmanTree(HuffmanTree &pHT, int *w, int n)//构造Huffman 树
{
	int min1;//最小元素序号
	int min2;//次小元素序号
	int m;

	m = 2 * n - 1;//Huffman树结点总数

	InitHuffmanTree(pHT, m);

	for (int i = 0; i < n; i++)
	{
		pHT[i].weight=w[i];//权值初始化
	}

	for (int i = n; i < m; i++)
	{
		min1=Select(pHT, i);
		pHT[min1].parent = i;
		pHT[i].lchild = min1;//最小值为左孩子
		min2 = Select(pHT, i);
		pHT[min2].parent = i;
		pHT[i].rchild = min2;//次小值为右孩子
		pHT[i].weight = pHT[min1].weight + pHT[min2].weight;
	}

	return 0;
}

void HuffmanCoding(HuffmanTree pHT, HuffmanCode &pHC,int n)//编码
{
	int start;
	char *Code;//存储编码
	int temp_c=0,temp_p=0;//分别为child和parent序号

	Code = new char[n];//Code = (char *)malloc(n * sizeof(char));//分配内存空间
	Code[n-1] = '\0';

	for (int i = 0; i < 256; i++)
	{
		start = n-1;
		temp_c = i;//孩子结点序号
		temp_p = pHT[temp_c].parent;//父结点序号

		while (temp_p >= 0)//从底一直到根结点，根结点的父结点为-1
		{
			--start;
			if (pHT[temp_p].lchild == temp_c)//为左孩子则为0，反之为1
				Code[start] = '0';
			else
				Code[start] = '1';
			temp_c = temp_p;
			temp_p = pHT[temp_c].parent;	
		}
		pHC[i] = new char[n - start];// (char *)malloc((256 - start) * sizeof(char));//分配内存空间
		strcpy(pHC[i], &Code[start]);//把存储编码的字符串值
	}

	delete Code;
}

void TestHuffmanCode(int root, HuffmanTree pHT, HuffmanCode pHC)//测试Huffman树编码
{
	if (pHT[root].lchild == -1 && pHT[root].rchild == -1)//-1是初始化无子节点的标志
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

void TestHuffmanTree(HuffmanTree pHT)///测试Huffman树
{
	for (int i = 0; i < 511; i++)
	{
		printf("pHT[%d]\t%d\t%d\t%d\t%d\n", i, pHT[i].weight, pHT[i].parent,pHT[i].lchild,pHT[i].rchild);
	}
}