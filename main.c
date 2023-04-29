#include "stm32f746xx.h"
#include <stdint.h>

void SysCall(void);

#define STACK_SIZE 256

typedef struct {
  unsigned int data; //данные
  unsigned int stack[STACK_SIZE];
} test_t;


volatile test_t *sched_current_task;
volatile test_t *sched_next_task;
volatile test_t *last_task;
volatile test_t *running_task;

volatile test_t task_A;
volatile test_t task_B;
volatile test_t task_C;

int cntA;
int cntB;
int cntC;

void Task_A(void) {
  while(1) {
    cntA++;
    SysCall();
  }
}

void Task_B(void) {
  while(1) {
    cntB++;
    SysCall();
  }
}

void Task_C(void) {
  while(1) {
    cntC++;
    SysCall();
  }
}

void ChangeRunningTask(void) {
    running_task = sched_next_task;

    tast_t *tmp1 = sched_current_task;
    tast_t *tmp2 = sched_next_task;
    tast_t *tmp3 = last_task;

    sched_current_task = tmp2;
    sched_next_task = tmp3 ;
    last_task = tmp1;

}



int main(void) {

  task_A.stack[STACK_SIZE-1] = 0x01000000UL;
  task_B.stack[STACK_SIZE-1] = 0x01000000UL;
  task_C.stack[STACK_SIZE-1] = 0x01000000UL;

  task_A.stack[STACK_SIZE-2] = (unsigned int)Task_A | 1;
  task_B.stack[STACK_SIZE-2] = (unsigned int)Task_B | 1;
  task_C.stack[STACK_SIZE-2] = (unsigned int)Task_C | 1;

  task_A.data = &task_A.stack[STACK_SIZE-16];
  task_B.data = &task_B.stack[STACK_SIZE-16];
  task_C.data = &task_C.stack[STACK_SIZE-16];

  sched_next_task = &task_B;
  sched_current_task = &task_A;
  last_task = &task_C;


  __NVIC_EnableIRQ(SVCall_IRQn);
  __set_PSP((uint32_t)(&(task_A.stack[STACK_SIZE-16])));
  __set_CONTROL(0x03);

  Task_A();

  while(1);

}
