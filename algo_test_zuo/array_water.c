/*
 * 题目：
 * 题意：
 * 		给定一个正方形数组array，把array想象成一个直方图
 * 		给这个直方图装水，能装下几格水	
 *
 * 题意解析:
 * 		假定数组【3，1，2，4，3】，则装水图可以表示成
 * 			 	  口
 * 			口****口口	
 * 			口**口口口
 *      	口口口口口
 *      其中带*号部分就是能装水的格子
 * 解题思路:
 * 		如果数组中某个值代表的直方图上方能装水，则装的水量是左右两边最大值中最小的那个值再减去该值，否则为0。
 * 		数组里面每个值对应的直方图上方能装多少水可以转化成以下公式。
 * 			Wi = max(0, min(Lmax, Rmax) - array[i])
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define min(a,b) (a>b?b:a)
#define max(a,b) (a>b?a:b)

/*
 * 解法一: 建立辅助数组，把某个位置的左侧最大值和右侧最大值全都记录起来，再做一次遍历按上述公式求出总和。
 * 时间复杂度：O(n), 空间复杂度O(n)
 */
int water_1(int array[], int len)
{
	int water = 0;
	int * left_arr = malloc(sizeof(int)*len);
	int * right_arr = malloc(sizeof(int)*len);

	//从左往右的最大值数列
	left_arr[0] = array[0];
	for(int i = 1; i < len; i++)
	{
		left_arr[i] = (left_arr[i-1] < array[i])?array[i]:left_arr[i-1];
	}

	//从右往左的最大值数列
	right_arr[len-1] = array[len-1];
	for(int i = len-2; i >= 0; i--)
	{
		right_arr[i] = (right_arr[i+1] < array[i])?array[i]:left_arr[i+1];
	}

	//计算每个位置能装的数量
	for(int i = 0; i < len; i++)
	{
		int tmp = min(left_arr[i], right_arr[i]) - array[i];	
		water += (tmp<0?0:tmp);
	}

	free(left_arr);
	free(right_arr);
	return water;
}

/*
 * 解法二：将辅助数组简化为左右端最大值的记录，从两端往中间计算，每个位置都用这两个最大值来进行计算即可。
 * 解法原理：左右两端维护的最大值记录里至少有一个是真实最大值,而另一端的真实最大值只会大于等于当前记录的最大值，因此只要这个真实最大值是两者中的最小值，则这一端当前位置能装的水量就能确定下来了。
 * 时间复杂度O(n) 空间复杂度O(1)
 */
int water_2(int array[], int len)
{
	int water = 0;
	int Lmax = array[0];
	int Rmax = array[len-1];
	int l_idx = 1;
	int r_idx = len - 2;
	int tmp;

	while(l_idx <= r_idx)
	{
		if(Lmax <= Rmax)
		{
			//从左往右算
			tmp = Lmax - array[l_idx];
			if(tmp > 0)
				water += tmp;
			else
				Lmax = array[l_idx];		
			l_idx++;
		}
		else
		{
			//从右往左算
			tmp = Rmax - array[r_idx];
			if(tmp > 0)
				water += tmp;
			else
				Rmax = array[r_idx];		
			r_idx--;
		}
	}

	return water;
}

//测试样例
void code_test()
{
	int test_array[6] = {3,1,2,4,1,5};
	printf("result=%d\n", water_2(test_array, 6));
}

//主函数
int main(int argc, char argv[])
{
	code_test();
}
