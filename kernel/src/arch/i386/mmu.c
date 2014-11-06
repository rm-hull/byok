#include <kernel/system.h>

#define DIRECTORY_SIZE 1024
#define PAGE_SIZE      4096

extern unsigned int *_end;

void mmu_install()
{
    // page directory comes right after the kernel -- the address must be page aligned
    unsigned int page_aligned_end = (((unsigned int)_end) & 0xFFFFF000) + 0x1000;
    unsigned int *page_directory = (unsigned int*)page_aligned_end;

    // set each entry to not present
    for (int i = 0; i < DIRECTORY_SIZE; i++)
    {
        // attribute: supervisor level, read/write, not present.
        page_directory[i] = 0 | 2;
    }

    // first page table comes right after the page directory
    unsigned int *first_page_table = page_directory + DIRECTORY_SIZE;

    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    unsigned int address = 0;

    for (int i = 0; i < DIRECTORY_SIZE; i++)
    {

        first_page_table[i] = address | 3;  // attr: supervisor level, read/write, not present
        address += PAGE_SIZE;               // advance to next page boundary
    }

    // moves page directory (ptr) into the cr3 register.
    __asm __volatile("mov %0, %%cr3":: "b"(page_directory));

    // switch paging bit on cr0
    unsigned int cr0;
    __asm __volatile("mov %%cr0, %0": "=b"(cr0));
    cr0 |= 0x80000000;
    __asm __volatile("mov %0, %%cr0":: "b"(cr0));
}
