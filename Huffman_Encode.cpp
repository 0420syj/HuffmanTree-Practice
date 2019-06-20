#include <iostream> 
#include <cstdlib> 
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <queue>

using namespace std;

map<char, string> encode;
map<char, int> freq;

struct MinHeapNode
{
	char data;
	int freq;
	MinHeapNode *left, *right;

	MinHeapNode(char data, int freq)
	{
		left = right = NULL;
		this->data = data;
		this->freq = freq;
	}
};

struct compare
{
	bool operator()(MinHeapNode* left, MinHeapNode* right)
	{
		return (left->freq > right->freq);
	};
};

void printEncode(struct MinHeapNode* root, string str)
{
	if (!root)
		return;
	if (root->data != '$')
		cout << root->data << ": " << str << "\n";
	printEncode(root->left, str + "0");
	printEncode(root->right, str + "1");
}

void storeEncode(struct MinHeapNode* root, string str)
{
	if (root == NULL)
		return;
	if (root->data != '$')
		encode[root->data] = str;
	storeEncode(root->left, str + "0");
	storeEncode(root->right, str + "1");
}

priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

void HuffmanCodes(int size)
{
	struct MinHeapNode *left, *right, *top;
	for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
		minHeap.push(new MinHeapNode(v->first, v->second));
	while (minHeap.size() != 1)
	{
		left = minHeap.top();
		minHeap.pop();
		right = minHeap.top();
		minHeap.pop();

		top = new MinHeapNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;
		minHeap.push(top);
	}
	storeEncode(minHeap.top(), "");
}

// 빈도수 계산 함수
void calcFreq(string str, int n)
{
	for (int i = 0; i < str.size(); i++)
		freq[str[i]]++;
}

// 파일 입력 함수
string readFile(string fd)
{
	ifstream infile;
	infile.open(fd);

	string inputString;
	string sumString;

	while (getline(infile, inputString))
		sumString += inputString + '\n';

	infile.close();

	// 문자열 갯수 출력
	// cout << sumString.length();

	return sumString;
}

// 인코딩 헤더 삽입 함수
void putHeader(ofstream &of, string header)
{
	for (auto i : header)
		of << i << ':' << freq[i] << '/';
}

// 이진파일 출력함수
void writeBin(string fd, string header, string str, bool makeHeader)
{
	ofstream outfile(fd, ofstream::binary);

	char array[100000] = { 0 };
	int cnt = 0;

	for (int i = 0; i < str.length(); i += 8, cnt++)
		array[i / 8] = stoi(str.substr(i, 8), nullptr, 2);

	if (makeHeader) // 참이면 인코딩 헤더 삽입
		putHeader(outfile, header);

	for (int i = 0; i < cnt; ++i)
		outfile.write((char*)(array + i * sizeof(array[0])), sizeof(array[0]));

	outfile.close();
}

// 텍스트 파일 출력함수
void writeText(string fd, string header, string str)
{
	ofstream outfile(fd);

	putHeader(outfile, header);

	outfile << str;
	outfile.close();
}

// 파일 크기 계산함수
int getFileSize(const string &fileName)
{
	ifstream file(fileName.c_str(), ifstream::in | ifstream::binary);

	if (!file.is_open())
		return -1;

	file.seekg(0, ios::end);
	int fileSize = file.tellg();
	file.close();

	return fileSize;
}

int main()
{
	int n = 0;
	cout << "Enter num : ";  cin >> n;

	// 특정 위치에 data1 ~ data5 라는 이름의 txt 파일이 있다고 가정
	// 1~5 사이의 숫자 n을 사용자로부터 입력받아 data(n).txt를 파일 입력
	// 압축하여 같은 위치에 compress(n) 라는 이름의 hft, hfb, hfx 파일 출력
	string fr = "data" + to_string(n) + ".txt";
	string fwt = "compress" + to_string(n) + ".hft";
	string fwb = "compress" + to_string(n) + ".hfb";
	//string fwx = "compress" + to_string(n) + ".hfx";

	string str = readFile(fr);

	string encodedString, decodedString;
	calcFreq(str, str.length());
	HuffmanCodes(str.length());

	string chArray;

	for (auto v = encode.begin(); v != encode.end(); v++)
		chArray += v->first;

	for (auto i : str)
		encodedString += encode[i];

	writeText(fwt, chArray, encodedString);       // text 파일 작성   (헤더 포함 O)
	writeBin(fwb, chArray, encodedString, true);  // binary 파일 작성 (헤더 포함 O)
	//writeBin(fwx, chArray, encodedString, false); // binary 파일 작성 (헤더 포함 X)

	int dataSize = 0, hftSize = 0, hfbSize = 0;
	// int hfxSize = 0;

	dataSize = getFileSize(fr);
	hftSize = getFileSize(fwt);
	hfbSize = getFileSize(fwb);
	//hfxSize = getFileSize(fwx);

	cout << "\n*********************" << endl;
	cout << " data" << to_string(n) << ".txt 압축중... " << endl;
	cout << "*********************\n" << endl;

	cout << "원본 파일 크기 : " << dataSize << " bytes\n";
	cout << "HFT 파일 크기 : " << hftSize << " bytes\n";

	cout.setf(ios::fixed);
	cout.precision(2);

	cout << "HFB 파일 크기 : " << hfbSize << " bytes " << "(압축률 : " << (double)hfbSize / dataSize * 100 << "%)\n\n";
	//cout << "HFX 파일 크기 : " << hfxSize << " bytes " << "(압축률 : " << (double)hfxSize / dataSize * 100 << "%)\n\n";

	system("pause");
	return 0;
}
