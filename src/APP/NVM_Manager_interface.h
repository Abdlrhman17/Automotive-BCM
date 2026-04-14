#ifndef NVM_MANAGER_INTERFACE_H_
#define NVM_MANAGER_INTERFACE_H_
/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "NvmStorage.h"
#include "NVM_interface.h"


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize NVM Manager
 * Read NVM data & validate integrity
 */
void NVM_Manager_Init(void);

/**
 * Save current system state to NVM
 * Write nvm_data_t structure to EEPROM/Flash
 */
void NVM_Manager_Save(void);

/**
 * Load system state from NVM
 * Read nvm_data_t structure from EEPROM/Flash
 */
NVM_Status_t NVM_Manager_Load(void);

/**
 * Returns nvm_buffer address
 */
nvm_storage_t* NVM_Manager_GetBuffer(void);

/**
 * Reads A block from Nvm (Synchronous)
 */
u8 NVM_ReadBlock(u16 blockID, u8* data);

/**
 * Writes A block to Nvm (Asynchronous)
 */
u8 NVM_WriteBlockAsync(u16 blockID, const u8* data);

/**
 * Returns Nvm current status 
 */
NVM_Status_t NVM_GetStatus(u16 blockID);

/**
 * Runs periodically to process a Block Write (One byte at a time)
 */
void NVM_MainFunction(void);

#endif /* NVM_MANAGER_INTERFACE_H_ */