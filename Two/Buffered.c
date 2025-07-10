#include <stdio.h>
#include <time.h>

int main() {
    FILE *fp = fopen("buffered_log.txt", "w");
    if (!fp) { perror("fopen"); return 1; }

    char line[100];
    for (int i = 0; i < 1000000; i++) {
        time_t now = time(NULL);
        snprintf(line, sizeof(line), "%d: %s", i, ctime(&now));
        fputs(line, fp); // buffered
    }

    fclose(fp);
    return 0;
}
