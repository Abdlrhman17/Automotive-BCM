#include "NVM_interface.h"
#include "Utils.h"
#include "atmega32_mem_map.h"

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
