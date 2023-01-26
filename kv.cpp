#pragma once
#include "basic_string.cpp"




struct ondisk_kv_hdr
{
    unsigned key_len;
    unsigned value_len;
    void parse_disk(char *ptr , long offset , basic_string *key , basic_string *value)
    {
        key -> len = key_len;
        key -> content = ptr + offset + 2 * sizeof(unsigned);
        value -> len = value_len;
        value -> content = ptr + offset + 2 * sizeof(unsigned) + key_len;
    }
};

struct kv
{
    basic_string key;
    basic_string value;

    kv(basic_string &&_key , basic_string &&_value) : key(_key) , value(_value) {};
    kv(const basic_string &_key ,const basic_string &_value) : key(_key) , value(_value) {};


    unsigned serialze(char *ptr);

};


unsigned kv::serialze(char *ptr)
{
    long result;
    long total = 0;

    if (ptr)
    {   
        (* (unsigned*) ptr) = key.len;
        ptr += sizeof(unsigned);
    }
    total += sizeof(unsigned);

    if (ptr)
    {
        (* (unsigned*) ptr) = value.len;
        ptr += sizeof(unsigned);
    }
    total += sizeof(unsigned);

    result = key.serialze(ptr);
    if (ptr)
        ptr += result;
    total += result;

    result = value.serialze(ptr);
    total += result;

    return total;
}


