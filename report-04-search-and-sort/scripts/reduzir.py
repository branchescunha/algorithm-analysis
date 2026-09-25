from pathlib import Path
import pandas as pd

BASE_DIR = Path(__file__).resolve().parents[1]
SOURCE = BASE_DIR / 'cnes_estabelecimentos.csv'
DEST = BASE_DIR / 'data' / 'destino_backup.csv'
CHAVE = 'CO_UNIDADE'


def main():
    if not SOURCE.exists():
        raise FileNotFoundError(
            f'Arquivo original nao encontrado: {SOURCE}\n'
            'Baixe o CSV do Portal de Dados Abertos e salve com esse nome na raiz do projeto.'
        )

    df = pd.read_csv(SOURCE, sep=';', encoding='latin1', low_memory=False, dtype={CHAVE: str})

    if df[CHAVE].isna().any():
        raise ValueError(f'A coluna {CHAVE} possui valores ausentes.')
    if not df[CHAVE].is_unique:
        raise ValueError(f'A coluna {CHAVE} nao e unica no arquivo original.')
    if df[CHAVE].is_monotonic_increasing or df[CHAVE].is_monotonic_decreasing:
        raise ValueError(f'A coluna {CHAVE} esta ordenada no arquivo original.')

    amostra = df.sample(n=10000, random_state=42)
    DEST.parent.mkdir(exist_ok=True)
    amostra.to_csv(DEST, index=False, sep=';', encoding='utf-8')

    print(f'Registros originais: {len(df)}')
    print(f'Valores unicos em {CHAVE}: {df[CHAVE].nunique()}')
    print(f'Coluna original ordenada: nao')
    print(f'Amostra salva em: {DEST}')


if __name__ == '__main__':
    main()
