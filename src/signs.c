#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "signs.h"
#include "keys.h"

Signature *init_signature(long *content, int size)
{
    Signature *s = malloc(sizeof(Signature));
    s->content = malloc(sizeof(long) * size);
    for (int i = 0; i < size; i++)
        s->content[i] = content[i];
    s->size = size;
    return s;
}

void free_signature(Signature *sign)
{
    free(sign->content);
    free(sign);
}

Signature *sign(char *mess, Key *sKey)
{
    int size = strlen(mess);
    long *s = encrypt(mess, sKey->val, sKey->n);
    Signature *rep = init_signature(s, size);
    free(s);
    return rep;
}

char *signature_to_str(Signature *sgn)
{
    char *result = malloc(10 * sgn->size * sizeof(char));
    result[0] = '#';
    int pos = 1;
    char buffer[156];
    for (int i = 0; i < sgn->size; i++)
    {
        sprintf(buffer, "%lx", sgn->content[i]);
        for (int j = 0; j < strlen(buffer); j++)
        {
            result[pos] = buffer[j];
            pos = pos + 1;
        }
        result[pos] = '#';
        pos = pos + 1;
    }
    result[pos] = '\0';
    result = realloc(result, (pos + 1) * sizeof(char));
    return result;
}

Signature *str_to_signature(char *str)
{
    int len = strlen(str);
    long *content = (long *)malloc(sizeof(long) * len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for (int i = 0; i < len; i++)
        if (str[i] != '#')
        {
            buffer[pos] = str[i];
            pos = pos + 1;
        }
        else if (pos != 0)
        {
            buffer[pos] = '\0';
            sscanf(buffer, "%lx", &(content[num]));
            num = num + 1;
            pos = 0;
        }
    content = realloc(content, num * sizeof(long));
    return init_signature(content, num);
}
