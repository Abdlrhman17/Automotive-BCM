
#ifndef NVM_INTERFACE_H_
#define NVM_INTERFACE_H_

#include "StdTypes.h"

typedef enum
{
	NVM_IDLE,
	NVM_BUSY,
	NVM_DONE,
	NVM_ERROR
}NVM_Status_t;


typedef struct
{
	u32 address;
	u16 size;
}NVM_BlockConfig_t;


typedef struct
{
	u8* data;     // pointer to RAM data
	u32 address;  // where to write in NVM
	u16 length;   // total size
	u16 index;    // progress 
	NVM_Status_t status;
}NVM_Job_t;


void NVM_WriteData(u16 eeaddress, u8 data);

u8 NVM_ReadData(u16 eeaddress);

u8 EEPROM_IsReady(void);

void EEPROM_SendByte(u16 address, u8 data);

void EEPROM_ForceWrite(void);

void NVM_Init(void);

u8 NVM_ReadBlock(u16 blockID, u8* data);

u8 NVM_WriteBlockAsync(u16 blockID, const u8* data);

NVM_Status_t NVM_GetStatus(u16 blockID);

void NVM_MainFunction(void);



#endif /* NVM_INTERFACE_H_ */