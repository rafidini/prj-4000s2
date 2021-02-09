#include <stdio.h>
#include <stdlib.h>

typedef unsigned char ubyte8;
typedef char byte8;
typedef char boolean;

typedef unsigned short ubyte16;
typedef short byte16;

typedef unsigned long ubyte32;
typedef long byte32;

typedef float float32;
typedef double float64;

// teste les longueurs des différents types de base

int main() 
{
  printf("size uint8   : %d\n", sizeof(uint8));
  printf("size int8    : %d\n", sizeof(int8));
  printf("size boolean : %d\n", sizeof(boolean));

  printf("size uint16  : %d\n", sizeof(uint16));
  printf("size int16   : %d\n", sizeof(int16));

  printf("size uint32  : %d\n", sizeof(uint32));
  printf("size int32   : %d\n", sizeof(int32));

  printf("size float32 : %d\n", sizeof(float32));
  printf("size float64 : %d\n", sizeof(float64));
}
