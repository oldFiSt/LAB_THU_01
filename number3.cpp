#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <string>
#include <immintrin.h>

const int Count_Thread = 6;
const unsigned long long int N = 10000000000;

std::string uint128ToString(unsigned __int128 value)
{
	if (value == 0)
		return "0";

	std::string result;

	while (value > 0)
	{
		result += '0' + value % 10;
		value /= 10;
	}

	std::reverse(result.begin(), result.end());

	return result;
}

void calculateSIMD(unsigned long long begin,
									 unsigned long long end,
									 unsigned __int128 &result)
{
	unsigned long long i = begin;

	__m256i vectorSum = _mm256_setzero_si256();
	__m256i values = _mm256_setr_epi64x(begin, begin + 1, begin + 2, begin + 3);
	__m256i step = _mm256_set1_epi64x(4);

	for (; i + 3 <= end; i += 4)
	{
		vectorSum = _mm256_add_epi64(vectorSum, values);
		values = _mm256_add_epi64(values, step);
	}

	unsigned long long temp[4];

	_mm256_storeu_si256(reinterpret_cast<__m256i *>(temp), vectorSum);
	unsigned __int128 sum = 0;

	sum += temp[0];
	sum += temp[1];
	sum += temp[2];
	sum += temp[3];

	for (; i <= end; i++)
	{
		sum += i;
	}

	result = sum;
}

int main()
{
	for (int i = 0; i < 10; i++)
	{
		std::thread threads[Count_Thread];
		unsigned __int128 results[Count_Thread] = {};
		unsigned long long part = N / Count_Thread;
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < Count_Thread; i++)
		{
			unsigned long long begin = i * part + 1;
			unsigned long long end;

			if (i == Count_Thread - 1)
			{
				end = N;
			}

			else
			{
				end = (i + 1) * part;
			}

			threads[i] = std::thread(calculateSIMD, begin, end, std::ref(results[i]));
		}

		for (int i = 0; i < Count_Thread; i++)
		{
			threads[i].join();
		}

		unsigned __int128 totalSum = 0;

		for (int i = 0; i < Count_Thread; i++)
		{
			totalSum += results[i];
		}

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> time = end - start;
		std::cout << "Time = " << time.count() << '\n';
		std::cout << "Sum = " << uint128ToString(totalSum) << '\n';
	}

	return 0;
}