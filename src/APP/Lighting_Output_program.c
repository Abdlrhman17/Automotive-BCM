/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Lighting_Output_interface.h"
#include "VehicleManager.h"
#include "Blinker_StateMachine_interface.h"
#include "DIO_interface.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// Timer for blinking period
#define BLINK_PERIOD_MS      500    // Blink every 500ms (on 500ms, off 500ms)

// Blinkers Pins
#define FRONT_LEFT_BLINKER_PIN     PINA2
#define FRONT_RIGHT_BLINKER_PIN    PINA3
#define REAR_LEFT_BLINKER_PIN      PINA4
#define REAR_RIGHT_BLINKER_PIN     PINA5


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Counter to monitor blinking period
static u16 blink_counter = 0;

// state of blinkers output
static u8 blink_output_state = 0;  // 0=OFF, 1=ON (for toggling)


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Lighting_Output_Init(void)
{
	// Turn off all blinkers
	DIO_WritePin(FRONT_RIGHT_BLINKER_PIN,LOW);
	DIO_WritePin(REAR_RIGHT_BLINKER_PIN,LOW);
	DIO_WritePin(FRONT_LEFT_BLINKER_PIN,LOW);
	DIO_WritePin(REAR_LEFT_BLINKER_PIN,LOW);
	
	blink_counter = 0;
	blink_output_state = 0;
	
	TRACE_INFO(TRACE_BLINKER,"Blinkers Output Initialized");
}

void Lighting_Output_Update(void)
{
	blinkers_state_t Local_BlinkersState = Blinker_GetState();
	
	if(Local_BlinkersState != BLINKERS_OFF)
	{
		blink_counter++;
		
		if(blink_counter >= BLINK_PERIOD_MS)	//Blinking Timer reached
		{
			blink_counter = 0;
			blink_output_state = !blink_output_state;	//Toggle blinker
			
			switch(Local_BlinkersState)		// Drive output
			{
				case BLINKERS_RIGHT:
				DIO_WritePin(FRONT_RIGHT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(REAR_RIGHT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(FRONT_LEFT_BLINKER_PIN,LOW);
				DIO_WritePin(REAR_LEFT_BLINKER_PIN,LOW);
				break;
				
				case BLINKERS_LEFT:
				DIO_WritePin(FRONT_LEFT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(REAR_LEFT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(FRONT_RIGHT_BLINKER_PIN,LOW);
				DIO_WritePin(REAR_RIGHT_BLINKER_PIN,LOW);
				break;
				
				case BLINKERS_HAZARD:
				DIO_WritePin(FRONT_RIGHT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(REAR_RIGHT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(FRONT_LEFT_BLINKER_PIN,blink_output_state);
				DIO_WritePin(REAR_LEFT_BLINKER_PIN,blink_output_state);
				break;
			}
		}
	}
	else		// Blinkers off
	{
		blink_output_state = 0;
		blink_counter = 0;
		
		DIO_WritePin(FRONT_LEFT_BLINKER_PIN,LOW);
		DIO_WritePin(REAR_LEFT_BLINKER_PIN,LOW);
		DIO_WritePin(FRONT_RIGHT_BLINKER_PIN,LOW);
		DIO_WritePin(REAR_RIGHT_BLINKER_PIN,LOW);
	}
}
