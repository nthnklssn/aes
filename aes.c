#include <stdlib.h>
#include <stdio.h>
#include "aes.h"


void subBytes(uint8_t data[4][4]){

  for(int j=0;j<4;j++)
  {
    for(int i=0;i<4;i++)
    {

      printf("%02X\n", data[i][j]);
      data[i][j] = sbox[data[i][j]];
      printf("%02X\n", data[i][j]);
    }
  }
}

void shiftRows(uint8_t data[4][4]){
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

void mixColumns(uint8_t data[4][4])
{
printf("DATA WERE NEED\n" );
  printf("%02X\n", mul2[data[0][0]] );
  printf("%02X\n", mul3[data[1][0]] );
  printf("%02X\n", data[0][0] );
  printf("%02X\n", data[0][0] );

for(int i = 0; i<4; i++)
{
  printf("%02X  %02X   %02X   %02X   %02X \n",   (mul2[(int)data[0][i]]), (mul3[(int)data[0][i]]), (data[0][i]), (data[0][i]),  ((data[0][i]) ^ (mul2[(int)data[0][i]]) ^ ((mul3[(int)data[0][i]]) ^ (data[0][i]))));
  data[0][i] = ((mul2[data[0][i]]) ^ ((mul3[data[0][i]]) ^ (data[0][i])) ^ (data[0][i]) );
  data[1][i] = ((data[1][i]) ^ (mul2[data[1][i]])) ^ ((mul3[data[1][i]]) ^ (data[1][i]));
  data[2][i] = ((data[2][i]) ^ (data[2][i])) ^ ((mul2[data[2][i]]) ^ (mul3[data[2][i]]));
  data[3][i] = ((mul3[data[3][i]]) ^ (data[3][i])) ^ ((data[3][i]) ^ (mul2[data[3][i]]));

}
}

int main()
{
  /* Round keys and raw data are organized in a way where MSB is top left and LSB is bottom right.
     Each character of key/text fills up vertically and then over to the next column.
  */
    uint8_t roundKeys[40][4] = {{ 0x00, 0x01, 0x02, 0x03 },
    { 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0a, 0x0b },
    { 0x0c, 0x0d, 0x0e, 0x0f }};
  /*  uint8_t rawData[4][4] =
    {{0x00, 0x44, 0x88, 0xcc},
    {0x11, 0x55, 0x99, 0xdd},
    {0x22, 0x66, 0xaa, 0xee},
    {0x33, 0x77, 0xbb, 0xff}};

    //shiftRows(rawData);
    //subBytes(rawData);*/
   uint8_t rawData[4][4] =
  {{0x19, 0xa0, 0x9a, 0xe9},
  {0x3d, 0xf4, 0xc6, 0xf8},
  {0xe3, 0xe2, 0x8d, 0x48},
  {0xbe, 0x2b, 0x2a, 0x08}};
  subBytes(rawData);
  for(int j=0;j<4;j++)
  {
    printf("\n" );
    for(int i=0;i<4;i++)
    {

      printf("%02X\n", rawData[i][j]);
    }
  }

    shiftRows(rawData);

      for(int j=0;j<4;j++)
      {
        printf("\n" );
        for(int i=0;i<4;i++)
        {

          printf("%02X\n", rawData[i][j]);
        }
      }
      mixColumns(rawData);

        for(int j=0;j<4;j++)
        {
          printf("\n" );
          for(int i=0;i<4;i++)
          {

            printf("%02X\n", rawData[i][j]);
          }
        }




}
