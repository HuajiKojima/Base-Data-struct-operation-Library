/*This Program has been protected by GNU GPL-3.0, 
please make public it when you're use it for business.
Made by Zhao Yongle. Contact me: zyl1999zyl1999@163.com*/

#include <iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

string alfa = "abcde";//样本关键字;
float Beta[5];		  //样本概率;
float Compass[5];	  //样本概率分布值;
float Delta[5];		  //随机生成的比较序列;

//所有数组初始化;
void All_Array_Assemble() 
{
	for (int i = 0; i < 5; i++)
	{
		Delta[i] = float(rand() % 10) / 10.0f;
	}

	Beta[0] = 0.1f;
	Beta[1] = 0.3f;
	Beta[2] = 0.0f;
	Beta[3] = 0.2f;
	Beta[4] = 0.4f;

	float Sum = 0.0f;
	for (int i = 0; i < 5; i++) 
	{
		for (int j = 0; j < i; j++)
		{
			Sum += Beta[i];
		}
		Compass[i] = Sum;
	}
}

//延时函数;time的填写格式为毫秒
void Delay(int time)	     
{
	clock_t now = clock();
	while (clock() - now < time);
}

//轮盘赌算法;
void Roulette() 
{
	//随机生成一个数组Delta，然后有序排列;
	//若分布值Compass[i]大于随机生成序列中Delta[j]，则Compass[i]对应的关键字被选择，将继续比较Delta[j+1];
	//若不大于，则不选择;
	//此时比较下一个关键字所对应的Compass;
	int i = 0, j = 0;
	for (; i < 5; i++)
	{
		for (; j < 5; j++)
		{
			if (Compass[i] > Delta[j])
			{
				cout << alfa[i];
			}
			else
			{
				break;
			}
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < 20; i++)
	{
		All_Array_Assemble();
		Roulette();
		cout << endl;
		Delay(100);
	}
	system("pause");
	return 0;
}