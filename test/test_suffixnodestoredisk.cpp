#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "SuffixNode.h"
#include "SuffixNodeStoreDisk.h"
#include "../global_defs.h"

using namespace std;

void test_suffixnodestoredisk(UnitTest &utf) {

  utf.begin_test_set("SuffixNodeStoreDisk tests");

  SuffixNode s;
  s.set_child(1,5);
  s.set_child(2,5);
  s.set_child(1,8);
  s.set_child(7,6);
  s.set_child(3,4);

  uint32_t idx = store.push_back(s);

  SuffixNode s2 = store.get(idx);
  utf.test_equality(s2.child_count(),4);
  utf.test_equality(s2.get_child(1),8);
  utf.test_equality(s2.get_child(2),5);
  utf.test_equality(s2.get_child(3),4);
  utf.test_equality(s2.get_child(7),6);
  utf.test_equality(s2.get_child(5),-1);

  utf.end_test_set();
}


