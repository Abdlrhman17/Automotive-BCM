#ifndef WIPER_OUTPUT_INTERFACE_H_
#define WIPER_OUTPUT_INTERFACE_H_
/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize Wiper Output
 * Set up PWM for wiper motor
 */
void Wiper_Output_Init(void);

/**
 * Update wiper motor outputs
 * Drive motor based on wiper state
 * Called periodically
 */
void Wiper_Output_Update(void);

#endif /* WIPER_OUTPUT_INTERFACE_H_ */