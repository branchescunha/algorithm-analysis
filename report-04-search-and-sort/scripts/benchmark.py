from pathlib import Path
import csv
import platform
import shutil
import subprocess

BASE_DIR = Path(__file__).resolve().parents[1]
DATA_DIR = BASE_DIR / 'data'
RESULTS_DIR = BASE_DIR / 'results'
TAMANHOS = [10, 40, 100, 300, 500, 1000, 2500, 5000]
REPETICOES = 20

PROGRAMAS = {
    'seq_iterativa': 'seq_iterativa',
    'seq_recursiva': 'seq_recursiva',
    'bin_iterativa': 'bin_iterativa',
    'bin_recursiva': 'bin_recursiva',
}


def executavel(nome):
    sufixo = '.exe' if platform.system() == 'Windows' else ''
    return BASE_DIR / f'{nome}{sufixo}'


def interpretar(saida):
    dados = {}
    for linha in saida.splitlines():
        if '=' in linha:
            k, v = linha.strip().split('=', 1)
            dados[k] = v
    return dados


def main():
    RESULTS_DIR.mkdir(exist_ok=True)
    linhas = []

    for tamanho in TAMANHOS:
        acumulado = {}
        comparacoes_ref = {}
        inseridos_ref = {}

        for nome in PROGRAMAS:
            acumulado[nome] = {
                'TEMPO_BUSCA': 0.0,
                'TEMPO_ORDENACAO': 0.0,
                'TEMPO_MANUTENCAO': 0.0,
                'TEMPO_TOTAL_ALGORITMOS': 0.0,
            }

        for _ in range(REPETICOES):
            for nome in PROGRAMAS:
                destino = RESULTS_DIR / 'destino_temporario.csv'
                shutil.copyfile(DATA_DIR / 'destino_backup.csv', destino)

                exe = executavel(nome)
                if not exe.exists():
                    raise FileNotFoundError(f'Executavel nao encontrado: {exe}')

                proc = subprocess.run(
                    [str(exe), str(DATA_DIR / f'teste{tamanho}.csv'), str(destino)],
                    capture_output=True,
                    text=True,
                    check=True,
                )
                dados = interpretar(proc.stdout)

                comparacoes_ref[nome] = int(dados['COMPARACOES'])
                inseridos_ref[nome] = int(dados['INSERIDOS'])

                for campo in acumulado[nome]:
                    acumulado[nome][campo] += float(dados.get(campo, 0.0))

        linha = {'k': tamanho}
        for nome in PROGRAMAS:
            linha[f'{nome}_inseridos'] = inseridos_ref[nome]
            linha[f'{nome}_comparacoes'] = comparacoes_ref[nome]
            for campo, soma in acumulado[nome].items():
                linha[f'{nome}_{campo.lower()}'] = soma / REPETICOES
        linhas.append(linha)

    destino_csv = RESULTS_DIR / 'estatisticas.csv'
    with destino_csv.open('w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=linhas[0].keys())
        writer.writeheader()
        writer.writerows(linhas)

    temporario = RESULTS_DIR / 'destino_temporario.csv'
    if temporario.exists():
        temporario.unlink()

    print(f'Estatisticas salvas em: {destino_csv}')


if __name__ == '__main__':
    main()
