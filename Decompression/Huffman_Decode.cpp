#include <iostream> 
#include <cstdlib> 
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <bitset>

#pragma warning(disable : 4996)

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

// 문자열 복호화 함수
string decodeString(struct MinHeapNode* root, string str)
{
	string result = "";

	struct MinHeapNode* cur = root;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '0')
			cur = cur->left;

		else
			cur = cur->right;

		if (cur->left == NULL && cur->right == NULL)
		{
			result += cur->data;
			cur = root;
		}
	}

	return result;
}

// 파일 입력 함수
string readFile(string fd)
{
	ifstream infile(fd, ios::in | ios::binary);

	infile.seekg(0, std::ios::end);
	size_t size = infile.tellg();
	std::string buffer(size, ' ');
	infile.seekg(0);

	while (infile.read(&buffer[0], size), !infile.eof())
		// cout << buffer << endl;

	infile.close();

	return buffer;
}

// 텍스트 파일 출력함수
void writeText(string fd, string str)
{
	ofstream outfile(fd);

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

// split 함수 구현
vector<string> spliter(const string& str, const char& ch)
{
	string next;
	vector<string> result;

	int cnt = 0; // 인코딩된 문자열이 잘려나가는 오류 방지

	for (string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == ch && cnt <= 10) // 테스트 입력 데이터의 인코딩 체계의 한 부분을 담은 문자열은 10을 넘지 않는다
		{
			if (!next.empty())
			{
				result.push_back(next);
				next.clear();
			}
			cnt = 0;
		}

		else
		{
			next += *it;
			cnt++;
		}
	}

	if (!next.empty())
		result.push_back(next);

	return result;
}

// Huffman Tree re-build를 위한 함수 (문자들의 빈도수를 계산하여, 그에 맞는 전체 문자열 생성)
string mergeString(vector<string> subString)
{
	string result;

	for (int i = 0; i < subString.size() - 1; i++)
	{
		char str[8]; char ch; // 문자값
		int cnt = 0;          // 빈도수
		string temp_int;      // 임시 저장

		strcpy(str, subString[i].c_str()); // char형 배열로 복사

		ch = str[0]; // ch에는 문자값이 저장

		for (int i = 2; str[i] != NULL; i++)
			temp_int += str[i];

		cnt = atoi(temp_int.c_str()); // cnt에는 빈도수 저장

		for (int i = 0; i < cnt; i++) // 해당 문자값을 빈도수만큼 작성
			result += ch;
	}
	return result;
}

// 비트값을 String형으로 변환
string Bit2String(string str)
{
	string result;

	for (int i = 0; i < str.size(); i++)
		result += bitset<8>(str[i]).to_string();

	return result;
}

// 두 개의 텍스트 파일이 동일한지 확인하는 함수
void compareFile(string file1, string file2)
{
	ifstream f1(file1, ios::in), f2(file2, ios::in);
	char ch1, ch2;    // 비교용 char 변수
	bool flag = true; // 플래그 기본값 : TRUE

	while (1)
	{
		// 두 파일을 한 글자씩 비교
		ch1 = f1.get();
		ch2 = f2.get();

		if (ch1 != ch2) 
		{
			// 불일치시 flag를 false로 바꾸고, 반복문 break
			flag = false;
			break;
		}
		
		// 파일 끝에 도달하면 반복문 break
		if (ch1 == EOF || ch2 == EOF)
			break;
	}

	cout << "********** 파일 비교 완료 **********\n\n";

	f1.close();
	f2.close();

	if (flag)
		cout << file1 << "과 " << file2 << "는 [일치]합니다\n\n";

	else
		cout << file1 << "과 " << file2 << "는 [불일치]합니다\n\n";
}

int main()
{
	// fd : data.txt
	// fr : compress.hfb
	// fw : compress.hfd

	int n = 0;
	cout << "Enter num : "; cin >> n;

	string fr = "compress" + to_string(n) + ".hfb";   // 입력 파일명
	string fw = "compress" + to_string(n) + ".hfd"; // 출력 파일명

	string read = readFile(fr); // 파일 입력받기

	vector<string> result = spliter(read, '/'); // 인코딩 체계 헤더와 인코딩된 문자열 분리 과정

	string str = mergeString(result); // encodedstring이 일렬로 합쳐진 버전
	
	calcFreq(str, str.length()); // 빈도수 계산
	HuffmanCodes(str.length());  // Huffman tree re-build

	string enc_str = Bit2String(result[result.size() - 1]); // 실제 인코딩된 문자열
	string dec_str = decodeString(minHeap.top(), enc_str);  // 디코딩된 문자열

	int rm_ch = dec_str.size() - str.size() + 1;         // dec_str에 불필요한 문자열값의 크기 계산
	dec_str = dec_str.substr(0, dec_str.size() - rm_ch); // dec_str에 불필요한 문자열 제거

	// cout << "\n디코딩 결과 : " << dec_str << endl;
	cout << "\n********** 압축 해제 완료 **********\n\n";

	writeText(fw, dec_str); // hfd 파일 출력

	string fd = "data" + to_string(n) + ".txt";
	int data_Size = getFileSize(fd); // data 파일 크기
	int fw_Size = getFileSize(fw);   // decompress 파일 크기

	cout << "     " << fd << " 크기 : " << data_Size << " bytes\n";
	cout << " " << fw << " 크기 : " << fw_Size << " bytes\n\n";

	compareFile(fd, fw); // 동일한 파일인지 확인

	system("pause");
	return 0;
}