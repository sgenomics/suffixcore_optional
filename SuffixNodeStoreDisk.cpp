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

#include <vector>
#include <zlib.h>
#include <map>
#include <algorithm>
#include "tialloc.h"
#include "global_defs.h"
#include "SuffixNodeStoreDisk.h"
#include "SuffixNode.h"
#include "stringify.h"
#include <errno.h>
#include <omp.h>

using namespace std;

#include "SuffixNode.h"
#include <stdint.h>

#define gz_buffer_size 256

SuffixNodeStoreDisk::SuffixNodeStoreDisk(string filename,bool compress) : basefilename(filename) {
  //omp_init_lock(&index_filehandle_lock);
 
  m_compress = compress;
  // open index file handle
  if(!m_compress) {
    index_filehandle_uc = fopen((filename + "/index").c_str(),"a+");
    data_filehandle_uc = vector<FILE *>(20000,(FILE *) 0);
  } else {
    cout << "filename: " << filename + "/index" << endl;
    index_filehandle_gz = gzopen((filename + "/index.gz").c_str(),"wh");
    cout << "index filehandle: " << index_filehandle_gz << endl;
    if(index_filehandle_gz == 0) {
      cout << "error filehandle not opened: " << errno << endl;
    }
    gzbuffer(index_filehandle_gz,gz_buffer_size*1024);
    data_filehandle_gz = vector<gzFile>(200000);
  }

  // open datafile, file handles
  //data_filehandle_lock = vector<omp_lock_t>(20000);
  for(size_t n=0;n<data_filehandle_lock.size();n++) {
    //omp_init_lock(&data_filehandle_lock[n]);
  }
}

gzFile SuffixNodeStoreDisk::get_data_filehandle_gz(uint32_t i) {
  if(data_filehandle_gz[i] == 0) {
    data_filehandle_gz[i] = gzopen((basefilename + "/" + stringify(i) + ".gz").c_str(),"wh");
    gzbuffer(data_filehandle_gz[i],gz_buffer_size*1024);
  }

  return data_filehandle_gz[i];
}

FILE *SuffixNodeStoreDisk::get_data_filehandle_uc(uint32_t i) {
  if(data_filehandle_uc[i] == 0) {
    data_filehandle_uc[i] = fopen((basefilename + "/" + stringify(i)).c_str(),"a+");
  }
  return data_filehandle_uc[i];
}

void SuffixNodeStoreDisk::set_compactmode(bool compact_mode) {
}

size_t SuffixNodeStoreDisk::push_back_norm() {
  SuffixNode s;
  return push_back(s,2);
}

size_t SuffixNodeStoreDisk::push_back_end() {
  SuffixNode s;
  return push_back(s);
}

size_t SuffixNodeStoreDisk::push_back(SuffixNode &s,int resize) {

  uint16_t filenum = s.get_data_alloc_size();
  uint32_t index   = push_data(filenum,s.get_data());

  size_t idx = push_idx_entry(filenum,index);

  return idx;
}

void SuffixNodeStoreDisk::push_back_nort(SuffixNode &s) {
  
  uint16_t filenum = s.get_data_alloc_size();
  uint32_t index   = push_data(filenum,s.get_data());
  push_idx_entry(filenum,index);
}

uint64_t SuffixNodeStoreDisk::push_idx_entry(uint16_t filenum,uint32_t index) {

  //omp_set_lock(&index_filehandle_lock);

  uint8_t data[6];
  *((uint32_t *) data)   = index;
  *((uint16_t *) (data+4)) = filenum;
 
  if(!m_compress) {
    // If writing in sequence (repeated calls to this function) this appears to be a NOP. 
    fseek((FILE *) index_filehandle_uc,(long)0,SEEK_END);
    fwrite(data,6,1,(FILE *) index_filehandle_uc);
    size_t i = (ftell(index_filehandle_uc)/6)-1;
    //omp_unset_lock(&index_filehandle_lock);
    return i;
  } else {
    gzwrite(index_filehandle_gz,data,6);
    size_t i = (gztell(index_filehandle_gz)/6)-1;
    //omp_unset_lock(&index_filehandle_lock);
    return i;
  }

}


void SuffixNodeStoreDisk::get_idx_entry(uint32_t idx,uint16_t &filenum,uint32_t &index) {

  char data[6];
  if(!m_compress) {
    fseek(index_filehandle_uc,((long)idx)*6,SEEK_SET);
    fread(data,6,1,index_filehandle_uc);
  } else {
    gzread(index_filehandle_gz,data,6);
  }

  index   = *((uint32_t *) data);
  filenum = *((uint16_t *)  (data+4));
}

SuffixNode SuffixNodeStoreDisk::get(uint32_t idx) {
  uint16_t filenum;
  uint32_t index;
  get_idx_entry(idx,filenum,index);

  void* data = read_data(filenum,index);

  SuffixNode node(-1);
  node.set_data(data);
  return node;
}

void *SuffixNodeStoreDisk::read_data(uint16_t filenum,uint32_t index) {

  void *data = tialloc::instance()->alloc(filenum);

  if(!m_compress) {
    fseek(get_data_filehandle_uc(filenum),((long)index)*filenum,SEEK_SET);
    fread(data,filenum,1,get_data_filehandle_uc(filenum));
  }
  return data;
}

void SuffixNodeStoreDisk::write_data(void *data,uint16_t filenum,uint32_t index) {
  if(!m_compress) {
    fseek(get_data_filehandle_uc(filenum),((long)index)*filenum,SEEK_SET);
    fwrite(data,filenum,1,get_data_filehandle_uc(filenum));
  } else {
    //omp_set_lock(&data_filehandle_lock[filenum]);
    gzwrite(get_data_filehandle_gz(filenum),data,filenum);
    //omp_unset_lock(&data_filehandle_lock[filenum]);
  }
}

uint32_t SuffixNodeStoreDisk::push_data(uint16_t filenum, void *data) {

  if(!m_compress) {
    //omp_set_lock(&data_filehandle_lock[filenum]);
    fseek(get_data_filehandle_uc(filenum),(long)0,SEEK_END);
    fwrite(data,filenum,1,get_data_filehandle_uc(filenum));
    uint32_t i = (ftell(get_data_filehandle_uc(filenum))-filenum)/filenum;
    //omp_unset_lock(&data_filehandle_lock[filenum]);
    return i;
  } else {
    //omp_set_lock(&data_filehandle_lock[filenum]);
    gzwrite(get_data_filehandle_gz(filenum),data,filenum);
    uint32_t i = (gztell(get_data_filehandle_gz(filenum))-filenum)/filenum;
    //omp_unset_lock(&data_filehandle_lock[filenum]);
    return i;
  }
}

void SuffixNodeStoreDisk::set(uint32_t idx, SuffixNode &s) {
  write_data(s.get_data(),s.get_data_alloc_size(),idx);
}

uint32_t SuffixNodeStoreDisk::size() {
  if(!m_compress) {
    fseek(index_filehandle_uc,(long)0,SEEK_END);
    size_t filesize = ftell(index_filehandle_uc);
    return filesize/6;
  } else {
    size_t filesize = gztell(index_filehandle_gz);
    return filesize/6;
  }
}

uint32_t SuffixNodeStoreDisk::next_idx(uint32_t i) {
  return i+1;
}

uint32_t SuffixNodeStoreDisk::last_idx() {
  return size()-1;
}

void SuffixNodeStoreDisk::stats() {
}

void SuffixNodeStoreDisk::force_compact() {
}

void SuffixNodeStoreDisk::compact() {
}

void SuffixNodeStoreDisk::close() {
  if(!m_compress) {
    fclose(index_filehandle_uc);
  } else {
    gzflush(index_filehandle_gz,Z_FINISH);
    gzclose(index_filehandle_gz);
  }

  for(size_t n=0;n<data_filehandle_uc.size();n++) {
    if(data_filehandle_uc[n] != 0) {
      fclose(data_filehandle_uc[n]);
    }
  }

  for(size_t n=0;n<data_filehandle_gz.size();n++) {
    if(data_filehandle_gz[n] != 0) {
      gzflush(data_filehandle_gz[n],Z_FINISH);
      gzclose(data_filehandle_gz[n]);
    }
  }
}
