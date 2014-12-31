#include <string.h>
#include <stdlib.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>

#include <editor/colorize.h>

int is_empty(char *token)
{
    return strlen(token) == 0;
}

int is_comment(char *token, char *line, int index)
{
    if (memcmp(token, "\\", 2) == 0 ||
        memcmp(token, "(", 2) == 0 ||
        memcmp(token, ".(", 3) == 0 ||
        memcmp(token, ")", 2) == 0)
        return 1;

    // Backslash trumps parens
    for (int i = index - 1; i >= 0; i--)
    {
        if (memcmp(&line[i], "\\ ", 2) == 0)
            return 1;
    }

    for (int i = index - 1; i >= 0; i--)
    {
        if (line[i] == ')')
            return 0;

        if (memcmp(&line[i], "( ", 2) == 0)
            return 1;
    }
    return 0;
}

int is_defn(char *token, char *line, int index)
{
    if (memcmp(token, ":", 2) == 0)
        return 1;

    if (index >= 2 && memcmp(&line[index - 2], ": ", 2) == 0)
        return 1;

    return 0;
}

int is_literal(char *token, char *line, int index)
{
    // check if is a base-10 number or if in quotes
    // 'in quotes' == odd-number of dbl-quotes before index

    int num;
    if (parsenum(token, &num, 10))
    {
        return 1;
    }

    if (memcmp(token, "\"", 2) == 0 || memcmp(token, ".\"", 3) == 0)
        return 1;

    int count = 0;
    for (int i = 0; i < index; i++)
    {
        if (line[i] == '\"')
            count++;
    }

    return count % 2 == 1;
}

int is_dict_word(char *token, context_t *ctx)
{
    entry_t *entry;
    if (find_entry(ctx->exe_tok, token, &entry) == 0)
        return 1;

    return 0;
}

uint8_t colorize(char *token, char *line, int index, context_t *ctx)
{
    if (is_empty(token))
        return COLOR_DEFAULT;

    if (is_comment(token, line, index))
        return COLOR_COMMENT;

    if (is_defn(token, line, index))
        return COLOR_DEFN;

    if (is_literal(token, line, index))
        return COLOR_LITERAL;

    if (is_dict_word(token, ctx))
        return COLOR_DICT_WORD;

    return COLOR_DEFAULT;
}
