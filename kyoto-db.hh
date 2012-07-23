/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "hashtable.hh"
#include <kchashdb.h>

class KyotoDBHashTable: public HashTable {
public:
    KyotoDBHashTable(size_t cs, size_t bs, std::string name): 
        cache_size(cs), dbfile(name), bucket_size(bs) {
        db.tune_alignment(0);
        db.tune_buckets(bucket_size);
        db.tune_map(cache_size*1024*1024);
        assert(db.open(dbfile.c_str(), kyotocabinet::HashDB::OWRITER | kyotocabinet::HashDB::OCREATE));
    }

    ~KyotoDBHashTable() {
        db.close();
    }

    bool lookup(std::string key) {
        std::string val;
        if (db.get(key, &val)) {
            return true;
        } else {
            return false;
        }
    }

    void insert(std::string key) {
        assert(db.set(key, ""));
    }

private:
    kyotocabinet::HashDB db;
    std::string dbfile;
    size_t cache_size;
    size_t bucket_size;

};


