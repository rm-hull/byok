#ifndef __READLINE_H
#define __READLINE_H

#include <stddef.h>

#include <kernel/tty.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *(*fn)(char *text, int state, void *free_vars);
    void *free_vars;
} complete_t;

extern void rl_yank(char *yank[], char *text, int len, int sz);
extern uint16_t rl_prev_word(char *buf, uint16_t index);
extern uint16_t rl_next_word(char *buf, uint16_t index);
extern uint16_t rl_token_start(char *buf, uint16_t index);
extern uint16_t rl_token_end(char *buf, uint16_t index, uint16_t sz);
extern int rl_replace_word(char *buf, char *orig_text, char *new_text, uint16_t index, uint16_t sz);
extern char *readline(char *buf, uint16_t sz, char **history, complete_t *completer, colorize_t *colorizer);

#ifdef __cplusplus
}
#endif

#endif
