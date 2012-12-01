#include "hufffman.h"


void huffman::PrintTreeInOrder(node_ptr roo)
{
   
	if (roo->left!=NULL&&roo->right!=NULL) {
		PrintTreeInOrder(roo->left);   
		cout << roo->id<<" "<<roo->freq <<endl;
		PrintTreeInOrder(roo->right);
     }
    }


huffman::huffman(string inFileName,string outFileFileName)
{
	this->inFileName = inFileName;
	this->outFileName = outFileFileName;
	size = 0;
	//初始化频率数组
	for (int i = 0; i < 256; i++)
	{
		node_array[i] = new huffmanNode();
	}
}
void huffman::create_node_array()
{
	for (int i = 0; i < size; i++)
	{
		node_array[buffer[i]]->id = buffer[i];
		node_array[buffer[i]]->freq ++;
	}
}


void huffman::createPriorityQueue()
{
	inFile.open(inFileName,ios::in );
	if(!inFile.is_open())
	{
		cout << "文件打开失败"<<endl;
	}

	//将文件中的内容保存在buffer中，得到文件size，关闭文件
	inFile.seekg(0,ifstream::end);
	size = (int)inFile.tellg();
	buffer =new unsigned char [size];
	inFile.seekg(0);
	inFile.read((char*)buffer,size);
	inFile.close();
	
	//构建节点数组并把非空节点push到优先队列中
	create_node_array();

	for (int i = 0; i < 256; i++)
	{
		if(node_array[i]->freq!=0)
		{
		   pQueue.push(node_array[i]);
		}
	}
}


//  从优先队列构建huffman树
void huffman::createHuffmanTree()
{
	while (pQueue.size() >1)
	{
		//取优先队列最顶元素保存在lowest中
		node_ptr  lowest= new huffmanNode();
		lowest = pQueue.top();
		pQueue.pop();

		//取次顶元素保存到secondLowest中
		node_ptr  secondLowest = new huffmanNode();
		secondLowest = pQueue.top();
		pQueue.pop();

		//将所取两元素分别作为左、右，构建新节点，插回优先队列
		node_ptr newParent = new huffmanNode();
		newParent->id = NULL;
		newParent->left = lowest;
		newParent->right = secondLowest;
		newParent->freq = lowest->freq +secondLowest->freq;
		pQueue.push(newParent);
	}

	//根据建立好的huffman 树计算对应的huffman编码
	root =pQueue.top();
	calculateHuffmanCode(root, root->code);
}



void huffman::calculateHuffmanCode(node_ptr treeRoot,string nodeCode)
{
	//如果左右为空，则为叶子，得到id对应的huffman编码，保存到huffmanTable中
	if(treeRoot->left ==NULL && treeRoot->right ==NULL)
	{
		pair<unsigned char,string> temp;
		temp.first = treeRoot->id;
		temp.second = nodeCode;
		huffmanTable.push_back(temp);
	}
	
	//非子页时，递归，左边nodeCode附加'0' ,右边附加'1'
	else 
	{
		string t1 =nodeCode ;
		string t2 =nodeCode;
		if(treeRoot->left != NULL)
		 	calculateHuffmanCode(treeRoot->left,t1.append(1,'0'));
		if(treeRoot->right !=NULL)
			  calculateHuffmanCode(treeRoot->right,t2.append(1,'1'));
	}
}

//用于将string类型的nodeCode转换为int类型
int stringToInt ( string s )
{
	int sum = 0;
	double two = 2.0;
	for(int i = 0;i < 8;i ++)
	{
		sum += int(pow(two,8 - i - 1) * (s[i] - '0'));
	}
	return sum;
}



void huffman::saveToFile()
{
	//以二进制打开和写入文件
	inFile.open(inFileName,ios::in|ios::binary);
	outFile.open ( outFileName , ios::app | ios::binary );
	
	if ( inFile.fail() || outFile.fail() )
	{
		cout << "打开文件失败"<<endl;
	}
	
	string buff = "";		//暂存取得的huffman编码
	unsigned char temp;		//暂存读入的字符
	
	while(inFile.peek() != EOF)
	{
		inFile.read((char *)&temp ,1);
		//cout<<"正在压缩……"<<endl;
		string s = "";

		//在huffmanTable中搜寻对应的huffman编码并附加到buff中
		for ( int i = 0 ; i < huffmanTable.size() ; i ++ ){
			if ( huffmanTable[i].first == temp ){
				s = huffmanTable[i].second;
				break;
			}
		}
		buff.append(s);

		//按8位写入
		while(buff.size() >= 8)
		{
			int tempD = stringToInt(buff);
			outFile.write((char *) &tempD,sizeof(unsigned char));
			buff.erase(0,8);
		}	
	}

	//处理剩余的字符
	if ( buff.size() != 0 ) 
	{
		while ( buff.size() != 8 ){
			buff.append("0");
		}

		int tempD = stringToInt(buff);
	    outFile.write((char *) &tempD,sizeof(unsigned char));
	}
	
	inFile.close();
	outFile.close();

}