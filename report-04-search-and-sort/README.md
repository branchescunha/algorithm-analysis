# Relatório 04 — Busca Sequencial, MergeSort e Busca Binária

Implementação e análise de duas estratégias para inserir registros em CSV sem duplicar a chave escolhida.

## Base de dados

Foi utilizado `cnes_estabelecimentos.csv`, proveniente do Portal de Dados Abertos, com dados do Cadastro Nacional de Estabelecimentos de Saúde (CNES).

A chave escolhida é `CO_UNIDADE` porque, na base analisada:

- possui **636.647 valores únicos em 636.647 registros**;
- **não está ordenada** no arquivo original;
- contém valores alfanuméricos, por isso é tratada como texto e comparada com `strcmp`.

O arquivo original possui cerca de 220 MiB e não é versionado no Git. Os arquivos efetivamente usados nos experimentos ficam em `data/`.

## Soluções

### Solução 1 — Busca sequencial

- `seq_iterativa.c`: busca sequencial iterativa;
- `seq_recursiva.c`: busca sequencial recursiva.

Complexidades da busca:

| Implementação | Tempo | Espaço auxiliar |
| --- | --- | --- |
| Iterativa | `Θ(N)` | `Θ(1)` |
| Recursiva | `Θ(N)` | `Θ(N)` |

Na versão recursiva:

```text
T(N) = T(N - 1) + c = Θ(N)
S(N) = S(N - 1) + f = Θ(N)
```

### Solução 2 — MergeSort + busca binária

As chaves são carregadas para um vetor em RAM. O vetor é ordenado, mas o CSV físico permanece desordenado; registros válidos são sempre anexados ao final do arquivo.

| Algoritmo | Tempo | Espaço auxiliar |
| --- | --- | --- |
| MergeSort iterativo | `Θ(N log N)` | `Θ(N)` |
| MergeSort recursivo | `Θ(N log N)` | `Θ(N)` |
| Busca binária iterativa | `Θ(log N)` | `Θ(1)` |
| Busca binária recursiva | `Θ(log N)` | `Θ(log N)` |

Recorrências principais:

```text
MergeSort:      T(N) = 2T(N/2) + Θ(N) = Θ(N log N)
Busca binária:  T(N) = T(N/2) + c = Θ(log N)
```

Após cada inserção válida, o índice em RAM é mantido ordenado por deslocamento no vetor, operação `Θ(N)` no pior caso.

## Metodologia experimental

### Workload principal

A base de destino começa sempre com `N = 1000` registros. Para cada `K`, metade das entradas já existe e metade é nova:

| K | Existentes | Novos |
| ---: | ---: | ---: |
| 10 | 5 | 5 |
| 20 | 10 | 10 |
| 40 | 20 | 20 |
| 80 | 40 | 40 |

Antes de cada execução, o destino é restaurado a partir de `data/destino_backup.csv`. Cada cenário de tempo é repetido 20 vezes.

A contagem de comparações é determinística. Para a busca sequencial, neste workload:

```text
Cseq(K,N) = (K/2)N + K²/4
```

Com `N = 1000`, os valores teóricos coincidem exatamente com os observados.

### Experimento variando N

Para tornar visível a diferença `Θ(N)` × `Θ(log N)`, `benchmark_crescimento.py` usa uma chave ausente e bases com:

```text
N = 125, 250, 500, 1000
```

Resultados de comparações:

| N | Sequencial | Binária |
| ---: | ---: | ---: |
| 125 | 125 | 7 |
| 250 | 250 | 8 |
| 500 | 500 | 9 |
| 1000 | 1000 | 10 |

Isso evidencia diretamente o crescimento linear e logarítmico.

## Resultados do workload principal

| K | Seq. iterativa | Seq. recursiva | Bin. iterativa | Bin. recursiva |
| ---: | ---: | ---: | ---: | ---: |
| 10 | 5.025 | 5.025 | 99 | 99 |
| 20 | 10.100 | 10.100 | 191 | 191 |
| 40 | 20.400 | 20.400 | 377 | 377 |
| 80 | 41.600 | 41.600 | 743 | 743 |

<p align="center">
  <img src="./charts/teoria_vs_experimento_k.png" width="75%">
</p>

<p align="center">
  <img src="./charts/crescimento_n_comparacoes.png" width="75%">
</p>

## Como executar

### Dependências

- GCC
- Python 3
- Pandas
- NumPy
- Matplotlib

```powershell
python.exe -m pip install pandas numpy matplotlib
```

### Preparar a base e os testes

Coloque `cnes_estabelecimentos.csv` na raiz desta pasta e execute:

```powershell
python.exe .\scripts\reduzir.py
python.exe .\scripts\gerar.py
```

### Compilar

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -O0 .\src\seq_iterativa.c -o seq_iterativa
gcc -std=c11 -Wall -Wextra -Wpedantic -O0 .\src\seq_recursiva.c -o seq_recursiva
gcc -std=c11 -Wall -Wextra -Wpedantic -O0 .\src\bin_iterativa.c -o bin_iterativa
gcc -std=c11 -Wall -Wextra -Wpedantic -O0 .\src\bin_recursiva.c -o bin_recursiva
```

### Executar os benchmarks

```powershell
python.exe .\scripts\benchmark.py
python.exe .\scripts\benchmark_crescimento.py
python.exe .\scripts\plot.py
```

São gerados:

```text
results/estatisticas.csv
results/comparacao_teorica.csv
results/crescimento_n.csv
charts/*.png
```

### Execução manual

```powershell
Copy-Item .\data\destino_backup.csv .\results\destino.csv -Force
.\seq_iterativa.exe .\data\teste40.csv .\results\destino.csv
```

## Estrutura

```text
report-04-search-and-sort/
├── charts/
│   ├── comparacoes_buscas.png
│   ├── crescimento_n_comparacoes.png
│   ├── mergesort_crescimento_n.png
│   ├── teoria_vs_experimento_k.png
│   └── tempos_algoritmos.png
├── data/
│   ├── destino_backup.csv
│   └── teste*.csv
├── report/
│   ├── Relatorio-04-Busca-Ordenacao.docx
│   └── Relatorio-04-Busca-Ordenacao.pdf
├── results/
│   ├── comparacao_teorica.csv
│   ├── crescimento_n.csv
│   └── estatisticas.csv
├── scripts/
│   ├── benchmark.py
│   ├── benchmark_crescimento.py
│   ├── gerar.py
│   ├── plot.py
│   └── reduzir.py
├── src/
│   ├── common.h
│   ├── seq_iterativa.c
│   ├── seq_recursiva.c
│   ├── bin_iterativa.c
│   └── bin_recursiva.c
├── .gitignore
└── README.md
```
