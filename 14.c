#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        return 1;
    }

    long long as_size_mb = atoll(argv[1]);
    long long page_size_kb = atoll(argv[2]);
    long long virtual_addr = atoll(argv[3]);

    long long as_size_bytes = as_size_mb * 1024 * 1024;
    long long page_size_bytes = page_size_kb * 1024;

    if (virtual_addr >= as_size_bytes) {
        printf("Error: Virtual address is outside the address space.\n");
        return 1;
    }

    long long num_pages = as_size_bytes / page_size_bytes;

    int *page_table = (int *)malloc(num_pages * sizeof(int));
    for (int i = 0; i < num_pages; i++) {
        page_table[i] = i + 100; 
    }

    long long page_number = virtual_addr / page_size_bytes;
    long long offset = virtual_addr % page_size_bytes;

    if (page_number >= num_pages || page_table[page_number] == -1) {
        printf("Page table miss!\n");
    } else {
        int frame_number = page_table[page_number];
        long long physical_addr = (frame_number * page_size_bytes) + offset;

        printf("Virtual Address: %lld\n", virtual_addr);
        printf("Page Number: %lld, Offset: %lld\n", page_number, offset);
        printf("Physical Address: %lld\n", physical_addr);
    }

    free(page_table);
    return 0;
}