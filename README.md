# OS161-Locks

Coding Assignment
We know: you've been itching to get to the coding. Well, you've finally arrived!

This is the assessable component of this assignment.

The following problems will give you the opportunity to write some fairly straightforward concurrent systems and get a practical understanding of how to use concurrency mechanisms to solve problems. We have provided you with basic driver code that starts a predefined number of threads that execute a predefined activity (in the form of calling functions that you must implement or modify).

Note: In this assignment, you are restricted to the lock, semaphore, and condition variable primitives provided in OS/161. The use of primitives such as thread_yield, atomic ops, and the like are discouraged. Moreover, they usually result in a poor solution involving busy waiting.

Note: In some instances, the comments within the code also form part of the specification and give guidance as to what is required.

Check that you have specified a seed to use in the random number generator by examining your sys161.conf file, and run your tests using Sys/161 command line args. It is much easier to debug initial problems when the sequence of execution and context switches is reproducible.

When you configure your kernel for ASST1, the driver code and extra menu options for executing your solutions are automatically compiled in.

Part 1: Concurrent Mathematics Problem
For the first problem, we ask you to solve a very simple mutual exclusion problem. The code in kern/asst1/math.c counts from 0 to 10000 by starting several threads that increment a common counter.

You will notice that as supplied, the code operates incorrectly and produces results like 345 + 1 = 352. An incorrect run is shown below in red.

Once the count of 10000 is reached, each thread signals the main thread that it is finished and then exits. Once all adder() threads exit, the main (math()) thread cleans up and exits.

OS/161 kernel: 1a
Starting 10 adder threads
In thread 6, 777 + 1 == 782?
In thread 1, 1053 + 1 == 783?
In thread 5, 782 + 1 == 1073?
In thread 0, 1040 + 1 == 784?
In thread 8, 1443 + 1 == 1455?
In thread 4, 1511 + 1 == 1522?
In thread 9, 1562 + 1 == 1568?
In thread 2, 1657 + 1 == 1666?
In thread 7, 1665 + 1 == 1667?
In thread 6, 4341 + 1 == 4344?
In thread 3, 6499 + 1 == 6505?
In thread 1, 7877 + 1 == 7894?
In thread 5, 7893 + 1 == 7895?
In thread 0, 9783 + 1 == 9834?
Adder threads performed 10000 adds
Adder 0 performed 1924 increments.
Adder 1 performed 1403 increments.
Adder 2 performed 95 increments.
Adder 3 performed 4822 increments.
Adder 4 performed 658 increments.
Adder 5 performed 264 increments.
Adder 6 performed 219 increments.
Adder 7 performed 9 increments.
Adder 8 performed 590 increments.
Adder 9 performed 92 increments.
The adders performed 10076 increments overall (expected 10000)
Your Task
Your task is to modify math.c by placing synchronisation primitives appropriately such that incrementing the counter works correctly. The statistics printed should also be consistent with the overall count.

Note that the number of increments each thread performs is dependent on scheduling and hence will vary. However, the total should equal the final count.

To test your solution, use the "1a" menu choice. Sample output from a correct solution in included below.

OS/161 kernel: 1a
Starting 10 adder threads
Adder threads performed 10000 adds
Adder 0 performed 919 increments.
Adder 1 performed 1037 increments.
Adder 2 performed 867 increments.
Adder 3 performed 1087 increments.
Adder 4 performed 1059 increments.
Adder 5 performed 905 increments.
Adder 6 performed 1132 increments.
Adder 7 performed 997 increments.
Adder 8 performed 958 increments.
Adder 9 performed 1039 increments.
The adders performed 10000 increments overall
Part 2: Simple Deadlock
This task involves modifying a simple example such that the example no longer deadlocks and is able to finish. The example is in twolocks.c.

In the example, bill() and ben() are two threads that both need to hold two locks at various times to make progress: lock_a and lock_b. While holding both locks, bill() and ben() calls "holds_lockX" that just consumes some CPU. The way the current code is written, the code deadlocks and trigger's OS/161's deadlock detection code, as shown below.

OS/161 kernel: 1b
Locking frenzy starting up
Hi, I'm Bill
Hi, I'm Ben
hangman: Detected lock cycle!
hangman: in ben thread (0x80031ed8);
hangman: waiting for lock_a (0x80032d04), but:
   lockable lock_a (0x80032d04)
   held by actor bill thread (0x80031f58)
   waiting for lockable lock_b (0x80032cc4)
   held by actor ben thread (0x80031ed8)
panic: Deadlock.
sys161: trace: software-requested debugger stop
sys161: Waiting for debugger connection...
You task is to modify the existing code such that:

The example runs to completion as shown below.
The solution still involves the locks being held at the three different points in the code as indicated by the "holds_lockX" function calls.
OS/161 kernel: 1b
Locking frenzy starting up
Hi, I'm Bill
Hi, I'm Ben
Ben says 'bye'
Bill says 'bye'
Locking frenzy finished
Part 3: Bounded-buffer producer/consumer problem
Your next task in this assignment is to implement a solution to a producer/consumer problem. In this producer/consumer problem one or more producer threads allocate data structures and copy the pointers into a fixed-sized buffer, while one or more consumer threads retrieve those pointers and de-allocate the data structures.

The code in kern/asst1/producerconsumer_driver.c starts up a number of producer and consumer threads. The producer threads attempt to send pointers to the data structures to the consumer threads by calling the producer_send() function with a pointer to the data structure. In turn, the consumer threads attempt to receive pointers to the allocated data structure from the producer threads by calling consumer_receive(). Unfortunately, these functions are currently unimplemented. Your job is to implement them.

Here's what you will see before you have implemented any code:

OS/161 kernel: 1c
run_producerconsumer: starting up
Consumer started
panic: Assertion failed: item != NULL, at ../../asst1/producerconsumer_driver.c:108 (consumer_thread)
And here's what you will see with a (possibly partially) correct solution:

OS/161 kernel: 1c
run_producerconsumer: starting up
Consumer started
Waiting for producer threads to exit...
Producer started
Consumer started
Consumer started
Producer started
Consumer started
Consumer started
Producer finished
Producer finished
All producer threads have exited.
Consumer finished normally
Consumer finished normally
Consumer finished normally
Consumer finished normally
Consumer finished normally
The files:
producerconsumer_driver.c: Starts the producer/consumer simulation by creating appropriate producer and consumer threads that will call producer_send() and consumer_receive(). You are welcome to (in fact, you are encouraged to) modify this simulation when testing your implementation, but remember that it will be overwritten when your solution is tested.
producerconsumer_driver.h: Contains prototypes for the functions in producerconsumer.c, as well as the description of the data structure that is passed from producer to consumer (uninterestingly named data_item_t). This file will also be overwritten when your solution is tested.
producerconsumer.c: Contains your implementation of producer_send() and consumer_receive(). It also contains the functions producerconsumer_startup() and producerconsumer_shutdown(), which you can implement to initialise your buffer structure and any synchronisation primitives you may need.
How to implement your solution
You must implement a data structure representing a buffer capable of holding at least BUFFER_SIZE data_item_t pointers. This means that calling producer_send() BUFFER_SIZE times should not block (or overwrite existing items, of course), but calling producer_send one more time should block, until an item has been removed from the buffer using consumer_receive(). A simple way to implement this data structure is to use an array of pointers as provided, though you will of course have to use appropriate synchronisation primitives to ensure that concurrent access is handled safely.

Your data structure should function as a circular buffer with first-in, first-out semantics.

Part 4: Dining Philosophers
No OS course is complete without having to solve the dining philosopher's problem. Now is your chance!

System Details
The system consists of the following files:

dining_driver.h which defines some constants and declares the prototypes of the functions that you will implement. We will replace this file in testing, potentially changing some of the constants.
dining_driver.c contains the code for the philosophers that eat and think, and the setup code that starts a number of philosophers as threads. Again, we will replace this file in testing, potentially inserting some test code, but preserving the behaviour of the philosophers.
dining.c contains an empty template for you to create your implementation of the functions required to synchronise access to the forks. This file is the one you should modify.
The structure of the system follows the typical dining philosopher structure where each philosopher thread does the following in a loop that eventually finishes.

think();
take_forks(); 
eat();   
put_forks();  
The supplied code has the addition of a fork-use count that is incremented during each eat(). If you have correctly synchronised the problem, eat() should have mutually exclusive access to the count for the left and right fork, and thus the count should accurately reflect the number of times the fork has been used. The supplied code is not synchronised, thus count is inaccurate and varies to race conditions on updates, as shown below.

Starting 5 philosopher threads
Philosopher 3 started
Philosopher 0 started
Philosopher 1 started
Philosopher 2 started
Philosopher 4 started
Philosopher 3 finished
Philosopher 0 finished
Philosopher 1 finished
Philosopher 2 finished
Philosopher 4 finished
Fork 0 used 1940 times.
Fork 1 used 1893 times.
Fork 2 used 1866 times.
Fork 3 used 2000 times.
Fork 4 used 2000 times.
Your Task
You task is to synchronise the philosophers such that they have mutually exclusive access to their forks. You do this by implementing the functions in dining.c, including declaring any types for data structures that you need.

Better solutions allow multiple philosophers to eat in parallel when they don't share forks in common.

You can't assume the constants in dining_driver.h remain the same in our testing. You can assume there will be at least 2 philosophers configured in the system.

To test your solution, use sys161 kernel "1d;q". Sample output from a potential working solution is included below.

OS/161 kernel: 1d
Starting 5 philosopher threads
Philosopher 3 started
Philosopher 4 started
Philosopher 2 started
Philosopher 1 started
Philosopher 0 started
Philosopher 3 finished
Philosopher 2 finished
Philosopher 1 finished
Philosopher 4 finished
Philosopher 0 finished
Fork 0 used 2000 times.
Fork 1 used 2000 times.
Fork 2 used 2000 times.
Fork 3 used 2000 times.
Fork 4 used 2000 times.
