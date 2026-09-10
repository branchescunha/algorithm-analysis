#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define MAX_NAME 512

void extract_name(const char *line, char *name)
{
    size_t i = 0;

    while (
        line[i] != '\0' &&
        line[i] != ',' &&
        line[i] != '\n' &&
        line[i] != '\r' &&
        i < MAX_NAME - 1)
    {
        name[i] = line[i];
        i++;
    }

    name[i] = '\0';
}

int ensure_destination(const char *filename, const char *header)
{
    FILE *file = fopen(filename, "r");

    if (file != NULL)
    {
        int first_character = fgetc(file);
        fclose(file);

        if (first_character != EOF)
        {
            return 1;
        }
    }

    file = fopen(filename, "w");

    if (file == NULL)
    {
        return 0;
    }

    fputs(header, file);

    size_t length = strlen(header);

    if (length > 0 && header[length - 1] != '\n')
    {
        fputc('\n', file);
    }

    fclose(file);

    return 1;
}

long long count_records(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    char line[MAX_LINE];
    char name[MAX_NAME];
    long long count = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        extract_name(line, name);

        if (name[0] != '\0')
        {
            count++;
        }
    }

    fclose(file);

    return count;
}

int name_exists(
    const char *destination_filename,
    const char *name,
    long long *comparisons)
{
    FILE *file = fopen(destination_filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    char line[MAX_LINE];
    char destination_name[MAX_NAME];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        extract_name(line, destination_name);

        if (destination_name[0] == '\0')
        {
            continue;
        }

        (*comparisons)++;

        if (strcmp(name, destination_name) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;
}

int append_line(const char *filename, const char *line)
{
    FILE *file = fopen(filename, "a");

    if (file == NULL)
    {
        return 0;
    }

    fputs(line, file);

    size_t length = strlen(line);

    if (length > 0 && line[length - 1] != '\n')
    {
        fputc('\n', file);
    }

    fclose(file);

    return 1;
}

int write_statistics(
    const char *filename,
    long long inserted,
    long long comparisons)
{
    FILE *file = fopen(filename, "a+");

    if (file == NULL)
    {
        return 0;
    }

    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0)
    {
        fprintf(file, "nomes_inseridos,comparacoes\n");
    }

    fprintf(file, "%lld,%lld\n", inserted, comparisons);

    fclose(file);

    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf(
            "Uso: %s <origem.csv> <destino.csv> <estatisticas.csv>\n",
            argv[0]);
        return 1;
    }

    const char *source_filename = argv[1];
    const char *destination_filename = argv[2];
    const char *statistics_filename = argv[3];

    FILE *source = fopen(source_filename, "r");

    if (source == NULL)
    {
        printf("ERRO: nao foi possivel abrir o arquivo de origem.\n");
        return 1;
    }

    char header[MAX_LINE];

    if (fgets(header, sizeof(header), source) == NULL)
    {
        printf("ERRO: arquivo de origem vazio.\n");
        fclose(source);
        return 1;
    }

    if (!ensure_destination(destination_filename, header))
    {
        printf("ERRO: nao foi possivel preparar o arquivo de destino.\n");
        fclose(source);
        return 1;
    }

    long long initial_records = count_records(destination_filename);
    long long inserted = 0;
    long long comparisons = 0;

    char line[MAX_LINE];
    char name[MAX_NAME];

    while (fgets(line, sizeof(line), source) != NULL)
    {
        extract_name(line, name);

        if (name[0] == '\0')
        {
            continue;
        }

        if (name_exists(destination_filename, name, &comparisons))
        {
            printf(
                "ERRO: nome redundante encontrado: %s\n",
                name);
            continue;
        }

        if (!append_line(destination_filename, line))
        {
            printf("ERRO: nao foi possivel escrever no arquivo de destino.\n");
            fclose(source);
            return 1;
        }

        inserted++;
    }

    fclose(source);

    if (!write_statistics(
            statistics_filename,
            inserted,
            comparisons))
    {
        printf("ERRO: nao foi possivel gerar o arquivo de estatisticas.\n");
        return 1;
    }

    printf("Cadastros iniciais: %lld\n", initial_records);
    printf("Nomes inseridos: %lld\n", inserted);
    printf("Comparacoes: %lld\n", comparisons);

    return 0;
}