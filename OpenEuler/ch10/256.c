#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 256
#define TLB_SIZE 16
#define PAGETABLE_SIZE 256
#define FRAMETABLE_SIZE 256
#define BACKING_SIZE PAGE_SIZE * PAGETABLE_SIZE
#define MEMORY_SIZE PAGE_SIZE * FRAMETABLE_SIZE

typedef struct TLB
{
    int page_number;
    int frame_number;
    int tlb_ref;
} TLB;

typedef struct PageTable
{
    int frame_number;
    int valid;
} PageTable;

TLB tlb[TLB_SIZE];
PageTable page_table[PAGETABLE_SIZE];
signed char main_memory[MEMORY_SIZE];
signed char *backing_store;

int frame_to_page[FRAMETABLE_SIZE];

int phys_page = 0;

int cntPageFaults = 0;
int cntTLBHits = 0;
int cntPageRefs = 0;
int tlb_ref = 0;

void backingStoreInit()
{
    backing_store = (signed char *)malloc(BACKING_SIZE);
    FILE *file = fopen("BACKING_STORE.bin", "rb");
    fread(backing_store, sizeof(signed char), BACKING_SIZE, file);
    fclose(file);
}
void tlbInit()
{
    tlb_ref = 0;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        tlb[i].page_number = -1;
        tlb[i].frame_number = -1;
        tlb[i].tlb_ref = 0;
    }
}
void pageTableInit()
{
    for (int i = 0; i < PAGETABLE_SIZE; i++)
    {
        page_table[i].frame_number = -1;
        page_table[i].valid = 0;
    }
    for (int i = 0; i < FRAMETABLE_SIZE; i++)
    {
        frame_to_page[i] = -1;
    }
}
int checkTLB(int page_number)
{
    for (int i = 0; i < TLB_SIZE; i++)
        if (tlb[i].page_number == page_number)
        {
            tlb[i].tlb_ref = ++tlb_ref;
            return tlb[i].frame_number;
        }
    return -1;
}
int checkPageTable(int page_number)
{
    if (page_table[page_number].valid)
        return page_table[page_number].frame_number;
    return -1;
}
void updateTLB(int page_number, int frame_number)
{
    int min_ref = tlb[0].tlb_ref;
    int min_index = 0;
    for (int i = 1; i < TLB_SIZE; i++)
        if (tlb[i].tlb_ref < min_ref)
        {
            min_ref = tlb[i].tlb_ref;
            min_index = i;
        }
    tlb[min_index].page_number = page_number;
    tlb[min_index].frame_number = frame_number;
    tlb[min_index].tlb_ref = ++tlb_ref;
}
void tlbFlush(int page_number)
{
    for (int i = 0; i < TLB_SIZE; i++)
        if (tlb[i].page_number == page_number)
        {
            tlb[i].page_number = -1;
            tlb[i].frame_number = -1;
            tlb[i].tlb_ref = 0;
            return;
        }
}
void updatePageTable(int page_number, int frame_number)
{
    int old_page = frame_to_page[frame_number];
    if(old_page != -1)
    {
        tlbFlush(old_page);
        page_table[old_page].frame_number = -1;
        page_table[old_page].valid = 0;
    }
    frame_to_page[frame_number] = page_number;
    page_table[page_number].frame_number = frame_number;
    page_table[page_number].valid = 1;
    memcpy(main_memory + frame_number * PAGE_SIZE, backing_store + page_number * PAGE_SIZE, PAGE_SIZE);
}

int main(int argc, char *argv[])
{
    backingStoreInit();
    tlbInit();
    pageTableInit();
    FILE *fp = fopen(argv[1], "r");
    while (1)
    {
        int logical_address;
        if (fscanf(fp, "%d", &logical_address) != 1)
            break;
        int page_number = logical_address / PAGE_SIZE;
        int offset = logical_address % PAGE_SIZE;
        int frame_number = -1;

        cntPageRefs++;
        if ((frame_number = checkTLB(page_number)) != -1)
        {
            // TLB hit
            cntTLBHits++;
        }
        else if ((frame_number = checkPageTable(page_number)) != -1)
        {
            // Pagetable hit
            updateTLB(page_number, frame_number);
        }
        else
        {
            // Page fault
            cntPageFaults++;
            
            frame_number = phys_page++;
            phys_page %= FRAMETABLE_SIZE;
            updatePageTable(page_number, frame_number);
            updateTLB(page_number, frame_number);
        }
        signed char value = main_memory[frame_number * PAGE_SIZE + offset];
        printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, frame_number * PAGE_SIZE + offset, value);
    }
    fclose(fp);
    printf("Number of Translated Addresses = %d\n", cntPageRefs);
    printf("Page Faults = %d\n", cntPageFaults);
    printf("Page Fault Rate = %.4f\n", (double)cntPageFaults / cntPageRefs);
    printf("TLB Hits = %d\n", cntTLBHits);
    printf("TLB Hit Rate = %.4f\n", (double)cntTLBHits / cntPageRefs);
    free(backing_store);
    return 0;
}
