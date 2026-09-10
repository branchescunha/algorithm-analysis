from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


BASE_DIR = Path(__file__).resolve().parent
RESULTS_DIR = BASE_DIR / "results"
CHARTS_DIR = BASE_DIR / "charts"

Q = 1


def main():
    statistics = pd.read_csv(
        RESULTS_DIR / "estatisticas.csv"
    )

    statistics["comparacoes_teoricas"] = (
        statistics["nomes_inseridos"] * Q
        + (
            statistics["nomes_inseridos"]
            * (statistics["nomes_inseridos"] - 1)
        ) // 2
    )

    statistics["diferenca"] = (
        statistics["comparacoes"]
        - statistics["comparacoes_teoricas"]
    )

    comparison = statistics.rename(
        columns={
            "comparacoes": "comparacoes_observadas"
        }
    )

    comparison.to_csv(
        RESULTS_DIR / "comparacao.csv",
        index=False
    )

    plt.figure(figsize=(9, 6))

    plt.plot(
        comparison["nomes_inseridos"],
        comparison["comparacoes_observadas"],
        marker="o",
        label="Experimental"
    )

    plt.plot(
        comparison["nomes_inseridos"],
        comparison["comparacoes_teoricas"],
        marker="x",
        linestyle="--",
        label="Teórico"
    )

    plt.title("Comparações entre nomes")
    plt.xlabel("Quantidade de nomes inseridos")
    plt.ylabel("Número de comparações")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(
        CHARTS_DIR / "comparacoes.png",
        dpi=200
    )

    plt.close()

    print(comparison.to_string(index=False))
    print("Grafico gerado em charts/comparacoes.png")


if __name__ == "__main__":
    main()