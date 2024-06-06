#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "playfair.h"
#include "bmp.h"


int main(void) {
  char *encrypted, *decrypted;
  encrypted = playfair_encrypt("SeCReT", "Hello world");
  decrypted = playfair_decrypt("SeCReT", encrypted);

  free(encrypted);
  free(decrypted);

  unsigned char *bmpencrypted = bmp_encrypt("CoMpUtEr", "Hello World!");
  char *bmpdecrypted = bmp_decrypt("CoMpUtEr", bmpencrypted);

  free(bmpdecrypted);
  free(bmpencrypted);
  return 0;
}