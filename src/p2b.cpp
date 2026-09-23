#include "p2a.h"
#include "timer.h"
#include <random>
#include <vector>
#include <cassert>
#include <iostream>

size_t rowMajorIndexA(size_t x, size_t y, size_t z)
{
    return z*256*256 + y*256 + x;
};

size_t rowMajorIndexK(size_t x, size_t y, size_t z)
{
    return z*4*4 + y*4 + x;
};

size_t rowMajorIndexConv(size_t x, size_t y, size_t z)
{
    return z*64*64 + y*64 + x;
}

int main()
{
    size_t size_row_major_K = 4*4*4;
    size_t size_row_major_out_conv =64*64*64;
    size_t size_row_major_A = 256*256*256;

    std::vector<uint64_t> K_a(size_row_major_K);
    std::vector<uint64_t> K_b(size_row_major_K);

    std::vector<uint64_t> A(size_row_major_A);
    std::vector<uint64_t> B(size_row_major_A);


    std::vector<uint64_t> out_conv_A(size_row_major_out_conv);
    std::vector<uint64_t> out_conv_B(size_row_major_out_conv);
    std::mt19937_64 rng(0);


    //populating A is not complex as it is flat
    //but because of the same reason, convolving 3d kernel is difficult
    for (size_t num=0; num< size_row_major_A; num++)
    {
        A[num] = rng();
    }

    for (size_t z=0; z<256; z++)
    {
        for (size_t y = 0; y < 256; y++)
        {
            for (size_t x = 0; x <256; x++)
            {
                B[morton3d(x, y, z)] = A[rowMajorIndexA(x, y, z)];

                //because 0<= x, y, z <4 for kernel
                if (x<4 && y<4 && z<4)
                {
                    K_a[rowMajorIndexK(x, y, z)] = x+y+z;
                    K_b[morton3d(x, y, z)] = x+y+z;
                }
            }
        }
    }

    Timer timing;

    //convolution for A:

    for (size_t z=0; z<64; z++)
    {
        for (size_t y = 0; y < 64; y++)
        {
            for (size_t x = 0; x <64; x++)
            {
                //because 0<= x, y, z <4 for kernel
                uint64_t dot_prod = 0;
                
                for (size_t kernel_z = 0; kernel_z < 4; kernel_z++)
                {
                    for (size_t kernel_y = 0; kernel_y < 4; kernel_y++)
                    {
                        for (size_t kernel_x = 0; kernel_x < 4; kernel_x++)
                        {
                            // Multiply overlapping values
                            uint64_t a_val = A[rowMajorIndexA(x*4 +kernel_x, y*4 + kernel_y, z*4 + kernel_z)];
                            uint64_t k_val = K_a[rowMajorIndexK(kernel_x, kernel_y, kernel_z)];
                            dot_prod += a_val*k_val;
                        }
                    }
                }
                out_conv_A[rowMajorIndexConv(x,y,z)] = dot_prod;
            
            }
        }
    }

    auto convolution_A_time = timing.click<Timer::Micros>();
    timing.restart();

    //convolution for  B:
    for (size_t out_num=0; out_num< size_row_major_out_conv; out_num++)
    {
        uint64_t dot_prod = 0;
        for (size_t kernel_num =0; kernel_num<64; kernel_num++)
        {
            dot_prod += B[out_num*64 + kernel_num]*K_b[kernel_num];
        }
        out_conv_B[out_num] = dot_prod;
    }

    auto convolution_B_time = timing.click<Timer::Micros>();

    //asserting that output entries are Equal:
    for (size_t z = 0; z < 64; z++)
    {
        for (size_t y = 0; y < 64; y++)
        {
            for (size_t x = 0; x < 64; x++)
            {
                assert(out_conv_A[rowMajorIndexConv(x, y, z)] == out_conv_B[morton3d(x, y, z)] && "Output entries are not equivalent");
            }
        }
    }
    std::cout << convolution_A_time << std::endl;
    std::cout << convolution_B_time << std::endl;
    return 0;









}