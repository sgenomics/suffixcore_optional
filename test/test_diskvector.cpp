#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "DiskVector.h"
using namespace std;

void test_diskvector(UnitTest &utf) {

  utf.begin_test_set("DiskVector tests");

  DiskVector<uint8_t> testdv("./testdv");
 
  utf.test_equality((int)testdv[0],(int) 10);
  utf.test_equality((int)testdv[1],(int) 15);
  utf.test_equality((int)testdv[2],(int) 10);
  utf.test_equality((int)testdv[3],(int) 15);
  utf.test_equality((int)testdv[4],(int) 11);
  utf.test_equality((int)testdv[5],(int)  1);
  utf.test_equality(testdv.size(),(size_t)6);
 
  utf.end_test_set();
}


