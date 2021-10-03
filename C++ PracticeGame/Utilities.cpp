typedef int s32;
typedef unsigned int u32;
typedef short s16;
typedef unsigned short u16;
typedef long long s64;
typedef unsigned long long u64;
typedef char s8;
typedef unsigned char u8;

//since all files are being compiled into one - the methods in renderer can be static
#define globalVar static
#define internal static


inline int
Clamp(int min, int val, int max)
{
	if (val < min)
	{
		return min;
	}
	if (val > max)
	{
		return max;
	}
	return val;

}