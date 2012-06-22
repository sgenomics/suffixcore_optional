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

SuffixNodeStoreDisk::SuffixNodeStoreDisk(string filename) : basefilename(filename) {
 
  // open index file handle
  index_filehandle = fopen((filename + "/index").c_str(),"a+");

  // open datafile, file handles
  data_filehandle = vector<FILE *>(1000000,(FILE *) 0);
}

FILE *SuffixNodeStoreDisk::get_data_filehandle(uint32_t i) {
  if(data_filehandle[i] == 0) {
    data_filehandle[i] = fopen((basefilename + "/" + stringify(i)).c_str(),"a+");
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

uint64_t SuffixNodeStoreDisk::push_idx_entry(uint16_t filenum,uint32_t index) {

  uint8_t data[6];
  *((uint32_t *) data)   = index;
  *((uint16_t *) (data+4)) = filenum;
 
  fseek(index_filehandle,(long)0,SEEK_END);
  fwrite(data,6,1,index_filehandle);

  return (ftell(index_filehandle)/6)-1;
}


void SuffixNodeStoreDisk::get_idx_entry(uint32_t idx,uint16_t &filenum,uint32_t &index) {

  char data[6];
  fseek(index_filehandle,((long)idx)*6,SEEK_SET);
  fread(data,6,1,index_filehandle);

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

  fseek(get_data_filehandle(filenum),((long)index)*filenum,SEEK_SET);
  fread(data,filenum,1,get_data_filehandle(filenum));
  return data;
}

void SuffixNodeStoreDisk::write_data(void *data,uint16_t filenum,uint32_t index) {
  fseek(get_data_filehandle(filenum),((long)index)*filenum,SEEK_SET);
  fwrite(data,filenum,1,get_data_filehandle(filenum));
}

uint32_t SuffixNodeStoreDisk::push_data(uint16_t filenum, void *data) {
  fseek(get_data_filehandle(filenum),(long)0,SEEK_END);
  fwrite(data,filenum,1,get_data_filehandle(filenum));

  return (ftell(get_data_filehandle(filenum))-filenum)/filenum;
}

void SuffixNodeStoreDisk::set(uint32_t idx, SuffixNode &s) {
  write_data(s.get_data(),s.get_data_alloc_size(),idx);
}

uint32_t SuffixNodeStoreDisk::size() {
  fseek(index_filehandle,(long)0,SEEK_END);
  size_t filesize = ftell(index_filehandle);
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
