//
// Tomás Oliveira e Silva,  October 2024
//
// João Ferreira, November 2024
// Chris, November 2024
//
// Arquiteturas de Alto Desempenho 2024/2025
//
// deti_coins_cpu_avx_search() --- find DETI coins using md5_cpu_avx().
//

#ifndef DETI_COINS_CPU_AVX_SEARCH
#define DETI_COINS_CPU_AVX_SEARCH

#include <inttypes.h>
#include "deti_coin_const.h"
#include "md5_cpu_avx.h"    // Tecnicamente desnecessário.

// Vetor de 4 signed ints de 32 bits (4 bytes) cada.
typedef int32_t i32x4 __attribute__ ((vector_size (16)));   // 4*4 = 16

#define BROADCAST(x) ((i32x4){x, x, x, x})
#define LENGTH(x) (sizeof(x) / sizeof(x[0]))


static void init_coins(i32x4 *v4array) {
    v4array[0] = BROADCAST((int32_t)k_deti_coin_header[0]);
    v4array[1] = BROADCAST((int32_t)k_deti_coin_header[1]);
    v4array[2] = BROADCAST((int32_t)k_deti_coin_header[2]);

    for (int i = 3; i < 12; i++) {
        v4array[i] = BROADCAST(' ');
    }

    v4array[12] = BROADCAST('\n');
}


static void deti_coins_cpu_avx_search(uint32_t n_random_words)
{
    i32x4 v4hash[4];    // Hash tem 128 bits de comprimento.
    i32x4 v4coin[13];   // 13 == 52 bytes / sizeof(int32_t) == 52/4

    init_coins(v4coin);

    uint64_t n_attempts = 0, n_coins = 0;

    while (!stop_request) {
        md5_cpu_avx(v4coin, v4hash);

        // Implementar resto do código aqui...

        n_attempts++;
    }

}

#endif
