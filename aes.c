#include <stdlib.h>
#include <stdio.h>
#include "aes.h"

static void encryptionKeyExpansion (uint8_t **, uint8_t *);
static void decryptionKeyExpansion (uint8_t **, uint8_t *);

int main()
{
  uint8_t roundKeys[40][4] = {{ 0x00, 0x01, 0x02, 0x03 },
  { 0x04, 0x05, 0x06, 0x07 },
  { 0x08, 0x09, 0x0a, 0x0b },
  { 0x0c, 0x0d, 0x0e, 0x0f }};
  uint8_t rawData[4][4] =
  {{0x00, 0x44, 0x88, 0xcc},
  {0x11, 0x55, 0x99, 0xdd},
  {0x22, 0x66, 0xaa, 0xee},
  {0x33, 0x77, 0xbb, 0xff}};
  shiftRows(rawData);




}

static void encryptionKeyExpansion (uint8_t ** key, uint8_t * rowConstant){

  uint8_t i;

  key[5][0] ^= access_sbox(key[3][3]) ^ (*rowConstant);
  key[5][1] ^= access_sbox(key[3][0]);
  key[5][2] ^= access_sbox(key[3][1]);
  key[5][3] ^= access_sbox(key[3][2]);
}
