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
struct semaphore* philosopherSems[NUM_PHILOSOPHERS];
struct semaphore *mutex;
static int philosopherStates[NUM_PHILOSOPHERS];


void test(unsigned long phil_num);
void test(unsigned long phil_num)
{
    //kprintf("%lu testing for forks\n", phil_num);
    if (philosopherStates[phil_num] == HUNGRY
        && philosopherStates[(phil_num-1) % NUM_PHILOSOPHERS] != EATING
        && philosopherStates[(phil_num+1) % NUM_PHILOSOPHERS] != EATING
        ){
            philosopherStates[phil_num] = EATING;
            V(philosopherSems[phil_num]);
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
    //kprintf("%lu: taking forks\n", phil_num);
    P(mutex);
    philosopherStates[phil_num] = HUNGRY;
    test(phil_num);
    V(mutex);
    P(philosopherSems[phil_num]);
}


/*
 * Put forks releases the mutually exclusive access to the
 * philosophers forks.
 */

void put_forks(unsigned long phil_num)
{
    //kprintf("%lu: putting forks down\n",phil_num);
    P(mutex);
    philosopherStates[phil_num] = THNIKING;
    test((phil_num-1) % NUM_PHILOSOPHERS);
    test((phil_num+1) % NUM_PHILOSOPHERS);
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
        philosopherSems[i] = sem_create("philosopher_sem_", 1);
        if (!philosopherSems[i]){
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
        sem_destroy(philosopherSems[i]);
    }
    sem_destroy(mutex);
}
