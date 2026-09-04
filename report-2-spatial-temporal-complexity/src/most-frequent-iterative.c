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

int most_frequent_iterative(const char *text, int n)
{
    if (track_stack)
    {
        stack_update();
    }

    int frequency[26] = {0};

    for (int i = 0; i < n; i++)
    {
        frequency[text[i] - 'a']++;
    }

    int max = 0;

    for (int i = 0; i < 26; i++)
    {
        if (frequency[i] > max)
        {
            max = frequency[i];
        }
    }

    return max;
}

void generate_string(char *text, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (i % 3 == 0)
        {
            text[i] = 'a';
        }
        else
        {
            text[i] = 'b' + (i % 25);
        }
    }

    text[n] = '\0';
}

double mean_time(const char *text, int n)
{
    clock_t begin = clock();

    for (int i = 0; i < TESTS; i++)
    {
        sink = most_frequent_iterative(text, n);
    }

    clock_t end = clock();

    double total =
        ((double)(end - begin) * 1000.0) / CLOCKS_PER_SEC;

    return total / TESTS;
}

size_t measure_stack(const char *text, int n)
{
    stack_begin();
    track_stack = 1;

    sink = most_frequent_iterative(text, n);

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

    char *text = malloc((size_t)n + 1);

    if (!text)
    {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    generate_string(text, n);

    double time = mean_time(text, n);
    size_t stack = measure_stack(text, n);

    printf("N             : %d\n", n);
    printf("Tempo medio   : %.6f ms\n", time);
    printf("Pilha maxima  : %zu bytes\n", stack);

    int result = write_csv(argv[2], n, time, stack);

    free(text);

    return result == 0 ? 0 : 1;
}