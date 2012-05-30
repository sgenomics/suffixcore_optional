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

#ifndef DISKVECTOR
#define DISKVECTOR

#include <stdio.h>
#include <stdint.h>

using namespace std;

template<class data_type>
class DiskVector {

public:
  DiskVector() {}

  DiskVector(string filename) {
    filehandle = fopen(filename.c_str(),"r");
  }

  data_type operator[](uint64_t index) {

    data_type data;

    fseek(filehandle,index*sizeof(data_type),SEEK_SET);
    fread(&data,sizeof(data_type),1,filehandle);
    return data;
  }

  void push_back(uint16_t i) {}

  size_t size() {
    fseek(filehandle,0,SEEK_END);
    size_t filesize = ftell(filehandle);
    return filesize/sizeof(uint16_t);
  }

  FILE *filehandle;
};


#endif
