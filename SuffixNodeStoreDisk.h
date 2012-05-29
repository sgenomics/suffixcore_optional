/* -
 * Copyright (c) 2012 Nava Whiteford <new@sgenomics.org>
 * suffixcore - core suffixtree algorithms
 *
 * A license to use this software is granted to users given access to the git repository at: https://github.com/sgenomics/suffixcore
 * A complete copy of this license is located in the license.txt file of this package.
 *
 * In Summary this software:
 *
 * Can be used for creating unlimited applications.
 * Can be distributed in binary or object form only.
 * Commercial use is allowed.
 * Can modify source-code but cannot distribute modifications (derivative works).
 */

#ifndef SUFFIXNODESTOREDISK
#define SUFFIXNODESTOREDISK

#include "global_defs.h"
#include <vector>
#include <map>
#include <algorithm>
#include "tialloc.h"

class SuffixNode;

using namespace std;

#include <stdint.h>

class SuffixNodeStoreDisk {

public:
  SuffixNodeStoreDisk(string filename);
  void set_compactmode(bool compact_mode);
  size_t push_back_norm();
  size_t push_back_end();
  size_t push_back(SuffixNode &s,int resize=-1);
  SuffixNode &get(int idx);
  void set(int idx, SuffixNode &s);
  int size();
  int next_idx(int i);
  int last_idx();
  void stats();
  void force_compact();
  void compact();

  vector<FILE *> data_filehandle;
  FILE *index_filehandle;

};


#endif
