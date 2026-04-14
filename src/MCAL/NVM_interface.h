
#ifndef NVM_INTERFACE_H_
#define NVM_INTERFACE_H_

#include "StdTypes.h"



void NVM_WriteData(u16 eeaddress, u8 data);

u8 NVM_ReadData(u16 eeaddress);

u8 EEPROM_IsReady(void);

void EEPROM_SendByte(u16 address, u8 data);

void EEPROM_ForceWrite(void);


#endif /* NVM_INTERFACE_H_ */