#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define REPETICOES 20

volatile long long sink_p3 = 0;

long long resolver_problema3_caso3(const int *vetor, int n, long long *operacoes) {
    if (n <= 1) {
        (*operacoes)++;
        return vetor[0];
    }

    long long custo_pares = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            (*operacoes)++;
            custo_pares += (vetor[i] ^ vetor[j]);
        }
    }

    int metade = n / 2;
    long long esq = resolver_problema3_caso3(vetor, metade, operacoes);
    long long dir = resolver_problema3_caso3(vetor + metade, metade, operacoes);

    return custo_pares + esq + dir;
}

int executar_e_registrar_caso3(int n, const char *arquivo_csv) {
    int *vetor = (int *)malloc((size_t)n * sizeof(int));
    if (!vetor) {
        fprintf(stderr, "Erro ao alocar memoria para N = %d\n", n);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        vetor[i] = (i % 5) + 1;
    }

    long long operacoes = 0;
    clock_t inicio = clock();

    for (int r = 0; r < REPETICOES; r++) {
        operacoes = 0;
        sink_p3 ^= resolver_problema3_caso3(vetor, n, &operacoes);
    }

    clock_t fim = clock();
    double tempo_medio_ms = (((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC) / REPETICOES;

    double ref_assintotica = (double)n * (double)n;
    long long formula_fechada_inducao = 2LL * n * n - n;

    FILE *fp = fopen(arquivo_csv, "a+");
    if (!fp) {
        perror("Erro ao abrir arquivo CSV");
        free(vetor);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "problema,caso_teorema,a,b,f_n,complexidade_teorica,n,operacoes_observadas,formula_exata_inducao,referencia_assintotica,tempo_medio_ms\n");
    }

    fprintf(fp, "Problema_3,Caso_3,2,2,n^2,Theta(n^2),%d,%lld,%lld,%.2f,%.6f\n",
            n, operacoes, formula_fechada_inducao, ref_assintotica, tempo_medio_ms);

    fclose(fp);
    free(vetor);

    printf("[Problema 3 - Caso 3] N = %5d | Ops = %10lld | Exata Inducao = %10lld | Tempo = %.6f ms\n",
           n, operacoes, formula_fechada_inducao, tempo_medio_ms);

    return 1;
}

int main(int argc, char *argv[]) {
    const char *arquivo_csv = "resultados_teorema_mestre.csv";

    if (argc >= 2) {
        int n = atoi(argv[1]);
        if (argc >= 3) {
            arquivo_csv = argv[2];
        }
        if (n <= 0) {
            fprintf(stderr, "Erro: N deve ser maior que zero.\n");
            return EXIT_FAILURE;
        }
        return executar_e_registrar_caso3(n, arquivo_csv) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    int tamanhos_padrao[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    int qtd = sizeof(tamanhos_padrao) / sizeof(tamanhos_padrao[0]);

    printf("=== Executando Bateria Completa: Problema 3 (Caso 3: a=2, b=2, f(n)=n^2) ===\n");
    for (int i = 0; i < qtd; i++) {
        if (!executar_e_registrar_caso3(tamanhos_padrao[i], arquivo_csv)) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}