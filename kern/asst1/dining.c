#include "opt-synchprobs.h"
#include <types.h>  /* required by lib.h */
#include <lib.h>    /* for kprintf */
#include <synch.h>  /* for P(), V(), sem_* */
#include <thread.h> /* for thread_fork() */
#include <test.h>

#include "dining_driver.h"

/*
 * Declare any data structures you might need to synchronise 
 * your forks here.
 */
enum {
        THNIKING = 0,
        HUNGRY = 1,
        EATING = 2,
};
struct semaphore* forkSemaphores[NUM_PHILOSOPHERS];
struct semaphore *mutex;
static int philosopherStates[NUM_PHILOSOPHERS];


void test(unsigned long phil_num);
void test(unsigned long phil_num)
{
    if (philosopherStates[phil_num] == HUNGRY
        && philosopherStates[(phil_num-1) % NUM_PHILOSOPHERS] != EATING
        && philosopherStates[(phil_num+1) % NUM_PHILOSOPHERS] != EATING
        ){
            philosopherStates[phil_num] = EATING;   /* adjust state of the philo */
            V(forkSemaphores[phil_num]);            /* release the philo's fork */
    }
}


/*
 * Take forks ensures mutually exclusive access to two forks
 * associated with the philosopher.
 * 
 * The left fork number = phil_num
 * The right fork number = (phil_num + 1) % NUM_PHILOSPHERS
 */

void take_forks(unsigned long phil_num)
{
    P(mutex);                                   /* queue critical region */

    philosopherStates[phil_num] = HUNGRY;       /* sets the philosopher to hungry */
    test(phil_num);                             /* checks if it can start eating */

    V(mutex);
    P(forkSemaphores[phil_num]);                /* queue taking the philo's fork */
}


/*
 * Put forks releases the mutually exclusive access to the
 * philosophers forks.
 */

void put_forks(unsigned long phil_num)
{
    P(mutex);

    philosopherStates[phil_num] = THNIKING;    /* Adjust philo state back to thinking... */
    test((phil_num-1) % NUM_PHILOSOPHERS);     /* signals the left philosopher that his fork is available */
    test((phil_num+1) % NUM_PHILOSOPHERS);     /* signals the right philosopher that his fork is available */

    V(mutex);
}


/* 
 * Create gets called before the philosopher threads get started.
 * Insert any initialisation code you require here.
 */

void create_forks()
{
    mutex = sem_create("mutex", 1);
    for (int i = 0; i<NUM_PHILOSOPHERS; i++){
        forkSemaphores[i] = sem_create("fork_sem_", 1);
        if (!forkSemaphores[i]){
            panic ("Dining Philosophers: %d sem create failed", i);
        }
    }

}


/*
 * Destroy gets called when the system is shutting down.
 * You should clean up whatever you allocated in create_forks()
 */

void destroy_forks()
{
    for (int i = 0; i<NUM_PHILOSOPHERS; i++){
        sem_destroy(forkSemaphores[i]);
    }
    sem_destroy(mutex);
}
