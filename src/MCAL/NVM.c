#include "NVM_interface.h"
#include "Utils.h"
#include "atmega32_mem_map.h"

NVM_Job_t    GLobalNvmJob;
extern NVM_BlockConfig_t NVM_Blocks[];


void NVM_WriteData(u16 eeaddress, u8 data)
{
	while(GET_BIT(EECR,EEWE));
	while(GET_BIT(SPMCR,SPMEN));
	EEAR = eeaddress;
	EEDR = data;
	cli();
	SET_BIT(EECR,EEMWE);
	SET_BIT(EECR,EEWE);	
	sei();
}

u8 NVM_ReadData(u16 eeaddress)
{
	while(GET_BIT(EECR,EEWE));
	while(GET_BIT(SPMCR,SPMEN));
	EEAR = eeaddress;
	SET_BIT(EECR,EERE);
	return EEDR;
}

u8 EEPROM_IsReady(void)
{
	return !(GET_BIT(EECR, EEWE));
}

void EEPROM_SendByte(u16 address, u8 data)
{
	EEAR = address;
	EEDR = data;
}

void EEPROM_ForceWrite(void)
{
	cli();
	SET_BIT(EECR,EEMWE);
	SET_BIT(EECR,EEWE);
	sei();
}


void NVM_Init(void)
{
	GLobalNvmJob.address = 0;
	GLobalNvmJob.data = NULL;
	GLobalNvmJob.index = 0;
	GLobalNvmJob.length = 0;
	GLobalNvmJob.status = NVM_IDLE;
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