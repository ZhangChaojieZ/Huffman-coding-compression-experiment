#pragma once
#include<iostream>
#include"Huffman.h"

using namespace std;

struct HTNode //哈夫曼结点
{
	int weight;//权值
	int parent;//权值
	int lchild;//权值
	int rchild;//权值
};

typedef char **HuffmanCode;//存储huffman编码

typedef HTNode *HuffmanTree;//动态数组存储Huffman树

int HufffmanTree(HuffmanTree &pHT, int *w, int n);//构造Huffman树

int Select(HuffmanTree pHT, int Size);//查找最小权值

void HuffmanCoding(HuffmanTree pHT, HuffmanCode &pHC,int n);//编码

void TestHuffmanTree(HuffmanTree pHT);//测试Huffman树

void TestHuffmanCode(int root, HuffmanTree pHT, HuffmanCode pHC);//测试Huffman树编码