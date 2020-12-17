#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<math.h>



typedef int KeyType;
typedef int BOOL;
typedef char DataType;
#define MaxSize 100001

typedef struct entry           //数据元素
{
	KeyType key;				//排序关键字，KeyType应该为可比较类型
	DataType data;				//data包含数据元素中的其他数据项
}Entry;

typedef struct list {				//顺序表
	int n;				//待排序数据元素数量
	Entry D[MaxSize];		//静态数组存储数据元素
}List;

//简单选择排序
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
void Swap(Entry *D, int i, int j)     //交换顺序表中两个元素
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

//直接插入排序
void InsertSort(List* list)
{
	int i, j;   //i为待插入元素下标
	Entry insertItem;
	for (i = 1; i < list->n; i++)
	{
		insertItem = list->D[i];
		for (j = i - 1; j >= 0; j--)
		{
			//不断将有序序列中的元素向后移动，为待插入元素空出一个位置
			if (insertItem.key < list->D[j].key)
				list->D[j + 1] = list->D[j];
			else
				break;
		}
		list->D[j + 1] = insertItem;  //待插入元素有序存放至有序序列中
	}
}

//冒泡排序
void BubbleSort(List *list) {
	int i, j;                    //i标识每趟排序范围最后一个元素下标,每趟排序元素下标范围是0~i
	for (i = list->n - 1; i > 0; i--) {
		BOOL isSwap = FALSE;    //教材这里错了,应该放到第二层循环前
		for (j = 0; j < i; j++) {
			if (list->D[j].key > list->D[j + 1].key) {
				Swap(list->D,j,j+1);
				isSwap = TRUE;
			}
		}
		if (!isSwap) break;     //如果本趟排序没有发生元素交换,排序完成
	}
}


//快速排序算法
//序列划分方法
int Partition(List *list, int low, int high) {
	int i = low, j = high + 1;
	Entry pivot = list->D[low];                 //pivot是分割元素
	do {
		do i++;
		while (i<=high&&list->D[i].key < pivot.key);      //i前进
		do j--;
		while (list->D[j].key > pivot.key);      //j前进
		if (i < j) Swap(list->D,i,j);
	} while (i < j);
	Swap(list->D,low, j);
	return j;                                   //此时j是分割元素下标
}

//快速排序
void QuickSort(List *list, int low, int high) {   //快速排序的递归函数
	int k;
	if (low < high) {                            //当前待排序序列至少包含2个元素
		k = Partition(list, low, high);
		QuickSort(list, low, k - 1);
		QuickSort(list, k + 1, high);
	}
}

//函数重载
void QuickSort(List *list) 
{                   //快速排序算法的主调用函数
	QuickSort(list, 0, list->n - 1);
}

//两路合并排序算法
//序列两路合并方法
//n1和n2是两个子序列长度,low是第一个子序列第一个元素下标
void Merge(List *list, Entry *temp, int low, int n1, int n2) {
	int i = low, j = low + n1; //i,j初始时分别指向两个序列的第一个元素
	while (i <= low + n1 - 1 && j <= low + n1 + n2 - 1) {
		if (list->D[i].key <= list->D[j].key)
			*temp++ = list->D[i++];
		else *temp++ = list->D[j++];
	}
	while (i <= low + n1 - 1)
		*temp++ = list->D[i++];  //剩余元素直接拷贝至temp
	while (j <= low + n1 + n2 - 1)
		*temp++ = list->D[j++];  //剩余元素直接拷贝至temp
}


//两路合并排序算法
void MergeSort(List *list) {
	Entry temp[MaxSize];
	int low, n1, n2, i, size = 1;
	while (size < list->n) {
		low = 0;      //low是一对待合并序列中第一个序列的第一个元素下标
		while (low + size < list->n) {
			//low+size < list->n说明至少存在两个子序列需要合并
			n1 = size;
			if (low + size * 2 < list->n) n2 = size;  //计算第二个序列长度
			else n2 = list->n - low - size;
			Merge(list, temp, low, n1, n2);
			low += n1 + n2;  //确定下一对待合并序列中第一个序列的第一个元素下标
		}
		for (i = 0; i < list->n; i++) {
			list->D[i] = temp[i];  //复制一趟合并排序结果
		}
		size *= 2;  //子序列长度翻倍
	}
}

//堆排序算法
void AdjustDown(Entry heap[], int current, int border)
{
	int p = current;
	int minChild;
	Entry temp;
	while (2 * p + 1 <= border)//若p不是叶结点，则执行调整
	{
		if ((2 * p + 2 <= border) && (heap[2 * p + 1].key > heap[2 * p + 2].key))
			minChild = 2 * p + 2;//右孩子存在，且较小，则minChild指向p的右孩子
		else
			minChild = 2 * p + 1;//右孩子不存在，或较大，则minChild指向p的左孩子
		if (heap[p] .key<= heap[minChild].key)
			break;  //若当前结点不大于其最小的孩子，则调整结束
		else  //否则将p和其最小孩子交换
		{
			temp = heap[p];
			heap[p] = heap[minChild];
			heap[minChild] = temp;
			p = minChild;//设置下轮循环待考察的元素的位置（即当前下移元素的位置）
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
	for (i = hp->n - 1; i > 0; i--)//i指向当前堆的堆底元素
	{
		Swap(hp->D,0,i);
		AdjustDown(hp->D, 0, i - 1);
	}
}



//初始化
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


//计算排序所需要时间
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
	printf("时间%.10f秒\n", len);
	StartTime = clock();
	InsertSort(&b);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[1] = len;
	printf("时间%.10f秒\n", len);
	StartTime = clock();
	BubbleSort(&c);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[2] = len;
	printf("时间%.10f秒\n", len);
	StartTime = clock();
	QuickSort(&d);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[3] = len;
	printf("时间%.10f秒\n", len);
	StartTime = clock();
	MergeSort(&e);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[4] = len;
	printf("时间%.10f秒\n", len);
	StartTime = clock();
	HeapSort(&f);
	EndTime = clock();
	len = (double(EndTime - StartTime)) / CLOCKS_PER_SEC;;
	record[5] = len;
	printf("时间%.10f秒\n\n", len);
	system("pause");
}


int main()
{
	List a,b,c,d,e,f;
	Init(&a);
	scanf_s("%d", &a.n);
	srand((unsigned)time(NULL));//先种种子
	for (int i = 0; i < a.n; i++)
		a.D[i].key = rand() % 100000 + 1;//产生0~10000的随机数
	b = a;
	c = a;
	d = a;
	e = a;
	f = a;
	SortAll(a,b,c,d,e,f);
	printf("a.n=%d", a.n);
	return 0;
}
