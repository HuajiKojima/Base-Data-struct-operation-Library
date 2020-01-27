/*This Program has been protected by GNU GPL-3.0, 
please make public it when you're use it for business.
Made by Zhao Yongle. Contact me: zyl1999zyl1999@163.com*/
//遗传算法:
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;

#define FINAL_INHERIT_CHANCE 1000		//最大进化代数;

//每条染色体上的信息子;
struct Chrome
{
	int Chrome_Info[5];
};

//染色体索引头;
struct Chrome_s
{
	Chrome*Info_Point;
};

//申请一组染色体;
Chrome_s Chromes[4];

//初始化本组染色体;
void Chrome_Init() 
{
	Chrome*Temp;
	for (int i = 0; i < 4; i++)
	{
		Temp = (Chrome*)malloc(sizeof(Chrome));
		Chromes[i].Info_Point = Temp;
		for (int j = 0; j < 5; j++)
		{
			Chromes[i].Info_Point->Chrome_Info[j] = -1;
		}
	}
}

//在数组既定位置插入信息;
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

//将既定染色体中的二进制信息子翻译为表现型（整型）;
int Bina_to_Dec(int Chrome_Sign) 
{
	int Result = 0;
	for (int i = 0; i < 5; i++)
	{
		Result += ((Chromes[Chrome_Sign].Info_Point->Chrome_Info[i])*int(pow(2.0f, 4 - i)));
	}
	return Result;
}

//将既定表现型（整型）翻译为二进制信息子并存入既定染色体中;
void Dec_to_Bina(int Chrome_Sign, int Dec_Num) 
{
	int *Info = Chromes[Chrome_Sign].Info_Point->Chrome_Info;
	for (int i = 4; i >= 0; i--)
	{
		Info[i] = Dec_Num % 2;
		Dec_Num = Dec_Num / 2;
	}
}

//交换既定两条染色体的信息;
void Swap_Info(int SignA, int SignB) 
{
	int Middle_Num = -1;
	int*InfoA = Chromes[SignA].Info_Point->Chrome_Info;
	int*InfoB = Chromes[SignB].Info_Point->Chrome_Info;
	for (int i = 2; i < 5; i++)
	{
		Middle_Num = InfoA[i];
		InfoA[i] = InfoB[i];
		InfoB[i] = Middle_Num;
	}
}

//染色体信息子突变函数;
void Mutation() 
{
	int Chrome_Sign = rand() % 4;
	int M_Position = rand() % 5;
	if ((Chromes[Chrome_Sign].Info_Point->Chrome_Info[M_Position]) - 1 == -1) 
	{
		Chromes[Chrome_Sign].Info_Point->Chrome_Info[M_Position] = 1;
	}
	else
	{
		Chromes[Chrome_Sign].Info_Point->Chrome_Info[M_Position] = 0;
	}
}

//轮盘赌算法选择进化的下一代;
int Roulette() 
{
	int Dec_Val[4];									//申请一维数组Dec_Val用来存储每条染色体的表现型
	float Sum = 0;									//用来存储表现型的总体适应度，为后续概率计算做准备
	float p_String[4];								//用来存储每条染色体对应的概率，为后续概率分布值做准备
	float p_Sum[4];									//存储每条染色体的概率分布值
	float Rand_Array[4];							//生成随机数带，作为轮盘赌选择的重要依据
	int Chrome_Sign[4];								//存储选择好表现型对应的染色体
	Chrome Temper[4];								//存储选择后的染色体遗传子，排除先前信息子可能被覆盖的情况
	int Max_Num_From_Dec_Val = -1;
	for (int k = 0; k < 4; k++)
	{
		Dec_Val[k] = Bina_to_Dec(k);
		Sum += float(Dec_Val[k]);
		Chrome_Sign[k] = -1;
		Rand_Array[k] = float(rand() % 1000) / 1000.0f;
	}
	for (int k = 0; k < 4; k++)
	{
		p_String[k] = (static_cast<float>(Dec_Val[k])) / Sum;
	}
	p_Sum[0] = p_String[0];
	for (int k = 1; k < 4; k++)
	{
		p_Sum[k] = p_String[k] + p_Sum[k - 1];
	}
	int i = 0, j = 0;
	for (; i < 4; i++)
	{
		for (; j < 4; j++)
		{
			if (p_Sum[i] > Rand_Array[j])
			{
				Arrary_Data_Inserter(Chrome_Sign, 4, i);
			}
			else
			{
				break;
			}
		}
	}
	for (int k = 0; k < 4; k++)
	{
		for (int l = 0; l < 5; l++)
		{
			Temper[k].Chrome_Info[l] = Chromes[Chrome_Sign[k]].Info_Point->Chrome_Info[l];
		}
	}
	for (int k = 0; k < 4; k++)
	{
		for (int l = 0; l < 5; l++)
		{
			Chromes[k].Info_Point->Chrome_Info[l] = Temper[k].Chrome_Info[l];
		}
	}
	for (int k = 0; k < 4; k++)
	{
		Dec_Val[k] = Bina_to_Dec(k);
		if (Dec_Val[k]>Max_Num_From_Dec_Val)
		{
			Max_Num_From_Dec_Val = Dec_Val[k];
		}
	}
	return Max_Num_From_Dec_Val;
}

//遗传算法核心函数;
void GA() 
{
	int Rand_Dec_Num = 0;
	Chrome_Init();
	for (int i = 0; i < 4; i++)
	{
		Rand_Dec_Num = rand() % 31;
		Dec_to_Bina(i, Rand_Dec_Num);
	}
	for (int i = 0; i < FINAL_INHERIT_CHANCE; i++)
	{
		cout << "第" << i + 1 << "次的选择结果为" << Roulette() << endl;
		Swap_Info(0, 1);
		Swap_Info(2, 3);
		Mutation();
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	GA();
	system("pause");
	return 0;
}