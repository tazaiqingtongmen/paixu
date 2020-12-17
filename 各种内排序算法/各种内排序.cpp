#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<math.h>



typedef int KeyType;
typedef int BOOL;
typedef char DataType;
#define MaxSize 100001

typedef struct entry           //����Ԫ��
{
	KeyType key;				//����ؼ��֣�KeyTypeӦ��Ϊ�ɱȽ�����
	DataType data;				//data��������Ԫ���е�����������
}Entry;

typedef struct list {				//˳���
	int n;				//����������Ԫ������
	Entry D[MaxSize];		//��̬����洢����Ԫ��
}List;

//��ѡ������
int FindMin(List list, int startIndex)
{
	int i, minIndex = startIndex;
	for (i = startIndex + 1; i < list.n; i++)
	{
		if (list.D[i].key < list.D[minIndex].key)
			minIndex = i;
	}
	return minIndex;
}
void Swap(Entry *D, int i, int j)     //����˳���������Ԫ��
{
	Entry temp;
	if (i == j)
		return;
	temp = *(D + i);
	*(D + i) = *(D + j);
	*(D + j) = temp;
}
void SelectSort(List *list)
{
	int minIndex, startIndex = 0;
	while (startIndex < list->n - 1)
	{
		minIndex = FindMin(*list, startIndex);
		Swap(list->D, startIndex, minIndex);
		startIndex++;
	}
}

//ֱ�Ӳ�������
void InsertSort(List* list)
{
	int i, j;   //iΪ������Ԫ���±�
	Entry insertItem;
	for (i = 1; i < list->n; i++)
	{
		insertItem = list->D[i];
		for (j = i - 1; j >= 0; j--)
		{
			//���Ͻ����������е�Ԫ������ƶ���Ϊ������Ԫ�ؿճ�һ��λ��
			if (insertItem.key < list->D[j].key)
				list->D[j + 1] = list->D[j];
			else
				break;
		}
		list->D[j + 1] = insertItem;  //������Ԫ��������������������
	}
}

//ð������
void BubbleSort(List *list) {
	int i, j;                    //i��ʶÿ������Χ���һ��Ԫ���±�,ÿ������Ԫ���±귶Χ��0~i
	for (i = list->n - 1; i > 0; i--) {
		BOOL isSwap = FALSE;    //�̲��������,Ӧ�÷ŵ��ڶ���ѭ��ǰ
		for (j = 0; j < i; j++) {
			if (list->D[j].key > list->D[j + 1].key) {
				Swap(list->D,j,j+1);
				isSwap = TRUE;
			}
		}
		if (!isSwap) break;     //�����������û�з���Ԫ�ؽ���,�������
	}
}


//���������㷨
//���л��ַ���
int Partition(List *list, int low, int high) {
	int i = low, j = high + 1;
	Entry pivot = list->D[low];                 //pivot�Ƿָ�Ԫ��
	do {
		do i++;
		while (i<=high&&list->D[i].key < pivot.key);      //iǰ��
		do j--;
		while (list->D[j].key > pivot.key);      //jǰ��
		if (i < j) Swap(list->D,i,j);
	} while (i < j);
	Swap(list->D,low, j);
	return j;                                   //��ʱj�Ƿָ�Ԫ���±�
}

//��������
void QuickSort(List *list, int low, int high) {   //��������ĵݹ麯��
	int k;
	if (low < high) {                            //��ǰ�������������ٰ���2��Ԫ��
		k = Partition(list, low, high);
		QuickSort(list, low, k - 1);
		QuickSort(list, k + 1, high);
	}
}

//��������
void QuickSort(List *list) 
{                   //���������㷨�������ú���
	QuickSort(list, 0, list->n - 1);
}

//��·�ϲ������㷨
//������·�ϲ�����
//n1��n2�����������г���,low�ǵ�һ�������е�һ��Ԫ���±�
void Merge(List *list, Entry *temp, int low, int n1, int n2) {
	int i = low, j = low + n1; //i,j��ʼʱ�ֱ�ָ���������еĵ�һ��Ԫ��
	while (i <= low + n1 - 1 && j <= low + n1 + n2 - 1) {
		if (list->D[i].key <= list->D[j].key)
			*temp++ = list->D[i++];
		else *temp++ = list->D[j++];
	}
	while (i <= low + n1 - 1)
		*temp++ = list->D[i++];  //ʣ��Ԫ��ֱ�ӿ�����temp
	while (j <= low + n1 + n2 - 1)
		*temp++ = list->D[j++];  //ʣ��Ԫ��ֱ�ӿ�����temp
}


//��·�ϲ������㷨
void MergeSort(List *list) {
	Entry temp[MaxSize];
	int low, n1, n2, i, size = 1;
	while (size < list->n) {
		low = 0;      //low��һ�Դ��ϲ������е�һ�����еĵ�һ��Ԫ���±�
		while (low + size < list->n) {
			//low+size < list->n˵�����ٴ���������������Ҫ�ϲ�
			n1 = size;
			if (low + size * 2 < list->n) n2 = size;  //����ڶ������г���
			else n2 = list->n - low - size;
			Merge(list, temp, low, n1, n2);
			low += n1 + n2;  //ȷ����һ�Դ��ϲ������е�һ�����еĵ�һ��Ԫ���±�
		}
		for (i = 0; i < list->n; i++) {
			list->D[i] = temp[i];  //����һ�˺ϲ�������
		}
		size *= 2;  //�����г��ȷ���
	}
}

//�������㷨
void AdjustDown(Entry heap[], int current, int border)
{
	int p = current;
	int minChild;
	Entry temp;
	while (2 * p + 1 <= border)//��p����Ҷ��㣬��ִ�е���
	{
		if ((2 * p + 2 <= border) && (heap[2 * p + 1].key > heap[2 * p + 2].key))
			minChild = 2 * p + 2;//�Һ��Ӵ��ڣ��ҽ�С����minChildָ��p���Һ���
		else
			minChild = 2 * p + 1;//�Һ��Ӳ����ڣ���ϴ���minChildָ��p������
		if (heap[p] .key<= heap[minChild].key)
			break;  //����ǰ��㲻��������С�ĺ��ӣ����������
		else  //����p������С���ӽ���
		{
			temp = heap[p];
			heap[p] = heap[minChild];
			heap[minChild] = temp;
			p = minChild;//��������ѭ���������Ԫ�ص�λ�ã�����ǰ����Ԫ�ص�λ�ã�
		}
	}
}

void CreateHeap(Entry heap[], int n)
{
	int i;
	for (i = (n - 2) / 2; i > -1; i--)
		AdjustDown(heap, i, n - 1);
}

void HeapSort(List *hp)
{
	int i;
	for (i = (hp->n - 2) / 2; i >= 0; i--)
		AdjustDown(hp->D, i, hp->n - 1);
	for (i = hp->n - 1; i > 0; i--)//iָ��ǰ�ѵĶѵ�Ԫ��
	{
		Swap(hp->D,0,i);
		AdjustDown(hp->D, 0, i - 1);
	}
}



//��ʼ��
int Init(List* l)
{
	int i;
	l->n=0;
	for (i = 0; i < MaxSize; i++)
	{
		l->D[i].key = 0;
		l->D[i].data = '0';
	}
	if (!l->D)
		return FALSE;
	return TRUE;
}


//������������Ҫʱ��
void SortAll(List a,List b,List c,List d,List e,List f)
{
	system("cls");
	clock_t StartTime, EndTime;
	double record[15];
	StartTime = clock();
	SelectSort(&a);
	EndTime = clock();
	double len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[0] = len;
	printf("ʱ��%.10f��\n", len);
	StartTime = clock();
	InsertSort(&b);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[1] = len;
	printf("ʱ��%.10f��\n", len);
	StartTime = clock();
	BubbleSort(&c);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[2] = len;
	printf("ʱ��%.10f��\n", len);
	StartTime = clock();
	QuickSort(&d);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[3] = len;
	printf("ʱ��%.10f��\n", len);
	StartTime = clock();
	MergeSort(&e);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[4] = len;
	printf("ʱ��%.10f��\n", len);
	StartTime = clock();
	HeapSort(&f);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[5] = len;
	printf("ʱ��%.10f��\n\n", len);
	system("pause");
}


int main()
{
	List a,b,c,d,e,f;
	Init(&a);
	scanf_s("%d", &a.n);
	srand((unsigned)time(NULL));//��������
	for (int i = 0; i < a.n; i++)
		a.D[i].key = rand() % 100000 + 1;//����0~10000�������
	b = a;
	c = a;
	d = a;
	e = a;
	f = a;
	SortAll(a,b,c,d,e,f);
	printf("a.n=%d", a.n);
	return 0;
}
