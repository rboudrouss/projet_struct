#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "prime.h"
#include "rsa.h"

#define SM 15
#define B 10

int main()
{

    // small primes
    int small_p[SM] = {2, 3, 5, 7, 41, 89, 139, 197, 239, 331, 599, 701, 829, 1103, 3797, 4271};
    // small composits
    int small_np[SM] = {1, 4, 6, 8, 9, 15, 21, 51, 221, 343, 413, 777, 973, 1547, 4477};
    // big primes
    // long big_p[B] = {2147483647, 2147482873, 2147480897, 2147478727, 2147470679, 2147469173, 2147465563, 2147463491, 2147462579, 2147462143};
    // big composits
    // long big_np[B] = {1074013595, 1074013577, 2147508417, 2147508345, 2147508277, 2147507647, 2147507361, 2147506863, 2147506447, 2147506153};

    printf("Testing 'prime.c'\n");

    printf("testing is_prime_naive function\n");
    for (int i = 0; i < SM; i++)
    {
        assert(is_prime_naive(small_p[i]));
        assert(!is_prime_naive(small_np[i]));
    }
    printf("testing modpow function\n");
    assert(modpow(4271, 5, 15) == modpow_naive(4271, 5, 15));
    assert(modpow(331, 13, 22) == modpow_naive(331, 13, 22));
    assert(modpow(89, 23, 17) == modpow_naive(89, 23, 17));
    assert(modpow(3797, 22, 33) == modpow_naive(3797, 22, 33));

    printf("testing 'rsa.c' file");

    return 0;
}