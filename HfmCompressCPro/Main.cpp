#include<iostream>
#include<fstream>
#include"Huffman.h"
#include"Compress.h"

using namespace std;

int main()
{
	cout << "========Huffman�ļ�ѹ��========" << endl;
	cout << "������Դ�ļ�����";
	char filename[256];
	cin >> filename;

	if (Compress(filename) == 1) {
		cout << "\nѹ����ɣ�" << endl;
	}
	else {
		cout << "\nѹ��ʧ��!" << endl;
	}

	return 0;
}