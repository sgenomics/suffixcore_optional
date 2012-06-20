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

#ifndef GLOBALDEFS
#define GLOBALDEFS

#define index_type uint32_t
#define invalid_idx 4294967295

#define suffixnodestore_type SuffixNodeStoreDisk

//define one of these to select a memory allocator
#define use_tialloc 1

#define alphabet_size  65534
#define symbol_type uint16_t
#define symbol_size  65534
#define final_symbol 65535
#define suffixnode_t SuffixNode
#define searchtrans_store_type vector<uint8_t>


#define store_type vector<symbol_type>
#include "SuffixNodeStoreDisk.h"

#endif
