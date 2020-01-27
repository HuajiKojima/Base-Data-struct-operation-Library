/*This Program has been protected by GNU GPL-3.0, 
please make public it when you're use it for business.
Made by Zhao Yongle. Contact me: zyl1999zyl1999@163.com*/
//贪心算法:
#include <iostream>
#include<stdlib.h>
using namespace std;

struct Neighbor_Link_Node
{
	int Node_Name;
	int Range_Weight;
	Neighbor_Link_Node*Next;
};//邻接点;

struct Vertex_Node
{
	int Node_Name;
	Neighbor_Link_Node*Next;
};//保存节点数据的数据类型;

//图的构建，传入参数为需要初始化的节点线性表，线性表长度;
void Graph_Create(Vertex_Node Vertexes[], int Length)
{
	int Judge_Val = 1;
	Neighbor_Link_Node*Temp;
	for (int i = 0; i < Length; i++)
	{
		Vertexes[i].Next = nullptr;
		Vertexes[i].Node_Name = i;
		cout << "输入录入位置为" << i << "的邻接点相关信息：\n";
		cout << "此节点是否含有邻接点？\n（1/0）";
		cin >> Judge_Val;
		while (Judge_Val)
		{
			Temp = (Neighbor_Link_Node*)malloc(sizeof(Neighbor_Link_Node));
			cout << "输入本邻接点的名称以及路径权值：\n";
			cin >> Temp->Node_Name;
			cin >> Temp->Range_Weight;
			Temp->Next = Vertexes[i].Next;
			Vertexes[i].Next = Temp;
			cout << "继续？（1/0）";
			cin >> Judge_Val;
		}
	}
}

//在指定长度的指定一维整型数组中寻找指定数据;
bool Data_Finder(int Array[], int Length, int Search_Data)
{
	for (int i = 0; i < Length; i++)
	{
		if (Array[i] == Search_Data)
		{
			return true;
		}
	}
	return false;
}

//在指定长度的指定一维整型数组中插入指定数据（类栈形式插入方法）
void Arrary_Data_Inserter(int Array[], int Length, int Insert_Val)
{
	for (int i = 0; i < Length; i++)
	{
		if (Array[i] == -1)
		{
			Array[i] = Insert_Val;
			break;
		}
	}
}

//贪心算法核心函数，参数列表分别为已构建的邻接表，图中点的个数，起始点记号，终止点记号;
int Greedy(Vertex_Node Vertexes[], int Length, int Start_Point, int End_Point)
{
	if (Start_Point != End_Point)
	{
		int*Has_Through = new int[Length];
		for (int i = 0; i < Length; i++)
		{
			Has_Through[i] = -1;	//数组初始化;
		}
		int Sum = 0;
		int Infi_Num = 10000;
		Vertex_Node Pointer = Vertexes[Start_Point];
		Neighbor_Link_Node*TempM = Pointer.Next;
		Neighbor_Link_Node*TempS = TempM;
		Arrary_Data_Inserter(Has_Through, Length, Start_Point);
		while (true)
		{
			Infi_Num = 10000;
			while (true)
			{
				if (TempM->Node_Name == End_Point)
				{
					Arrary_Data_Inserter(Has_Through, Length, TempM->Node_Name);
					Sum += TempM->Range_Weight;
					{
						cout << "局部最优解为：\n";
						for (int i = 0; i < Length; i++)
						{
							cout << Has_Through[i];
						}
						cout << "权值总和为：" << Sum << "\n";
					}
						delete Has_Through;
						return 1;
				}
				if ((TempM->Range_Weight < Infi_Num) && (!Data_Finder(Has_Through, Length, TempM->Node_Name)))
				{
						Infi_Num = TempM->Range_Weight;
						TempS = TempM;
				}
				if (TempM->Next == nullptr)
				{
					break;
				}
				TempM = TempM->Next;
			}
			Arrary_Data_Inserter(Has_Through, Length, TempS->Node_Name);
			Sum += TempS->Range_Weight;
			Pointer = Vertexes[TempS->Node_Name];
			if (Pointer.Node_Name == End_Point)
			{
				cout << "局部最优解为：\n";
				for (int i = 0; i < Length; i++)
				{
					cout << Has_Through[i];
				}
				cout << "权值总和为：" << Sum << "\n";
				break;
			}
			TempM = Pointer.Next;
			TempS = TempM;
		}
		delete Has_Through;
		return 1;
	}
	else
	{
		cout << "起终点一致，无路径\n";
		return 0;
	}
}

int main()
{
	Vertex_Node Graph[5];	//建立测试用图例，5个已知点，具体连接方式详见项目Wiki;
	Graph_Create(Graph, 5);
	Greedy(Graph, 5, 0, 3);	//测试数据：5个已知点，起始点为0，终止点为3;
	cout << endl;
	Greedy(Graph, 5, 0, 4);	//测试数据：5个已知点，起始点为0，终止点为4;
	cout << endl;
	Greedy(Graph, 5, 0, 1);	//测试数据：5个已知点，起始点为0，终止点为1;
	cout << endl;
	system("pause");
	return 0;
}