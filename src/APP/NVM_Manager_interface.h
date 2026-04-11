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
 * Returns: Pointer to loaded data (or NULL if invalid)
 */
nvm_storage_t* NVM_Manager_Load(void);


#endif /* NVM_MANAGER_INTERFACE_H_ */