#ifndef _ACTIONS_H
#define _ACTIONS_H 1

#include <collections/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

extern hashtable_t *actions_init();
extern editor_t *process_key(hashtable_t *htbl, editor_t *ed);

#ifdef __cplusplus
}
#endif

#endif
