#include <fcntl.h>
#include <unistd.h>
int main(void) {
    char buffer[4096];
    int fd = openat(AT_FDCWD, "/flag", O_RDONLY);
    if (fd < 0) return 1;
    ssize_t count;
    while ((count = read(fd, buffer, sizeof buffer)) > 0) {
        ssize_t offset = 0;
        while (offset < count) {
            ssize_t written = write(STDOUT_FILENO, buffer + offset, (size_t)(count - offset));
            if (written < 0) return 1;
            offset += written;
        }
    }
    close(fd);
    return count < 0;
}
