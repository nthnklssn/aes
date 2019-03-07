#include <stdlib.h>
#include <stdio.h>
#include "aes.h"

static void encrypt (uint8_t [4][4], keyStruct *);
static void decrypt (uint8_t [4][4], keyStruct *);
static void initialize (keyStruct * , uint8_t [ROUNDS][4][4]);
static void encryptionKeyExpansion (keyStruct *, uint8_t, int);
static void decryptionKeyExpansion (keyStruct *, uint8_t, int);
static void printDataBlock (uint8_t [4][4]);
static void printRoundKeyBlocks (keyStruct *, int, int);
static void addRoundKey (keyStruct *, uint8_t [4][4], int round);
static void inverseAddRoundKey (keyStruct *, uint8_t [4][4], int round);
static void subBytes(uint8_t [4][4]);
static void shiftRows(uint8_t [4][4]);

static uint8_t roundConstants[ROUNDS];

int main()
{
  /* Round keys and raw data are organized in a way where MSB is top left and LSB is bottom right.
  Each character of key/text fills up vertically and then over to the next column.
  */
  keyStruct * keys = malloc(sizeof(keyStruct));
  uint8_t roundKeys[ROUNDS][4][4] =
  {{{ 0x00, 0x01, 0x02, 0x03 },
  { 0x04, 0x05, 0x06, 0x07 },
  { 0x08, 0x09, 0x0a, 0x0b },
  { 0x0c, 0x0d, 0x0e, 0x0f }}};
  uint8_t testKeys[ROUNDS][4][4] =
  {{{ 0x2b, 0x28, 0xab, 0x09 },
  { 0x7e, 0xae, 0xf7, 0xcf },
  { 0x15, 0xd2, 0x15, 0x4f },
  { 0x16, 0xa6, 0x88, 0x3c }}};
  uint8_t rawData[4][4] =
  {{0x00, 0x44, 0x88, 0xcc},
  {0x11, 0x55, 0x99, 0xdd},
  {0x22, 0x66, 0xaa, 0xee},
  {0x33, 0x77, 0xbb, 0xff}};

  uint8_t testData[4][4] =
  {{0x04, 0xe0, 0x48, 0x28},
  {0x66, 0xcb, 0xf8, 0x06},
  {0x81, 0x19, 0xd3, 0x26},
  {0xe5, 0x9a, 0x7a, 0x4c}};


  initialize(keys,roundKeys);
  printf("Original: \n");
  printDataBlock(rawData);
  encrypt(rawData, keys);
  printf("Encrypted: \n");
  printDataBlock(rawData);
  //inverseAddRoundKey(keys, rawData, 1);
  //printf("Decrypted: \n");
  //printDataBlock(rawData);

}

static void encrypt(uint8_t dataBlock[4][4], keyStruct * keys){

  int i;

  addRoundKey(keys, dataBlock, 0);
  for(i=1;i<ROUNDS;i++){
    subBytes(dataBlock);
    shiftRows(dataBlock);
    if(i != (ROUNDS - 1)){
      //mixColumns(dataBlock);
    }
    addRoundKey(keys, dataBlock, i);
  }

}

static void decrypt(uint8_t dataBlock[4][4], keyStruct * keys){

  int i;

  for(i=(ROUNDS - 1);i>=0;i--){
    inverseAddRoundKey(keys, dataBlock, i);
    if(i != (ROUNDS - 1)){
      //mixColumns(dataBlock); -- NEED INVERSE VERSION
    }
    //shiftRows(dataBlock); -- NEED INVERSE VERSION
    //subBytes(dataBlock); -- NEED INVERSE VERSION
  }
  inverseAddRoundKey(keys, dataBlock, i);
}

static void initialize (keyStruct * keyStruct, uint8_t key[ROUNDS][4][4]){

  //rowConstants[0] = 1;
  int i,j,k;
  for(i=0;i<ROUNDS;i++){
    if(i == 0){
      roundConstants[i] = 1;
    }else if((i > 0) && (roundConstants[i - 1] < 0x50)){
      roundConstants[i] = roundConstants[i - 1] * 2;
    }else if((i > 0) && (roundConstants[i - 1] >= 0x50)){
      roundConstants[i] = (2 * roundConstants[i - 1]);// ^ 0x11;
      roundConstants[i] ^= 0x1b;
    }
  }


  for(i=0; i<ROUNDS; i++){
    for(j=0;j<4;j++){
      for(k=0;k<4;k++){
        keyStruct->encryptKey[i][j][k] = key [i][j][k];
      }
    }
  }

  for(i=1;i<ROUNDS;i++){
    encryptionKeyExpansion(keyStruct, roundConstants[i - 1], i);
  }
}

static void subBytes(uint8_t data[4][4]){

  int j,i;

  for(j=0;j<4;j++)
  {
    for(i=0;i<4;i++)
    {

      //printf("%02X\n", data[i][j]);
      data[i][j] = sbox[data[i][j]];
      //printf("%02X\n", data[i][j]);
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

static void addRoundKey (keyStruct * keys, uint8_t rawData[4][4], int round){

  //XORs the raw data in each element of a block with the matching block of the rounds' key

  int i,j;

  for(j=0;j<4;j++){
    for(i=0;i<4;i++){
      rawData[i][j] ^= keys->encryptKey[round][i][j];
    }
  }
}


static void inverseAddRoundKey (keyStruct * keys, uint8_t rawData[4][4], int round){

    //XORs the raw data in each element of a block with the matching block of the rounds' key

  int i,j;

  for(j=0;j<4;j++){
    for(i=0;i<4;i++){
      rawData[i][j] ^= keys->encryptKey[round][i][j];
    }
  }
}

static void encryptionKeyExpansion (keyStruct * keys, uint8_t rowConstant, int block){

  uint8_t i;

  //Rotating first column upwards

  keys->encryptKey[block][0][0] = keys->encryptKey[block -1][1][3];
  keys->encryptKey[block][1][0] = keys->encryptKey[block -1][2][3];
  keys->encryptKey[block][2][0] = keys->encryptKey[block -1][3][3];
  keys->encryptKey[block][3][0] = keys->encryptKey[block -1][0][3];

  //Translating through sbox

  keys->encryptKey[block][0][0] = access_sbox( keys->encryptKey[block][0][0]);
  keys->encryptKey[block][1][0] = access_sbox( keys->encryptKey[block][1][0]);
  keys->encryptKey[block][2][0] = access_sbox( keys->encryptKey[block][2][0]);
  keys->encryptKey[block][3][0] = access_sbox( keys->encryptKey[block][3][0]);

  //XORing the top row of the first column with the row constant

  keys->encryptKey[block][0][0] ^= (rowConstant);

  //XORing the resultant column with the first column of the previous block

  keys->encryptKey[block][0][0] ^= keys->encryptKey[block - 1][0][0];
  keys->encryptKey[block][1][0] ^= keys->encryptKey[block - 1][1][0];
  keys->encryptKey[block][2][0] ^= keys->encryptKey[block - 1][2][0];
  keys->encryptKey[block][3][0] ^= keys->encryptKey[block - 1][3][0];

  //getting each subsequent column by XORing it with it's equivalent in the previous block, and the column right before it in the same block


  for (i=1;i<4;i++){
    keys->encryptKey[block][0][i] = keys->encryptKey[block][0][i-1] ^ keys->encryptKey[block -1][0][i];
    keys->encryptKey[block][1][i] = keys->encryptKey[block][1][i-1] ^ keys->encryptKey[block -1][1][i];
    keys->encryptKey[block][2][i] = keys->encryptKey[block][2][i-1] ^ keys->encryptKey[block -1][2][i];
    keys->encryptKey[block][3][i] = keys->encryptKey[block][3][i-1] ^ keys->encryptKey[block -1][3][i];
  }

}

static void printRoundKeyBlocks (keyStruct * keys, int startBlock, int endBlock){

  //prints large sets of datablocks, used for printing the round keys.

  int i,j,k;

  if((endBlock < ROUNDS) && (startBlock >= 0)){
    for(i=startBlock;i<=endBlock;i++){
      printDataBlock(keys->encryptKey[i]);
      printf("\n");
    }
  }else{
    printf("ERROR!\n" );
    return;
  }
}

static void printDataBlock (uint8_t data[4][4]){

  //prints out a 4x4 datablock in the correct orientation

  int i,j;

  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      if(j != 3){
        printf("%02X, ", data[i][j]);
      }else{
        printf("%02X ", data[i][j]);
      }
    }
    printf("\n");
  }
}
