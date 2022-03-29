void gen_keys(int up_bound, int low_bound, int *pub_exp, int *pub_prod, int *priv_key);
int* encrypt_naive(char* msg, int msg_size, int pub_exp, int pub_prod);
char* decrypt_naive(int* msg, int msg_size, int priv_key, int pub_prod);
