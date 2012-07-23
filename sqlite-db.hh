/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "hashtable.hh"
#include "sqlite-objects.hh"

class SQLiteHashTable: public HashTable {
public:
    SQLiteHashTable(size_t cs, std::string name): cache_size(cs), dbfile(name) {

        std::stringstream cache_cmd;
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE 
            | SQLITE_OPEN_PRIVATECACHE | SQLITE_OPEN_NOMUTEX;

        db =  new SQLiteDB(dbfile, flags);
        db->set_journal_mode("OFF");
        db->start_transaction();
        cache_cmd<<"pragma cache_size="<<cache_size*1024;
        PreparedStatement create(db->getDB(), "CREATE TABLE K(key varchar(255));"),
                          cache(db->getDB(), cache_cmd.str().c_str());
        create.execute();
        cache.execute();

        insert_stmt = new PreparedStatement(db->getDB(), "INSERT INTO K VALUES(?);");
        read_stmt = new PreparedStatement(db->getDB(), "SELECT key FROM K WHERE key=?;");
    }

    ~SQLiteHashTable() {
        db->commit_transaction();
        delete read_stmt;
        delete insert_stmt;
        delete db;
    }

    bool lookup(std::string key) {
        read_stmt->reset();
        read_stmt->bind(1, key.c_str(), key.length());
        return read_stmt->fetch();
    }

    void insert(std::string key) {
        insert_stmt->reset();
        insert_stmt->bind(0, key.c_str(), key.length());
        assert(insert_stmt->execute());
    }

private:
    SQLiteDB *db;
    PreparedStatement *insert_stmt;
    PreparedStatement *read_stmt;
    size_t cache_size;
    std::string dbfile;

};

