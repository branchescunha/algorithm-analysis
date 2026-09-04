#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define TESTS 32

uintptr_t stack_start;
size_t stack_max;
int track_stack = 0;
volatile long long sink;

void stack_begin()
{
    char marker;
    stack_start = (uintptr_t)&marker;
    stack_max = 0;
}

void stack_update()
{
    char marker;
    uintptr_t current = (uintptr_t)&marker;

    size_t distance = stack_start > current
                          ? stack_start - current
                          : current - stack_start;

    if (distance > stack_max)
    {
        stack_max = distance;
    }
}

long long count_zeros_iterative(const int *matrix, int n)
{
    if (track_stack)
    {
        stack_update();
    }

    long long count = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[(size_t)i * n + j] == 0)
            {
                count++;
            }
        }
    }

    return count;
}

void generate_matrix(int *matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[(size_t)i * n + j] =
                ((i + j) % 5 == 0) ? 0 : ((i + j) % 9) + 1;
        }
    }
}

double mean_time(const int *matrix, int n)
{
    clock_t begin = clock();

    for (int i = 0; i < TESTS; i++)
    {
        sink = count_zeros_iterative(matrix, n);
    }

    clock_t end = clock();

    double total =
        ((double)(end - begin) * 1000.0) / CLOCKS_PER_SEC;

    return total / TESTS;
}

size_t measure_stack(const int *matrix, int n)
{
    stack_begin();
    track_stack = 1;

    sink = count_zeros_iterative(matrix, n);

    track_stack = 0;

    return stack_max;
}

int write_csv(const char *file, int n, double time, size_t stack)
{
    FILE *fp = fopen(file, "a+");

    if (!fp)
    {
        perror("Erro ao abrir arquivo");
        return -1;
    }

    fseek(fp, 0, SEEK_END);

    if (ftell(fp) == 0)
    {
        fprintf(fp, "n,time,stack\n");
    }

    fprintf(fp, "%d,%.6f,%zu\n", n, time, stack);

    fclose(fp);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <N> <arquivo_csv>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n <= 0)
    {
        printf("N deve ser maior que zero.\n");
        return 1;
    }

    int *matrix = malloc((size_t)n * n * sizeof(int));

    if (!matrix)
    {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    generate_matrix(matrix, n);

    double time = mean_time(matrix, n);
    size_t stack = measure_stack(matrix, n);

    printf("N             : %d\n", n);
    printf("Tempo medio   : %.6f ms\n", time);
    printf("Pilha maxima  : %zu bytes\n", stack);

    int result = write_csv(argv[2], n, time, stack);

    free(matrix);

    return result == 0 ? 0 : 1;
}