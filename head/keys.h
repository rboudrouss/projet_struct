#ifndef __KEY__
#define __KEY__

#define STR_SIZE 256

typedef struct _key
{
    long val;
    long n;
} Key;

void init_key(Key *key, long val, long n);
void free_key(Key *key);
void init_pair_keys(Key *pKey, Key *sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);

#endif