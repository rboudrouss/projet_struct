int* erathostene(int n, int* nb_primes); //returns the n first prime numbers using the sieve of erathostenes, and sets *nbrPrimes=pi(n)
int gen_prime(int low_bound, int up_bound);
int gen_strong_prime(int low_bound, int up_bound); // Gordon's algorithm
bool witness(int n, int mant, int exp, int base);
bool miller_rabin(int n, int nb_base);
bool miller_rabin_deterministic(int n); // Works for n<2^64
bool fermat(int n, int nb_test);
bool naive_primality_base(int n, int *base, int base_size);
bool naive_primality(int n);