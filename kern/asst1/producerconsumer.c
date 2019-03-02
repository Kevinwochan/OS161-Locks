/* This file will contain your solution. Modify it as you wish. */
#include <types.h>
#include <lib.h>
#include <synch.h>
#include "producerconsumer_driver.h"

/* Declare any variables you need here to keep track of and
   synchronise your bounded. A sample declaration of a buffer is shown
   below. It is an array of pointers to items.
   
   You can change this if you choose another implementation. 
   However, you should not have a buffer bigger than BUFFER_SIZE 
*/

data_item_t * item_buffer[BUFFER_SIZE];
struct semaphore *empty, *full, *mutex;
volatile int empty_slot;


/* consumer_receive() is called by a consumer to request more data. It
   should block on a sync primitive if no data is available in your
   buffer. */

data_item_t * consumer_receive(void)
{
        data_item_t * item = NULL;

        P(full);        // decrement the amount of full slots
        P(mutex);       // BEGIN critical region, accessing item buffer array
        item = item_buffer[--empty_slot];
        V(mutex);       // END Critical region
        V(empty);       // increment the amount of empty slots

        return item;
}

/* procucer_send() is called by a producer to store data in your
   bounded buffer. */

void producer_send(data_item_t *item)
{
        P(empty);       // decrement the amount of empty slots, sleep if empty
        P(mutex);       // BEGIN critical region, accessing item buffer array
        item_buffer[empty_slot++] = item;
        V(mutex);       // END Critical region
        V(full);        // increment the amount of full slots and wake up consumer
}




/* Perform any initialisation (e.g. of global data) you need
   here. Note: You can panic if any allocation fails during setup */

void producerconsumer_startup(void)
{
        empty_slot = 0;
        mutex = sem_create("mutex", 1);
        empty = sem_create("empty", BUFFER_SIZE);
        full = sem_create("full", 0);

        if (!mutex || !empty || !full){
                panic("run_producerconsumer: couldn't create semaphores written for assignment\n");
        }
}

/* Perform any clean-up you need here */
void producerconsumer_shutdown(void)
{
        sem_destroy(mutex);
        sem_destroy(empty);
        sem_destroy(full);
}

