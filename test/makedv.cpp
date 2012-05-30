#include <fstream>
#include <iostream>
#include <stdint.h>
using namespace std;

void write(uint16_t c,FILE *f) {

  uint8_t *cc = (uint8_t *) &c;

  putc(cc[0],f);
  putc(cc[1],f);
}

int main() {

  FILE *f;
  f = fopen("./ttestdv","w");

  uint16_t c;

  c = 100;
  write(c,f);
  c = 1000;
  write(c,f);
  c = 10000;
  write(c,f);
  c = 32545;
  write(c,f);
  c = 2;
  write(c,f);
  c = 1;
  write(c,f);
  c = 60000;
  write(c,f);


  fclose(f);
}
