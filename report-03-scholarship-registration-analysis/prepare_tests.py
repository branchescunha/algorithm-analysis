import csv
from pathlib import Path


BASE_DIR = Path(__file__).resolve().parent
DATA_DIR = BASE_DIR / "data"
SOURCE_FILE = DATA_DIR / "bolsistas-2025.csv"

SIZES = [10, 20, 40, 80, 160]


def read_source():
    with SOURCE_FILE.open(
        "r",
        encoding="cp1252",
        newline=""
    ) as file:
        reader = csv.reader(file)
        rows = list(reader)

    return rows[0], rows[1:]


def write_csv(path, header, rows):
    with path.open(
        "w",
        encoding="cp1252",
        newline=""
    ) as file:
        writer = csv.writer(file)
        writer.writerow(header)
        writer.writerows(rows)


def main():
    header, records = read_source()

    base_record = records[0]
    base_name = base_record[0]

    unique_records = []
    used_names = {base_name}

    for record in records[1:]:
        name = record[0]

        if name not in used_names:
            unique_records.append(record)
            used_names.add(name)

        if len(unique_records) == max(SIZES):
            break

    if len(unique_records) < max(SIZES):
        raise RuntimeError("Nao ha nomes unicos suficientes para os testes.")

    write_csv(
        DATA_DIR / "cadastro-base.csv",
        header,
        [base_record]
    )

    for size in SIZES:
        write_csv(
            DATA_DIR / f"teste-{size}.csv",
            header,
            unique_records[:size]
        )

    write_csv(
        DATA_DIR / "teste-redundancia.csv",
        header,
        [
            base_record,
            unique_records[0]
        ]
    )

    print("Arquivos de teste gerados com sucesso.")
    print(f"Q = 1: {base_name}")


if __name__ == "__main__":
    main()