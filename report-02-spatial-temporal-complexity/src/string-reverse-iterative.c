#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define TESTS 32

uintptr_t stack_start;
size_t stack_max;
int track_stack = 0;
volatile int sink;

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

int reverse_iterative(const char *input, char *output, int n)
{
    if (track_stack)
    {
        stack_update();
    }

    for (int i = 0; i < n; i++)
    {
        output[i] = input[n - 1 - i];
    }

    output[n] = '\0';

    return n;
}

void generate_string(char *text, int n)
{
    for (int i = 0; i < n; i++)
    {
        text[i] = 'a' + (i % 26);
    }

    text[n] = '\0';
}

double mean_time(const char *input, char *output, int n)
{
    clock_t begin = clock();

    for (int i = 0; i < TESTS; i++)
    {
        sink = reverse_iterative(input, output, n);
    }

    clock_t end = clock();

    double total =
        ((double)(end - begin) * 1000.0) / CLOCKS_PER_SEC;

    return total / TESTS;
}

size_t measure_stack(const char *input, char *output, int n)
{
    stack_begin();
    track_stack = 1;

    sink = reverse_iterative(input, output, n);

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

    char *input = malloc((size_t)n + 1);
    char *output = malloc((size_t)n + 1);

    if (!input || !output)
    {
        printf("Erro ao alocar memoria.\n");
        free(input);
        free(output);
        return 1;
    }

    generate_string(input, n);

    double time = mean_time(input, output, n);
    size_t stack = measure_stack(input, output, n);

    printf("N             : %d\n", n);
    printf("Tempo medio   : %.6f ms\n", time);
    printf("Pilha maxima  : %zu bytes\n", stack);

    int result = write_csv(argv[2], n, time, stack);

    free(input);
    free(output);

    return result == 0 ? 0 : 1;
}