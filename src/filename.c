#include <stdlib.h>
#include <time.h>
#include <string.h>

char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyz1234567890";

    srand((unsigned int) time(NULL) + (unsigned int) rand());
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char *create_output_path(char *dir)
{
    size_t baselen = 20 * sizeof(char);
    size_t dirlen = strlen(dir) * sizeof(char);
    char *filename = malloc(baselen);
    char *path = malloc(dirlen + sizeof(char) + baselen);

    rand_string(filename, baselen);
    memcpy(filename + baselen - 5, ".pdf", 4);

    memcpy(path, dir, dirlen);
    memcpy(path + dirlen, "/", 1);
    memcpy(path + dirlen + 1, filename, 21 * sizeof(char));

    return path;
}
