typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

inline int
clamp(int min, int val, int max) {					// Clamps the value within the bounds
	if (val > max) return max;
	if (val < min) return min;
	return val;
}

//#include <random>

internal int
randomizer(int start, int end) {

	//std::random_device rd;
	
	//std::uniform_int_distribution<int> dist(start, end);

	//return dist(rd);

	float rand_percent = static_cast<float>(rand()) / RAND_MAX;

	int rand_ind = start + static_cast<int>(rand_percent * end);

	return rand_ind;
}