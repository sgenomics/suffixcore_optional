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

using namespace std;

#include "SuffixNode.h"
#include <stdint.h>

SuffixNodeStoreDisk::SuffixNodeStoreDisk(string filename,bool compress) : basefilename(filename) {
  omp_init_lock(&index_filehandle_lock);
 
  m_compress = compress;
  // open index file handle
  if(!compress) {
    index_filehandle = fopen((filename + "/index").c_str(),"a+");
  } else {
    index_filehandle = gzopen((filename + "/index").c_str(),"a+");
  }

  // open datafile, file handles
  data_filehandle = vector<void *>(20000,(void *) 0);
  data_filehandle_lock = vector<omp_lock_t>(20000);
}

void *SuffixNodeStoreDisk::get_data_filehandle(uint32_t i) {
  if(data_filehandle[i] == 0) {
     if(!m_compress) {
      data_filehandle[i] = fopen((basefilename + "/" + stringify(i)).c_str(),"a+");
    } else {
      data_filehandle[i] = gzopen((basefilename + "/" + stringify(i)).c_str(),"a+");
    }
    omp_init_lock(&data_filehandle_lock[i]);
    return data_filehandle[i];
  } else {
    return data_filehandle[i];
  }
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

  #pragma omp task 
  {
    push_idx_entry(filenum,index);
  }
}

uint64_t SuffixNodeStoreDisk::push_idx_entry(uint16_t filenum,uint32_t index) {

  omp_set_lock(&index_filehandle_lock);

  uint8_t data[6];
  *((uint32_t *) data)   = index;
  *((uint16_t *) (data+4)) = filenum;
 
  if(!m_compress) {
    // If writing in sequence (repeated calls to this function) this appears to be a NOP. 
    fseek((FILE *) index_filehandle,(long)0,SEEK_END);
    fwrite(data,6,1,(FILE *) index_filehandle);
  } else {
    gzwrite(index_filehandle,data,6);
  }
  size_t i = (gztell(index_filehandle)/6)-1;
  omp_unset_lock(&index_filehandle_lock);

  return i;
}


void SuffixNodeStoreDisk::get_idx_entry(uint32_t idx,uint16_t &filenum,uint32_t &index) {

  char data[6];
  if(!m_compress) {
    fseek((FILE *) index_filehandle,((long)idx)*6,SEEK_SET);
    fread(data,6,1,(FILE *) index_filehandle);
  } else {
    gzread(index_filehandle,data,6);
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
    fseek((FILE *) get_data_filehandle(filenum),((long)index)*filenum,SEEK_SET);
    fread(data,filenum,1,(FILE *) get_data_filehandle(filenum));
  }
  return data;
}

void SuffixNodeStoreDisk::write_data(void *data,uint16_t filenum,uint32_t index) {
  if(!m_compress) {
    fseek((FILE *) get_data_filehandle(filenum),((long)index)*filenum,SEEK_SET);
    fwrite(data,filenum,1,(FILE *) get_data_filehandle(filenum));
  } else {
    gzwrite(get_data_filehandle(filenum),data,filenum);
  }
}

uint32_t SuffixNodeStoreDisk::push_data(uint16_t filenum, void *data) {

  
  get_data_filehandle(filenum);
  omp_set_lock(&data_filehandle_lock[filenum]);

  if(!m_compress) {
    fseek((FILE *) get_data_filehandle(filenum),(long)0,SEEK_END);
    fwrite(data,filenum,1,(FILE *) get_data_filehandle(filenum));
  } else {
    gzwrite(get_data_filehandle(filenum),data,filenum);
  }
  size_t i = (gztell(get_data_filehandle(filenum))-filenum)/filenum;

  omp_unset_lock(&data_filehandle_lock[filenum]);

  return i;
}

void SuffixNodeStoreDisk::set(uint32_t idx, SuffixNode &s) {
  write_data(s.get_data(),s.get_data_alloc_size(),idx);
}

uint32_t SuffixNodeStoreDisk::size() {
  if(!m_compress) {
    fseek((FILE *) index_filehandle,(long)0,SEEK_END);
  }
  size_t filesize = gztell(index_filehandle);
  return filesize/6;
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
  gzclose(index_filehandle);

  for(size_t n=0;n<data_filehandle.size();n++) {
    if(data_filehandle[n] != 0) gzclose(data_filehandle[n]);
  }
}
