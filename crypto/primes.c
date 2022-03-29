#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "primes.h"
#include "misc.h"

int* erathostene(int n, int *nb_primes){
	n++; //c'est le plus simple pour régler les problèmes d'indice
	bool sieve[n];
	memset(sieve, true, n*sizeof(bool));
	int j, limit;
	limit = 1+sqrti(n);

	// Crible d'erathostene
	for(int i=2; i<limit; i++){
		if (sieve[i]){
			for(j=2*i; j<n; j+=i){
				sieve[j] = false;
			}
		}
	}
	
	// Calcul pi(n)

	*nb_primes = 0;
	for(j=2; j<n; j++)
		*(nb_primes)+=(int) (sieve[j]);

	// malloc un tableau avec les nombres premiers inférieurs à n
	int* res;
	res = malloc((*nb_primes)*sizeof(int));
	j=0;
	for(int i=2; i<n; i++){
		if (sieve[i]){
			res[j]=i;
			j++;
		}
	}

	return res;
}

// remplacer par naive_primality_base(res, primes, *nb_primes) && miller_rabin(res) si on chercher des entiers plus grands que 2^64 et rajouter un crible avant
int gen_prime(int low_bound, int up_bound){
	int res;
	while(1){
		res = 2*gen_ran_int(low_bound/2, (up_bound-1)/2) +1;
		if (miller_rabin_deterministic(res)) 
			return res;
	}
}

int gen_strong_prime(int low_bound, int up_bound){ // Gordon's algorithm TODO: implémentation correcte (je crois) mais int overflow constant
	int s, t, i, j, r, p;
	s = gen_prime(low_bound, up_bound);
	t = gen_prime(low_bound, up_bound);
	i = gen_ran_int(0,100);
	j = 2*t;
	r = j*i+1;

	while(1){
		if(miller_rabin_deterministic(r))
			break;
		r += j;
	}

	j = gen_ran_int(0,100);
	i = 2*r*s;
	p = 2*s*exp_mod(s, r-2, r)-1 + i*j;
	while(1){
		if(miller_rabin_deterministic(p))
			break;
		p+=i;
	}
	return p;
}

bool witness(int n, int mant, int exp, int base){ // n-1 = mant* 2^exp
	int x;
	x = exp_mod(base, mant, n);
	if ((x==1) || (x==n-1))
		return false;
	for(int i=1; i<exp; i++){
		x = (x*x)%n;
		if (x==n-1)
			return false;
	}
	return true;
}

bool miller_rabin(int n, int nb_base){
	int exp, mant, base;
	exp = __builtin_ctz(n-1);
    mant = n>>exp;
	for(; nb_base>0; --nb_base){
		base = gen_ran_int(2, n-2);
		if (witness(n, mant, exp, base))
			return false;
	}
	return true;
}

bool miller_rabin_deterministic(int n){ // Works for n<2^64
	int base[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	int exp, mant;
	exp = __builtin_ctz(n-1);
    mant = n>>exp;

    if (n<=37)
    	return naive_primality_base(n, base, 12);

	for(int i=0; i<12; i++){
		if (witness(n, mant, exp, base[i]))
			return false;
	}
	return true;
}

bool fermat(int n, int nb_test){
	int res;
	for(; nb_test>0; --nb_test){
		res = exp_mod(gen_ran_int(2, n-2), n-1, n); 
		if ((res != 1) && (res != n-1))
			return false;
	}	
	return true;
}

bool naive_primality_base(int n, int *base, int base_size){
	for(int i=0; i<base_size; i++)
		if (!(n % base[i]))
			return n==base[i];
	return true;
}

bool naive_primality(int n){
	int nb_primes, *primes;
	nb_primes = (n>500000) ? 500000 : n;
	primes = erathostene(nb_primes, &nb_primes);
	return naive_primality_base(n, primes, nb_primes);
}