#ifndef LIBSFP_H
#define LIBSFP_H

/**
   @file
   @brief Main libsfp public header file
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#include <libsfp_regs.h>
#include <libsfp_types.h>

/** Struct to store brief information for SFP module */
typedef struct {
  char vendor[17];      /** SFP module vendor name */
  char partnum[17];     /** SFP module part number */
  float txpower;        /** SFP module tx power (mW) */
  float rxpower;        /** SFP module rx power (mW) */
  uint32_t bitrate;     /** SFP module bitrate (bit/s)*/
  uint32_t spmode;      /** SFP module speed mode see LIBSFP_SPEED_MODE_* */
} libsfp_brief_info_t;

/** @brief Callback used for reading SFP module register memory
 *
 *  @param udata   User provided data pointer\n
 *                 (see libsfp_set_user_data to change)
 *  @param addr    Memory bank address of SFP module\n
 *                 see LIBSFP_DEF_* constants for defaut values\n
 *                 see libsfp_set_addresses to changes default values
 *  @param start   offset in bytes to start reading from
 *  @param count   count of bytes to read
 *  @param data    pointer to buffer to store data
 */
typedef int(*libsfp_readregs_cb_t)(void *udata, uint8_t addr,
                                 uint16_t start, uint16_t count, void *data);


/** @brief Callback used for writing SFP module register memory
 *
 *  @param udata   User provided data pointer\n
 *                 (see libsfp_set_user_data to change)
 *  @param addr    Memory bank address of SFP module\n
 *                 see LIBSFP_DEF_* constants for defaut values\n
 *                 see libsfp_set_addresses to changes default values
 *  @param start   offset in bytes to start writing from
 *  @param count   count of bytes to write
 *  @param data    pointer to buffer with data
 */
typedef int(*libsfp_writeregs_cb_t)(void *udata, uint8_t addr,
                                 uint16_t start, uint16_t count, const void *data);


/** @brief Callback to print SFP module parameter name.
 *
 *  @param udata   User provided data pointer (see libsfp_set_user_data).
 *  @param name    SFP module parameter name.
 */
typedef void (*libsfp_printname_cb_t)( void *udata, const char *name );


/** @brief Callback to print SFP module parameter value.
 *
 *  @param udata   User provided data pointer (see libsfp_set_user_data).
 *  @param value    SFP module parameter value.
 */
typedef void (*libsfp_printvalue_cb_t)( void *udata, const char *value );

/** @brief Callback to print the newline.
 *
 *  @param udata   User provided data pointer (see libsfp_set_user_data).
 */
typedef void (*libsfp_printnewline_cb_t)( void *udata );


typedef struct libsfp_print_callbacks_s {
  libsfp_printname_cb_t name;
  libsfp_printvalue_cb_t value;
  libsfp_printnewline_cb_t newline;
} libsfp_print_callbacks_t;


#define LIBSFP_FLAGS_PRINT_LONGOPT      1      /**< Output bit options as long list */
#define LIBSFP_FLAGS_PRINT_HEXOUTPUT    2      /**< Output hex values */
#define LIBSFP_FLAGS_PRINT_UNKNOWN      4      /**< Print fields with unknown values */
#define LIBSFP_FLAGS_PRINT_CALIBRATIONS 0x08   /**< Print calibrations section info */
#define LIBSFP_FLAGS_PRINT_THRESHOLDS   0x10   /**< Print thresholds section info */
#define LIBSFP_FLAGS_PRINT_BITOPTIONS   0x20   /**< Print bit options fields */
#define LIBSFP_FLAGS_PRINT_LASERAUTO    0x40   /**< Automaticaly detect laser/copper module
                                                    and do not printing not used values */
#define LIBSFP_FLAGS_PRINT_CSUM         0x80   /**< Print checsum information */
#define LIBSFP_FLAGS_PRINT_VENDOR       0x100  /**< Print vendor specific data dump */
#define LIBSFP_FLAGS_CSUM_CHECK         0x200  /**< Check csum after reading */


#define LIBSFP_SPEED_MODE_UNKNOWN   0     /**< Unknown speed */
#define LIBSFP_SPEED_MODE_1G        1000  /**< 1 Gb/s */
#define LIBSFP_SPEED_MODE_10G       10000 /**< 10 Gb/s */
#define LIBSFP_SPEED_MODE_20G       20000 /**< 20 Gb/s */

#define LIBSFP_DEF_A0_ADDRESS (0xA0>>1)       /**< Default A0 Bank address */
#define LIBSFP_DEF_A2_ADDRESS (0xA2>>1)       /**< Default A2 Bank address */

/** Main libsfp library handle struct\n
 *  Use only pointer to this type
*/
typedef struct {
} libsfp_t;

/**
 * @brief Create library handle with default parameters
 * @param h - pointer to address of library handle
 * @return 0 on success
*/
int libsfp_init(libsfp_t **h);

/**
 * @brief Create library handle with default parameters
 * @return libsfp handle or 0 if error occured
 */
libsfp_t *libsfp_create();

/**
 * @brief Free library handle and its memory
 * @param h - pointer to library handle
 * @return 0 on success
*/
int libsfp_free(libsfp_t *h);

/**
 * @brief Assign callback function address for reading access to SFP
 * @param h - pointer to library handle
 * @param readregs - address of callback function
 * @return 0 on success
 */
int libsfp_set_readreg_callback(libsfp_t *h, libsfp_readregs_cb_t readreg);

/**
 * @brief Assign callback function address for reading access to SFP
 * @param h - pointer to library handle
 * @param readregs - address of callback function
 * @return 0 on success
 */
int libsfp_set_writereg_callback(libsfp_t *h, libsfp_writeregs_cb_t writereg);

/**
 * @brief Assign name print callback function address
 * @param h - pointer to library handle
 * @param readregs - address of the callback function
 * @return 0 on success
 */
int libsfp_set_print_callbacks(libsfp_t *h, const libsfp_print_callbacks_t *callbacks);

/**
 * @brief Assign userdata pointer for callback functions
 * @param h - pointer to library handle
 * @param udata - pointer to user data
 * @return 0 on success
 */
int libsfp_set_user_data(libsfp_t *h, void *udata);

/**
 * @brief Assign some option flags used for modify output text information
 * @param h - pointer to library handle
 * @param flags - file pointer
 * @return 0 on success
 */
int libsfp_set_flags(libsfp_t *h, uint32_t flags);

/**
 * @brief Assign bus addresses of two SFP memory banks
 * @param h - pointer to library handle
 * @param a0addr - address of first bank  A0
 * @param a2addr - address of second bank A2
 * @return 0 on success
 */
int libsfp_set_addresses(libsfp_t *h, uint8_t a0addr, uint8_t a2addr);

/**
 * @brief Read and output information selected by flags
 *        as text to specified file
 * @param h     - library handle
 * @return 0 on success
 */
int libsfp_showinfo(libsfp_t *h);

/**
 * @brief Read brief information for SFP module an store it to
 *        specified place
 * @param h    - library handle
 * @param info - struct to store information
 * @return
 */
int libsfp_readinfo_brief(libsfp_t *h, libsfp_brief_info_t *info);

/**
 * @brief Get SFP module max speed (See LIBSFP_SPEED_MODE_* constants)
 * @param h      - library handle
 * @param smode  - speed
 * @return 0 on success
 */
int libsfp_get_speed_mode(libsfp_t *h, uint32_t *smode);

/**
 * @brief Check that SFP module is a copper ethernet module
 * @param h   - library handle
 * @param ans - answer (0/1 - Other/Copper ethernet)
 * @return 0 on success
 */
int libsfp_is_copper_eth(libsfp_t *h, uint8_t *ans);

/**
 * @brief Get copper length in case of copper ethernet module
 * @param h    -  library handle
 * @param ans  -  length of cable (m)
 * @return 0 on success
 */
int libsfp_get_copper_length(libsfp_t *h, uint8_t *ans);

/**
 * @brief Check that SFP module is Dirrect Attach cable module
 * @param h   - library handle
 * @param ans - answer (0/1 - Normal/Dirrect Attach)
 * @return 0 on success
 */
int libsfp_is_directattach(libsfp_t *h, uint8_t *ans);

/**
 * @brief Get SFP module pins state (if supported)
 * @param h      library handle
 * @param value  pointer to bit value\n
 *               see LIBSFP_A2_STATUSCONTROL_* constants
 * @return 0 on success
 */
int libsfp_get_pins_state(libsfp_t *h, uint8_t *value);

/**
 * @brief Set SFP module soft pins (if supported)
 * @param h      library handle
 * @param mask   bit mask to set \n
 *               see LIBSFP_A2_STATUSCONTROL_*_SET constants
 * @param value  bit value\n
 *               see LIBSFP_A2_STATUSCONTROL_*_SET constants
 * @return 0 on success
 */
int libsfp_set_soft_pins_state(libsfp_t *h, uint8_t mask, uint8_t value);


#ifdef __cplusplus
}
#endif

#endif
