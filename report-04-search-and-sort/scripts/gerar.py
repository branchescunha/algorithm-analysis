from pathlib import Path
import pandas as pd

BASE_DIR = Path(__file__).resolve().parents[1]
SOURCE = BASE_DIR / 'cnes_estabelecimentos.csv'
BASE = BASE_DIR / 'data' / 'destino_backup.csv'
DATA_DIR = BASE_DIR / 'data'
CHAVE = 'CO_UNIDADE'
TAMANHOS = [10, 20, 40, 80]


def main():
    if not SOURCE.exists() or not BASE.exists():
        raise FileNotFoundError('Execute reduzir.py depois de disponibilizar o CSV original.')

    df_origem = pd.read_csv(SOURCE, sep=';', encoding='latin1', low_memory=False, dtype={CHAVE: str})
    df_base = pd.read_csv(BASE, sep=';', encoding='utf-8', low_memory=False, dtype={CHAVE: str})

    chaves_base = set(df_base[CHAVE])
    candidatos_novos = df_origem[~df_origem[CHAVE].isin(chaves_base)]

    for tamanho in TAMANHOS:
        metade = tamanho // 2
        repetidos = df_base.head(metade)
        novos = candidatos_novos.head(metade)
        teste = pd.concat([repetidos, novos], ignore_index=True)
        teste = teste.sample(frac=1, random_state=100 + tamanho).reset_index(drop=True)

        if teste[CHAVE].duplicated().any():
            raise ValueError(f'O teste {tamanho} contem chaves repetidas internamente.')

        destino = DATA_DIR / f'teste{tamanho}.csv'
        teste.to_csv(destino, index=False, sep=';', encoding='utf-8')
        print(f'{destino.name}: {metade} existentes + {metade} novos')


if __name__ == '__main__':
    main()
