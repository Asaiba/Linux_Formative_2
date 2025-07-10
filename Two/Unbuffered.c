#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main() {
    int fd = open("unbuffered_log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }

    char line[100];
    for (int i = 0; i < 1000000; i++) {
        time_t now = time(NULL);
        snprintf(line, sizeof(line), "%d: %s", i, ctime(&now));
        write(fd, line, strlen(line)); // unbuffered write
    }

    close(fd);
    return 0;
}
