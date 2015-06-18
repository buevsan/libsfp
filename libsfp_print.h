#ifndef LIBSFP_PRINT_H__
#define LIBSFP_PRINT_H__

#include "libsfp_int.h"

/**
 * @brief Output information selected by flags
 *        as text to specified file
 * @param h     - library handle
 * @param dump  - pointer to struct that store information
 * @return 0 on success
 */
void libsfp_printinfo(libsfp_t *h, libsfp_dump_t *dump);


#endif
