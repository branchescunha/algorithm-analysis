import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

ARQUIVO_CSV = "resultados_teorema_mestre.csv"

def carregar_e_consolidar(caminho_csv):

    df = pd.read_csv(caminho_csv)

    colunas_numericas = [
        "a", "b", "n", "operacoes_observadas",
        "formula_exata_inducao", "referencia_assintotica", "tempo_medio_ms"
    ]
    for col in colunas_numericas:
        df[col] = pd.to_numeric(df[col], errors="coerce")

    resumo = (
        df.groupby(
            ["problema", "caso_teorema", "a", "b", "f_n", "complexidade_teorica", "n"],
            as_index=False
        )
        .agg(
            baterias=("tempo_medio_ms", "count"),
            operacoes_observadas=("operacoes_observadas", "first"),
            formula_exata_inducao=("formula_exata_inducao", "first"),
            referencia_assintotica=("referencia_assintotica", "first"),
            tempo_medio_ms=("tempo_medio_ms", "mean"),
            tempo_desvio_ms=("tempo_medio_ms", "std"),
        )
        .sort_values(["problema", "n"])
    )
    resumo["tempo_desvio_ms"] = resumo["tempo_desvio_ms"].fillna(0.0)
    return resumo


def escalar_curva_teorica(valores_teoricos, tempos_observados):

    x = np.asarray(valores_teoricos, dtype=float)
    y = np.asarray(tempos_observados, dtype=float)
    denom = np.sum(x * x)
    if denom == 0:
        return np.zeros_like(x)
    c = np.sum(x * y) / denom
    return c * x


def plotar_problema_individual(df_prob, titulo_principal, label_assintotico, nome_arquivo):

    n = df_prob["n"].values
    ops_obs = df_prob["operacoes_observadas"].values
    ops_ind = df_prob["formula_exata_inducao"].values
    ref_assint = df_prob["referencia_assintotica"].values
    tempo_obs = df_prob["tempo_medio_ms"].values
    tempo_std = df_prob["tempo_desvio_ms"].values

    tempo_teorico_escalado = escalar_curva_teorica(ref_assint, tempo_obs)

    fig, axes = plt.subplots(1, 2, figsize=(14, 5.5))
    fig.suptitle(titulo_principal, fontsize=14, fontweight="bold")

    ax1 = axes[0]
    ax1.plot(n, ops_obs, "o-", linewidth=2, markersize=6, label="Experimental (Operações C)")
    ax1.plot(n, ops_ind, "s--", linewidth=1.8, markersize=5, label="Fórmula Fechada (Indução)")
    ax1.plot(n, ref_assint, "d:", linewidth=1.8, markersize=5, label=f"Termo Dominante {label_assintotico}")
    ax1.set_xscale("log")
    ax1.set_yscale("log")
    ax1.set_title("Validação Matemática: Operações × Indução (Log-Log)")
    ax1.set_xlabel("Tamanho da Entrada (N) [escala log]")
    ax1.set_ylabel("Quantidade de Operações [escala log]")
    ax1.grid(True, which="both", linestyle="--", alpha=0.5)
    ax1.legend()

    ax2 = axes[1]
    ax2.errorbar(
        n, tempo_obs, yerr=tempo_std, fmt="o-", linewidth=2, markersize=6,
        capsize=4, label="Tempo Experimental Médio (ms)"
    )
    ax2.plot(
        n, tempo_teorico_escalado, "r--", linewidth=2,
        label=f"Curva Teórica Escalada c·{label_assintotico}"
    )
    ax2.set_title("Validação Prática: Tempo de Execução × Curva Teórica")
    ax2.set_xlabel("Tamanho da Entrada (N)")
    ax2.set_ylabel("Tempo Médio (ms)")
    ax2.grid(True, which="both", linestyle="--", alpha=0.5)
    ax2.legend()

    plt.tight_layout(rect=[0, 0.02, 1, 0.94])
    plt.savefig(nome_arquivo, dpi=300)
    plt.close(fig)
    print(f"[OK] Gráfico salvo: {nome_arquivo}")


def plotar_painel_comparativo(resumo):

    fig, axes = plt.subplots(1, 2, figsize=(14, 5.5))
    fig.suptitle("Comparação Global dos Três Casos do Teorema Mestre", fontsize=14, fontweight="bold")

    cores = {"Problema_1": "#1f77b4", "Problema_2": "#2ca02c", "Problema_3": "#d62728"}
    marcadores = {"Problema_1": "o", "Problema_2": "s", "Problema_3": "^"}
    rotulos = {
        "Problema_1": "Problema 1 (Caso 1): T(n)=4T(n/2)+n → Θ(n²)",
        "Problema_2": "Problema 2 (Caso 2): T(n)=3T(n/3)+n → Θ(n log n)",
        "Problema_3": "Problema 3 (Caso 3): T(n)=2T(n/2)+n² → Θ(n²)",
    }

    for prob, grupo in resumo.groupby("problema"):
        c = cores.get(prob, "black")
        m = marcadores.get(prob, "o")
        lbl = rotulos.get(prob, prob)

        axes[0].plot(
            grupo["n"], grupo["operacoes_observadas"],
            marker=m, linestyle="-", linewidth=2, color=c, label=lbl
        )
        axes[1].plot(
            grupo["n"], grupo["tempo_medio_ms"],
            marker=m, linestyle="-", linewidth=2, color=c, label=lbl
        )

    axes[0].set_xscale("log")
    axes[0].set_yscale("log")
    axes[0].set_title("Crescimento de Operações por Tamanho de Entrada (Log-Log)")
    axes[0].set_xlabel("Tamanho da Entrada (N)")
    axes[0].set_ylabel("Operações Executadas")
    axes[0].grid(True, which="both", linestyle="--", alpha=0.5)
    axes[0].legend()

    axes[1].set_xscale("log")
    axes[1].set_yscale("log")
    axes[1].set_title("Tempo Médio de Execução em Função de N (Log-Log)")
    axes[1].set_xlabel("Tamanho da Entrada (N)")
    axes[1].set_ylabel("Tempo Médio (ms)")
    axes[1].grid(True, which="both", linestyle="--", alpha=0.5)
    axes[1].legend()

    plt.tight_layout(rect=[0, 0.02, 1, 0.94])
    plt.savefig("painel_comparativo_geral.png", dpi=300)
    plt.close(fig)
    print("[OK] Gráfico salvo: painel_comparativo_geral.png")


def main():
    resumo = carregar_e_consolidar(ARQUIVO_CSV)

    resumo.to_csv("resumo_tabelas_relatorio.csv", index=False)
    print("[OK] Tabela consolidada salva em: resumo_tabelas_relatorio.csv\n")
    print(resumo.to_string(index=False))
    print()

    p1 = resumo[resumo["problema"] == "Problema_1"]
    if not p1.empty:
        plotar_problema_individual(
            p1,
            "Problema 1 (Caso 1 - Recursividade Dominante): T(n) = 4T(n/2) + n  [a=4, b=2, Θ(n²)]",
            "n²",
            "grafico_problema1_caso1.png"
        )

    p2 = resumo[resumo["problema"] == "Problema_2"]
    if not p2.empty:
        plotar_problema_individual(
            p2,
            "Problema 2 (Caso 2 - Custo Equilibrado): T(n) = 3T(n/3) + n  [a=3, b=3, Θ(n log₃ n)]",
            "n·log₃(n)",
            "grafico_problema2_caso2.png"
        )

    p3 = resumo[resumo["problema"] == "Problema_3"]
    if not p3.empty:
        plotar_problema_individual(
            p3,
            "Problema 3 (Caso 3 - Custo Externo Dominante): T(n) = 2T(n/2) + n²  [a=2, b=2, Θ(n²)]",
            "n²",
            "grafico_problema3_caso3.png"
        )

    plotar_painel_comparativo(resumo)


if __name__ == "__main__":
    main()