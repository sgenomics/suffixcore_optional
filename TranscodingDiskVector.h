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

#ifndef TRANSCODINGDISKVECTOR
#define TRANSCODINGDISKVECTOR

#include <stdio.h>

using namespace std;

class TranscodingDiskVector {

public:

  TranscodingDiskVector() {}

  TranscodingDiskVector(string filename) {
    filehandle = fopen(filename.c_str(),"r");
  }

  uint16_t operator[](uint64_t index) {

    uint16_t data;

    fseek(filehandle,index*sizeof(uint16_t),SEEK_SET);
    fread(&data,sizeof(uint16_t),1,filehandle);
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
