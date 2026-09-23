#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

static void merge(Chave *vetor, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    Chave *esq = malloc((size_t)n1 * sizeof(Chave));
    Chave *dir = malloc((size_t)n2 * sizeof(Chave));
    if (esq == NULL || dir == NULL) {
        free(esq);
        free(dir);
        fprintf(stderr, "ERRO: memoria insuficiente.\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++) esq[i] = vetor[inicio + i];
    for (int j = 0; j < n2; j++) dir[j] = vetor[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        vetor[k++] = strcmp(esq[i].valor, dir[j].valor) <= 0 ? esq[i++] : dir[j++];
    }
    while (i < n1) vetor[k++] = esq[i++];
    while (j < n2) vetor[k++] = dir[j++];

    free(esq);
    free(dir);
}

static void mergesort_recursivo(Chave *vetor, int inicio, int fim) {
    if (inicio >= fim) return;
    int meio = inicio + (fim - inicio) / 2;
    mergesort_recursivo(vetor, inicio, meio);
    mergesort_recursivo(vetor, meio + 1, fim);
    merge(vetor, inicio, meio, fim);
}

static int busca_bin_recursiva(
    const Chave *vetor,
    int inicio,
    int fim,
    const char *alvo,
    long long *comparacoes,
    int *posicao
) {
    if (inicio > fim) {
        *posicao = inicio;
        return 0;
    }

    int meio = inicio + (fim - inicio) / 2;
    (*comparacoes)++;
    int cmp = strcmp(vetor[meio].valor, alvo);
    if (cmp == 0) {
        *posicao = meio;
        return 1;
    }
    if (cmp > 0) {
        return busca_bin_recursiva(vetor, inicio, meio - 1, alvo, comparacoes, posicao);
    }
    return busca_bin_recursiva(vetor, meio + 1, fim, alvo, comparacoes, posicao);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <novos.csv> <destino.csv>\n", argv[0]);
        return 1;
    }

    VetorChaves base = {0};
    if (!carregar_chaves(argv[2], &base)) {
        fprintf(stderr, "ERRO: nao foi possivel carregar o destino.\n");
        return 1;
    }

    clock_t inicio_sort = clock();
    mergesort_recursivo(base.dados, 0, base.tamanho - 1);
    clock_t fim_sort = clock();
    double tempo_sort = (double)(fim_sort - inicio_sort) / CLOCKS_PER_SEC;

    FILE *novos = fopen(argv[1], "r");
    FILE *destino = fopen(argv[2], "a");
    if (novos == NULL || destino == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel abrir os arquivos.\n");
        if (novos) fclose(novos);
        if (destino) fclose(destino);
        free(base.dados);
        return 1;
    }

    char linha[MAX_LINHA];
    fgets(linha, sizeof(linha), novos);

    long long comparacoes = 0;
    int inseridos = 0;
    double tempo_busca = 0.0;
    double tempo_manutencao = 0.0;

    while (fgets(linha, sizeof(linha), novos) != NULL) {
        char chave[MAX_CHAVE];
        if (!extrair_chave(linha, chave)) continue;

        int posicao = 0;
        clock_t inicio = clock();
        int existe = busca_bin_recursiva(
            base.dados, 0, base.tamanho - 1, chave, &comparacoes, &posicao
        );
        clock_t fim = clock();
        tempo_busca += (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (!existe) {
            fputs(linha, destino);
            inicio = clock();
            inserir_ordenado(&base, posicao, chave);
            fim = clock();
            tempo_manutencao += (double)(fim - inicio) / CLOCKS_PER_SEC;
            inseridos++;
        }
    }

    double total = tempo_sort + tempo_busca + tempo_manutencao;
    printf("ALGORITMO=BIN_RECURSIVA\n");
    printf("INSERIDOS=%d\n", inseridos);
    printf("COMPARACOES=%lld\n", comparacoes);
    printf("TEMPO_ORDENACAO=%.9f\n", tempo_sort);
    printf("TEMPO_BUSCA=%.9f\n", tempo_busca);
    printf("TEMPO_MANUTENCAO=%.9f\n", tempo_manutencao);
    printf("TEMPO_TOTAL_ALGORITMOS=%.9f\n", total);

    fclose(novos);
    fclose(destino);
    free(base.dados);
    return 0;
}
