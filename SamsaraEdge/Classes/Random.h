#include <cstdlib>
//float randFloat(float minRand, float maxRand);
//int randInt(int minRand, int maxRand);
//float randFloatTo(float maxRand);
//int randIntTo(int maxRand);

//������������֮����������
inline float randFloat(float minRand, float maxRand)
{
	return (((float)rand() / RAND_MAX)* (maxRand - minRand) + minRand);
}
//������������֮����������
inline int randInt(int minRand, int maxRand)
{
	return ((rand() % (maxRand - minRand + 1)) + minRand);
}
//��0�����ֵ֮��ĸ�������
inline float randFloatTo(float maxRand)
{
	return ((float)rand() / RAND_MAX)*maxRand;
}
//��0�����ֵ֮�����������
inline int randIntTo(int maxRand)
{
	return (rand() % (maxRand + 1));
}