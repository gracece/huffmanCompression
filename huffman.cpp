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
	//��ʼ��Ƶ������
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
		cout << "�ļ���ʧ��"<<endl;
	}

	//���ļ��е����ݱ�����buffer�У��õ��ļ�size���ر��ļ�
	inFile.seekg(0,ifstream::end);
	size = (int)inFile.tellg();
	buffer =new unsigned char [size];
	inFile.seekg(0);
	inFile.read((char*)buffer,size);
	inFile.close();
	
	//�����ڵ����鲢�ѷǿսڵ�push�����ȶ�����
	create_node_array();

	for (int i = 0; i < 256; i++)
	{
		if(node_array[i]->freq!=0)
		{
		   pQueue.push(node_array[i]);
		}
	}
}


//  �����ȶ��й���huffman��
void huffman::createHuffmanTree()
{
	while (pQueue.size() >1)
	{
		//ȡ���ȶ����Ԫ�ر�����lowest��
		node_ptr  lowest= new huffmanNode();
		lowest = pQueue.top();
		pQueue.pop();

		//ȡ�ζ�Ԫ�ر��浽secondLowest��
		node_ptr  secondLowest = new huffmanNode();
		secondLowest = pQueue.top();
		pQueue.pop();

		//����ȡ��Ԫ�طֱ���Ϊ���ң������½ڵ㣬������ȶ���
		node_ptr newParent = new huffmanNode();
		newParent->id = NULL;
		newParent->left = lowest;
		newParent->right = secondLowest;
		newParent->freq = lowest->freq +secondLowest->freq;
		pQueue.push(newParent);
	}

	//���ݽ����õ�huffman �������Ӧ��huffman����
	root =pQueue.top();
	calculateHuffmanCode(root, root->code);
}



void huffman::calculateHuffmanCode(node_ptr treeRoot,string nodeCode)
{
	//�������Ϊ�գ���ΪҶ�ӣ��õ�id��Ӧ��huffman���룬���浽huffmanTable��
	if(treeRoot->left ==NULL && treeRoot->right ==NULL)
	{
		pair<unsigned char,string> temp;
		temp.first = treeRoot->id;
		temp.second = nodeCode;
		huffmanTable.push_back(temp);
	}
	
	//����ҳʱ���ݹ飬���nodeCode����'0' ,�ұ߸���'1'
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

//���ڽ�string���͵�nodeCodeת��Ϊint����
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
	//�Զ����ƴ򿪺�д���ļ�
	inFile.open(inFileName,ios::in|ios::binary);
	outFile.open ( outFileName , ios::app | ios::binary );
	
	if ( inFile.fail() || outFile.fail() )
	{
		cout << "���ļ�ʧ��"<<endl;
	}
	
	string buff = "";		//�ݴ�ȡ�õ�huffman����
	unsigned char temp;		//�ݴ������ַ�
	
	while(inFile.peek() != EOF)
	{
		inFile.read((char *)&temp ,1);
		//cout<<"����ѹ������"<<endl;
		string s = "";

		//��huffmanTable����Ѱ��Ӧ��huffman���벢���ӵ�buff��
		for ( int i = 0 ; i < huffmanTable.size() ; i ++ ){
			if ( huffmanTable[i].first == temp ){
				s = huffmanTable[i].second;
				break;
			}
		}
		buff.append(s);

		//��8λд��
		while(buff.size() >= 8)
		{
			int tempD = stringToInt(buff);
			outFile.write((char *) &tempD,sizeof(unsigned char));
			buff.erase(0,8);
		}	
	}

	//����ʣ����ַ�
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