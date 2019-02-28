#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

#include "mythread.h"
#include "interrupt.h"

#include "queue.h"

TCB* scheduler();
void activator();
void timer_interrupt(int sig);
void disk_interrupt(int sig);

/* Array of state thread control blocks: the process allows a maximum of N threads */
static TCB t_state[N];

/* Current running thread */
static TCB* running;
static int current = 0;

// Creates three empty queues, for the 2 different priorities and for the waiting queue
static struct queue * q_low;
static struct queue * q_high;
static struct queue * q_waiting;

/* Variable indicating if the library is initialized (init == 1) or not (init == 0) */
static int init=0;

/* Thread control block for the idle thread */
static TCB idle;
static void idle_function(){
  while(1);
}

/* Initialize the thread library */
void init_mythreadlib() {
  int i;
  /* Create context for the idle thread */
  if(getcontext(&idle.run_env) == -1){
    perror("*** ERROR: getcontext in init_thread_lib");
    exit(-1);
  }
  idle.state = IDLE;
  idle.priority = SYSTEM;
  idle.function = idle_function;
  idle.run_env.uc_stack.ss_sp = (void *)(malloc(STACKSIZE));
  idle.tid = -1;
  if(idle.run_env.uc_stack.ss_sp == NULL){
    printf("*** ERROR: thread failed to get stack space\n");
    exit(-1);
  }
  idle.run_env.uc_stack.ss_size = STACKSIZE;
  idle.run_env.uc_stack.ss_flags = 0;
  idle.ticks = QUANTUM_TICKS;
  makecontext(&idle.run_env, idle_function, 1);

  t_state[0].state = INIT;
  t_state[0].priority = LOW_PRIORITY;
  t_state[0].ticks = QUANTUM_TICKS;
  if(getcontext(&t_state[0].run_env) == -1){
    perror("*** ERROR: getcontext in init_thread_lib");
    exit(5);
  }

  for(i=1; i<N; i++){
    t_state[i].state = FREE;
  }

  t_state[0].tid = 0;

  running = &t_state[0];

  /*
    First thread that runs after idle. Print the message
  */
  printf("*** THREAD READY : SET CONTEXT TO %d\n", running->tid);

  /* Initialize disk and clock interrupts */
  init_disk_interrupt();
  init_interrupt();

  /*
    Initialize our queues for low and high priorities, and waiting
  */
  q_low = queue_new();
  q_high = queue_new();
  q_waiting = queue_new();
}


/* Create and intialize a new thread with body fun_addr and one integer argument */
int mythread_create (void (*fun_addr)(),int priority)
{
  int i;

  if (!init) { init_mythreadlib(); init=1;}
  for (i=0; i<N; i++)
    if (t_state[i].state == FREE) break;
  if (i == N) return(-1);
  if(getcontext(&t_state[i].run_env) == -1){
    perror("*** ERROR: getcontext in my_thread_create");
    exit(-1);
  }
  t_state[i].state = INIT;
  t_state[i].priority = priority;
  t_state[i].function = fun_addr;
  t_state[i].ticks = QUANTUM_TICKS;
  t_state[i].run_env.uc_stack.ss_sp = (void *)(malloc(STACKSIZE));
  if(t_state[i].run_env.uc_stack.ss_sp == NULL){
    printf("*** ERROR: thread failed to get stack space\n");
    exit(-1);
  }
  t_state[i].tid = i;
  t_state[i].run_env.uc_stack.ss_size = STACKSIZE;
  t_state[i].run_env.uc_stack.ss_flags = 0;
  makecontext(&t_state[i].run_env, fun_addr, 1);


  printf("*** THREAD %d READY\n", t_state[i].tid);

  /*
      We disable the interrupts to perform an atomic action
  */
  disable_interrupt ();
  /*
      If the current thread has low priority and the new one has high,
      we should preempt the former.
  */
  if (mythread_getpriority()==LOW_PRIORITY && priority==HIGH_PRIORITY) {
      TCB* aux = running;
      printf("*** THREAD %d PREEMTED : SETCONTEXT OF %d\n", running->tid, t_state[i].tid);
      running = &t_state[i];
      current = running->tid;
      aux->ticks = QUANTUM_TICKS;
      enqueue (q_low , aux);
      swapcontext (&(aux->run_env), &(running->run_env));
  /*
      Otherwise, we enqueue the new thread in its corresponding queue,
      according to its priority.
  */
  } else if (priority==LOW_PRIORITY){
      TCB * t = & t_state [i];
      enqueue (q_low , t);
  } else if (priority==HIGH_PRIORITY){
      TCB * t = & t_state [i];
      enqueue (q_high , t);
  }
  enable_interrupt ();
  return i;
} /****** End my_thread_create() ******/

/* Read disk syscall */
int read_disk()
{
    /*
        If the data is not in cache, we should interrupt the thread
    */
    if (!data_in_page_cache()){
        printf("*** THREAD %d READ FROM DISK\n", running->tid);
        running->ticks = QUANTUM_TICKS;
        running->state = WAITING;
        /*
            We insert the thread in the waiting queue,
            and swap context to the next thread.
        */
        enqueue (q_waiting , running);
        TCB* next = scheduler();
        TCB* aux = running;
        printf("*** SWAPCONTEXT FROM %d TO %d\n", running->tid, next->tid);
        running = next;
        current = running->tid;
        swapcontext (&(aux->run_env), &(running->run_env));
    }
    return 1;
}

/* Disk interrupt  */
void disk_interrupt(int sig)
{
    disable_disk_interrupt ();

    /*
        We will only do something if we have processes waiting in the queue
    */
    if (!queue_empty(q_waiting)) {
        /*
            We will change the state of the thread to ready, and
            insert it in its corresponding queue (according to priority)
        */
        TCB * ready = dequeue ( q_waiting ) ;
        printf("*** THREAD READY %d\n", ready->tid);
        ready->state = INIT;
        if (ready->priority==HIGH_PRIORITY){
            /*
                If the current thread has low priority and the new one has high,
                we should preempt the former.
            */
            if (mythread_gettid() != -1 && mythread_getpriority() == LOW_PRIORITY){
                TCB* aux = running;
                printf("*** THREAD %d PREEMTED : SETCONTEXT OF %d\n", running->tid, ready->tid);
                running = ready;
                current = running->tid;
                aux->ticks = QUANTUM_TICKS;
                enqueue (q_low , aux);
                swapcontext (&(aux->run_env), &(running->run_env));
            /*
                Otherwise, just enqueue it in the high prior. ready threads queue
            */
            } else {
                enqueue(q_high, ready);
            }
        } else {
            enqueue(q_low, ready);
        }
        /*
            If the current thread is the idle one, we should swap
            context by calling scheduler. This function, obviously,
            will return the thread that we have just stored, because
            we only run the idle process when no other process is ready.
        */
        if(mythread_gettid() == -1){
            TCB* next = scheduler();
            TCB* aux = running;
            printf("*** THREAD READY : SET CONTEXT TO %d\n", next->tid);
            running = next;
            current = running->tid;
            swapcontext (&(aux->run_env), &(running->run_env));
        }
    }
    enable_disk_interrupt ();
}


/* Free terminated thread and exits */
void mythread_exit() {
  int tid = mythread_gettid();

  printf("*** THREAD %d FINISHED\n", tid);
  t_state[tid].state = FREE;
  free(t_state[tid].run_env.uc_stack.ss_sp);

  /*
    Find the next thread in the scheduler and activate it
  */
  TCB* next = scheduler();
  activator(next);
}

/* Sets the priority of the calling thread */
void mythread_setpriority(int priority) {
  int tid = mythread_gettid();
  t_state[tid].priority = priority;
}

/* Returns the priority of the calling thread */
int mythread_getpriority() {
  int tid = mythread_gettid();
  return t_state[tid].priority;
}


/* Get the current thread id.  */
int mythread_gettid(){
  if (!init) { init_mythreadlib(); init=1;}
  return current;
}


/* FIFO para alta prioridad, RR para baja*/
TCB* scheduler(){

  disable_interrupt ();
  /*
    We first check if there are elements in the high priority,
    queue, as they are more important
    We do not need to check that it is in INIT, because being in
    the queue implies that the thread is ready to continue execution.
  */
  if (!queue_empty(q_high)) {
      TCB * candidate = dequeue ( q_high ) ;
      current = candidate->tid;
      enable_interrupt();
      return candidate;
  }

    /*
       Otherwise, we move to the low priority queue
       We do not need to check that it is in INIT, because being in
       the queue implies that the thread is ready to continue execution.
    */
  if (!queue_empty(q_low)) {
      TCB * candidate = dequeue ( q_low ) ;
      current = candidate->tid;
      enable_interrupt();
      return candidate;
  }

  /*
    Otherwise, if all the threads are waiting, run the idle thread
  */
  if (!queue_empty(q_waiting)) {
      current = idle.tid;
      return &idle;
  }

  printf("*** FINISH\n");
  free(idle.run_env.uc_stack.ss_sp);
  free(q_low);
  free(q_high);
  free(q_waiting);
  exit(1);
}


/* Timer interrupt  */
void timer_interrupt(int sig)
{
    /*
        We disable the interrupts to perform an atomic action
    */
    disable_interrupt ();
    /*
        Round Robin is only used for low priority threads
    */
    if (mythread_gettid() != -1 && mythread_getpriority() == LOW_PRIORITY) {
        running->ticks--;
        /*
            If the number of ticks is zero, we need to swap to the next thread (Round Robin)
        */
        if (running->ticks<=0){
            running->ticks = QUANTUM_TICKS;
            running->state = INIT;
            /*
                We will enqueue the current thread and find the next one with the scheduler
            */
            enqueue (q_low , running);
            TCB* next = scheduler();
            /*
                If the current and next thread are the same, we do not need to swap
            */
            if (next != running) {
                /*
                    We update the 'running' and 'current' variables, and set the context to the next thread
                */
                TCB* aux = running;
                printf("*** SWAPCONTEXT FROM %d TO %d\n", running->tid, next->tid);
                running = next;
                current = running->tid;
                swapcontext (&(aux->run_env), &(running->run_env));
            }
        }
    }
    enable_interrupt();
}

/* Activator */
void activator(TCB* next){
    /*
        We update the 'running' and 'current' variables, and set the context to the next thread
    */
    TCB * aux = running;
    running = next;
    printf("*** THREAD %d TERMINATED : SETCONTEXT OF %d\n", aux->tid, running->tid);
    setcontext (&(next->run_env));
    printf("mythread_free: After setcontext, should never get here!!...\n");
}
