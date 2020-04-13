/*
Ahmed Alelg - 201507470
Homework 04
*/
#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#define VIRTUAL_ADDR_SIZE 32
#define PAGE_SIZE 12 //4096B=4kB

int main(int argc, char *argv[]) {
    if(argc < 2)
        {
            printf("Please provide a physical address\n"); return -1;}
    unsigned int virt_addr = atoi(argv[1]);
    unsigned int pageNumber  = virt_addr >> PAGE_SIZE;
    unsigned int distance = VIRTUAL_ADDR_SIZE - PAGE_SIZE;
    unsigned int offset = (virt_addr << (distance)) >> distance ;

    printf("\nThe address %u contains:\npage numer = %u\noffset = %u\n", virt_addr, pageNumber, offset);
}