#include <kos.h>
#include <stdio.h>
#include <string.h>
#include <dc/time.h>

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);

#define BLOCK_SIZE 512
#define NUM_BLOCKS 2048  // ~1MB

int main() {
    uint8_t *buffer = (uint8_t*)malloc(BLOCK_SIZE);
    memset(buffer, 0xAA, BLOCK_SIZE);

    FILE *f;
    uint64 start, end;
    double elapsed;

    dbglog(DBG_INFO, "Benchmarking SD card write speed...\n");

    f = fopen("/sd/bench_test.bin", "wb");
    if (!f) {
        dbglog(DBG_ERROR, "Failed to open file for writing.\n");
        return -1;
    }

    start = timer_ms_gettime64();
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        fwrite(buffer, 1, BLOCK_SIZE, f);
    }
    fclose(f);
    end = timer_ms_gettime64();
    elapsed = (end - start) / 1000.0;

    dbglog(DBG_INFO, "Wrote %d KB in %.2f seconds (%.2f KB/s)\n",
           (BLOCK_SIZE * NUM_BLOCKS) / 1024, elapsed,
           ((BLOCK_SIZE * NUM_BLOCKS) / 1024.0) / elapsed);

    // Now read
    dbglog(DBG_INFO, "Benchmarking SD card read speed...\n");
    f = fopen("/sd/bench_test.bin", "rb");
    if (!f) {
        dbglog(DBG_ERROR, "Failed to open file for reading.\n");
        return -1;
    }

    start = timer_ms_gettime64();
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        fread(buffer, 1, BLOCK_SIZE, f);
    }
    fclose(f);
    end = timer_ms_gettime64();
    elapsed = (end - start) / 1000.0;

    dbglog(DBG_INFO, "Read %d KB in %.2f seconds (%.2f KB/s)\n",
           (BLOCK_SIZE * NUM_BLOCKS) / 1024, elapsed,
           ((BLOCK_SIZE * NUM_BLOCKS) / 1024.0) / elapsed);

    free(buffer);
    while(1);
    return 0;
}
