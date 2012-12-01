#include "hufffman.h"

int main(int argc,char *argv[])
{
	if(argc ==1)
	{
	cout << "²ÎÊý²»×ã!"<<endl;
		exit(-1);
	}
	huffman comp(argv[1],argv[2]);
	comp.createPriorityQueue();
	comp.createHuffmanTree();

	comp.saveToFile();
	cout << endl;
	return 0;
}