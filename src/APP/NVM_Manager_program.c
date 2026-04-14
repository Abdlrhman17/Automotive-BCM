/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "NVM_Manager_interface.h"
#include "Fault_Monitor_interface.h"
#include "Trace.h"
#include "NvmStorage.h"
#include "NVM_interface.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
#define NVM_FAULT_TABLE_ADDRESS		0x0000     
#define NVM_FAULT_TABLE_INDX		0  
  
// Data validity marker
#define NVM_MAGIC_NUMBER     0xABCD   

#define NVM_DATA_SIZE        sizeof(nvm_storage_t)

//Fault indexes in NVM faults Array
#define IGNITION_FAULT_INDEX		0
#define LOCK_FAULT_INDEX			1
#define SPEED_SIGNAL_FAULT_INDEX	2


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// In-RAM copy
static nvm_storage_t nvm_buffer;  

// Is NVM data valid ?
static u8 nvm_valid = FALSE;

// Global var to How nvm deals with each Block
NVM_Job_t GLobalNvmJob;

NVM_BlockConfig_t NVM_Blocks[] =
{
	[NVM_FAULT_TABLE_INDX] = {NVM_FAULT_TABLE_ADDRESS,sizeof(nvm_storage_t)}
};


/* ============================================ */
/*       PRIVATE FUNCTION PROTOTYPES            */
/* ============================================ */
static u16 CalculateChecksum(nvm_storage_t* data);
static u8 ValidateNVMData(nvm_storage_t* storage);


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void NVM_Manager_Init(void)
{	
	// Initialize NvmJob var
	GLobalNvmJob.address = 0;
	GLobalNvmJob.data = NULL;
	GLobalNvmJob.index = 0;
	GLobalNvmJob.length = 0;
	GLobalNvmJob.status = NVM_IDLE;
	
	// Try to load existing data
	if(NVM_Manager_Load() == NVM_VALID)
	{
		// Valid data found
		nvm_valid = TRUE;
		TRACE_INFO(TRACE_ECU, "NVM data loaded successfully");
	}
	
	else
	{
		// No valid data - initialize defaults
		nvm_buffer.checksum = 0;
		nvm_buffer.magic_number = NVM_MAGIC_NUMBER;
		
		//Initialize faultsArray
		//IGNITION
		nvm_buffer.faultsArray[IGNITION_FAULT_INDEX].degraded_fault_id = IGNITION_SIGNAL_FAULT;
		nvm_buffer.faultsArray[IGNITION_FAULT_INDEX].is_active = FALSE;
		nvm_buffer.faultsArray[IGNITION_FAULT_INDEX].last_lock_state = LOCKED;
		nvm_buffer.faultsArray[IGNITION_FAULT_INDEX].last_op_state = ECU_OP_STATE_NORMAL;
		nvm_buffer.faultsArray[IGNITION_FAULT_INDEX].occurance_counter = 0;
		
		//LOCK ACTUATOR
		nvm_buffer.faultsArray[LOCK_FAULT_INDEX].degraded_fault_id = LOCK_ACTUATOR_FAULT;
		nvm_buffer.faultsArray[LOCK_FAULT_INDEX].is_active = FALSE;
		nvm_buffer.faultsArray[LOCK_FAULT_INDEX].last_lock_state = LOCKED;
		nvm_buffer.faultsArray[LOCK_FAULT_INDEX].last_op_state = ECU_OP_STATE_NORMAL;
		nvm_buffer.faultsArray[LOCK_FAULT_INDEX].occurance_counter = 0;
		
		//SPEED SIGNAL
		nvm_buffer.faultsArray[SPEED_SIGNAL_FAULT_INDEX].degraded_fault_id = SPEED_SIGNAL_FAULT;
		nvm_buffer.faultsArray[SPEED_SIGNAL_FAULT_INDEX].is_active = FALSE;
		nvm_buffer.faultsArray[SPEED_SIGNAL_FAULT_INDEX].last_lock_state = LOCKED;
		nvm_buffer.faultsArray[SPEED_SIGNAL_FAULT_INDEX].last_op_state = ECU_OP_STATE_NORMAL;
		nvm_buffer.faultsArray[SPEED_SIGNAL_FAULT_INDEX].occurance_counter = 0;
		
		TRACE_INFO(TRACE_ECU, "NVM empty - using defaults");
	}
}

void NVM_Manager_Save(void)
{
	nvm_buffer.magic_number = NVM_MAGIC_NUMBER;
	nvm_buffer.checksum = CalculateChecksum(&nvm_buffer);
	
	NVM_WriteBlockAsync(NVM_FAULT_TABLE_INDX, (u8*)&nvm_buffer);	
	TRACE_INFO(TRACE_ECU, "NVM data saved");
}

NVM_Status_t NVM_Manager_Load(void)
{
	nvm_storage_t storage;
	
	NVM_ReadBlock(NVM_FAULT_TABLE_INDX, (u8*)&storage);
	
	// Validate data
	if(ValidateNVMData(&storage))
	{
		// Valid data
		nvm_buffer = storage;	
		return NVM_VALID;
	}
	else
	{
		// Invalid or missing data
		return NVM_ERROR;
	}
}

u8 NVM_ReadBlock(u16 blockID, u8* data)
{
	u16 i;

	if (data == NULL)
	{
		return NOK;
	}

	u16 address = NVM_Blocks[blockID].address;
	u16 length  = NVM_Blocks[blockID].size;

	for (i = 0; i < length; i++)
	{
		data[i] = NVM_ReadData(address + i);
	}

	return OK;
}

u8 NVM_WriteBlockAsync(u16 blockID, const u8* data)
{
	if(GLobalNvmJob.status == NVM_BUSY)
	{
		return NOK;
	}
	
	GLobalNvmJob.address = NVM_Blocks[blockID].address;
	GLobalNvmJob.length = NVM_Blocks[blockID].size;
	
	GLobalNvmJob.data = (u8*)data;
	
	GLobalNvmJob.index = 0;
	
	GLobalNvmJob.status = NVM_BUSY;
	
	return OK;
}

NVM_Status_t NVM_GetStatus(u16 blockID)
{
	if(GLobalNvmJob.address == NVM_Blocks[blockID].address)
	{
		return GLobalNvmJob.status;
	}
	else
	{
		return NVM_ERROR;
	}
}

void NVM_MainFunction(void)
{
	if(GLobalNvmJob.status == NVM_BUSY)
	{
		if(EEPROM_IsReady())
		{
			EEPROM_SendByte(GLobalNvmJob.address + GLobalNvmJob.index,
			GLobalNvmJob.data[GLobalNvmJob.index]);
			EEPROM_ForceWrite();
			
			GLobalNvmJob.index++;
			
			if(GLobalNvmJob.index >= GLobalNvmJob.length)
			{
				GLobalNvmJob.status = NVM_DONE;
			}
		}
	}
}

nvm_storage_t* NVM_Manager_GetBuffer(void)
{
	return &nvm_buffer;
}

/* ============================================ */
/*      PRIVATE FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

static u16 CalculateChecksum(nvm_storage_t* data)
{
	// Simple checksum: sum all bytes
	u16 checksum = 0;
	u8* ptr = (u8*)data -> faultsArray;
	
	for(u16 i = 0; i < sizeof(data -> faultsArray); i++)
	{
		checksum += ptr[i];
	}
	
	return checksum;
}

static u8 ValidateNVMData(nvm_storage_t* storage)
{
	
	// Check magic number
	if(storage->magic_number != NVM_MAGIC_NUMBER)
	{
		TRACE_ERROR(TRACE_ECU, "NVM: Invalid magic number");
		return FALSE;
	}
	
	// Check checksum
	u16 calculated = CalculateChecksum(storage);
	if(calculated != storage->checksum)
	{
		TRACE_ERROR(TRACE_ECU, "NVM: Checksum mismatch");
		return FALSE;
	}
	
	// Data is valid
	return TRUE;
}