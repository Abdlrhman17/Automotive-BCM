#ifndef STATEOWNERSHIP_H_
#define STATEOWNERSHIP_H_

/* ignition_state_t
 * Owner: IgnitionSM
 * Readers: Lighting, Wipers, Blinkers
 * Change: Event-driven
 */

/* door_state_t
 * Owner: DoorSM
 * Readers: Lighting, VehicleMovement, Lock
 * Change: Mechanical-driven
 */

/* Lock_state_t
 * Owner: LockSM
 * Readers: VehicleMovement, Door
 * Change: Event-driven
 */

/* blinker_state_t
 * Owner: BlinkerSM
 * Readers: 
 * Change: Event-driven
 */

/* wiper_control_state_t
 * Owner: WiperControlSM
 * Readers: 
 * Change: Event-driven
 */

#endif /* STATEOWNERSHIP_H_ */