#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main() {
    pid_t pid;
    int status;

    // Temporary file creation
    char *filename = "tempfile.txt";
    int fd = open(filename, O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // Write to file
    const char *msg = "This is a temp file.\n";
    write(fd, msg, strlen(msg));
    lseek(fd, 0, SEEK_SET); // Go back to beginning

    // Read from file
    char buffer[256];
    read(fd, buffer, sizeof(buffer));
    printf("Parent read: %s", buffer);
    close(fd);

    // Fork and execvp
    pid = fork();
    if (pid == 0) {
        // Child
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        // Parent
        wait(&status);
        printf("Child completed.\n");
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
