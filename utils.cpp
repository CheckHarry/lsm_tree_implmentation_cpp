#pragma once
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


char* map_file(const char *file_name)
{
    // map file without overwrite the file
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

char* create_file(const char *file_name , long file_size)
{
    // map file overwrite the file
    long result;
    int fd = open(file_name , O_CREAT | O_RDWR | O_TRUNC , (mode_t) 0600);
    lseek(fd , file_size - 1 , SEEK_SET);
    
    char *ptr = (char*) mmap(0 , file_size , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0);
    
    result = write(fd, "", 1);
    if (result != 1) {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }

    return ptr;
}