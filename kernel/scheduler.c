#include "types.h"
#include "riscv.h"
#include "proc.h"
#include "trap.h"
#include "scheduler.h"

extern struct cpu cpu;
extern struct proc proc[];

void swtch(struct context *old, struct context *new);

void
scheduler(void)
{
    struct proc *p;
    struct cpu *c = &cpu;

    c->proc = 0;

    for(;;) {
        intr_on();

        for(p = proc; p < &proc[NPROC]; p++) {
            if(p->state == RUNNABLE) {
                p->state = RUNNING;
                c->proc = p;
                swtch(&c->context, &p->context);
                c->proc = 0;
            }
        }
    }
}

void
yield(void)
{
    struct proc *p = cpu.proc;
    p->state = RUNNABLE;
    swtch(&p->context, &cpu.context);
}