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

#include <vector>
#include <map>
#include <algorithm>
#include "tialloc.h"
#include <stdio.h>
#include <omp.h>
#include <zlib.h>

class SuffixNode;

using namespace std;

#include <stdint.h>

class SuffixNodeStoreDisk {

public:
  SuffixNodeStoreDisk() {}
  SuffixNodeStoreDisk(string filename,bool compress=false);
  void set_compactmode(bool compact_mode);
  size_t push_back_norm();
  size_t push_back_end();
  size_t push_back(SuffixNode &s,int resize=-1);
  void push_back_nort(SuffixNode &s);
  SuffixNode get(uint32_t idx);
  void set(uint32_t idx, SuffixNode &s);
  uint32_t  size();
  uint32_t  next_idx(uint32_t i);
  uint32_t  last_idx();
  void stats();
  void force_compact();
  void compact();

  FILE  *get_data_filehandle_uc(uint32_t i);
  gzFile get_data_filehandle_gz(uint32_t i);

  template<class copying_type>
  void copy(copying_type &other) {
    #pragma omp parallel for
    for(uint32_t n=0;n<other.size();n++) {
      push_back_nort(other.get(n));
    }
  }
  
  uint64_t push_idx_entry(uint16_t filenum,uint32_t index);
  void     get_idx_entry(uint32_t idx,uint16_t &filenum,uint32_t &index);
  void    *read_data(uint16_t filenum,uint32_t index);
  void     write_data(void *data,uint16_t filenum,uint32_t index);
  uint32_t push_data(uint16_t filenum, void *data);
  void close();

  vector<gzFile> data_filehandle_gz;
  vector<FILE *> data_filehandle_uc;
  vector<omp_lock_t> data_filehandle_lock;

  gzFile index_filehandle_gz;
  FILE *index_filehandle_uc;
  omp_lock_t index_filehandle_lock;

  string basefilename;
  bool m_compress;
};


#endif
