#ifndef HASHTABLE
#define HASHTABLE 1

#include <iostream>

class HashTable {
public:
    virtual bool lookup(std::string key) = 0;
    virtual void insert(std::string key) = 0;
    virtual ~HashTable() {};
};

#endif
