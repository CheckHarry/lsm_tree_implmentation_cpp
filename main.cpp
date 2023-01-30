#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>
#include <random>
#include <sstream> 
#include "basic_string.cpp"
#include "kv.cpp"
#include "sstable.cpp"
#include "utils.cpp"



//#define MAKE






void flush_v(const char *file_name , std::vector<std::pair<std::string , std::string>> &s )
{
    std::map<basic_string , basic_string> mapping;
    
    basic_string b;
    basic_string c;

    for (auto &p : s)
    {
        b.setstr(p.first.c_str() , p.first.size());
        c.setstr(p.second.c_str() , p.second.size());
        mapping.insert({std::move(b) , std::move(c)});
    }

    flush_to_file(file_name , &mapping);
}


int main()
{
    
unsigned tomod = 198723;
unsigned opnum = 97321;
   
#ifdef MAKE
    using data_type = std::vector<std::pair<std::string , std::string>>;
    
    data_type v1 = {
        {"Harry" , "Jane1"} ,
        {"Germany" , "Berlin"}
    };
    
    flush_v("db_testing_level1_0.db" , v1);

    data_type v2 = {
        {"Harry" , "Jane2"} ,
    };
    
    flush_v("db_testing_level1_1.db" , v2);
    
    
#else
    
    auto ptr1 = map_file("db_testing_level1_0.db");
    auto ptr2 = map_file("db_testing_level1_1.db");
    
    sstable table1(ptr1);
    sstable table2(ptr2);

    merge_sstable("abc" , std::vector<sstable*>{&table1 , &table2});

    /*
    basic_string a;
    a.setstr(std::to_string(9999).c_str() , 4);
    int i = _sstable.find(a);
    std::cout << " len : " << _sstable.len() << '\n';
    for (int i = 0 ; i < _sstable.len() ; i ++)
    {
        std::cout << i << " : "; 
        _sstable[i].first.print();
        std::cout << " : ";
        _sstable[i].second.print();
        std::cout << "\n";
    }
    std::cout << i << '\n';*/
#endif
}