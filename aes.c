#include <stdlib.h>
#include <stdio.h>

static void encryptionKeyExpansion (unit8_t *, unit8_t *);
static void decryptionKeyExpansion (unit8_t *, unit8_t *);

int main()
{
    uint8_t roundKeys[40][4];
    roundKeys[0] = [0x00, 0x04, 0x08, 0x0c ];
    roundKeys[1] = [0x01, 0x05, 0x09, 0x0d ];
    roundKeys[2] = [0x02, 0x06, 0x0a, 0x0e ];
    roundKeys[3] = [0x03, 0x07, 0x0b, 0x0f ];
    uint8_t rawData = [0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff];




}

static void encryptionKeyExpansion (uint8_t * key, uint8_t * rowConstant){

  uint8_t i;
  key[5][0] ^= sbox(key[3][0]) ^ (*rc);
  key[5][1] ^= sbox(key[3][1]);
  key[5][2] ^= sbox(key[3][2]);
  key[5][3] ^= sbox(key[3][3]);
}
