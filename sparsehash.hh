/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "hashtable.hh"
#include <google/sparse_hash_set>

class SparseHashTable: public HashTable {
public:
    bool lookup(std::string key) {
        google::sparse_hash_set<std::string>::const_iterator it = store.find(key);
        if (it != store.end()) {
            return true;
        } else {
            return false;
        }
    }

    void insert(std::string key) {
        store.insert(key);
    }
    
private:
    google::sparse_hash_set<std::string> store;
};


