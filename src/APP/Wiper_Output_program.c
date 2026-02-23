/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Wiper_Output_interface.h"
#include "Wiper_StateMachine_interface.h"
#include "TIMERS.h"
#include "Trace.h"

/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// PWM duty cycles for different speeds
#define WIPER_SPEED_OFF           0
#define WIPER_SPEED_INTERMITTENT  64      // 25% duty cycle
#define WIPER_SPEED_LOW           128    // 50% duty cycle
#define WIPER_SPEED_HIGH          255   // 100% duty cycle

// For intermittent timing
#define INTERMITTENT_PERIOD_MS    3000  // Wipe every 3 seconds

#define WIPE_DURATION_MS		  500  // 500 MS


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Counter to monitor intermittent period
static u16 intermittent_counter = 0;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Wiper_Output_Init(void)
{
	TIMER0_Init(TIMER0_PHASECORRECT_MODE,TIMER0_SCALER_8);
	OCR0 = 0;
	intermittent_counter = 0;
	
	TRACE_INFO(TRACE_WIPER, "Wiper output initialized");
}

void Wiper_Output_Update(void)
{
	wiper_control_state_t Local_WiperState = Wiper_GetState();
	
	// Drive motor based on state
	switch(Local_WiperState)
	{
		case WIPERS_OFF:
		OCR0 = WIPER_SPEED_OFF;
		intermittent_counter = 0;
		break;
		
		case WIPERS_INTERMITTENT:		// Wipe periodically
		if(intermittent_counter < INTERMITTENT_PERIOD_MS)
		{
			intermittent_counter++;
			OCR0 = WIPER_SPEED_OFF;  // Off while waiting
		}
		else if(intermittent_counter < INTERMITTENT_PERIOD_MS + WIPE_DURATION_MS)
		{
			intermittent_counter++;
			OCR0 = WIPER_SPEED_LOW;  // On during wipe
		}
		else
		{
			intermittent_counter = 0;  // Reset for next cycle
		}
		break;
		
		case WIPERS_LOW:			// Continuous low speed
		OCR0 = WIPER_SPEED_LOW;
		break;
		
		case WIPERS_HIGH:		// Continuous high speed
		OCR0 = WIPER_SPEED_HIGH;
		break;
	}
}