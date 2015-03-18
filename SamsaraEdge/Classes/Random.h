#include <cstdlib>
//float randFloat(float minRand, float maxRand);
//int randInt(int minRand, int maxRand);
//float randFloatTo(float maxRand);
//int randIntTo(int maxRand);

//【两个浮点数之间的随机数】
inline float randFloat(float minRand, float maxRand)
{
	return (((float)rand() / RAND_MAX)* (maxRand - minRand) + minRand);
}
//【两个整形数之间的随机数】
inline int randInt(int minRand, int maxRand)
{
	return ((rand() % (maxRand - minRand + 1)) + minRand);
}
//【0到最大值之间的浮点数】
inline float randFloatTo(float maxRand)
{
	return ((float)rand() / RAND_MAX)*maxRand;
}
//【0到最大值之间的整形数】
inline int randIntTo(int maxRand)
{
	return (rand() % (maxRand + 1));
}