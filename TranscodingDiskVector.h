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
  TranscodingDiskVector(string filename) {
    filehandle = fopen(filename.c_str(),"r");
  }

  uint16_t operator[](uint64_t index) {

    uint16_t data;

    fseek(filehandle,index*size_of(uint16_t),SEEK_SET);
    fread(&data,size_of(uint16_t),1,filehandle);
    return data;
  }

  FILE *filehandle;
};


#endif
