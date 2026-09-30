#include <iostream>
#include <cstdint>
#include <thread>
#include <ctime>
#include <chrono>
#include <vector>

const int N = 4096;
const int Count_Thread = 6;
int32_t A[N][N];
int32_t B[N][N];

int32_t C_one[N][N];
int32_t C_thread[N][N];

void put_data()
{
	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < N; j++)
		{
			A[i][j] = std::rand() % 11;
			B[i][j] = std::rand() % 11;
		}
	}
}

void mut_one()
{
	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < N; j++)
		{
			int32_t sum = 0;

			for (int k = 0; k < N; k++)
			{
				sum += A[i][k] * B[k][j];
			}

			C_one[i][j] = sum;
		}
	}
}

void mut_thread(int beginRow, int endRow)
{
	for (int i = beginRow; i < endRow; i++)
	{

		for (int j = 0; j < N; j++)
		{
			int32_t sum = 0;

			for (int k = 0; k < N; k++)
			{
				sum += A[i][k] * B[k][j];
			}

			C_thread[i][j] = sum;
		}
	}
}

void create_thread()
{
	std::thread threads[Count_Thread];
	int part = N / Count_Thread;

	for (int i = 0; i < Count_Thread; i++)
	{
		int beginRow = i * part;

		int endRow;

		if (i == Count_Thread - 1)
		{
			endRow = N;
		}
		else
		{
			endRow = (i + 1) * part;
		}

		threads[i] =
				std::thread(mut_thread, beginRow, endRow);
	}

	for (int i = 0; i < Count_Thread; i++)
	{
		threads[i].join();
	}
}

bool checkMatrices()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (C_one[i][j] != C_thread[i][j])
			{
				return false;
			}
		}
	}

	return true;
}

int main()
{
	put_data();
	auto start_one = std::chrono::high_resolution_clock::now();
	mut_one();
	auto end_one = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_one = end_one - start_one;
	std::cout << "Time one thread = " << time_one.count() << '\n';

	auto start_thread = std::chrono::high_resolution_clock::now();
	create_thread();
	auto end_thread = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_thread = end_thread - start_thread;
	std::cout << "Time with 6 thread = " << time_thread.count() << '\n';

	if (checkMatrices())
	{
		std::cout << "Matrices are equal\n";
	}
	else
	{
		std::cout << "Matrices are NOT equal\n";
	}

	return 0;
}