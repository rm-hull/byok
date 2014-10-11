#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

typedef struct {
    volatile unsigned int slock;
} spinlock_t;

#define spin_lock_init(x)     do { *(x) = (spinlock_t) { 1 }; } while (0)

#define barrier()             __asm__ __volatile__("": : :"memory")
#define spin_is_locked(x)     (*(volatile signed char *)(&(x)->slock) <= 0)
#define spin_unlock_wait(x)   do { barrier(); } while(spin_is_locked(x))

static inline void spin_lock(spinlock_t *lock)
{
    __asm__ volatile (
        "1: lock; decb %0   \n"
        "   jns 3f          \n"
        "2: rep             \n"
        "   nop             \n"
        "   cmpb $0, %0     \n"
        "   jle 2b          \n"
        "   jmp 1b          \n"
        "3:                 \n"
        :"=m" (lock->slock) : : "memory");
}

static inline void spin_unlock(spinlock_t *lock)
{
    __asm__ volatile (
        "movb $1, %0"
        :"=m" (lock->slock) : : "memory");
}

#endif
