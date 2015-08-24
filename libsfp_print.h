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


/**
 * @brief The default name print function. It prints to stdout.
 *
 * @param udata   User provided data pointer (see libsfp_set_user_data).
 * @param name    SFP module parameter name.
 */
void libsfp_printname_default( void *udata, const char *name );

/**
 * @brief The default value print function. It prints to stdout.
 *
 * @param udata   User provided data pointer (see libsfp_set_user_data).
 * @param value   SFP module parameter value.
 */
void libsfp_printvalue_default( void *udata, const char *value );

/**
 * @brief The default newline print function. It prints to stdout.
 *
 * @param udata   User provided data pointer (see libsfp_set_user_data).
 */
void libsfp_printnewline_default( void *udata );



#endif
