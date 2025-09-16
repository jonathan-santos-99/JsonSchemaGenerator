#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "gen.h"
#include "error.h"

const char *
shift(int *argc, const char ***argv)
{
    JSG_ASSERT(*argc > 0);
    return (*argc)--, *(*argv)++;
}

void
usage(const char *program)
{
    printf("Usage: %s [-f | --file <JSON_FILE>] [JSON_TEXT]\n", program);
}

void
report_error(const char *msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
}

char *
read_entire_file(const char *path)
{
    char *content = NULL;
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fprintf(
            stderr,
            "ERROR: could not open file %s: %s\n",
            path, strerror(errno)
        );
        goto cleanup;
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(
            stderr,
            "ERROR: could not read file %s: %s\n",
            path, strerror(errno)
        );
        goto cleanup;
    }

    long size = ftell(f);

    rewind(f);

    content = malloc(sizeof(char) * (size + 1));
    size_t nbytes = fread(content, sizeof(char), size, f);
    if ((long) nbytes < size) {
        fprintf(stderr, "ERROR: could not read entire file %s\n", path);
        goto cleanup;
    }

    content[size] = '\0';

    fclose(f);
    return content;

cleanup:
    if (f) {
        fclose(f);
    }

    if (content) {
        free(content);
    }

    return NULL;
}

int
main(int argc, const char **argv)
{
    Generator gen;
    const char *program = shift(&argc, &argv);
    if (argc == 0) {
        usage(program);
        return 1;
    }

    const char *next_arg = shift(&argc, &argv);
    if (strcmp(next_arg, "-f") == 0 || strcmp(next_arg, "--file") == 0) {
        if (argc == 0) {
            fprintf(stderr, "ERROR: missing %s argument\n", next_arg);
            usage(program);
            return 1;
        }

        const char *json = read_entire_file(shift(&argc, &argv));
        if (json == NULL) {
            return 1;
        }

        generator_init(&gen, json);
        gen.lexer.read_from_file = true;
    } else {
        gen.lexer.read_from_file = false;
        generator_init(&gen, next_arg);
    }

    int status = 1;
    if (generate_schema(&gen)) {
        status = 0;
        printf("%.*s", gen.sb.count, gen.sb.items);
    } else {
        Token reason = gen.lexer.last_token;
        fprintf(stderr, "%s", error_str(reason));
    }

    generator_denit(&gen);
    return status;
}