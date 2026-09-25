from pathlib import Path
import csv
import math
import platform
import shutil
import subprocess

BASE_DIR = Path(__file__).resolve().parents[1]
DATA_DIR = BASE_DIR / 'data'
RESULTS_DIR = BASE_DIR / 'results'
BASE_CSV = DATA_DIR / 'destino_backup.csv'
TAMANHOS = [125, 500, 1000, 5000, 10000]
REPETICOES = 50
PROGRAMAS = ['seq_iterativa', 'seq_recursiva', 'bin_iterativa', 'bin_recursiva']
ALVO = '~~~~ALVO_AUSENTE~~~~'


def executavel(nome):
    sufixo = '.exe' if platform.system() == 'Windows' else ''
    return BASE_DIR / f'{nome}{sufixo}'


def interpretar(saida):
    dados = {}
    for linha in saida.splitlines():
        if '=' in linha:
            chave, valor = linha.strip().split('=', 1)
            dados[chave] = valor
    return dados


def preparar_arquivos():
    with BASE_CSV.open('r', encoding='utf-8', newline='') as f:
        reader = csv.reader(f, delimiter=';')
        header = next(reader)
        linhas = list(reader)

    indice_chave = header.index('CO_UNIDADE')
    if any(linha[indice_chave] == ALVO for linha in linhas):
        raise ValueError('A chave artificial de teste ja existe na base.')

    modelo = list(linhas[0])
    modelo[indice_chave] = ALVO
    alvo_csv = RESULTS_DIR / 'alvo_ausente.csv'
    with alvo_csv.open('w', encoding='utf-8', newline='') as f:
        writer = csv.writer(f, delimiter=';')
        writer.writerow(header)
        writer.writerow(modelo)

    bases = {}
    for n in TAMANHOS:
        caminho = RESULTS_DIR / f'base_n{n}.csv'
        with caminho.open('w', encoding='utf-8', newline='') as f:
            writer = csv.writer(f, delimiter=';')
            writer.writerow(header)
            writer.writerows(linhas[:n])
        bases[n] = caminho

    return alvo_csv, bases


def main():
    RESULTS_DIR.mkdir(exist_ok=True)
    alvo_csv, bases = preparar_arquivos()
    linhas_saida = []

    for n in TAMANHOS:
        acumulado = {
            nome: {'TEMPO_BUSCA': 0.0, 'TEMPO_ORDENACAO': 0.0, 'TEMPO_TOTAL_ALGORITMOS': 0.0}
            for nome in PROGRAMAS
        }
        comparacoes = {}

        for _ in range(REPETICOES):
            for nome in PROGRAMAS:
                destino = RESULTS_DIR / 'destino_crescimento_tmp.csv'
                shutil.copyfile(bases[n], destino)
                proc = subprocess.run(
                    [str(executavel(nome)), str(alvo_csv), str(destino)],
                    capture_output=True,
                    text=True,
                    check=True,
                )
                dados = interpretar(proc.stdout)
                comparacoes[nome] = int(dados['COMPARACOES'])
                for campo in acumulado[nome]:
                    acumulado[nome][campo] += float(dados.get(campo, 0.0))

        linha = {
            'n': n,
            'seq_teorico': n,
            'bin_limite_teorico': math.floor(math.log2(n)) + 1,
        }
        for nome in PROGRAMAS:
            linha[f'{nome}_comparacoes'] = comparacoes[nome]
            for campo, soma in acumulado[nome].items():
                linha[f'{nome}_{campo.lower()}'] = soma / REPETICOES
        linhas_saida.append(linha)

    saida = RESULTS_DIR / 'crescimento_n.csv'
    with saida.open('w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=linhas_saida[0].keys())
        writer.writeheader()
        writer.writerows(linhas_saida)

    for path in RESULTS_DIR.glob('base_n*.csv'):
        path.unlink()
    for nome in ['alvo_ausente.csv', 'destino_crescimento_tmp.csv']:
        path = RESULTS_DIR / nome
        if path.exists():
            path.unlink()

    print(f'Estatisticas de crescimento salvas em: {saida}')


if __name__ == '__main__':
    main()
