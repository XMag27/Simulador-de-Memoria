#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAGE_TABLE_ENTRIES 256
#define PAGE_SIZE 256
#define FRAME_COUNT 256
#define FRAME_SIZE 256
#define PHYSICAL_MEMORY_SIZE FRAME_COUNT * FRAME_SIZE

