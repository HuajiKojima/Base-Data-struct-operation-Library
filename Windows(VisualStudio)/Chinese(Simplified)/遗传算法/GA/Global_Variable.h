#pragma once
#include <fstream>
#include <io.h>

#define MAX_VERTEX_NUM 5		//图中所有点的个数;
#define CHROMES_NUM 20			//参与运算的染色体个数;
#define SWAP_START_POINT 2		//染色体交叉运算开始点位;
#define MAX_INHERATE_NUM 10		//最大允许进化代数;
//#define MAX_LOOP_NUM 10			//最大允许循环数，循环计数器超出此循环数则会被认为再无支路可供遍历;

using namespace std;

//邻接表模型数据类型定义;
struct Link_And_Weight
{
	int VertexName;
	int Weight;
};
struct Graph
{
	int Link_Point_Num;
	Link_And_Weight Link_Vertex[MAX_VERTEX_NUM - 1];
};

//染色体模型数据类型定义;
struct Chrome 
{
	int Info[MAX_VERTEX_NUM];
	int Weight_Sum;
};

//全局变量：构建邻接表内存模型G以及用来进行GA运算的染色体模型Chromes;
Graph G[MAX_VERTEX_NUM];
Chrome Chromes[CHROMES_NUM];

//文件调用部分;
//依据用户定义创建图的邻接表模型并存入文件中;
void FWrite_Data()
{
	//打开相应数据文件;
	ofstream FWrite_In;
	FWrite_In.open("Test.dat");

	//邻接表初始化;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		G[i].Link_Point_Num = 0;
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			G[i].Link_Vertex[j].VertexName = -1;
			G[i].Link_Vertex[j].Weight = -1;
		}
	}

	//在内存区域创建邻接表模型;
	int Judge_Val = 1;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		cout << "输入序号为" << i << "的点的邻接点信息:" << endl;
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			cout << "邻接点信息：";
			cin >> G[i].Link_Vertex[j].VertexName;
			cout << "边权值：";
			cin >> G[i].Link_Vertex[j].Weight;
			cout << "继续？（1/0）";
			cin >> Judge_Val;
			if (Judge_Val == 0)
			{
				break;
			}
			G[i].Link_Point_Num += 1;
		}
	}

	//将内存区域的邻接表模型转存至指定文件中;
	char Transfer[MAX_VERTEX_NUM];
	char Transfer_Num;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			Transfer[j] = G[i].Link_Vertex[j].VertexName;
			FWrite_In << Transfer[j];
		}
	}
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			Transfer[j] = G[i].Link_Vertex[j].Weight;
			FWrite_In << Transfer[j];
		}
	}
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		Transfer_Num = G[i].Link_Point_Num;
		FWrite_In << Transfer_Num;
	}

	//关闭已打开的文件;
	FWrite_In.close();
}
//取出存入相应文件中的邻接表相应数据并加载入内存模型中;
void FRead_Data() 
{
	ifstream FRead_Out;
	FRead_Out.open("Test.dat");
	char Read_Chara;

	//邻接表初始化;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			G[i].Link_Vertex[j].VertexName = -1;
			G[i].Link_Vertex[j].Weight = -1;
		}
	}

	//将文件中的相应数据载入内存模型;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			FRead_Out >> Read_Chara;
			G[i].Link_Vertex[j].VertexName = int(Read_Chara);
		}
	}
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		for (int j = 0; j < (MAX_VERTEX_NUM - 1); j++)
		{
			FRead_Out >> Read_Chara;
			G[i].Link_Vertex[j].Weight = int(Read_Chara);
		}
	}
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		FRead_Out >> Read_Chara;
		G[i].Link_Point_Num = int(Read_Chara);
	}

	//关闭已打开的文件;
	FRead_Out.close();
}
//判断是否存在先前已由用户定义好的邻接表文件并确定如何调用;
void FIs_Open()
{
	ifstream Judge_FOpen;
	Judge_FOpen.open("Test.dat");
	if (Judge_FOpen.is_open())
	{
		FRead_Data();
	}
	else
	{
		FWrite_Data();
	}
}
//寻找特定类型数组中特定元素;
bool bArray_Search(int Target_Array[], int Length, int Search_Num, int Array_Sign);
bool bLink_Array_Search(Link_And_Weight Target_Array[], int Length, int Search_Num);
int iLink_Array_Search(Link_And_Weight Target_Array[], int Length, int Search_Num);
//查找指定整型数组中的最值，默认查找最大值，若要查找最小值，请为最后一个参数传入-1;
int Array_BV_Num(int Target_Array[], int Length, int Operation_Num = 1);

//以下是遗传算法各调用函数声明;
//染色体构建函数;
void Chrome_Create(int Start_Sign);
//检查特定染色体是否符合哈密顿回路;
bool Chrome_Check_IsHamilton(int Start_Point, int Chrome_Sign);
//检查特定染色体是否有重复信息子出现;
bool Chrome_Check_IsDouble(int Chrome_Sign);
//将特定染色体的信息覆盖到另一个指定染色体上;
void Chrome_Info_Paste(int Chrome_SignA, int Chrome_SignB);
//染色体交叉;
void Chrome_Info_Swap(int Chrome_SignA, int Chrome_SignB);
//染色体变异;
void Chrome_Mutation();
//遗传算法主函数;
void GA_Main(int Start_Sign);
