#include "sqlite-db.hh"
#include "kyoto-db.hh"
#include "sparsehash.hh"
#include "timing.hh"
#include <iomanip>
#include <iostream>

#define BUCKET_SIZE 100000000
#define COUNT 100
#define CACHE_SIZE 100
#define KEY_PREFIX "empire_GIFT_REQUEST_"

void benchmark(HashTable *h, uint64_t c) {
    Timing t_lookup("Lookup"), t_insert("Insert"),
           t_formkey("Framing keys"), t_total("Total");
    bool found;

    for(; c; c--) {
        t_formkey.start();
        std::stringstream ss;
        ss<<KEY_PREFIX<<std::setw(11) << std::setfill('0')<<c;
        t_formkey.stop();
        t_lookup.start();
        found = h->lookup(ss.str());
        t_lookup.stop();

        if (!found) {
            t_insert.start();
            h->insert(ss.str());
            t_insert.stop();
        }
   }
}

double getUsedMemory() {
    size_t memsize;
    FILE *f = fopen("/proc/self/statm", "r");
    if (!f) {
        perror("");
        exit(1);
    }
    fscanf(f, "%ld", &memsize);
    fclose(f);

    return memsize/1024.0;
}

void usage(char **argv) {
    std::cout<<"Usage: "<<argv[0]<<" -d dbfile -c cache_size (MB) -b buckets -n keycount -t sqlite|kyotodb|sparsehash"<<std::endl;
    exit(1);
}

int main(int argc, char **argv) {

    std::string dbfile("none"), type;
    size_t cache_size(CACHE_SIZE), bucket_size(BUCKET_SIZE);
    uint64_t count(COUNT);
    int c;
    HashTable *ht;
    stringstream ss_mem;

    while ((c = getopt (argc, argv, "d:c:b:n:t:")) != -1) { 
        switch (c) 
        {   
            case 'd':
                dbfile = optarg;
                break;
            case 'c':
                cache_size = atoi(optarg);
                break;
            case 'b':
                bucket_size = atoi(optarg);
                break;
            case 'n':
                count = atoi(optarg);
                break;
            case 't':
                type = optarg;
                break;
            case '?':
                std::cout<<"Option -"<<(char) optopt<<" requires argument"<<std::endl;
            default:
                usage(argv);
        }
    }

    if (type == "sqlite") {
        if (dbfile == "none") {
            usage(argv);
        }
        ht = new SQLiteHashTable(cache_size, dbfile);
    } else if (type == "kyotodb") {
        if (dbfile == "none") {
            usage(argv);
        }
        ht = new KyotoDBHashTable(cache_size, bucket_size, dbfile);
    } else if (type == "sparsehash") {
        ht = new SparseHashTable;
    } else {
        std::cout<<"Hashtable type not supported"<<std::endl;
        usage(argv);
    }
    std::cout<<"Going to insert "<<count<<" keys into "<<type<<std::endl;
    benchmark(ht, count);
    std::cout<<"Memory consumed: "<<getUsedMemory()<<"M"<<std::endl;
    delete ht;
}

