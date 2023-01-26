#pragma once
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include <queue>
// as in my design , each kv pair is of differen size
// so need a side table for O(log(n)) search

using table_len_type = unsigned;
class side_table
{
    /*
    Record the absolute address 
    
    */
public:
    
    table_len_type len;
    long *offset_table = nullptr;
    

    side_table(unsigned _len)
    {
        // initialize side table
        len = _len;
        offset_table = (long*) malloc(len * sizeof(long));
    }

    side_table() {};

    void init(const unsigned _len)
    {
        len = _len;
        offset_table = (long*) malloc(len * sizeof(long));
    }

    void init_from_disk(char *ptr)
    {
        len = *((table_len_type*) ptr);
        offset_table = (long*) (ptr + sizeof(table_len_type));
    }

    unsigned get_on_disk_len() const
    {
        return  len * sizeof(long) + sizeof(table_len_type);
    }

    unsigned serialze(char *ptr) const
    {
        *((table_len_type*)ptr) = len;
        ptr += sizeof(table_len_type);
        memcpy(ptr , offset_table , len * sizeof(long));
        return len * sizeof(long) + sizeof(table_len_type);
    }

    void table_free()
    {
        free((void*) offset_table);
    }

};


struct sstable_hdr
{
    unsigned data_offset;
    unsigned side_table_offset;
};



unsigned flush_memtable(char *ptr , std::map<basic_string , basic_string> *mem_table)
{
    /*
    If ptr == nullptr , we don't actually flush to the file but return the total length
    */

    if (ptr == nullptr)
    {
        unsigned to_return = 0;
        to_return += sizeof(sstable_hdr);
        
        for (auto p : *mem_table)
        {   
            kv _kv(p.first , p.second);
            to_return += _kv.serialze(nullptr);
        }

        to_return += sizeof(table_len_type);
        to_return += mem_table-> size() * sizeof(long);
        return to_return;
    }

    side_table st;
    sstable_hdr *hdr_ptr;
    hdr_ptr = (sstable_hdr*) ptr;

    char * data_segment_start_ptr = ptr + sizeof(sstable_hdr);

  
    hdr_ptr -> data_offset = (unsigned) (data_segment_start_ptr - ptr);
    st.init(mem_table -> size());
    
        
    int count = 0;
    for (auto p : *mem_table)
    {   
        kv _kv(p.first , p.second);

        unsigned len = _kv.serialze(data_segment_start_ptr);

        
        st.offset_table[count] = (long)(data_segment_start_ptr - ptr);

        data_segment_start_ptr += len;
        count ++;
    }

    
    hdr_ptr -> side_table_offset = (unsigned) (data_segment_start_ptr - ptr);

    data_segment_start_ptr += st.serialze(data_segment_start_ptr);

    st.table_free();
    return (unsigned) (data_segment_start_ptr - ptr);
}



void flush_to_file(const char *file_name , std::map<basic_string , basic_string> *mem_table)
{
    long result;
    
    int fd = open(file_name , O_CREAT | O_RDWR | O_TRUNC , (mode_t) 0600);
    //int fd = open("abc.txt" , O_CREAT | O_RDWR , (mode_t) 0600);
    long file_size = flush_memtable(nullptr , mem_table);
    lseek(fd , file_size - 1 , SEEK_SET);
    char *ptr = (char*) mmap(0 , file_size , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0);
    
    result = write(fd, "", 1);
    if (result != 1) {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }

    flush_memtable(ptr , mem_table);
}

class sstable // to manage on disk sstable
{
public:
    sstable_hdr *hdr;
    side_table st;
    char * disk_ptr;

    sstable(char *ptr) : disk_ptr(ptr)
    {
        hdr = (sstable_hdr *) disk_ptr;
        st.init_from_disk(disk_ptr + hdr -> side_table_offset);
    }

    const std::pair<basic_string , basic_string> operator[](unsigned i)
    {
        ondisk_kv_hdr *a;
        a = (ondisk_kv_hdr *)(disk_ptr + st.offset_table[i]);
        basic_string key;
        basic_string value;
        a -> parse_disk(disk_ptr , st.offset_table[i] , &key , &value);
        return {key , value};
    }

    int find(basic_string k)
    {
        int left = 0 , right = len() - 1;
        while (left <= right)
        {
            unsigned mid = (left + right) >> 1;
            auto p = operator[](mid);
            if (p.first < k)
                left = mid + 1;
            else if (p.first == k)
                return mid;
            else
                right = mid - 1;
        }

        return -1;
    }

    unsigned len() const
    {
        return st.len;
    }
};


void merge_sstable(const char *file_name , std::vector<sstable*> sstable_list)
{
    using tuple_type = std::tuple<basic_string , basic_string , int>;
    unsigned sstable_no = sstable_list.size();
    auto cmp = [](std::tuple<basic_string , basic_string , int> &x , std::tuple<basic_string , basic_string , int> &y)
    {
        if (std::get<0>(x) == std::get<0>(x))   
            return std::get<0>(x) > std::get<0>(x);
        else
            return std::get<2>(x) < std::get<2>(x);
    };
    std::priority_queue< tuple_type , std::vector<tuple_type> , decltype(cmp)> pq(cmp);

    std::vector<unsigned> track(sstable_no);
    basic_string cur;

    for (int i = 0 ; i < sstable_no ; i ++)
    {
        pq.push(
            {(*sstable_list[i])[0].first , (*sstable_list[i])[0].second , i}
        );
    }

    while (!pq.empty())
    {
        std::get<0>(pq.top()).print();
        std::cout << " : ";
        std::get<1>(pq.top()).print();
        std::cout << " : " << std::get<2>(pq.top()) << '\n';
        pq.pop();
    }


}