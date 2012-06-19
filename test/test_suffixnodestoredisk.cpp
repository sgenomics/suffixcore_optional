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

  SuffixNode::store = &store;
  SuffixNode::end_marker_value = 50;
  
  SuffixNode s5;
  for(size_t n=0;n<1000;n++) {
    cout << "n: " << n << endl;
    s5.set_child(n,n);
  }
  uint32_t idx5 = store.push_back(s5);

  SuffixNode s6;
  for(size_t n=0;n<1000;n++) {
    cout << "n: " << n << endl;
    s6.set_child(n,n);
  }
  uint32_t idx6 = store.push_back(s6);


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
  s2.set_depth(10);
  s2.set_label_start(10);
  s2.set_label_end(20);
  uint32_t idx2 = store.push_back(s2);

  SuffixNode s4;
  s4.set_label_start(10);
  uint32_t idx4 = store.push_back(s4);

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
 
  scheck = store.get(idx4); 
  utf.test_equality(scheck.child_count(),0);

  scheck = store.get(idx5);
  utf.test_equality(scheck.child_count(),1000);

  scheck = store.get(idx6);
  utf.test_equality(scheck.child_count(),1000);

  SuffixNode s3;
  s3.set_parent(idx2);
  s3.set_label_start(0);
  utf.test_equality(s3.get_depth_raw(),10);

  vector<size_t> sizes;
  vector<size_t> idxs;
  // random tests
  for(size_t i=0;i<500;i++) {
    SuffixNode srand;
    int rsize = rand()%500;
    for(size_t n=0;n<rsize;n++) {
      cout << "n: " << n << endl;
      srand.set_child(n,rand());
    }
    uint32_t idxr = store.push_back(srand);
    sizes.push_back(rsize);
    idxs.push_back(idxr);
  }

  for(size_t i=0;i<500;i++) {
    scheck = store.get(idxs[i]);
    utf.test_equality(scheck.child_count(),(int32_t) sizes[i]);
  }

  utf.end_test_set();
}
