#include<fstream>
#include<iostream>
#include<string>
#include<queue>
#include<vector>
using namespace std;

//哈夫曼树节点
class huffmanNode
{
public:
huffmanNode()
{
id = NULL;	
freq = 0;	
left = NULL;	
right = NULL;	
parent = NULL;	
}
unsigned char id;	//对应ASCII码
int freq;	 //出现频率
string code;
huffmanNode* left;  //左节点指针
huffmanNode* right; //右节点指针
huffmanNode* parent;//父母节点指针（压缩未用到）

//符号重载用于指针间的复制
huffmanNode& operator=(const huffmanNode& rhs)
{
if (this != &rhs)
{
id = rhs.id;
freq= rhs.freq;
left = rhs.left;
right = rhs.right;
parent = rhs.parent;
}
return *this;
}
};
typedef huffmanNode* node_ptr;

//哈夫曼类
class huffman
{
protected:
node_ptr node_array [256];	//频率数组，下标与ascii码对应
int size;	 //文件大小
fstream inFile,	 //文件输入流
outFile;	 //文件输出流
node_ptr child,	     
 root,	 //哈夫曼树根节点
 parent;
string inFileName;	 //读取的文件名	
string outFileName;	 //输出的文件名

unsigned char * buffer;	 //将读取的文件流保存在buffer中
vector<pair<unsigned char,string> > huffmanTable;	//储存ASCII字符的对应huffman编码表

//比较类，用于优先队列的大小排序依据
class compare
{
public:
bool operator()(const node_ptr& c1,
const node_ptr& c2)const
{return (*c1).freq >(*c2).freq;}
};

//用优先队列保存huffman节点，按照频率大小 （compare类中定义规则）
priority_queue<node_ptr,vector<node_ptr>,compare>pQueue;



public:
huffman();

huffman(string inFileName,string outFileName);

//产生节点数组
void create_node_array();	

//构造优先队列
void createPriorityQueue();

//构造huffman数
void createHuffmanTree();

//计算huffman编码
void calculateHuffmanCode(node_ptr root,string code);

//保存到文件
void saveToFile();

//遍历数，用于检验huffman树正确性
void PrintTreeInOrder(node_ptr root);
};

