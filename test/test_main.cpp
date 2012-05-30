#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "UnitTest.h"
#include "global_defs.h"
#include "../SuffixNodeStoreDisk.h"
#include "test_suffixnodestoredisk.h"
#include "test_diskvector.h"

using namespace std;

int main(int argc,char ** argv) {
  UnitTest utf("41J Tests");

  test_suffixnodestoredisk(utf);
  test_diskvector(utf);
  utf.test_report();
}
