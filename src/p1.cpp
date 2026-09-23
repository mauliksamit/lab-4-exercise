#include <cstddef>
#include "timer.h"
#include <vector>
#include <random>
#include <iostream>
constexpr size_t SIZE = 16000000;

int main()
{
    std::vector<uint64_t> row_major(SIZE);

    std::vector<uint64_t> col_major(SIZE);

    uint64_t sum_row_major = 0;
    uint64_t sum_col_major = 0;


    std::mt19937_64 rng(0);

    for (int row = 0; row < 4000; row++)
    {
        for (int col = 0; col < 4000; col++)
        {
            row_major[row*4000 + col] = rng();
        }
       
    }

    rng.seed(0);

    for (int col = 0; col < 4000; col++)
    {
        for (int row = 0; row < 4000; row++)
        {
            col_major[row*4000 + col] = rng();
        }
       
    }
    Timer timing;

    for (int row = 0; row < 4000; row++)
    {
        for (int col = 0; col < 4000; col++)
        {
            sum_row_major += row_major[row*4000 + col];
        }
       
    }

    auto row_major_sum_time = timing.click<Timer::Micros>();

    timing.restart();
    for (int col = 0; col < 4000; col++)
    {
        for (int row = 0; row < 4000; row++)
        {
            sum_col_major += col_major[row*4000 + col];
        }
       
    }

    auto col_major_sum_time = timing.click<Timer::Micros>();

    std::cout << row_major_sum_time << " " << sum_row_major << std::endl;
    std::cout << col_major_sum_time << " " << sum_col_major << std::endl;
    

    return 0;
}
