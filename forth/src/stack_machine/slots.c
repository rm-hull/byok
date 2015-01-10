#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/slots.h>
#include <collections/hashtable.h>

#define NUM_SLOTS 50
#define SLOT_SIZ (64 * 16)

#define SLOT_DIRTY 1<<0

#define is_dirty(slot) ((slot)->flags & SLOT_DIRTY)

typedef struct
{
    int flags;
    char buffer[SLOT_SIZ];
} slot_t;

slot_t *slots[NUM_SLOTS];

void slot_flush(int n)
{
    if (n < 0 || n >= NUM_SLOTS)
        return;

    slot_t *slot = slots[n];
    if (slot != NULL && is_dirty(slot))
    {
        // TODO: Save to disk & reset dirty-flag
        slot->flags &= ~SLOT_DIRTY;
    }
}

const char *EXAMPLE_SLOT =
    "( Large letter \"F\")\n: STAR   42 emit ;\n: STARS  ( #)  0 do  star  loop ;\n: MARGIN   cr  30 spaces ;\n: BLIP   margin star ;\n: BAR   margin 5 stars ;\n: F   bar blip bar blip blip  cr ;\n\n\nF\n";

slot_t *slot_reload(int n)
{
    assert(n >= 0 && n < NUM_SLOTS);  // invalid slot

    slot_t *slot = slots[n];
    if (slot == NULL)
        slots[n] = slot = calloc(0, sizeof(slot_t));

    assert(slot != NULL);
    memset(slot->buffer, 0, SLOT_SIZ);
    slot->flags &= ~SLOT_DIRTY;

    // TODO: Load from disk - in the meantime: wing it
    if (n == 12) {
        memcpy(slot->buffer, EXAMPLE_SLOT, strlen(EXAMPLE_SLOT) + 1);
    }

    return slot;
}

char *slot_buffer(int n)
{
    if (n < 0 || n >= NUM_SLOTS)
        return NULL;  // invalid slot

    slot_t *slot = slots[n];
    if (slot == NULL)
        slot = slot_reload(n);

    return slot->buffer;
}

void slot_mark_dirty(int n)
{
    if (n < 0 || n >= NUM_SLOTS)
        return;

    slot_t *slot = slots[n];
    if (slot != NULL)
        slot->flags |= SLOT_DIRTY;
}
