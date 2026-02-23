#ifndef LIGHTING_OUTPUT_INTERFACE_H_
#define LIGHTING_OUTPUT_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize Lighting Output
 */
void Lighting_Output_Init(void);

/**
 * Update blinker lamp outputs
 * Generate blink pattern based on state
 * Called periodically
 */
void Lighting_Output_Update(void);

#endif /* LIGHTING_OUTPUT_INTERFACE_H_ */