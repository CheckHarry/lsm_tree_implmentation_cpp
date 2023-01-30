#pragma once
#include <vector>
#include <map>
#include <string>
#include "meta_data.cpp"
#include "basic_string.cpp"
#include "sstable.cpp"


#define THRESHOLD 400





struct kv_db
{
    using memtable_type = std::map<basic_string , basic_string>;
    memtable_type *cur_map;    
    std::vector<sstable*> sstable_list;
    meta_data meta;
    unsigned count = 3;

    kv_db()
    {
        cur_map = new memtable_type();
    }

    void run() {};

    void insert(const basic_string &key , const basic_string &value )
    {
        cur_map->insert({key , value});
    }


    void destruct()
    {
        std::string file_name = "db_testing_level1_" + std::to_string(count) + ".db";
        flush_to_file(file_name.c_str() , cur_map);
    }

};



