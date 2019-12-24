#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <unistd.h>

char *mememem(char *haystack, size_t haystack_len, char *needle, size_t needle_len) {
    for (size_t i = 0; i < haystack_len; i++) {
        if (i + needle_len > haystack_len) return NULL;
        if (memcmp(&haystack[i], needle, needle_len) == 0) return &haystack[i];
    }
    return NULL;
}

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

    char *hello = mememem(data, len, "Hello", 5);
    if (hello) {
        memcpy(hello, "Blyat", 5);
    }

    printf("Before injected code\n");
    ((F) data)();
    printf("After injected code\n");

    return 0;
}
