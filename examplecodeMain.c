#include <stdlib.h>
#include <stdio.h>
#include "aes256.h"

#define DUMP(s, i, buf, sz)  {printf(s);                   \
                              for (i = 0; i < (sz);i++)    \
                                  printf("%02x ", buf[i]); \
                              printf("\n");}

int main (int argc, char *argv[])
{
    aes256_context ctx;
    uint8_t key[32];
    uint8_t buf[16], i;

    printf( "Enter a value for text to encrypt:");
    fgets( buf, 100, stdin );
    for(i=0;i<3;i++)
    {
      printf("%c  %02x\n", buf[i], buf[i] );
    }
    /* put a test vector */
        printf( "Enter a value for key to encrypt:");
    fgets( key, 100, stdin );
    for(i=0;i<3;i++)
    {
      printf("%c  %02x\n", key[i], key[i] );
    }


    DUMP("txt: ", i, buf, sizeof(buf));
    DUMP("key: ", i, key, sizeof(key));
    printf("---\n");

    aes256_init(&ctx, key);
    aes256_encrypt_ecb(&ctx, buf);

    DUMP("enc: ", i, buf, sizeof(buf));
    printf("tst: 8e a2 b7 ca 51 67 45 bf ea fc 49 90 4b 49 60 89\n");

    aes256_init(&ctx, key);
    aes256_decrypt_ecb(&ctx, buf);
    DUMP("dec: ", i, buf, sizeof(buf));

    aes256_done(&ctx);

    return 0;
}
