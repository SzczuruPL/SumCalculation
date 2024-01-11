#define _CRT_SECURE_NO_DEPRECATE //usuń
#define HAVE_STRUCT_TIMESPEC //usuń
#include <pthread.h> //usuń
#include <stdio.h>
#include <stdlib.h>

double* arr = NULL;
int arr_size = 0;
pthread_mutex_t arr_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int number;
} ThreadData;

void* calculateSum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int result = 0;

    for (int i = 1; i <= data->number; ++i) {
        result += i;
    }

    //printf("Input: %d - Result: %d\n", data->number, result);

    pthread_mutex_lock(&arr_mutex);

    arr_size++;
    arr = realloc(arr, arr_size * sizeof(double));

    arr[arr_size - 1] = (double)result;

    pthread_mutex_unlock(&arr_mutex);
    return NULL;
}

int main() {
    FILE* file;
    ThreadData* threadData;

    pthread_t thread;

    file = fopen("numbers.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Błąd otwarcia pliku.\n");
        return 1;
    }

    //while (fscanf(file, "%d", &threadData[0].number) == 1) {
    while (1) {
        threadData = (ThreadData*)malloc(sizeof(ThreadData));

        if (fscanf(file, "%d", &threadData->number) != 1) {
            free(threadData);
            break;
        }

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        pthread_create(&thread, &attr, calculateSum, (void*)threadData);

        pthread_attr_destroy(&attr);
    }


    fclose(file);

    printf("Wczytane liczby:\n");
    for (int i = 0; i < arr_size; ++i) {
        printf("%d: %lf\n", i+1, arr[i]);
    }
    printf("\n");


    free(arr);
    pthread_mutex_destroy(&arr_mutex);

    return 0;
}