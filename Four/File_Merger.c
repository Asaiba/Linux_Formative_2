#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define MAX_FILES 64
#define MAX_LINE 1024
#define MAX_ENTRIES 1000000

// Structure to store each log entry with parsed timestamp
typedef struct {
    char line[MAX_LINE];
    time_t timestamp;
} LogEntry;

// Global array to store log entries
LogEntry entries[MAX_ENTRIES];
int entry_count = 0;
pthread_mutex_t entry_lock;

// Parse timestamp from log line (customize this format as needed)
time_t parse_timestamp(const char *line) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    // Assuming log line starts with "YYYY-MM-DD HH:MM:SS"
    strptime(line, "%Y-%m-%d %H:%M:%S", &tm);
    return mktime(&tm);
}

// Thread data structure
typedef struct {
    char *filename;
} ThreadData;

void* read_log_file(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    FILE *fp = fopen(data->filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        pthread_mutex_lock(&entry_lock);
        if (entry_count >= MAX_ENTRIES) {
            pthread_mutex_unlock(&entry_lock);
            break;
        }
        strncpy(entries[entry_count].line, line, MAX_LINE);
        entries[entry_count].timestamp = parse_timestamp(line);
        entry_count++;
        pthread_mutex_unlock(&entry_lock);
    }

    fclose(fp);
    return NULL;
}

// Compare function for qsort
int compare_entries(const void *a, const void *b) {
    return ((LogEntry*)a)->timestamp - ((LogEntry*)b)->timestamp;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s merged_log.txt file1.txt file2.txt ... <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[argc - 1]);
    int num_files = argc - 3;

    if (num_threads > num_files) num_threads = num_files;

    pthread_t threads[MAX_FILES];
    ThreadData thread_data[MAX_FILES];
    pthread_mutex_init(&entry_lock, NULL);

    // Create threads to read log files
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].filename = argv[2 + i];
        pthread_create(&threads[i], NULL, read_log_file, &thread_data[i]);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sort all collected entries
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);

    // Write sorted entries to output
    FILE *out = fopen(argv[1], "w");
    if (!out) {
        perror("fopen output");
        return 1;
    }

    for (int i = 0; i < entry_count; i++) {
        fputs(entries[i].line, out);
    }

    fclose(out);
    pthread_mutex_destroy(&entry_lock);
    printf("Merged %d log entries into %s\n", entry_count, argv[1]);
    return 0;
}