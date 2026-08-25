#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *stack_start = NULL;
long long stack_depth = 0;
volatile unsigned long long sink = 0;

void iniciar_marcador_da_pilha(void *start)
{
    stack_start = start;
    stack_depth = 0;
}

void atualizar_marcador_da_pilha(void)
{
    char a;
    long long depth = llabs((long long)((char *)&a - (char *)stack_start));

    if (depth > stack_depth)
    {
        stack_depth = depth;
    }
}

unsigned long long fatorial_iterativo(long long n)
{
    unsigned long long resultado = 1;

    atualizar_marcador_da_pilha();

    for (long long i = 2; i <= n; i++)
    {
        resultado *= (unsigned long long)i;
    }

    return resultado;
}

double get_mean_time(long long n, int tests)
{
    clock_t begin = clock();

    for (int i = 0; i < tests; i++)
    {
        sink ^= fatorial_iterativo(n);
    }

    clock_t end = clock();

    double total_ms =
        ((double)(end - begin) * 1000.0) / CLOCKS_PER_SEC;

    return total_ms / tests;
}

int write_into_csv(
    const char *csvFile,
    long long n,
    double mean_time)
{
    FILE *fp = fopen(csvFile, "a+");

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

    printf("N              : %lld\n", n);
    printf("Tempo medio    : %.6f ms\n", mean_time);
    printf("Pilha maxima   : %lld bytes\n", stack_depth);

    fprintf(
        fp,
        "%lld,%.6f,%lld\n",
        n,
        mean_time,
        stack_depth);

    fclose(fp);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(
            stderr,
            "Uso: %s <N> <quantidade_testes> <arquivo_csv>\n",
            argv[0]);

        return EXIT_FAILURE;
    }

    long long n = atoll(argv[1]);
    int tests = atoi(argv[2]);

    if (n < 0)
    {
        fprintf(
            stderr,
            "N deve ser maior ou igual a zero.\n");

        return EXIT_FAILURE;
    }

    if (tests <= 0)
    {
        fprintf(
            stderr,
            "A quantidade de testes deve ser maior que zero.\n");

        return EXIT_FAILURE;
    }

    char stack_base;

    iniciar_marcador_da_pilha(&stack_base);

    double mean_time = get_mean_time(n, tests);

    int rc = write_into_csv(
        argv[3],
        n,
        mean_time);

    return rc == 0
               ? EXIT_SUCCESS
               : EXIT_FAILURE;
}