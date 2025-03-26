#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  int A = 0x11223344;
  unsigned char *p = (unsigned char *)&A; // get the last byte of A
  printf("%x\n", *p);
  if (*p == 0x44) {
    printf("little endian\n");
  } else {
    printf("big endian\n");
  }
  if (argc == 2) {
    if (*(argv[1]) == 'H') { /* heap alloc */
      int *B = (int *)malloc(sizeof(int) * 1);
      if (NULL != B) {
        /* reverse bytes in A to B */
        *B = 0;
        unsigned char *pB = (unsigned char *)B;
        unsigned char *pA = (unsigned char *)&A;
        for (int i = 0; i < sizeof(int); i++) {
          *(pB + i) = *(pA + sizeof(int) - 1 - i);
        }
        printf("A = 0x%x, B = 0x%x\n", A, *B);
        free(B);
        return 0;
      } else
        return 2;
    } else if (*(argv[1]) == 'S') { /* stack alloc */
      int B = 0;
      unsigned char *pB = (unsigned char *)&B;
      unsigned char *pA = (unsigned char *)&A;
      for (int i = 0; i < sizeof(int); i++) {
        *(pB + i) = *(pA + sizeof(int) - 1 - i);
      }
      printf("0xA = %x, 0xB = %x\n", A, B);
      return 0;
    } else
      return 1;
  } else
    return 1;
}
