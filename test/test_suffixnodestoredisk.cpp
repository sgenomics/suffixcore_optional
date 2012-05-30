#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "global_defs.h"
#include "SuffixNodeStoreDisk.h"
#include "./suffixcore/SuffixNode.h"

using namespace std;

void test_suffixnodestoredisk(UnitTest &utf) {

  utf.begin_test_set("SuffixNodeStoreDisk tests");

  SuffixNodeStoreDisk store("./test_store");

  SuffixNode s1;
  s1.set_child(1,5);
  s1.set_child(2,5);
  s1.set_child(1,8);
  s1.set_child(7,6);
  s1.set_child(3,4);
  uint32_t idx1 = store.push_back(s1);

  SuffixNode s2;
  s2.set_child(1,2);
  s2.set_child(2,3);
  s2.set_child(3,4);
  s2.set_child(7,5);
  s2.set_depth(100);
  uint32_t idx2 = store.push_back(s2);

  SuffixNode scheck;

  scheck = store.get(idx1);
  utf.test_equality(scheck.child_count(),4);
  utf.test_equality(scheck.get_child(1),8);
  utf.test_equality(scheck.get_child(2),5);
  utf.test_equality(scheck.get_child(3),4);
  utf.test_equality(scheck.get_child(7),6);
  utf.test_equality(scheck.get_child(5),-1);

  scheck = store.get(idx2);
  utf.test_equality(scheck.child_count(),4);
  utf.test_equality(scheck.get_child(1),2);
  utf.test_equality(scheck.get_child(2),3);
  utf.test_equality(scheck.get_child(3),4);
  utf.test_equality(scheck.get_child(7),5);
  utf.test_equality(scheck.get_child(50),-1);
  

  SuffixNode s3;
  s3.set_parent(idx2);
  utf.test_equality(s3.get_depth(),100);

  utf.end_test_set();
}


