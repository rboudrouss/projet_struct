#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "rsa.h"
#include "primes.h"
#include "misc.h"

int main(){
	int size, *arr;
	// printf("%d\n%d\n", sqrti(145), sqrti(100));

	printf("%d\n", gen_strong_prime(0,1000));
	// printf("%d\n", gen_strong_prime(0,100000));
	return 0;
}
