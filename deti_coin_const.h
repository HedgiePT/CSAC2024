#pragma once

#include <inttypes.h>

// First bytes of every coin.
// Spells "DETI coin   ", but flipped (due to endianess).
const uint32_t k_coin_std_header[3] = {
    0x49544544, // ITED
    0x696f6320, // ioc␣
    0x2020206e  // ␣␣␣n
};

// const unsigned int k_deti_coin_header_length = 10;
const int k_coin_length = 52;
const int k_coin_body_start = 10;
const int k_coin_body_end = k_coin_length - 1;
const int k_coin_body_length = k_coin_body_end - k_coin_body_start;
