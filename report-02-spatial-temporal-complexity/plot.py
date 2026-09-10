from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt


BASE = Path(__file__).resolve().parent
RESULTS = BASE / "results"
CHARTS = BASE / "charts"

CHARTS.mkdir(exist_ok=True)


def read(name):
    return pd.read_csv(RESULTS / name)


def generate(iterative, recursive, column, ylabel, title, filename):
    plt.figure(figsize=(9, 5))

    plt.plot(
        iterative["n"],
        iterative[column],
        marker="o",
        label="Iterativo"
    )

    plt.plot(
        recursive["n"],
        recursive[column],
        marker="o",
        label="Recursivo"
    )

    plt.title(title)
    plt.xlabel("N")
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(CHARTS / filename, dpi=150)
    plt.close()


def main():
    matrix_i = read("matrix-zero-iterative.csv")
    matrix_r = read("matrix-zero-recursive.csv")

    reverse_i = read("string-reverse-iterative.csv")
    reverse_r = read("string-reverse-recursive.csv")

    frequent_i = read("most-frequent-iterative.csv")
    frequent_r = read("most-frequent-recursive.csv")

    generate(
        matrix_i,
        matrix_r,
        "time",
        "Tempo medio (ms)",
        "Contador de Zeros - T(N)",
        "matrix-zero-time.png"
    )

    generate(
        matrix_i,
        matrix_r,
        "stack",
        "Pilha maxima (bytes)",
        "Contador de Zeros - P(N)",
        "matrix-zero-stack.png"
    )

    generate(
        reverse_i,
        reverse_r,
        "time",
        "Tempo medio (ms)",
        "Inversor de String - T(N)",
        "string-reverse-time.png"
    )

    generate(
        reverse_i,
        reverse_r,
        "stack",
        "Pilha maxima (bytes)",
        "Inversor de String - P(N)",
        "string-reverse-stack.png"
    )

    generate(
        frequent_i,
        frequent_r,
        "time",
        "Tempo medio (ms)",
        "Maior Ocorrencia - T(N)",
        "most-frequent-time.png"
    )

    generate(
        frequent_i,
        frequent_r,
        "stack",
        "Pilha maxima (bytes)",
        "Maior Ocorrencia - P(N)",
        "most-frequent-stack.png"
    )

    print("Graficos gerados na pasta charts.")


if __name__ == "__main__":
    main()