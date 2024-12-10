//
// Tomás Oliveira e Silva,  October 2024
//
// João Ferreira, November 2024
// Chris, November 2024
//
// Arquiteturas de Alto Desempenho 2024/2025
//
// deti_coins_cpu_avx2_search() --- find DETI coins using md5_cpu_avx2().
//

#ifndef DETI_COINS_CPU_AVX2_SEARCH
#define DETI_COINS_CPU_AVX2_SEARCH

// Includes desnecessários; apenas ajudam a IDE a compreender o código.
#include "md5_cpu_avx2.h"

// Includes importantes
#include <inttypes.h>
#include "deti_coin_const.h"
#include <stdio.h>

// Vetor de 8 signed ints de 32 bits (4 bytes) cada.
typedef int32_t i32x8 __attribute__ ((vector_size (32)));   // 8*4 = 32

#define BROADCAST(x) ((i32x8){x, x, x, x, x, x, x, x})
#define BROADCAST_INC(x) ((i32x8){x, x+1, x+2, x+3, x+4, x+5, x+6, x+7})

#define NEXT_VALUE_TO_TRY(v)                                                   \
do                                                                             \
{                                                                              \
    v++;                                                                       \
                                                                               \
    if ((v & 0xFF) == 0x7F)                                                    \
    {                                                                          \
        v += 0xA1;                                                             \
        if (((v >> 8) & 0xFF) == 0x7F)                                         \
        {                                                                      \
            v += 0xA1 << 8;                                                    \
            if  (((v >> 16) & 0xFF) == 0x7F)                                   \
            {                                                                  \
                v += 0xA1 << 16;                                               \
                if (((v >> 24) & 0xFF) == 0x7F)                                \
                {                                                              \
                    v += 0xA1 << 24;                                           \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }                                                                          \
} while (0)


static void deti_coins_cpu_avx2_search(uint32_t n_random_words)
{
    i32x8 v8hash[4];    // Hash tem 128 bits de comprimento.
    i32x8 v8coin[13];   // 13 == 52 bytes / sizeof(int32_t) == 52/4

    // INIT v8coin
    v8coin[0] = BROADCAST((int32_t)k_coin_std_header[0]);
    v8coin[1] = BROADCAST((int32_t)k_coin_std_header[1]);
    v8coin[2] = BROADCAST((int32_t)k_coin_std_header[2]);
    for (int i = 3; i < 10; i++) {
        v8coin[i] = BROADCAST(0x20202020);
    }
    int32_t v1 = 0x7e7e7e70;
    int32_t v2 = 0x20202020;
    v8coin[10] = BROADCAST(v1);
    v8coin[11] = BROADCAST(v2);
    v8coin[12] = BROADCAST_INC(0x0a202020);

    uint64_t n_attempts = 0, n_coins = 0;

    if (n_random_words > 8)
        n_random_words = 8;

    while (!stop_request) {
        //printf("v8coin[10]=%x, [11]=%x, [12]=%x\n", v8coin[10][3], v8coin[11][3], v8coin[12][3]);
        md5_cpu_avx2(v8coin, v8hash);
        for(uint32_t lane = 0; lane < n_random_words; lane++)
            if(v8hash[3][lane] == 0)
            {
                uint32_t coin[13];
                for(uint32_t idx = 0; idx < 13; idx++)
                    coin[idx] = v8coin[idx][lane];
                save_deti_coin(coin);
                n_coins++;
            }

        NEXT_VALUE_TO_TRY(v1);
        v8coin[10] = BROADCAST(v1);
        if (v1 == 0x20202020)
        {
            NEXT_VALUE_TO_TRY(v2);
            v8coin[11] = BROADCAST(v2);
        }

        n_attempts += n_random_words;
    }

    STORE_DETI_COINS();
    printf("deti_coins_cpu_avx2_search: %lu DETI coin%s found in %lu attempt%s (expected %.2f coins)\n",n_coins,(n_coins == 1ul) ? "" : "s",n_attempts,(n_attempts == 1ul) ? "" : "s",(double)n_attempts / (double)(1ul << 32));

}


#undef BROADCAST
#undef BROADCAST_INC

#endif
