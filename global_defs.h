#ifndef GLOBALDEFS
#define GLOBALDEFS

#define suffixnodestore_type SuffixNodeStoreMemVec
//#define suffixnodestore_type SuffixNodeStore

//define one of these to delect a storage mode
//#define object_store_type ObjectStoreMem
#define object_store_type ObjectStoreMemVec
//#define object_store_type ObjectStoreDisk

//define one of these to select a memory allocator
//#define use_malloc 1
#define use_tialloc 1

#define alphabet_size  40
#define symbol_size  41
#define final_symbol 39

#define store_type string

#endif
