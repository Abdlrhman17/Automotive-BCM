#ifndef EVENTDISPATCHER_INTERFACE_H_
#define EVENTDISPATCHER_INTERFACE_H_

/**
 * Dequeues from the GlobalEventQueue & filters the event to the diff state machines
 */
void EventDispatcher_ProcessEvents(void);

#endif /* EVENTDISPATCHER_INTERFACE_H_ */