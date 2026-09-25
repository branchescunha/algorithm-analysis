from pathlib import Path
import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

BASE_DIR = Path(__file__).resolve().parents[1]
RESULTS = BASE_DIR / 'results' / 'estatisticas.csv'
CRESCIMENTO = BASE_DIR / 'results' / 'crescimento_n.csv'
CHARTS = BASE_DIR / 'charts'
CHARTS.mkdir(exist_ok=True)


def main():
    df = pd.read_csv(RESULTS)

    # Assumptions used in the K-sized insertion experiments.
    n0 = 10000
    comparacao = pd.DataFrame({
        'k': df['k'],
        'n_inicial': n0,
        'existentes': df['k'] // 2,
        'novos': df['k'] // 2,
        'seq_teorico': (df['k'] // 2) * n0 + (df['k'] ** 2) // 4,
        'seq_observado': df['seq_iterativa_comparacoes'],
        'bin_observado': df['bin_iterativa_comparacoes'],
    })

    # Conservative upper bound for the binary searches, taking the actual
    # order of duplicate/new records in each generated test file into account.
    import csv
    with (BASE_DIR / 'data' / 'destino_backup.csv').open('r', encoding='utf-8', newline='') as f:
        rd = csv.reader(f, delimiter=';')
        header = next(rd)
        idx = header.index('CO_UNIDADE')
        chaves_iniciais = {row[idx] for row in rd if len(row) > idx}

    limites = []
    for k in comparacao['k']:
        n = n0
        total = 0
        chaves = set(chaves_iniciais)
        with (BASE_DIR / 'data' / f'teste{k}.csv').open('r', encoding='utf-8', newline='') as f:
            rd = csv.reader(f, delimiter=';')
            header_teste = next(rd)
            idx_teste = header_teste.index('CO_UNIDADE')
            for row in rd:
                chave = row[idx_teste]
                total += math.floor(math.log2(n)) + 1
                if chave not in chaves:
                    chaves.add(chave)
                    n += 1
        limites.append(total)
    comparacao['bin_limite_superior'] = limites
    comparacao['seq_diferenca'] = comparacao['seq_observado'] - comparacao['seq_teorico']
    comparacao.to_csv(BASE_DIR / 'results' / 'comparacao_teorica.csv', index=False)

    plt.figure(figsize=(9, 5.5))
    plt.plot(df['k'], df['seq_iterativa_comparacoes'], marker='o', label='Sequencial iterativa')
    plt.plot(df['k'], df['seq_recursiva_comparacoes'], marker='x', linestyle='--', label='Sequencial recursiva')
    plt.plot(df['k'], df['bin_iterativa_comparacoes'], marker='o', label='Binária iterativa')
    plt.plot(df['k'], df['bin_recursiva_comparacoes'], marker='x', linestyle='--', label='Binária recursiva')
    plt.yscale('log')
    plt.xlabel('Quantidade de registros de entrada (K)')
    plt.ylabel('Comparações de CO_UNIDADE')
    plt.title('Comparações observadas: busca sequencial vs. busca binária')
    plt.grid(True, which='both', linestyle='--', alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(CHARTS / 'comparacoes_buscas.png', dpi=200)
    plt.close()

    plt.figure(figsize=(9, 5.5))
    plt.plot(comparacao['k'], comparacao['seq_teorico'], marker='o', label='Sequencial - teoria exata')
    plt.plot(comparacao['k'], comparacao['seq_observado'], marker='x', linestyle='--', label='Sequencial - observado')
    plt.plot(comparacao['k'], comparacao['bin_limite_superior'], marker='o', label='Binária - limite teórico')
    plt.plot(comparacao['k'], comparacao['bin_observado'], marker='x', linestyle='--', label='Binária - observado')
    plt.yscale('log')
    plt.xlabel('Quantidade de registros de entrada (K)')
    plt.ylabel('Comparações de CO_UNIDADE')
    plt.title('Teoria × experimento com N inicial = 10000 e 50% de duplicatas')
    plt.grid(True, which='both', linestyle='--', alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(CHARTS / 'teoria_vs_experimento_k.png', dpi=200)
    plt.close()

    plt.figure(figsize=(9, 5.5))
    plt.plot(df['k'], df['seq_iterativa_tempo_total_algoritmos'], marker='o', label='Sequencial iterativa')
    plt.plot(df['k'], df['seq_recursiva_tempo_total_algoritmos'], marker='x', linestyle='--', label='Sequencial recursiva')
    plt.plot(df['k'], df['bin_iterativa_tempo_total_algoritmos'], marker='o', label='Binária + índice iterativos')
    plt.plot(df['k'], df['bin_recursiva_tempo_total_algoritmos'], marker='x', linestyle='--', label='Binária + Mergesort recursivos')
    positivos = df[[
        'seq_iterativa_tempo_total_algoritmos',
        'seq_recursiva_tempo_total_algoritmos',
        'bin_iterativa_tempo_total_algoritmos',
        'bin_recursiva_tempo_total_algoritmos',
    ]].to_numpy()
    if np.all(positivos > 0):
        plt.yscale('log')
    plt.xlabel('Quantidade de registros de entrada (K)')
    plt.ylabel('Tempo médio dos algoritmos (s)')
    plt.title('Tempo experimental médio (20 execuções por cenário)')
    plt.grid(True, which='both', linestyle='--', alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(CHARTS / 'tempos_algoritmos.png', dpi=200)
    plt.close()

    if CRESCIMENTO.exists():
        crescimento = pd.read_csv(CRESCIMENTO)

        plt.figure(figsize=(9, 5.5))
        plt.plot(crescimento['n'], crescimento['seq_teorico'], marker='o', label='Sequencial - teoria Θ(N)')
        plt.plot(crescimento['n'], crescimento['seq_iterativa_comparacoes'], marker='x', linestyle='--', label='Sequencial - observado')
        plt.plot(crescimento['n'], crescimento['bin_limite_teorico'], marker='o', label='Binária - teoria Θ(log N)')
        plt.plot(crescimento['n'], crescimento['bin_iterativa_comparacoes'], marker='x', linestyle='--', label='Binária - observado')
        plt.yscale('log')
        plt.xlabel('Tamanho da base (N)')
        plt.ylabel('Comparações para uma chave ausente')
        plt.title('Crescimento por N: teoria × experimento')
        plt.grid(True, which='both', linestyle='--', alpha=0.5)
        plt.legend()
        plt.tight_layout()
        plt.savefig(CHARTS / 'crescimento_n_comparacoes.png', dpi=200)
        plt.close()

        n = crescimento['n'].to_numpy(dtype=float)
        referencia = n * np.log2(n)
        tempo_iter = crescimento['bin_iterativa_tempo_ordenacao'].to_numpy(dtype=float)
        escala = tempo_iter[-1] / referencia[-1] if referencia[-1] > 0 else 1.0

        plt.figure(figsize=(9, 5.5))
        plt.plot(crescimento['n'], crescimento['bin_iterativa_tempo_ordenacao'], marker='o', label='MergeSort iterativo - observado')
        plt.plot(crescimento['n'], crescimento['bin_recursiva_tempo_ordenacao'], marker='x', linestyle='--', label='MergeSort recursivo - observado')
        plt.plot(crescimento['n'], referencia * escala, linestyle=':', label='Referência N log₂N (escalada)')
        plt.xlabel('Tamanho da base (N)')
        plt.ylabel('Tempo médio da ordenação (s)')
        plt.title('MergeSort: crescimento experimental com N')
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.legend()
        plt.tight_layout()
        plt.savefig(CHARTS / 'mergesort_crescimento_n.png', dpi=200)
        plt.close()

    print(comparacao.to_string(index=False))
    if CRESCIMENTO.exists():
        print('\nCrescimento por N:')
        print(pd.read_csv(CRESCIMENTO).to_string(index=False))
    print(f'Graficos gerados em: {CHARTS}')


if __name__ == '__main__':
    main()
