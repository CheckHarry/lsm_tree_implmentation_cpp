#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "basic_string.cpp"
#include <unistd.h>
#include <stdlib.h>
#include "kv.cpp"
#include "random"
#include <string>
#include <map>
#include <iostream>
#include "sstable.cpp"
#include <sstream> 



//#define MAKE


char* map_file(const char *file_name)
{
    long result;
    int fd = open(file_name , O_CREAT | O_RDWR , (mode_t) 0600);
    long file_size = lseek(fd , 0 , SEEK_END);
    char *ptr = (char*) mmap(0 , file_size , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0);
    
    result = write(fd, "", 1);
    if (result != 1) {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }

    return ptr;
}


int main()
{
    
unsigned tomod = 198723;
unsigned opnum = 97321;
   
#ifdef MAKE
    
    std::map<basic_string , basic_string> mapping;
    
    basic_string b;
    basic_string c;

    std::string a1 = "Harry";
    std::string a2 = "Jane1";

    b.setstr(a1.c_str() , a1.size());
    c.setstr(a2.c_str() , a2.size());


    mapping.insert({b , c});

    flush_to_file("db_testing_level1.db" , &mapping);

    std::map<basic_string , basic_string> mapping2;
    

    a1 = "Harry";
    a2 = "Jane2";

    b.setstr(a1.c_str() , a1.size());
    c.setstr(a2.c_str() , a2.size());


    mapping2.insert({b , c});

    flush_to_file("db_testing_level2.db" , &mapping2);
    
#else
    
    auto ptr1 = map_file("db_testing_level1.db");
    auto ptr2 = map_file("db_testing_level2.db");
    
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