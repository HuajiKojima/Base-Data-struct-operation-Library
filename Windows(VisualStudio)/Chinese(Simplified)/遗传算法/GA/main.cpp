#include <iostream>
#include <ctime>
#include <cstdlib>
#include "windows.h"

#include "Global_Variable.h"


using namespace std;

bool bArray_Search(int Target_Array[], int Length, int Search_Num, int Array_Sign) 
{
	for (int i = 0; i < Length; i++)
	{
		if ((Target_Array[i]==Search_Num)&&(i != Array_Sign))
		{
			return true;
		}
	}
	return false;
}

bool bLink_Array_Search(Link_And_Weight Target_Array[], int Length, int Search_Num)
{
	for (int i = 0; i < Length; i++)
	{
		if (Target_Array[i].VertexName == Search_Num)
		{
			return true;
		}
	}
	return false;
}

int iLink_Array_Search(Link_And_Weight Target_Array[], int Length, int Search_Num)
{
	for (int i = 0; i < Length; i++)
	{
		if (Target_Array[i].VertexName == Search_Num)
		{
			return i;
		}
	}
	return -1;
}

int Array_BV_Num(int Target_Array[], int Length, int Operation_Num)
{
	for (int i = 0; i < Length; i++)
	{
		Target_Array[i] = Target_Array[i] * Operation_Num;
	}
	int Min_Num = -(2*MAX_VERTEX_NUM);
	for (int i = 0; i < Length; i++)
	{
		if (Target_Array[i] > Min_Num)
		{
			Min_Num = Target_Array[i];
		}
	}
	return (Min_Num*Operation_Num);
}

void Chrome_Create(int Start_Sign)
{
	for (int i = 0; i < CHROMES_NUM; i++)
	{
		Chromes[i].Info[0] = Start_Sign;
		for (int j = 0; j < MAX_VERTEX_NUM; j++)
		{
			Chromes[i].Info[j] = -1;
		}
	}
	for (int i = 0; i < CHROMES_NUM; i++)
	{
		Chromes[i].Info[0] = Start_Sign;
		for (int j = 1; j < MAX_VERTEX_NUM; j++)
		{
			Chromes[i].Info[j] = rand() % MAX_VERTEX_NUM;
			while (bArray_Search(Chromes[i].Info, MAX_VERTEX_NUM, Chromes[i].Info[j], j))
			{
				Chromes[i].Info[j] = rand() % MAX_VERTEX_NUM;
			}
		}
	}
}

bool Chrome_Check_IsHamilton(int Start_Point, int Chrome_Sign)
{
	//检查染色体信息子中是否存在重复现象，存在的话直接判定为假;
	if (Chrome_Check_IsDouble(Chrome_Sign))
	{
		Chromes[Chrome_Sign].Weight_Sum = -1;
		return false;
	}
	//第一种假设情况不成立，则对路径进行遍历寻找;
	int A, B, C, Weight_Sum = 0;
	B = 1;
	C = Start_Point;
	while (B < MAX_VERTEX_NUM)
	{
		A = Chromes[Chrome_Sign].Info[B];
		if (bLink_Array_Search(G[C].Link_Vertex, MAX_VERTEX_NUM - 1, A))
		{
			B += 1;
			Weight_Sum += G[C].Link_Vertex[iLink_Array_Search(G[C].Link_Vertex, MAX_VERTEX_NUM - 1, A)].Weight;
			C = A;
		}
		else
		{
			Chromes[Chrome_Sign].Weight_Sum = -1;
			return false;
		}
	}
	if (bLink_Array_Search(G[Start_Point].Link_Vertex, MAX_VERTEX_NUM - 1, A))
	{
		Weight_Sum += G[Start_Point].Link_Vertex[iLink_Array_Search(G[Start_Point].Link_Vertex, MAX_VERTEX_NUM - 1, A)].Weight;
		Chromes[Chrome_Sign].Weight_Sum = Weight_Sum;
		return true;
	}
	else
	{
		return false;
	}
}

bool Chrome_Check_IsDouble(int Chrome_Sign)
{
	int Point_Array[MAX_VERTEX_NUM];
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		Point_Array[i] = -1;
	}
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		Point_Array[i] = Chromes[Chrome_Sign].Info[i];
	}
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		if (bArray_Search(Point_Array, MAX_VERTEX_NUM, Point_Array[i], i))
		{
			return true;
		}
	}

	return false;
}

void Chrome_Info_Paste(int Chrome_SignA, int Chrome_SignB)
{
	Chromes[Chrome_SignA] = Chromes[Chrome_SignB];
}

void Chrome_Info_Swap(int Chrome_SignA, int Chrome_SignB)
{
	Chrome Temp_Chrome;
	for (int i = SWAP_START_POINT; i < MAX_VERTEX_NUM; i++)
	{
		Temp_Chrome.Info[i]=Chromes[Chrome_SignA].Info[i];
		Chromes[Chrome_SignA].Info[i] = Chromes[Chrome_SignB].Info[i];
		Chromes[Chrome_SignB].Info[i] = Temp_Chrome.Info[i];
	}
}

void Chrome_Mutation()
{
	int Chrome_M_Point = rand() % CHROMES_NUM;
	int Chrome_Info_M_Point = rand() % MAX_VERTEX_NUM;
	while (Chrome_Info_M_Point == 0)
	{
		Chrome_Info_M_Point = rand() % MAX_VERTEX_NUM;
	}
	Chromes[Chrome_M_Point].Info[Chrome_Info_M_Point] = rand() % MAX_VERTEX_NUM;
}

void GA_Main(int Start_Sign) 
{	
	//运算所需变量与数组汇总以及初始化;

	//进化次数计数器，作为进化收敛的判定条件;
	int Inherit_Num = 0;
	//存储某个染色体是否满足哈密顿回路或是否满足精英选择策略的标记;
	bool Is_Hamilton[CHROMES_NUM];
	//寻找符合哈密顿回路的染色体序号;
	int Judge_Counter = 0;
	//为精英选择策略提供数据;
	int Weight_Sum_Array[CHROMES_NUM];
	for (int i = 0; i < CHROMES_NUM; i++)
	{
		Weight_Sum_Array[i] = -1;
	}
	//精英选择策略中需要的目前染色体群中最路径权值最长的染色体标号;
	int Max_Weight_Sum = -1;
	//精英选择策略中需要的目前染色体群中最路径权值最短的染色体标号;
	int Min_Weight_Sum = -1;
	//精英选择策略标准;
	int Level_Weight_Sum = -1;
	//染色体交换时的父代标号;
	int Parent_F = -1, Parent_M = -1;

	//运算开始;

	//判断是否存在先前已由用户定义好的邻接表文件并确定如何调用;
	FIs_Open();
	//随机初始化相应数量CHROMES_NUM的染色体;
Recreate_Loop:Chrome_Create(Start_Sign);
	/*cout << "染色体初始化\n";
	for (int i = 0; i < CHROMES_NUM; i++)
	{
		for (int j = 0; j < MAX_VERTEX_NUM; j++)
		{
			cout << Chromes[i].Info[j];
		}
		cout << endl;
	}*/

	for (int i = 0; i < CHROMES_NUM; i++)
	{
		/*1.对符合条件的染色体自动计算权值路径之和并存入染色体中;
		  2.识别某条染色体是否符合哈密顿回路;*/
		Is_Hamilton[i] = Chrome_Check_IsHamilton(Start_Sign, i);
	}
	for (int i = 0; i < CHROMES_NUM; i++)
	{
		while (Is_Hamilton[Judge_Counter] == false)
		{
			Judge_Counter += 1;
		}
		if (Judge_Counter == CHROMES_NUM)
		{
			cout << "警告：所有产生的染色体均不满足条件，重新生成\n";
			system("pause");
			Chrome_Create(Start_Sign);
			goto Recreate_Loop;
		}
		if (Is_Hamilton[i] == false)
		{
			Chrome_Info_Paste(i, Judge_Counter);
			Is_Hamilton[i] = true;
		}
	}
	
	//进化运算主循环;
	while (Inherit_Num < MAX_INHERATE_NUM)
	{
		//染色体交换过程;
Create_Loop:for (int i = 0; i < (CHROMES_NUM % 2)*0.8; i++)
		{
			Parent_F = rand() % CHROMES_NUM;
			Parent_M = rand() % CHROMES_NUM;
			while (Parent_M == Parent_F)
			{
				Parent_F = rand() % CHROMES_NUM;
				Parent_M = rand() % CHROMES_NUM;
			}
			Chrome_Info_Swap(Parent_F, Parent_M);
		}
		//调试跟踪;
		/*cout << "交换后代\n";
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			for (int j = 0; j < MAX_VERTEX_NUM; j++)
			{
				cout << Chromes[i].Info[j];
			}
			cout << endl;
		}*/
		//染色体突变过程;
		for (int i = 0; i < CHROMES_NUM*0.2; i++)
		{
			Chrome_Mutation();
		}
		/*cout << "突变后代\n";
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			for (int j = 0; j < MAX_VERTEX_NUM; j++)
			{
				cout << Chromes[i].Info[j];
			}
			cout << endl;
		}*/
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			/*1.对符合条件的染色体自动计算权值路径之和并存入染色体中;
			  2.识别某条染色体是否符合哈密顿回路;*/
			Is_Hamilton[i] = Chrome_Check_IsHamilton(Start_Sign,i);
		}
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			while (Is_Hamilton[Judge_Counter] == false)
			{
				Judge_Counter += 1;
			}
			if (Judge_Counter == CHROMES_NUM)
			{
				cout << "警告：所有产生的染色体均不满足条件，重新生成并交配\n";
				system("pause");
				Chrome_Create(Start_Sign);
				goto Create_Loop;
			}
			if (Is_Hamilton[i] == false)
			{
				Chrome_Info_Paste(i, Judge_Counter);
				Is_Hamilton[i] = true;
			}
		}
		//做精英选择前的准备;
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			Weight_Sum_Array[i] = Chromes[i].Weight_Sum;
		}
		Judge_Counter = 0;
		Max_Weight_Sum = Array_BV_Num(Weight_Sum_Array, CHROMES_NUM);
		Min_Weight_Sum = Array_BV_Num(Weight_Sum_Array, CHROMES_NUM, -1);
		Level_Weight_Sum = Min_Weight_Sum + (Max_Weight_Sum - Min_Weight_Sum)*0.2;
		//精英选择策略主循环;
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			if (Chromes[i].Weight_Sum <= Level_Weight_Sum) 
			{
				Is_Hamilton[i] = true;
			}
			else
			{
				Is_Hamilton[i] = false;
			}
			while (Is_Hamilton[Judge_Counter] == false)
			{
				Judge_Counter += 1;
			}
			if (Is_Hamilton[i] == false)
			{
				Chrome_Info_Paste(i, Judge_Counter);
				Is_Hamilton[i] = true;
			}
		}
		/*cout << "精英选择后代\n";
		cout << Max_Weight_Sum << "\t" << Min_Weight_Sum << endl;
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			for (int j = 0; j < MAX_VERTEX_NUM; j++)
			{
				cout << Chromes[i].Info[j];
			}
			cout << endl;
		}*/
		//显示本代结果;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		cout << "第" << Inherit_Num + 1 << "代进化结果如下：\n";
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "第"<< i <<"条线路为：\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			for (int j = 0; j < MAX_VERTEX_NUM; j++) 
			{
				cout << Chromes[i].Info[j];
			}
			cout << endl;
			cout << "权值为：\n";
			cout << Chromes[i].Weight_Sum << endl;
		}
		cout << endl;
		//所有循环内局部变量与数组全部初始化;
		for (int i = 0; i < CHROMES_NUM; i++)
		{
			Is_Hamilton[i] = false;
		}
		Judge_Counter = 0;
		for (int i = 0; i < CHROMES_NUM; i++) 
		{
			Weight_Sum_Array[i] = -1;
		}
		Max_Weight_Sum = -1;
		Min_Weight_Sum = -1;
		Level_Weight_Sum = -1;
		Parent_F = -1;
		Parent_M = -1;
		//完成一代后计数器加一;
		Inherit_Num += 1;
	}

	//运算结束;
}

int main() 
{
	srand(static_cast<unsigned int>(time(0)));
	system("title GA");
	GA_Main(0);
	system("pause");
	return 0;
}