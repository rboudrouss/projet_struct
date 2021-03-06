#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "signs.h"

Signature *init_signature(long *content, int size)
{
    Signature *s = malloc(sizeof(Signature));
    if (size == 0 || !content)
    {
        s->size = 0;
        s->content = NULL;
        return s;
    }
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
    if(!sgn){
        printf("Warning : no signe given to sgn_to_str function\n");
        return strdup("#");
    }
    if(sgn->size==0 || !sgn->content)
        return strdup("#");
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
    if (!str)
        return NULL;
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
    Signature* sgn = init_signature(content,num);
    free(content);
    return sgn;
}
