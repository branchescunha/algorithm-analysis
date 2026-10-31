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
| 05 | Aplicação do Teorema Mestre | Concluído |
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

## Relatório 05 — Aplicação do Teorema Mestre

Análise teórica e experimental de algoritmos recursivos sob o paradigma de **Divisão e Conquista**, aplicando o **Teorema Mestre** na forma `T(n) = aT(n/b) + f(n)` e validando formalmente os resultados por **indução matemática**.

Foram projetados três problemas independentes em C, contemplando os três casos do teorema e atendendo às restrições estruturais (`a ≠ 2` nos Casos 1 e 2, e `a ≠ b` em pelo menos um deles):

| Problema | Recorrência | Caso do Teorema Mestre | Fórmula Fechada (Indução) | Tempo | Espaço auxiliar |
| --- | --- | --- | --- | ---: | ---: |
| Problema 1 (`a=4, b=2`) | `T(n) = 4T(n/2) + n` | Caso 1 — Recursividade Dominante | `2n² - n` | `Θ(n²)` | `Θ(log₂ n)` |
| Problema 2 (`a=3, b=3`) | `T(n) = 3T(n/3) + n` | Caso 2 — Custo Equilibrado | `n log₃(n) + n` | `Θ(n log n)` | `Θ(log₃ n)` |
| Problema 3 (`a=2, b=2`) | `T(n) = 2T(n/2) + n²` | Caso 3 — Custo Externo Dominante | `2n² - n` | `Θ(n²)` | `Θ(log₂ n)` |

### Validação experimental

Os experimentos foram executados para potências de `2` nos Problemas 1 e 3 (`N` de `16` até `16.384`) e potências de `3` no Problema 2 (`N` de `27` até `1.594.323`), com `20` repetições internas e `3` baterias independentes consolidadas em um mesmo arquivo CSV.

A contagem empírica de operações coincidiu com **erro zero** em relação às fórmulas fechadas demonstradas por indução em todos os cenários:

| Cenário | Maior N testado | Operações Observadas | Fórmula Indução | Diferença | Tempo Médio |
| --- | ---: | ---: | ---: | ---: | ---: |
| Problema 1 (Caso 1) | 16.384 | 536.854.528 | 536.854.528 | 0 | ~491,00 ms |
| Problema 2 (Caso 2) | 1.594.323 | 22.320.522 | 22.320.522 | 0 | ~7,83 ms |
| Problema 3 (Caso 3) | 16.384 | 536.854.528 | 536.854.528 | 0 | ~63,10 ms |

Embora os Problemas 1 e 3 pertençam à mesma classe assintótica `Θ(n²)` e executem exatamente o mesmo número de operações (`2n² - n`), o Problema 1 apresentou tempo de execução cerca de **7,8 vezes maior** devido ao *overhead* de gerenciamento da pilha de chamadas recursivas (`4¹⁴` folhas no Caso 1 contra laços iterativos contíguos no cache L1 no Caso 3).

<p align="center">
  <img src="./report-5-Master-Theorem/charts/grafico_problema1_caso1.png" width="48%">
  <img src="./report-5-Master-Theorem/charts/grafico_problema2_caso2.png" width="48%">
</p>

<p align="center">
  <img src="./report-5-Master-Theorem/charts/grafico_problema3_caso3.png" width="48%">
  <img src="./report-5-Master-Theorem/charts/painel_comparativo_geral.png" width="48%">
</p>

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
├── report-5-Master-Theorem/
│   ├── charts/
│   ├── report/
│   ├── results/
│   ├── scripts/
│   └── src/
│
├── .gitignore
└── README.md
```

## Tecnologias

- C
- GCC
- Python
- NumPy
- Pandas
- Matplotlib
- CSV
- Git
- GitHub