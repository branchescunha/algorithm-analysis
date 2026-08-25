import os
import pandas as pd
import matplotlib.pyplot as plt

RESULTADOS = "resultados"
GRAFICOS = "graficos"


def carregar(nome):
    return pd.read_csv(os.path.join(RESULTADOS, nome))


def gerar_grafico(
    iterativo,
    recursivo,
    coluna,
    ylabel,
    titulo,
    arquivo
):
    plt.figure(figsize=(10, 6))

    plt.plot(
        iterativo["n"],
        iterativo[coluna],
        marker="o",
        label="Iterativo"
    )

    plt.plot(
        recursivo["n"],
        recursivo[coluna],
        marker="o",
        label="Recursivo"
    )

    plt.title(titulo)
    plt.xlabel("N")
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(
        os.path.join(GRAFICOS, arquivo),
        dpi=200
    )

    plt.close()


def main():
    os.makedirs(GRAFICOS, exist_ok=True)

    fat_i = carregar("fatorial_iterativo.csv")
    fat_r = carregar("fatorial_recursivo.csv")

    fib_i = carregar("fibonacci_iterativo.csv")
    fib_r = carregar("fibonacci_recursivo.csv")

    gerar_grafico(
        fat_i,
        fat_r,
        "time",
        "Tempo medio (ms)",
        "Fatorial - T(N)",
        "fatorial_tempo.png"
    )

    gerar_grafico(
        fat_i,
        fat_r,
        "stack",
        "Profundidade maxima da pilha (bytes)",
        "Fatorial - P(N)",
        "fatorial_pilha.png"
    )

    gerar_grafico(
        fib_i,
        fib_r,
        "time",
        "Tempo medio (ms)",
        "Fibonacci - T(N)",
        "fibonacci_tempo.png"
    )

    gerar_grafico(
        fib_i,
        fib_r,
        "stack",
        "Profundidade maxima da pilha (bytes)",
        "Fibonacci - P(N)",
        "fibonacci_pilha.png"
    )

    print("Graficos gerados na pasta graficos.")


if __name__ == "__main__":
    main()