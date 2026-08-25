# Algorithm Analysis

Repositório dedicado aos estudos, implementações e relatórios desenvolvidos na disciplina de Análise de Algoritmos do curso de Ciência da Computação.

O projeto reúne implementações em C e experimentos relacionados à análise de desempenho, complexidade de algoritmos, recursividade e utilização da pilha de execução.

## Relatórios

| Relatório | Tema | Status |
| --- | --- | --- |
| 01 | Análise Empírica — Fatorial e Fibonacci | Concluído |
| 02 | A definir | Pendente |
| 03 | A definir | Pendente |
| 04 | A definir | Pendente |
| 05 | A definir | Pendente |
| 06 | A definir | Pendente |
| 07 | A definir | Pendente |

## Relatório 01 — Análise Empírica

Comparação entre implementações iterativas e recursivas dos algoritmos de Fatorial e Fibonacci.

Foram analisados:

- tempo médio de execução T(N);
- profundidade máxima da pilha P(N);
- comportamento das versões iterativas e recursivas;
- variação do desempenho conforme o valor de N.

### Resultados

<p align="center">
  <img src="./report-01-empirical-analysis/graficos/fatorial_tempo.png" width="48%">
  <img src="./report-01-empirical-analysis/graficos/fatorial_pilha.png" width="48%">
</p>

<p align="center">
  <img src="./report-01-empirical-analysis/graficos/fibonacci_tempo.png" width="48%">
  <img src="./report-01-empirical-analysis/graficos/fibonacci_pilha.png" width="48%">
</p>

## Tecnologias utilizadas

- C
- GCC
- Python
- Pandas
- Matplotlib
- CSV

## Estrutura do projeto

```text
algorithm-analysis/
│
├── report-01-empirical-analysis/
│   ├── graficos/
│   ├── relatorio/
│   ├── resultados/
│   ├── fatorial_iterativo.c
│   ├── fatorial_recursivo.c
│   ├── fibonacci_iterativo.c
│   ├── fibonacci_recursivo.c
│   └── plot.py
│
├── .gitignore
└── README.md
```
