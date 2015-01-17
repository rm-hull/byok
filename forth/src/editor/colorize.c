#include <string.h>
#include <stdlib.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>

#include <editor/colorize.h>

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

int is_literal(char *token, char *line, int index, context_t *ctx)
{
    // check if is a number (in current base) or if in quotes
    // 'in quotes' == odd-number of dbl-quotes before index

    int num;
    if (parsenum(token, &num, ctx->base))
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
        return is_set(entry, FLAG_USER_DEFINED) ? 1 : 2;

    return 0;
}

uint8_t colorize(char *token, char *line, int index, context_t *ctx)
{
    if (strlen(token) == 0) // empty?
        return COLOR_DEFAULT;

    if (is_comment(token, line, index))
        return COLOR_COMMENT;

    if (is_defn(token, line, index))
        return COLOR_DEFN;

    if (is_literal(token, line, index, ctx))
        return COLOR_LITERAL;

    switch (is_dict_word(token, ctx))
    {
        case 1:  return COLOR_USER_DICT_WORD;
        case 2:  return COLOR_SYS_DICT_WORD;
    }

    return COLOR_DEFAULT;
}
