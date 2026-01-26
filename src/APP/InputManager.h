#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

typedef enum
{
    INPUT_IGNITION_SWITCH = 0,
    INPUT_DRIVER_DOOR_SWITCH,
    INPUT_LOCK_BUTTON,
    INPUT_BLINKER_STALK,
    INPUT_WIPER_STALK,
    INPUT_SPEED_SIGNAL,
    INPUT_COUNT
}input_signal_t;

void InputManager_Init(void);

void InputManager_Update(void);

#endif /* INPUTMANAGER_H_ */