#ifndef NVM_MANAGER_INTERFACE_H_
#define NVM_MANAGER_INTERFACE_H_
/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "NvmStorage.h"


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize NVM Manager
 * TODO: Read NVM data, validate integrity
 */
void NVM_Manager_Init(void);

/**
 * Save current system state to NVM
 * TODO: Write nvm_data_t structure to EEPROM/Flash
 */
void NVM_Manager_Save(void);

/**
 * Load system state from NVM
 * TODO: Read nvm_data_t structure from EEPROM/Flash
 * Returns: Pointer to loaded data (or NULL if invalid)
 */
nvm_storage_t* NVM_Manager_Load(void);

/**
 * Clear NVM data (factory reset)
 * TODO: Erase all saved data
 */
void NVM_Manager_Clear(void);

#endif /* NVM_MANAGER_INTERFACE_H_ */