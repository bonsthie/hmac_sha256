#include "include/sha256.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define HASH_SIZE 32  // SHA-256 produces a 32-byte hash

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the file.
    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    // Get the size of the file.
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t filesize = st.st_size;

    // Memory-map the file.
    uint8_t *data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);  // File descriptor is no longer needed after mmap.

    uint8_t hash1[HASH_SIZE];
    uint8_t hash2[HASH_SIZE];

    // Time the standard sha256 function.
    struct timespec start, end;
    double elapsed_sha256, elapsed_sha256_avx2;

    // Timing sha256
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        munmap(data, filesize);
        return EXIT_FAILURE;
    }

    sha256(data, filesize, hash1);

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        munmap(data, filesize);
        return EXIT_FAILURE;
    }
    elapsed_sha256 = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    // Time the AVX2 sha256 function.
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        munmap(data, filesize);
        return EXIT_FAILURE;
    }

    sha256_avx2(data, filesize, hash2);

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        munmap(data, filesize);
        return EXIT_FAILURE;
    }
    elapsed_sha256_avx2 = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;

    // Unmap the file.
    if (munmap(data, filesize) == -1) {
        perror("munmap");
        return EXIT_FAILURE;
    }

    // Compare the hashes to ensure they are the same.
    if (memcmp(hash1, hash2, HASH_SIZE) != 0) {
        fprintf(stderr, "Error: Hashes do not match!\n");
        return EXIT_FAILURE;
    }

    // Print the results.
    printf("sha256 time:       %f seconds\n", elapsed_sha256);
    printf("sha256_avx2 time:  %f seconds\n", elapsed_sha256_avx2);

    // Optionally, print the hash.
    printf("Hash: ");
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("%02x", hash1[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
