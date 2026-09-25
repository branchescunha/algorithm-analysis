# Relatório 04 — Busca Sequencial, MergeSort e Busca Binária

Implementação e análise de duas estratégias para inserir registros em CSV sem duplicar a chave escolhida.

## Base de dados

Foi utilizado `cnes_estabelecimentos.csv`, proveniente do Portal de Dados Abertos, com dados do Cadastro Nacional de Estabelecimentos de Saúde (CNES).

A chave escolhida é `CO_UNIDADE` porque, na base integral analisada:

- possui **636.647 valores únicos em 636.647 registros**;
- **não está ordenada** no arquivo original;
- contém valores alfanuméricos, por isso é tratada como texto e comparada com `strcmp`.

O script `reduzir.py` valida essas condições com `isna()`, `is_unique`, `is_monotonic_increasing` e `is_monotonic_decreasing` e gera uma amostra reproduzível de **10.000 registros** (`random_state=42`) para os experimentos.

O CSV integral possui cerca de 220 MiB e não é versionado no Git. Os arquivos efetivamente usados nos testes ficam em `data/`.

## Solução 1 — Busca sequencial

- `src/seq_iterativa.c`: busca sequencial iterativa;
- `src/seq_recursiva.c`: busca sequencial recursiva.

| Implementação | Tempo | Espaço auxiliar |
| --- | ---: | ---: |
| Iterativa | `Θ(N)` | `Θ(1)` |
| Recursiva | `Θ(N)` | `Θ(N)` |

Demonstração temporal da versão recursiva:

```text
T(N) = T(N - 1) + c
T(N) = T(N - 2) + 2c
...
T(N) = T(0) + Nc
T(N) = Θ(N)
```

Na versão iterativa, o laço executa no máximo `N` comparações, chegando igualmente a `T(N)=Θ(N)`.

## Solução 2 — MergeSort + busca binária

As chaves são carregadas em um vetor na RAM. O vetor é ordenado; o CSV físico permanece na ordem original e os registros aceitos são anexados ao final do arquivo.

| Algoritmo | Tempo | Espaço auxiliar |
| --- | ---: | ---: |
| MergeSort iterativo | `Θ(N log N)` | `Θ(N)` |
| MergeSort recursivo | `Θ(N log N)` | `Θ(N)` |
| Busca binária iterativa | `Θ(log N)` | `Θ(1)` |
| Busca binária recursiva | `Θ(log N)` | `Θ(log N)` |

Recorrências principais:

```text
MergeSort:
T(N) = 2T(N/2) + cN
T(N) = Θ(N log N)

Busca binária:
T(N) = T(N/2) + c
T(N) = Θ(log N)
```

Após cada inserção válida, o índice em RAM é mantido ordenado por deslocamento no vetor (`memmove`), operação `Θ(N)` no pior caso.

## Metodologia experimental

### Workload principal

A base de destino começa sempre com `M = 10.000` registros. Para um lote de tamanho `N`, metade das entradas já existe e metade é nova:

```text
q = N/2
```

Os lotes usados são:

```text
N = 10, 40, 100, 300, 500, 1000, 2500, 5000
```

Cada cenário de tempo é repetido 20 vezes. Antes de cada execução, o arquivo de destino é restaurado a partir de `data/destino_backup.csv`.

Para esse workload, o custo exato da busca sequencial é:

```text
T(N) = q(q+1)/2 + qM + q(q-1)/2
T(N) = qM + q²
T(N) = (N/2)M + N²/4
```

Com `M=10.000`, os valores teóricos coincidem exatamente com os observados.

| N | Seq. teórica | Seq. observada | Bin. observada | Limite binário |
| ---: | ---: | ---: | ---: | ---: |
| 10 | 50.025 | 50.025 | 129 | 140 |
| 40 | 200.400 | 200.400 | 512 | 560 |
| 100 | 502.500 | 502.500 | 1.269 | 1.400 |
| 300 | 1.522.500 | 1.522.500 | 3.857 | 4.200 |
| 500 | 2.562.500 | 2.562.500 | 6.444 | 7.000 |
| 1000 | 5.250.000 | 5.250.000 | 12.857 | 14.000 |
| 2500 | 14.062.500 | 14.062.500 | 32.409 | 35.000 |
| 5000 | 31.250.000 | 31.250.000 | 65.308 | 70.000 |

### Experimento variando N

Para tornar visível a diferença `Θ(N)` × `Θ(log N)`, `benchmark_crescimento.py` usa uma chave garantidamente ausente em bases com:

```text
N = 125, 500, 1000, 5000, 10000
```

| N | Sequencial | Binária |
| ---: | ---: | ---: |
| 125 | 125 | 7 |
| 500 | 500 | 9 |
| 1000 | 1000 | 10 |
| 5000 | 5000 | 13 |
| 10000 | 10000 | 14 |

Isso evidencia diretamente o crescimento linear e logarítmico.

## Resultados visuais

<p align="center">
  <img src="./charts/teoria_vs_experimento_k.png" width="75%">
</p>

<p align="center">
  <img src="./charts/crescimento_n_comparacoes.png" width="75%">
</p>

<p align="center">
  <img src="./charts/tempos_algoritmos.png" width="75%">
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

Coloque `cnes_estabelecimentos.csv` na raiz de `report-04-search-and-sort` e execute:

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

## Estrutura

```text
report-04-search-and-sort/
├── charts/
├── data/
├── report/
├── results/
├── scripts/
├── src/
├── .gitignore
└── README.md
```
