#define TEST_MSG "uwu"
#define NB_TEST 2

#define SM 15
#define B 10

static int small_p[SM] = {2, 3, 5, 7, 41, 89, 139, 197, 239, 331, 599, 701, 829, 1103, 3797};
// small composits
static int small_np[SM] = {1, 4, 6, 8, 9, 15, 21, 51, 221, 343, 413, 777, 973, 1547, 4477};
// big primes
static long big_p[B] = {2147483647, 2147482873, 2147480897, 2147478727, 2147470679, 2147469173, 2147465563, 2147463491, 2147462579, 2147462143};


void keys_tests();
void prime_tests();
void protec_test();
void rsa_tests();
void signs_tests();