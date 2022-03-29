#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/random.h>
#include <limits.h>
#include "rsa.h"
#include "primes.h"
#include "misc.h"

void gen_keys(int up_bound, int low_bound, int *pub_exp, int *pub_prod, int *priv_key){
	int p, q, phi;
	p = gen_prime(up_bound, low_bound); // Remplacer par gen_strong_prime
	q = gen_prime(up_bound, low_bound); // bis
	*pub_prod = p*q;
	phi = *pub_prod -p-q+1;
	*pub_exp = 65537; // Efficace (sauf si on envoie le message à plus de 2^16 personnes)
	*priv_key = ((inv_mod(*pub_exp, phi) % phi) + phi ) % phi;
}

int* encrypt_naive(char* msg, int msg_size, int pub_exp, int pub_prod){
	int* res = malloc(msg_size*sizeof(int));
	for(int i=0; i<msg_size; i++)
		res[i] = exp_mod((int) msg[i], pub_exp, pub_prod);
	return res;
}

char* decrypt_naive(int* msg, int msg_size, int priv_key, int pub_prod){
	char* res = malloc(msg_size*sizeof(char));
	for(int i=0; i<msg_size; i++)
		res[i] = exp_mod(msg[i], priv_key, pub_prod);
	return res;
}