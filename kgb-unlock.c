#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <unistd.h>

typedef void (*F)(void);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./kgb-unlock <secret.raw>");
        exit(-1);
    }

    const char *filepath = argv[1];

    int fd = open(filepath, O_RDONLY);
    assert(fd >= 0);

    struct stat fd_stat;
    int err = fstat(fd, &fd_stat);
    assert(err == 0);

    size_t len = fd_stat.st_size;
    char *data = mmap(NULL, len,
                       PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE, fd, 0);

    // TODO: memmem is not portable
    char *hello = memmem(data, len, "Hello", 5);
    if (hello) {
        memcpy(hello, "Blyat", 5);
    }

    printf("Before injected code\n");
    ((F) data)();
    printf("After injected code\n");

    return 0;
}
