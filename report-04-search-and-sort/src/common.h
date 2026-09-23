#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 2048
#define MAX_CHAVE 64

typedef struct {
    char valor[MAX_CHAVE];
} Chave;

typedef struct {
    Chave *dados;
    int tamanho;
    int capacidade;
} VetorChaves;

static int extrair_chave(const char *linha, char *chave) {
    const char *inicio = strchr(linha, ';');
    if (inicio == NULL) return 0;
    inicio++;

    const char *fim = strchr(inicio, ';');
    if (fim == NULL) fim = inicio + strlen(inicio);

    size_t tamanho = (size_t)(fim - inicio);
    while (tamanho > 0 && (inicio[tamanho - 1] == '\r' || inicio[tamanho - 1] == '\n')) {
        tamanho--;
    }
    if (tamanho == 0 || tamanho >= MAX_CHAVE) return 0;

    memcpy(chave, inicio, tamanho);
    chave[tamanho] = '\0';
    return 1;
}

static int garantir_capacidade(VetorChaves *vetor, int necessario) {
    if (necessario <= vetor->capacidade) return 1;

    int nova = vetor->capacidade == 0 ? 1024 : vetor->capacidade * 2;
    while (nova < necessario) nova *= 2;

    Chave *tmp = realloc(vetor->dados, (size_t)nova * sizeof(Chave));
    if (tmp == NULL) return 0;

    vetor->dados = tmp;
    vetor->capacidade = nova;
    return 1;
}

static int adicionar_chave(VetorChaves *vetor, const char *chave) {
    if (!garantir_capacidade(vetor, vetor->tamanho + 1)) return 0;
    strncpy(vetor->dados[vetor->tamanho].valor, chave, MAX_CHAVE - 1);
    vetor->dados[vetor->tamanho].valor[MAX_CHAVE - 1] = '\0';
    vetor->tamanho++;
    return 1;
}

static int carregar_chaves(const char *arquivo, VetorChaves *vetor) {
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) return 0;

    char linha[MAX_LINHA];
    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) != NULL) {
        char chave[MAX_CHAVE];
        if (extrair_chave(linha, chave)) {
            if (!adicionar_chave(vetor, chave)) {
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);
    return 1;
}

static inline void inserir_ordenado(VetorChaves *vetor, int posicao, const char *chave) {
    if (!garantir_capacidade(vetor, vetor->tamanho + 1)) {
        fprintf(stderr, "ERRO: memoria insuficiente.\n");
        exit(1);
    }

    memmove(
        &vetor->dados[posicao + 1],
        &vetor->dados[posicao],
        (size_t)(vetor->tamanho - posicao) * sizeof(Chave)
    );

    strncpy(vetor->dados[posicao].valor, chave, MAX_CHAVE - 1);
    vetor->dados[posicao].valor[MAX_CHAVE - 1] = '\0';
    vetor->tamanho++;
}

#endif
