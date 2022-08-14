#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#ifndef _PSICA_H
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#ifndef EOF
#define EOF (-1)
#endif
#ifndef EEOF
#define EEOF (-2)
#endif
#ifndef NULL
#define NULL
#endif
#ifndef MAX_BUFFER
#define MAX_BUFFER 5000
#endif
#ifndef MAX_PATH
#ifdef _MAX_PATH
#define MAX_PATH _MAX_PATH
#else
#define MAX_PATH 260
#endif
#endif
#define N 26
#define n 11
#define r 10
#define beta 50
#define TimeToTest 50
#endif // _PSICA_H
using namespace std;
typedef unsigned long long int Element;
typedef const void* CPVOID;
vector<int> hashpi, archashpi;


/* 子函数 */
void init_hashpi()
{
	for (int i = 0; i < beta; ++i)
	{
		hashpi.push_back(i);
		archashpi.push_back(NULL);// initial
	}
	random_shuffle(hashpi.begin(), hashpi.end());
	for (int i = 0; i < beta; ++i)
		archashpi[hashpi[i]] = i;
	return;
}

int pi(int index)
{
	return hashpi[index % beta];
}

int arcpi(int value)
{
	return archashpi[value % beta];
}

Element getRandom()//获取随机数
{
	Element random = rand();
	random <<= 32;
	random += rand();
	return random;
}

void getInput(Element array[], int size)//获得输入
{
	char buffer[MAX_BUFFER] = { 0 }, cTmp[MAX_PATH] = { 0 };
	rewind(stdin);
	fflush(stdin);
	fgets(buffer, MAX_BUFFER, stdin);
	int cIndex = 0, eIndex = 0;
	for (int i = 0; i < MAX_BUFFER; ++i)
		if (buffer[i] >= '0' && buffer[i] <= '9')
			cTmp[cIndex++] = buffer[i];
		else if (cIndex)
		{
			char* endPtr;
			array[eIndex++] = strtoull(cTmp, &endPtr, 0);
			if (eIndex >= size)
				return;
			cIndex = 0;// Rewind cIndex
			memset(cTmp, 0, strlen(cTmp));// Rewind cTmp
		}
	return;
}

Element r_i(Element ele, int i)//哈希函数
{
	return ele << i;
}

int compare(CPVOID a, CPVOID b)//比较函数
{
	return (int)(*(Element*)a - *(Element*)b);
}

int BinarySearch(Element array_lists[], int nBegin, int nEnd, Element target, unsigned int& compareCount)
{
	if (nBegin > nEnd)
		return EOF;//未能找到目标
	int nMid = (nBegin + nEnd) >> 1;//使用位运算加速
	++compareCount;
	if (array_lists[nMid] == target)//找到目标
		return nMid;
	else if (array_lists[nMid] > target)//分而治之
		return BinarySearch(array_lists, nBegin, nMid - 1, target, compareCount);
	else
		return BinarySearch(array_lists, nMid + 1, nEnd, target, compareCount);
}

Element encode(Element a, Element b)
{
	return a + b;
}

Element decode(Element a, Element b)
{
	return a - b;
}


/* 类 */
class Receiver_R
{
private:
	Element X[n] = { 0 };
	Element k = 0;
	Element X_c[beta] = { 0 };
	Element Z[beta] = { 0 };
	Element W[beta] = { 0 };
	Element U[beta] = { 0 };
	Element Z_pi[beta] = { 0 };

public:
	void input_X()
	{
		getInput(this->X, n);
		return;
	}
	void auto_input_X()
	{
		int index = 0;
		set<Element> s;
		while (s.size() < n)
			s.insert(getRandom());
		for (auto ele = s.begin(); ele != s.end(); ++ele)
			this->X[index++] = *ele;
		return;
	}
	void choose_k()
	{
		this->k = getRandom();
		return;
	}
	Element send_k()
	{
		return this->k;
	}
	void hash_X_to_X_c()
	{
		for (int i = 0; i < n; ++i)
		{
			int index = r_i(this->X[i], 1) % beta;
			if (0 != this->X_c[index])// already exist
			{
				int new_index = r_i(this->X_c[index], 2) % beta;
				if (0 != this->X_c[new_index])// still already exist
					;// abundant
				else
					this->X_c[new_index] = this->X_c[index];
			}
			else
				this->X_c[index] = X[i];
		}
		return;
	}
	Element* help_calc_Z_pi()
	{
		for (int i = 0; i < beta; ++i)
			this->Z_pi[i] = this->X_c[pi(i)] ^ this->Z[i];
		return this->Z_pi;
	}
	void printArray()
	{
		cout << "X = { " << this->X[0];
		for (int i = 1; i < n; ++i)
			cout << ", " << this->X[i];
		cout << " }" << endl << endl;
		cout << "X_c: " << endl;
		for (int i = 0; i < beta; ++i)
			if (this->X_c[i])
				cout << "X_c[" << i << "] = " << this->X_c[i] << endl;
		cout << endl;
		return;
	}
	void obtain_Z()
	{
		for (int i = 0; i < beta; ++i)
			this->Z[i] = this->X_c[i];
		return;
	}
	Element* send_Z()
	{
		return this->Z;
	}
	void receive_W(Element* W)
	{
		for (int i = 0; i < beta; ++i)
			this->W[i] = *(W + i);
		return;
	}
	void generate_U()
	{
		for (int i = 0; i < beta; ++i)
			this->U[i] = getRandom();
		return;
	}
	void printIntersection()
	{
		Element intersection[beta] = { 0 };
		int index = 0;
		unsigned int comparation = 0;
		qsort(this->W, beta, sizeof(Element), compare);
		qsort(this->U, beta, sizeof(Element), compare);
		for (int i = 0; i < beta; ++i)
			if (this->W[i] && BinarySearch(this->U, 0, beta - 1, this->W[i], comparation))
				intersection[index++] = this->W[i];
		if (intersection[0])
		{
			cout << "| U ∩ W | = | { " << intersection[0];
			for (int i = 1; i < beta; ++i)
				if (intersection[i])
					cout << ", " << intersection[i];
				else
				{
					cout << " } | = " << index << endl;
					break;
				}
		}
		else
			cout << "| U ∩ W | = 0" << endl;
		return;
	}
	void printSize()
	{
		cout << "sizeof(R) = " << sizeof(this) << " KB" << endl;
		cout << "\tsizeof(R.X) = " << sizeof(this->X) << " B" << endl;
		cout << "\tsizeof(R.k) = " << sizeof(this->k) << " B" << endl;
		cout << "\tsizeof(R.X_c) = " << sizeof(this->X_c) << " B" << endl;
		cout << "\tsizeof(R.Z) = " << sizeof(this->Z) << " B" << endl;
		cout << "\tsizeof(R.W) = " << sizeof(this->W) << " B" << endl;
		cout << "\tsizeof(R.U) = " << sizeof(this->U) << " B" << endl;
		cout << "\tsizeof(R.Z_pi) = " << sizeof(this->Z_pi) << " B" << endl;
		return;
	}
};
Receiver_R R;

class Sender_S
{
private:
	Element Y[N] = { 0 };
	Element k = 0;
	Element V[beta] = { 0 };
	Element Z_pi[beta] = { 0 };
	Element T[N] = { 0 };

public:
	void input_Y()
	{
		getInput(this->Y, N);
	}
	void auto_input_Y()
	{
		int index = 0;
		set<Element> s;
		while (s.size() < N)
			s.insert(getRandom());
		for (auto ele = s.begin(); ele != s.end(); ++ele)
			this->Y[index++] = *ele;
		return;
	}
	void receive_k(Element k)
	{
		this->k = k;
		return;
	}
	void rand_V()
	{
		for (int i = 0; i < beta; ++i)
			V[i] = this->k - rand();
		return;
	}
	void calc_Z_pi(Element* Z)
	{
		for (int i = 0; i < beta; ++i)
			this->Z_pi[i] = *(Z + i);
		return;
	}
	void calc_T()
	{
		for (int i = 0; i < N; ++i)
		{
			Element q_j = 0, I_i = 0;
			for (int j = 0; j < r; ++j)
			{
				q_j = arcpi(r_i(this->Y[i], j) % beta);
				I_i = this->Y[i] ^ this->Z_pi[q_j];
			}
			T[i] = encode(I_i, this->V[i]);
		}
		return;
	}
	Element* send_T()
	{
		return this->T;
	}
	void printSize()
	{
		cout << "sizeof(S) = " << sizeof(this) << " KB" << endl;
		cout << "\tsizeof(S.Y) = " << sizeof(this->Y) << " B" << endl;
		cout << "\tsizeof(S.k) = " << sizeof(this->k) << " B" << endl;
		cout << "\tsizeof(S.V) = " << sizeof(this->V) << " B" << endl;
		cout << "\tsizeof(S.Z_pi) = " << sizeof(this->Z_pi) << " B" << endl;
		cout << "\tsizeof(S.T) = " << sizeof(this->T) << " B" << endl;
		return;
	}
};
Sender_S S;

class Cloud_C
{
private:
	Element Z[beta] = { 0 };
	Element T[N] = { 0 };
	Element W[beta] = { 0 };

public:
	void receive_Z(Element* Z)
	{
		for (int i = 0; i < beta; ++i)
			this->Z[i] = *(Z + i);
		return;
	}
	void receive_T(Element* T)
	{
		for (int i = 0; i < N; ++i)
		{
			this->T[i] = *(T + i);
			W[i] = decode(this->T[i], this->Z[i]);
		}
		return;
	}
	Element* send_W()
	{
		return this->W;
	}
	void printSize()
	{
		cout << "sizeof(C) = " << sizeof(this) << " KB" << endl;
		cout << "\tsizeof(C.Z) = " << sizeof(this->Z) << " B" << endl;
		cout << "\tsizeof(C.T) = " << sizeof(this->T) << " B" << endl;
		cout << "\tsizeof(C.W) = " << sizeof(this->W) << " B" << endl;
		return;
	}
};
Cloud_C C;


/* 主函数 */
void initial(bool isAuto)
{
	init_hashpi();// setup pi
	if (isAuto)
	{
		S.auto_input_Y();
		R.auto_input_X();
	}
	else
	{
		cout << "Please input array Y with size " << N << ": " << endl;
		S.input_Y();// Sender S has input Y
		cout << endl;
		cout << "Please input array X with size " << n << ": " << endl;
		R.input_X();// Sender R has input X
		cout << endl;
	}
	return;
}

void setup()
{
	R.choose_k();// The receiver chooses a random PRG key k
	S.receive_k(R.send_k());// k is sent to S
	S.rand_V();// rand beta
	return;
}

void distribution()
{
	R.hash_X_to_X_c();
	R.printArray();
	R.obtain_Z();// ss1
	S.calc_Z_pi(R.help_calc_Z_pi());// ss2
	C.receive_Z(R.send_Z());// Z is sent to C
	return;
}

void computation()
{
	S.calc_T();
	C.receive_T(S.send_T());// T is sent to C
	R.receive_W(C.send_W());// W is sent to R
	R.generate_U();
	R.printIntersection();
	return;
}



/* main 函数 */
void test()
{
	time_t t;
	srand((unsigned int)time(&t));
	initial(true);
	setup();
	distribution();
	computation();
	return;
}

int main()
{
	clock_t start_time = clock();
	for (int i = 0; i < TimeToTest; ++i)
	{
		cout << "/**************************************** Time: " << i + 1 << " ****************************************/" << endl;
		test();
		cout << endl << endl;
	}
	clock_t end_time = clock();
	cout << endl;
	cout << "Time: " << end_time - start_time << " / " << TimeToTest << " = " << (double)(end_time - start_time) / TimeToTest << "ms" << endl;
	R.printSize();
	S.printSize();
	C.printSize();
	cout << endl;
	return EXIT_SUCCESS;
}