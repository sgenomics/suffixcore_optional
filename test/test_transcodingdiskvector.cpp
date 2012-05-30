#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "TranscodingDiskVector.h"
using namespace std;

void test_transcodingdiskvector(UnitTest &utf) {

  utf.begin_test_set("TranscodingDiskVector tests");

  TranscodingDiskVector testtdv("./testdv");
 
  uint16_t testv;

  testv = testtdv[0];
  utf.test_equality((int)((char *)&(testv))[0],(int)10);
  utf.test_equality((int)((char *)&(testv))[1],(int)15);

  testv = testtdv[1];
  utf.test_equality((int)((char *)&(testv))[0],(int)10);
  utf.test_equality((int)((char *)&(testv))[1],(int)15);

  testv = testtdv[2];
  utf.test_equality((int)((char *)&(testv))[0],(int)11);
  utf.test_equality((int)((char *)&(testv))[1],(int)1);

  utf.end_test_set();
}
