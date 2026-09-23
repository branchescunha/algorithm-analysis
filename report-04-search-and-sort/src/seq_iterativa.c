#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

static int busca_seq_iterativa(
    const Chave *vetor,
    int n,
    const char *alvo,
    long long *comparacoes
) {
    for (int i = 0; i < n; i++) {
        (*comparacoes)++;
        if (strcmp(vetor[i].valor, alvo) == 0) return 1;
    }
    return 0;
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

    while (fgets(linha, sizeof(linha), novos) != NULL) {
        char chave[MAX_CHAVE];
        if (!extrair_chave(linha, chave)) continue;

        clock_t inicio = clock();
        int existe = busca_seq_iterativa(base.dados, base.tamanho, chave, &comparacoes);
        clock_t fim = clock();
        tempo_busca += (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (!existe) {
            fputs(linha, destino);
            if (!adicionar_chave(&base, chave)) {
                fprintf(stderr, "ERRO: memoria insuficiente.\n");
                fclose(novos);
                fclose(destino);
                free(base.dados);
                return 1;
            }
            inseridos++;
        }
    }

    printf("ALGORITMO=SEQ_ITERATIVA\n");
    printf("INSERIDOS=%d\n", inseridos);
    printf("COMPARACOES=%lld\n", comparacoes);
    printf("TEMPO_BUSCA=%.9f\n", tempo_busca);
    printf("TEMPO_TOTAL_ALGORITMOS=%.9f\n", tempo_busca);

    fclose(novos);
    fclose(destino);
    free(base.dados);
    return 0;
}
