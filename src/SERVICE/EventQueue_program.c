#include "EventQueue_interface.h"


void EVENTQUEUE_voidInit(Events_Queue_t* Copy_pvQueue)
{
	Copy_pvQueue->Front = -1;
	Copy_pvQueue->Rear = -1;
}

u8 EVENTQUEUE_u8isEmpty(Events_Queue_t* Copy_pvQueue)
{
	if(Copy_pvQueue->Front == -1)
	{
		return TRUE;
	}
	return FALSE;
}

u8 EVENTQUEUE_u8isFull(Events_Queue_t* Copy_pvQueue)
{
	if(((Copy_pvQueue->Rear + 1) % EVENT_QUEUE_SIZE) == Copy_pvQueue-> Front)
	{
		return TRUE;
	}
	return FALSE;
}

u8 EVENTQUEUE_u8enQueue(Events_Queue_t* Copy_pvQueue, ecu_event_t Copy_u8Event)
{
	if(EVENTQUEUE_u8isFull(Copy_pvQueue))
	{
		return NOK;
	}
	if(EVENTQUEUE_u8isEmpty(Copy_pvQueue))
	{
		Copy_pvQueue->Front = 0;
	}
	
	Copy_pvQueue->Rear = (Copy_pvQueue->Rear + 1) % EVENT_QUEUE_SIZE;
	Copy_pvQueue->EventsArr[Copy_pvQueue->Rear] = Copy_u8Event;
		
	return OK;
}

u8 EVENTQUEUE_u8deQueue(Events_Queue_t* Copy_pvQueue, ecu_event_t* Copy_pvEvent)
{
	if(EVENTQUEUE_u8isEmpty(Copy_pvQueue))
	{
		return NOK;
	}
	
	*Copy_pvEvent = Copy_pvQueue->EventsArr[Copy_pvQueue->Front];
	
	if(Copy_pvQueue->Front == Copy_pvQueue->Rear)
	{
		Copy_pvQueue->Front = Copy_pvQueue->Rear = -1;
	}
	else
	{
		Copy_pvQueue->Front = (Copy_pvQueue->Front + 1)  % EVENT_QUEUE_SIZE;
	}
	return OK;
}

