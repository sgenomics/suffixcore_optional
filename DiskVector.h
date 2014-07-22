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
#define gz_buffer_size 256

using namespace std;

template<class data_type>
class DiskVector {

public:
  DiskVector() {}

  DiskVector(string filename,bool compress=false) {
    m_compress = compress;

    if(!m_compress) {
      filehandle_uc = fopen(filename.c_str(),"a+");
    } else {
      filehandle_gz = gzopen((filename + ".gz" ).c_str(),"wh");
      gzbuffer(filehandle_gz,gz_buffer_size*1024);
    }
    cout << "filehandle uc: " << filehandle_uc << endl;
    cout << "filehandle gz: " << filehandle_gz << endl;
  }

  data_type operator[](uint64_t index) {
    data_type data;

    if(!m_compress) {
      fseek((FILE *) filehandle_uc,((long)index)*sizeof(data_type),SEEK_SET);
      fread(&data,sizeof(data_type),1,(FILE *) filehandle_uc);
    }
    return data;
  }

  size_t push_back(data_type i) {

    size_t writepos;
    if(!m_compress) {
      fseek(filehandle_uc,(long)0,SEEK_END);
      writepos = ftell(filehandle_uc);
      fwrite(&i,sizeof(data_type),1,filehandle_uc);
    } else {
      writepos = gztell(filehandle_gz);
      gzwrite(filehandle_gz,&i,sizeof(data_type));
    }
    return writepos/sizeof(data_type);
  }

  size_t size() {
    size_t filesize;

    if(!m_compress) {
      fseek(filehandle_uc,(long)0,SEEK_END);
      filesize = ftell((FILE *) filehandle_uc);
    } else {
      filesize = gztell(filehandle_gz);
    }
    return filesize/sizeof(data_type);
  }

  void close() {
    if(filehandle_uc != 0) fclose(filehandle_uc);

    if(filehandle_gz != 0) {
      gzflush(filehandle_gz,Z_FINISH);
      gzclose(filehandle_gz);
    }
  }

  FILE *filehandle_uc;
  gzFile filehandle_gz;
  bool m_compress;
};


#endif
