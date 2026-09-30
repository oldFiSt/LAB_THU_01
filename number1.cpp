#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <string>

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

int main(int, char **)
{
    std::cout << "Без потоков!\n";
    unsigned __int128 sum = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned long long int i = 1; i <= N; i++)
    {
        sum += i;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "Time = " << time.count() << '\n';
    std::cout << uint128ToString(sum) << '\n';
}
