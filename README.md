# Algorithm Analysis

Repositório dedicado aos estudos, implementações e relatórios desenvolvidos na disciplina de **Análise de Algoritmos** do curso de Ciência da Computação.

O projeto reúne implementações em C, análises de complexidade temporal e espacial, demonstrações matemáticas e validações experimentais.

## Relatórios

| Relatório | Tema | Status |
| --- | --- | --- |
| 01 | Análise Empírica — Fatorial e Fibonacci | Concluído |
| 02 | Complexidade Espacial e Temporal | Concluído |
| 03 | Cadastro de Bolsistas | Concluído |
| 04 | Busca Sequencial, MergeSort e Busca Binária | Concluído |
| 05 | A definir | Pendente |
| 06 | A definir | Pendente |
| 07 | A definir | Pendente |

---

## Relatório 01 — Análise Empírica

Comparação entre implementações iterativas e recursivas dos algoritmos de **Fatorial** e **Fibonacci**.

Foram analisados tempo de execução, profundidade da pilha e crescimento dos algoritmos em função do tamanho da entrada.

<p align="center">
  <img src="./report-01-empirical-analysis/graficos/fatorial_tempo.png" width="48%">
  <img src="./report-01-empirical-analysis/graficos/fatorial_pilha.png" width="48%">
</p>

<p align="center">
  <img src="./report-01-empirical-analysis/graficos/fibonacci_tempo.png" width="48%">
  <img src="./report-01-empirical-analysis/graficos/fibonacci_pilha.png" width="48%">
</p>

---

## Relatório 02 — Complexidade Espacial e Temporal

Análise teórica e experimental de versões iterativas e recursivas para:

- contador de zeros em matriz;
- inversão de string;
- maior ocorrência de uma mesma letra.

Foram analisadas as complexidades temporal e espacial, com demonstrações por indução e validação experimental.

<p align="center">
  <img src="./report-02-spatial-temporal-complexity/charts/matrix-zero-time.png" width="48%">
  <img src="./report-02-spatial-temporal-complexity/charts/matrix-zero-stack.png" width="48%">
</p>

<p align="center">
  <img src="./report-02-spatial-temporal-complexity/charts/string-reverse-time.png" width="48%">
  <img src="./report-02-spatial-temporal-complexity/charts/string-reverse-stack.png" width="48%">
</p>

<p align="center">
  <img src="./report-02-spatial-temporal-complexity/charts/most-frequent-time.png" width="48%">
  <img src="./report-02-spatial-temporal-complexity/charts/most-frequent-stack.png" width="48%">
</p>

---

## Relatório 03 — Cadastro de Bolsistas

Implementação e análise de um programa em C responsável pela transferência de registros entre arquivos CSV, evitando a inserção de nomes já existentes no arquivo de destino.

A redundância é verificada por **busca sequencial**, utilizando exclusivamente o campo `Nome`.

Considerando `n` novos registros e `Q` registros existentes inicialmente:

```text
C(n,Q) = nQ + n(n - 1) / 2
```

Para `Q` fixo:

```text
Θ(n²)
```

### Validação experimental

Nos experimentos foi utilizado `Q = 1`.

| Nomes inseridos | Observado | Teórico | Diferença |
| ---: | ---: | ---: | ---: |
| 10 | 55 | 55 | 0 |
| 20 | 210 | 210 | 0 |
| 40 | 820 | 820 | 0 |
| 80 | 3240 | 3240 | 0 |
| 160 | 12880 | 12880 | 0 |

<p align="center">
  <img src="./report-03-scholarship-registration-analysis/charts/comparacoes.png" width="70%">
</p>

---

## Relatório 04 — Busca Sequencial, MergeSort e Busca Binária

Comparação teórica e experimental de algoritmos de busca e ordenação aplicados à detecção de registros duplicados em uma base pública do **Cadastro Nacional de Estabelecimentos de Saúde — CNES**.

A chave utilizada é `CO_UNIDADE`. A validação sobre a base integral confirmou **636.647 valores únicos em 636.647 registros** e ausência de ordenação crescente ou decrescente.

Foram implementados:

| Algoritmo | Tempo | Espaço auxiliar |
| --- | ---: | ---: |
| Busca sequencial iterativa | `Θ(N)` | `Θ(1)` |
| Busca sequencial recursiva | `Θ(N)` | `Θ(N)` |
| MergeSort iterativo | `Θ(N log N)` | `Θ(N)` |
| MergeSort recursivo | `Θ(N log N)` | `Θ(N)` |
| Busca binária iterativa | `Θ(log N)` | `Θ(1)` |
| Busca binária recursiva | `Θ(log N)` | `Θ(log N)` |

### Validação experimental

O workload principal utiliza uma base inicial com **10.000 registros** e lotes de `10`, `40`, `100`, `300`, `500`, `1000`, `2500` e `5000` registros, sempre com metade duplicados e metade novos.

Para a busca sequencial, com `M=10.000` fixo e `N` representando o tamanho do lote:

```text
T(N) = (N/2)M + N²/4
```

Os valores teóricos coincidiram exatamente com os observados em todos os cenários. No maior lote (`N=5000`), a busca sequencial executou **31.250.000 comparações**, enquanto a busca binária executou **65.308**, abaixo do limite teórico de **70.000**.

No experimento que varia o tamanho da estrutura, uma chave ausente produziu:

| N | Sequencial | Binária |
| ---: | ---: | ---: |
| 125 | 125 | 7 |
| 500 | 500 | 9 |
| 1000 | 1000 | 10 |
| 5000 | 5000 | 13 |
| 10000 | 10000 | 14 |

Isso torna visível a diferença entre crescimento `Θ(N)` e `Θ(log N)`.

<p align="center">
  <img src="./report-04-search-and-sort/charts/teoria_vs_experimento_k.png" width="48%">
  <img src="./report-04-search-and-sort/charts/crescimento_n_comparacoes.png" width="48%">
</p>

<p align="center">
  <img src="./report-04-search-and-sort/charts/tempos_algoritmos.png" width="60%">
</p>

Os detalhes de metodologia, preparação dos dados, compilação, execução e resultados estão em `report-04-search-and-sort/README.md`.

---

## Estrutura do projeto

```text
algorithm-analysis/
│
├── report-01-empirical-analysis/
│   ├── graficos/
│   ├── relatorio/
│   ├── resultados/
│   ├── *.c
│   └── plot.py
│
├── report-02-spatial-temporal-complexity/
│   ├── charts/
│   ├── report/
│   ├── results/
│   ├── src/
│   └── plot.py
│
├── report-03-scholarship-registration-analysis/
│   ├── charts/
│   ├── data/
│   ├── report/
│   ├── results/
│   ├── src/
│   ├── prepare_tests.py
│   └── plot.py
│
├── report-04-search-and-sort/
│   ├── charts/
│   ├── data/
│   ├── report/
│   ├── results/
│   ├── scripts/
│   ├── src/
│   └── README.md
│
├── .gitignore
└── README.md
```

## Tecnologias

- C
- GCC
- Python
- Pandas
- Matplotlib
- CSV
- Git
- GitHub