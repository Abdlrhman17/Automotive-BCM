#include "Scheduler_interface.h"
#include "StdTypes.h"

/*Task attributes*/
typedef struct
{
	u16 Periodicity;
	void (*TaskFunc)(void);
	u8 State;
	u16 FirstDelay;
}Task_t;

/*Tasks Array*/
static Task_t SystemTasks[TASK_NUM] = {{NULL}};


Task_feedback SCHEDULER_taskfeedbackCreateTask(u8 Copy_u8Priority, void (*Copy_pvTaskFunc)(void), u16 Copy_u16Periodicity, u16 Copy_u16FirstDelay)
{
	u8 Local_u8ErrorState = OK;
	
	if(!Copy_u8Priority > TASK_NUM)
	{
		if(Copy_pvTaskFunc != NULL)
		{
			if(SystemTasks[Copy_u8Priority].TaskFunc == NULL)
			{
				SystemTasks[Copy_u8Priority].Periodicity = Copy_u16Periodicity;
				SystemTasks[Copy_u8Priority].TaskFunc = Copy_pvTaskFunc;
				SystemTasks[Copy_u8Priority].State = READY;
				SystemTasks[Copy_u8Priority].FirstDelay = Copy_u16FirstDelay;
			}
			else
			{
				/*Task Priority isn't free*/
				Local_u8ErrorState = NOK_PRIORITY_OCC;
			}
		}
		else
		{
			Local_u8ErrorState = NOK_NULL_PTR;
		}
	}
	else
	{
		/*Priority out of tasks array bounds*/
		Local_u8ErrorState = NOK_PRIORITY_INVALID;
	}
	
	return Local_u8ErrorState;
}


static void SCHEDULER_voidStart(void)
{
	u8 Local_u8TaskCounter;
	
	/*Loop on the tasks*/
	for(Local_u8TaskCounter = 0; Local_u8TaskCounter < TASK_NUM; ++Local_u8TaskCounter)
	{
		if(SystemTasks[Local_u8TaskCounter].FirstDelay == 0 && SystemTasks[Local_u8TaskCounter].State == READY)
		{
			if(SystemTasks[Local_u8TaskCounter].TaskFunc != NULL)
			{
				SystemTasks[Local_u8TaskCounter].TaskFunc();
			}
			/*Make the First delay the task's own counter*/
			SystemTasks[Local_u8TaskCounter].FirstDelay = SystemTasks[Local_u8TaskCounter].Periodicity - 1;
		}
		else
		{
			/*If it isn't the task turn*/
			SystemTasks[Local_u8TaskCounter].FirstDelay--;
		}
	}
}


void SCHEDULER_voidSuspendTask(u8 Copy_u8Priority)
{
	SystemTasks[Copy_u8Priority].State = SUSPENDED;
}


void SCHEDULER_voidResumeTask(u8 Copy_u8Priority)
{
	SystemTasks[Copy_u8Priority].State = READY;
}


void SCHEDULER_voidDeleteTask(u8 Copy_u8Priority)
{
	SystemTasks[Copy_u8Priority].TaskFunc = NULL;
}
