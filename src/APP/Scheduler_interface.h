#ifndef SCHEDULER_INTERFACE_H_
#define SCHEDULER_INTERFACE_H_

#define TASK_NUM		3

typedef enum
{
	BLOCKED = 0,
	READY,
	SUSPENDED
	
}Task_State_t;

typedef enum
{
	OK = 0,								
	NOK_NULL_PTR,                       /* ERROR: Null pointer as a task */
	NOK_PRIORITY_OCC,					/* ERROR: Priority is taken by another task */	
	NOK_PRIORITY_INVALID,				/* ERROR: Priority entered is out of tasks array bounds */
	NOK_UNKNOWN
	
}Task_feedback;


Task_feedback SCHEDULER_taskfeedbackCreateTask(u8 Copy_u8Priority, void (*Copy_pvTaskFunc)(void), u16 Copy_u16Periodicity, u16 Copy_u16FirstDelay);

static void SCHEDULER_voidStart(void);

void SCHEDULER_voidSuspendTask(u8 Copy_u8Priority);

void SCHEDULER_voidResumeTask(u8 Copy_u8Priority);

void SCHEDULER_voidDeleteTask(u8 Copy_u8Priority);

#endif /* SCHEDULER_INTERFACE_H_ */