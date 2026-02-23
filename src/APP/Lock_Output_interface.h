#ifndef LOCK_OUTPUT_INTERFACE_H_
#define LOCK_OUTPUT_INTERFACE_H_
/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize Lock Output
 */
void Lock_Output_Init(void);

/**
 * Update lock actuator outputs
 * Read lock state and drive actuators
 * Called periodically
 */
void Lock_Output_Update(void);

#endif /* LOCK_OUTPUT_INTERFACE_H_ */