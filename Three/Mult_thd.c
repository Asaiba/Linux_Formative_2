#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX 100000
#define THREADS 20

int total_palindromes = 0;
pthread_mutex_t lock;

bool is_palindrome(int n) {
    int rev = 0, temp = n;
    while (temp) {
        rev = rev * 10 + temp % 10;
        temp /= 10;
    }
    return rev == n;
}

void* count_palindromes(void* arg) {
    int index = *(int*)arg;
    int start = index * (MAX / THREADS);
    int end = (index + 1) * (MAX / THREADS);

    int count = 0;
    for (int i = start; i < end; ++i)
        if (is_palindrome(i)) count++;

    pthread_mutex_lock(&lock);
    total_palindromes += count;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    int ids[THREADS];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, count_palindromes, &ids[i]);
    }

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    printf("The Synchronize total is %d\n", total_palindromes);
    return 0;
}
