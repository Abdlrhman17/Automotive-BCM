#ifndef EVENTQUEUE_INTERFACE_H_
#define EVENTQUEUE_INTERFACE_H_

#include "Events.h"
#include "StdTypes.h"

#define EVENT_QUEUE_SIZE		15

typedef struct
{
	ecu_event_t EventsArr[EVENT_QUEUE_SIZE];
	s8 Front;
	s8 Rear;
}Events_Queue_t;

void EVENTQUEUE_voidInit(Events_Queue_t* Copy_pvQueue);

u8 EVENTQUEUE_u8isEmpty(Events_Queue_t* Copy_pvQueue);

u8 EVENTQUEUE_u8isFull(Events_Queue_t* Copy_pvQueue);

u8 EVENTQUEUE_u8enQueue(Events_Queue_t* Copy_pvQueue, ecu_event_t Copy_u8Event);

u8 EVENTQUEUE_u8deQueue(Events_Queue_t* Copy_pvQueue, ecu_event_t* Copy_pvEvent);

#endif /* EVENTQUEUE_INTERFACE_H_ */