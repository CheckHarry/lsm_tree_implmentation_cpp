#pragma once
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>



class basic_string
{
    /*
    A string class to manage stack-allocated string on heap-allocated memory.
    using to_free to handle.
    */
    
public:
    unsigned len = 0;
    unsigned capacity = 0;
    char *content = nullptr;
    bool to_free = false;

    basic_string(){};

    basic_string(const basic_string& rhs)
    {
        mkstr(rhs.content , rhs.len);
    }

    basic_string(basic_string&& rhs)
    {
        content = rhs.content;
        len = rhs.len;
        capacity = rhs.capacity;
        to_free = true;
        rhs.content = nullptr;
        rhs.len = 0;
        rhs.capacity = 0;
        rhs.to_free = false;
    }

    void operator=(const basic_string& rhs)
    {
        // incase this string is 
        if (to_free)
        {
            free(content);
            content = nullptr;
            to_free = false;
        }

        if (rhs.to_free) // rhs is a dynamically allocated basic string
            setstr(rhs.content , rhs.len);
        else
        {
            content = rhs.content;
            len = rhs.len;
            to_free = false;
        }
    }

    bool operator==(const basic_string& rhs) const
    {
        if (len != rhs.len)
            return false;

        for (int i = 0 ; i < len ; i ++)
        {
            if (content[i] != rhs.content[i])
                return false;
        }
        
        return true;
    }

    unsigned serialze(char *ptr) const
    {
        if (ptr)
            memcpy(ptr , content , len);
        return len;
    }

    void mkstr(const char *s , unsigned _len)
    {   
        to_free = true;
        content = (char *) malloc(_len);
        len = _len;
        capacity = _len;
        memcpy(content , s , len);
    }

    bool operator<(const basic_string  &rhs) const
    {
        unsigned l = len > rhs.len ? rhs.len : len;

        for (unsigned i = 0 ; i < l ; i ++)
        {
            if (content[i] < rhs.content[i])
                return true;
            else if (content[i] > rhs.content[i])
                return false;
        }

        if (len < rhs.len)
            return true;
        else
            return false;
    }

    bool operator>(const basic_string  &rhs) const
    {
        return !(*this < rhs);
    }

    void setstr(const char *s , unsigned _len)
    {
        if (content)
        {
            if (_len > capacity)
            {
                free(content);
                content = (char *) malloc(_len);
                len = _len;
                capacity = _len;
                memcpy(content , s , len);
            }
            else
            {
                memcpy(content , s , _len);
                len = _len;
            }   
        }
        else
        {
            mkstr(s , _len);
        }
        
    }

    void print() const
    {
        for (unsigned i = 0 ; i < len ; i ++)
        {
            putchar(content[i]);
        }
    }
    
    ~basic_string()
    {
        if (to_free && content)
        {
            //printf("free %s \n" , content);
            free((void*) content);
            to_free = false;
            content = nullptr;
        }
            
    }
};