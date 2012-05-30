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
  uint8_t testv0;
  uint8_t testv1;

  testv = testtdv[0];
  testv0 = testv >> 8;
  testv1 = (uint8_t) testv;
  utf.test_equality((int)testv0,(int)10);
  utf.test_equality((int)testv1,(int)15);

  testv = testtdv[1];
  testv0 = testv >> 8;
  testv1 = (uint8_t) testv;
  utf.test_equality((int)testv0,(int)10);
  utf.test_equality((int)testv1,(int)15);

  testv = testtdv[2];
  testv0 = testv >> 8;
  testv1 = (uint8_t) testv;
  utf.test_equality((int)testv0,(int)11);
  utf.test_equality((int)testv1,(int)1);

  utf.test_equality(testtdv.size(),3);

  utf.end_test_set();
}
