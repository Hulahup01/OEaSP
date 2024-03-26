#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_THREADS 8

typedef struct {
    int *array;
    int start_index;
    int end_index;
} pthrData;

void bubbleSort(int* array, int start_index, int end_index) {
    for (int i = start_index; i < end_index; i++) {
        for (int j = end_index; j > i; j--) {
            if (array[j - 1] > array[j]) {
                int temp = array[j - 1];
                array[j - 1] = array[j];
                array[j] = temp;
            }
        }
    }
}

void merge(int *array, int *temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (array[i] <= array[j])
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }
    while (i <= mid)
        temp[k++] = array[i++];
    
    while (j <= right)
        temp[k++] = array[j++];
    
    for (i = left; i <= right; ++i)
        array[i] = temp[i];
    
}

void mergeSort(int *array, int *temp, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(array, temp, left, mid);
        mergeSort(array, temp, mid + 1, right);
        merge(array, temp, left, mid, right);
    }
}

void* threadSort(void* arg) {
    pthrData *data = (pthrData*) arg;
    bubbleSort(data->array, data->start_index, data->end_index);
    return NULL;
}

void parallel_sort(int array[], int size, int num_threads) {
    pthread_t threads[num_threads];
    pthrData thread_data[num_threads];
    int segment_size = size / num_threads;
    int remainder = size % num_threads;

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].array = array;
        thread_data[i].start_index = i * segment_size;
        thread_data[i].end_index = (i + 1) * segment_size - 1 + (i == num_threads - 1 ? remainder : 0);
        pthread_create(&threads[i], NULL, threadSort, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    int *temp = (int *)malloc(size * sizeof(int));
 
    mergeSort(array, temp, 0, size - 1);

    free(temp);
}

double getElapsedTime(struct timeval start, struct timeval end) {
    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed_time += (end.tv_usec - start.tv_usec) / 1000.0;
    return elapsed_time;
}

int main() {
    srand(time(NULL));
    int size, num_threads;
    struct timeval start_time, end_time;

    printf("Enter the size of the array: ");
    scanf("%d", &size);
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    if (num_threads > MAX_THREADS) {
        printf("[!] Maximum number of threads: 8 !");
        num_threads = MAX_THREADS;  
    }

    int array[size];
    for (int i = 0; i < size; i++)
        array[i] = rand() % 100;

    printf("Not sorted array: ");
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");

    gettimeofday(&start_time, NULL);
    parallel_sort(array, size, num_threads);
    gettimeofday(&end_time, NULL);

    printf("Sorted array: ");
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");

    double elapsed_time = getElapsedTime(start_time, end_time);

    printf("\nTime taken: %f ms\n", elapsed_time);

    return 0;
}