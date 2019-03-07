#include <stdlib.h>
#include <stdio.h>
#include "aes.h"

static void initialize (keyStruct * , uint8_t [10][4][4]);
static void encryptionKeyExpansion (keyStruct *, uint8_t *, int);
static void decryptionKeyExpansion (keyStruct *, uint8_t *, int);
static void printRoundKey (keyStruct *);
static void subBytes(uint8_t [4][4]);
static void shiftRows(uint8_t [4][4]);

int main()
{
  keyStruct * keys = malloc(sizeof(keyStruct));
  uint8_t roundKeys[10][4][4] = {{{ 0x00, 0x01, 0x02, 0x03 },
  { 0x04, 0x05, 0x06, 0x07 },
  { 0x08, 0x09, 0x0a, 0x0b },
  { 0x0c, 0x0d, 0x0e, 0x0f }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }},
  {{ 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00 }}
  };
  uint8_t rawData[4][4] =
  {{0x00, 0x44, 0x88, 0xcc},
  {0x11, 0x55, 0x99, 0xdd},
  {0x22, 0x66, 0xaa, 0xee},
  {0x33, 0x77, 0xbb, 0xff}};
  shiftRows(rawData);
  initialize(keys,roundKeys);
  printRoundKey(keys);
}

static void initialize (keyStruct * keyStruct, uint8_t key[10][4][4]){

  uint8_t rowConstant = 1;
  int i;
  int j;
  int k;
  for(i=0; i<10; i++){
    for(j=0;j<4;j++){
      for(k=0;k<4;k++){
        keyStruct->encryptKey[i][j][k] = keyStruct->decryptKey[i][j][k] = key [i][j][k];
      }
    }
  }
  encryptionKeyExpansion(keyStruct, &rowConstant, 1);

  for(i=2;i<10;i++){
    rowConstant *= 2;
    encryptionKeyExpansion(keyStruct, &rowConstant, i);
  }
}

static void subBytes(uint8_t data[4][4]){

int j,i;

  for(j=0;j<4;j++)
  {
    for(i=0;i<4;i++)
    {

      printf("%02X\n", data[i][j]);
      data[i][j] = sbox[data[i][j]];
      printf("%02X\n", data[i][j]);
    }
  }
}

static void shiftRows(uint8_t data[4][4]){
  uint8_t placeHolder[4];
  //Second row
  placeHolder[0] = data[1][0];
  placeHolder[1] = data[1][1];
  placeHolder[2] = data[1][2];
  placeHolder[3] = data[1][3];
  data[1][0] = placeHolder[1];
  data[1][1] = placeHolder[2];
  data[1][2] = placeHolder[3];
  data[1][3] = placeHolder[0];

  //third row

  placeHolder[0] = data[2][0];
  placeHolder[1] = data[2][1];
  placeHolder[2] = data[2][2];
  placeHolder[3] = data[2][3];
  data[2][0] = placeHolder[2];
  data[2][1] = placeHolder[3];
  data[2][2] = placeHolder[0];
  data[2][3] = placeHolder[1];

  // fourth row
  placeHolder[0] = data[3][0];
  placeHolder[1] = data[3][1];
  placeHolder[2] = data[3][2];
  placeHolder[3] = data[3][3];
  data[3][0] = placeHolder[3];
  data[3][1] = placeHolder[0];
  data[3][2] = placeHolder[1];
  data[3][3] = placeHolder[2];
}

static void encryptionKeyExpansion (keyStruct * keys, uint8_t * rowConstant, int block){

  uint8_t i;

  keys->encryptKey[block][0][0] = keys->encryptKey[block -1][1][0] ^ access_sbox(keys->encryptKey[block -1][3][3]) ^ (*rowConstant);
  keys->encryptKey[block][0][1] = keys->encryptKey[block -1][1][1] ^ access_sbox(keys->encryptKey[block -1][3][0]);
  keys->encryptKey[block][0][2] = keys->encryptKey[block -1][1][2] ^ access_sbox(keys->encryptKey[block -1][3][1]);
  keys->encryptKey[block][0][3] = keys->encryptKey[block -1][1][3] ^ access_sbox(keys->encryptKey[block -1][3][2]);

  for (i=1;i<4;i++){
    keys->encryptKey[block][i][0] = keys->encryptKey[block][i-1][0] ^ keys->encryptKey[block -1][i][0];
    keys->encryptKey[block][i][1] = keys->encryptKey[block][i-1][1] ^ keys->encryptKey[block -1][i][1];
    keys->encryptKey[block][i][2] = keys->encryptKey[block][i-1][2] ^ keys->encryptKey[block -1][i][2];
    keys->encryptKey[block][i][3] = keys->encryptKey[block][i-1][3] ^ keys->encryptKey[block -1][i][3];
  }
}

static void printRoundKey (keyStruct * keys){
  int i,j,k;

  for(i=0;i<10;i++){
    for(j=0;j<4;j++){
      for(k=0;k<4;k++){
        if(k != 3){
          printf("%02X, ", keys->encryptKey[i][j][k]);
        }else{
          printf("%02X ", keys->encryptKey[i][j][k]);
        }
      }
      printf("\n");
    }
    printf("\n");
  }
}
