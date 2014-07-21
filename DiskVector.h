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
#include <zlib.h>

using namespace std;

template<class data_type>
class DiskVector {

public:
  DiskVector() {}

  DiskVector(string filename,bool compress=false) {
    m_compress = compress;

    if(!m_compress) {
      filehandle = fopen(filename.c_str(),"a+");
    } else {
      filehandle = gzopen(filename.c_str(),"a+");
    }
    cout << "filehandle: " << filehandle << endl;
  }

  data_type operator[](uint64_t index) {
    data_type data;

    if(!m_compress) {
      fseek((FILE *) filehandle,((long)index)*sizeof(data_type),SEEK_SET);
      fread(&data,sizeof(data_type),1,(FILE *) filehandle);
    }
    return data;
  }

  size_t push_back(data_type i) {

    size_t writepos;
    if(!m_compress) {
      fseek((FILE *) filehandle,(long)0,SEEK_END);
      writepos = ftell((FILE *) filehandle);
      fwrite(&i,sizeof(data_type),1,(FILE *) filehandle);
    } else {
      writepos = gztell(filehandle);
      gzwrite(filehandle,&i,sizeof(data_type));
    }
    return writepos/sizeof(data_type);
  }

  size_t size() {
    size_t filesize;

    if(!m_compress) {
      fseek((FILE *) filehandle,(long)0,SEEK_END);
      filesize = ftell((FILE *) filehandle);
    } else {
      filesize = gztell(filehandle);
    }
    return filesize/sizeof(data_type);
  }

  void *filehandle;
  bool m_compress;
};


#endif
