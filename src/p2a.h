#ifndef P2A_H
#define P2A_H

#include <cstdint>
#include <cassert>

inline uint64_t expand(uint64_t input, uint32_t scale) {

    assert(scale >= 1 && "Scale should be >=1");
    uint64_t output = 0;
    uint32_t shift_amount = 0;

    while (input > 0 && shift_amount< 64)
    {
        uint64_t current_bit = input & 1ULL;
        output |= (current_bit<< shift_amount);
        input >>= 1;
        shift_amount += scale;
    }

    return output;
}

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z)
{
    uint64_t x_interim_expansion = expand(x, 3);
    uint64_t y_interim_expansion = expand(y, 3)<<1;
    uint64_t z_interim_expansion = expand(z, 3)<<2;
    return x_interim_expansion | y_interim_expansion | z_interim_expansion;

}
#endif