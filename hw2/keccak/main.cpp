#include <stdio.h>
#include <stdint.h>
#include <ctime>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"

uint64_t C[5];
uint64_t D[5];
uint64_t B[5][5];

uint8_t* padding(uint8_t*, int32_t*);

inline int mod(int a, int b)
{
    if (b < 0)
    {
        a = -a;
        b = -b;
    }
    int ret = a % b;
    if (ret < 0) ret += b;
    return ret;
}

uint64_t** sha3_round(uint64_t** A, uint64_t RC)
{
    uint8_t x = 0, y = 0;
    uint64_t tmp = 0, t = 0;

    /* Theta step */
    for (x = 0; x < 5; x++)
    {
        C[x] = A[x][0] ^ A[x][1] ^ A[x][2] ^ A[x][3] ^ A[x][4];
    }

    for (x = 0; x < 5; x++)
    {
        tmp = C[(x + 1) % 5];
        D[x] = C[(x + 4) % 5] ^ ((tmp << 1) | (tmp >> 63));
    }

    for (x = 0; x < 5; x++)
    {
        for (y = 0; y < 5; y++)
        {
            A[x][y] = A[x][y] ^ D[x];
        }
    }

    /*Rho and pi steps*/
    for (x = 0; x < 5; x++)
    {
        for (y = 0; y < 5; y++)
        {
            t = (2 * x + 3 * y) % 5;
            B[y][t] = (A[x][y] << r[x][y]);
            B[y][t] |= (A[x][y] >> (64 - r[x][y]));
        }
    }

    /*Xi state*/
    for (x = 0; x < 5; x++)
    {
        for (y = 0; y < 5; y++)
        {
            tmp = (x + 1) % 5;
            t = (x + 2) % 5;
            A[x][y] = B[x][y] ^ ((~B[tmp][y]) & B[t][y]);
        }
    }

    /*Last step*/
    A[0][0] ^= RC;

    return A;
}

uint64_t** keccak_f(uint64_t** A)
{
    for (int32_t i = 0; i < 24; i++)
    {
        A = sha3_round(A, RC[i]);
    }
    return A;
}


uint8_t* padding(uint8_t* M, int32_t* S)
{
    int32_t i = *S;
    int32_t newS = (*S + 72 - (*S % 72));
    uint8_t* nM = (uint8_t*)malloc(*S + (72 - (*S % 72)));
    /*Copy string*/
    for (int32_t j = 0; j < *S; j++)
    {
        *(nM + j) = *(M + j);
    }
    *(nM + i) = 0x01;
    i++;
    while (i < (newS - 1))
    {
        *(nM + i) = 0x00;
        i++;
    }
    *(nM + i) = 0x80;
    i++;
    *S = i;
    return nM;
}

uint8_t* sponge(uint8_t* M, int32_t size)
{
    int32_t r = 72;
    int32_t w = 8;
    /*Padding*/
    if ((size % r) != 0)
    { //r=72 bytes
        M = padding(M, &size);
    }
    uint64_t* nM = (uint64_t*) M;
    /// Initialization
    uint64_t** S = (uint64_t**) calloc(5, sizeof(uint64_t*));
    for (uint64_t i = 0; i < 5; i++) S[i] = (uint64_t*) calloc(5, sizeof(uint64_t));
    /// Absorbing Phase
    for (int32_t i = 0; i < size / r; i++)
    { //Each block has 72 bytes
        for (int32_t y = 0; y < 5; y++)
        {
            for (int32_t x = 0; x < 5; x++)
            {
                if ((x + 5 * y) < (r / w))
                {
                    S[x][y] = S[x][y] ^ *(nM + i * 9 + x + 5 * y);
                }
            }
        }
        S = keccak_f(S);
    }
    /// Squeezing phase
    int32_t b = 0;
    uint64_t* Z = (uint64_t*) calloc(9, sizeof(uint64_t));
    while (b < 8)
    {
        for (int32_t y = 0; y < 5; y++)
        {
            for (int32_t x = 0; x < 5; x++)
            {
                if ((x + 5 * y) < (r / w))
                {
                    *(Z + b) ^= S[x][y];
                    b++;
                }
            }
        }
    }
    return (uint8_t*) Z;
}

char* read_file(const char *fileName, int *size) {
    FILE* file = fopen(fileName, "rb");

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const int BUF_SIZE = 1 << 20;
    char *array = new char[*size + 10];
    for (int i = 0; i < *size; i += BUF_SIZE)
        fread(array + i, sizeof(uint8_t), BUF_SIZE, file);
    fclose(file);

    return array;
}

void test_speed(const char *fname) {
    int32_t size = 0;
    char *s = read_file(fname, &size);
    int32_t *psize = &size;
    
    int begin = clock();
    uint8_t *hash_str = sponge((uint8_t *)s, *psize);
    int end = clock();

    double timeUsed = (1.0 * (end - begin) / CLOCKS_PER_SEC);
    printf("Time: %.3lf s\n", timeUsed);
    printf("Speed: %.3lf MB/s\n", 1.0 * size / (1 << 20) / timeUsed);

    delete [] s;
}

int main(int argc, char **argv)
{
    int32_t size;
    char *s = read_file(argv[1], &size);
    //printf("%s\n", s);

    int32_t *psize = &size;
    uint8_t *hash_str = sponge((uint8_t *)s, *psize);
    for(int32_t i=0;i<64;i++)
        printf("%X",hash_str[i]);
    printf("\n");

    if(argc > 2) {
        printf("Test speed\n");
        test_speed(argv[2]);
    }

    return 0;
}