
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define REPETICOES 20

volatile long long sink_p2 = 0;

long long resolver_problema2_caso2(const int *vetor, int n, long long *operacoes) {
    if (n <= 1) {
        (*operacoes)++;
        return (n == 1) ? vetor[0] : 0;
    }

    int terco = n / 3;
    long long p1 = resolver_problema2_caso2(vetor, terco, operacoes);
    long long p2 = resolver_problema2_caso2(vetor + terco, terco, operacoes);
    long long p3 = resolver_problema2_caso2(vetor + 2 * terco, n - 2 * terco, operacoes);

    long long combinacao = 0;
    for (int i = 0; i < n; i++) {
        (*operacoes)++;
        combinacao += vetor[i];
    }

    return p1 + p2 + p3 + combinacao;
}

int executar_e_registrar_caso2(int n, const char *arquivo_csv) {
    int *vetor = (int *)malloc((size_t)n * sizeof(int));
    if (!vetor) {
        fprintf(stderr, "Erro ao alocar memoria para N = %d\n", n);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        vetor[i] = (i % 7) + 1;
    }

    long long operacoes = 0;
    clock_t inicio = clock();

    for (int r = 0; r < REPETICOES; r++) {
        operacoes = 0;
        sink_p2 ^= resolver_problema2_caso2(vetor, n, &operacoes);
    }

    clock_t fim = clock();
    double tempo_medio_ms = (((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC) / REPETICOES;

    double log3_n = log((double)n) / log(3.0);
    double ref_assintotica = (double)n * log3_n;
    long long formula_fechada_inducao = (long long)llround((double)n * log3_n + (double)n);

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

    fprintf(fp, "Problema_2,Caso_2,3,3,n,Theta(n log n),%d,%lld,%lld,%.2f,%.6f\n",
            n, operacoes, formula_fechada_inducao, ref_assintotica, tempo_medio_ms);

    fclose(fp);
    free(vetor);

    printf("[Problema 2 - Caso 2] N = %5d | Ops = %10lld | Exata Inducao = %10lld | Tempo = %.6f ms\n",
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
        return executar_e_registrar_caso2(n, arquivo_csv) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    int tamanhos_padrao[] = {27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323};
    int qtd = sizeof(tamanhos_padrao) / sizeof(tamanhos_padrao[0]);

    printf("=== Executando Bateria Completa: Problema 2 (Caso 2: a=3, b=3, f(n)=n) ===\n");
    for (int i = 0; i < qtd; i++) {
        if (!executar_e_registrar_caso2(tamanhos_padrao[i], arquivo_csv)) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}