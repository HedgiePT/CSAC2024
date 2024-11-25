#pragma once

#include <inttypes.h>

// First bytes of every coin.
// Spells "DETI coin   ", but flipped (due to endianess).
const uint32_t COIN_STD_HEADER[3] = {
    0x49544544, // ITED
    0x696f6320, // ioc␣
    0x2020206e  // ␣␣␣n
};

// const unsigned int k_deti_coin_header_length = 10;
const int COIN_LENGTH = 52;
const int COIN_BODY_START = 10;
const int COIN_BODY_END = COIN_LENGTH - 1;
