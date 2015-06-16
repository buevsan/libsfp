#ifndef LIBSFP_H
#define LIBSFP_H

/**
   @file
   @brief Main libsfp header file
*/

#include <stdint.h>
#include <stdio.h>

typedef struct {
  union {
    uint16_t u16;
    uint8_t d[2];
  };
} __attribute__((packed)) sfp_uint16_field_t;

typedef struct {
  union {
    uint32_t u32;
    uint8_t d[4];
  };
} __attribute__((packed)) sfp_uint32_field_t;

typedef struct {
  uint8_t identifier;
  uint8_t ext_identifier;
  uint8_t connector;
  uint8_t transceiver[8];
  uint8_t encoding;
  uint8_t br_nominal;
  uint8_t rate_identifier;
  uint8_t length_km;
  uint8_t length_100m;
  uint8_t length_50um;
  uint8_t length_625um;
  uint8_t length_active;
  uint8_t length_50um_om3;
  uint8_t vendor_name[16];
  uint8_t transceiver2;
  uint8_t vendor_oui[3];
  uint8_t vendor_pn[16];
  uint8_t vendor_rev[4];
  sfp_uint16_field_t wavelength;
  uint8_t unallocated;
  uint8_t cc_base;
} __attribute__((packed)) sfp_base_fields_t;

typedef struct {
  sfp_uint16_field_t options;
  uint8_t br_max;
  uint8_t br_min;
  uint8_t vendor_sn[16];
  uint8_t date_code[8];
  uint8_t diag_mon_type;
  uint8_t en_options;
  uint8_t sff8472_comp;
  uint8_t cc_ext;
} __attribute__((packed)) sfp_extended_fields_t;

typedef struct {
  sfp_uint16_field_t temp_alarm_high;
  sfp_uint16_field_t temp_alarm_low;
  sfp_uint16_field_t temp_warn_high;
  sfp_uint16_field_t temp_warn_low;
  sfp_uint16_field_t voltage_alarm_high;
  sfp_uint16_field_t voltage_alarm_low;
  sfp_uint16_field_t voltage_warn_high;
  sfp_uint16_field_t voltage_warn_low;
  sfp_uint16_field_t bias_alarm_high;
  sfp_uint16_field_t bias_alarm_low;
  sfp_uint16_field_t bias_warn_high;
  sfp_uint16_field_t bias_warn_low;
  sfp_uint16_field_t txpower_alarm_high;
  sfp_uint16_field_t txpower_alarm_low;
  sfp_uint16_field_t txpower_warn_high;
  sfp_uint16_field_t txpower_warn_low;
  sfp_uint16_field_t rxpower_alarm_high;
  sfp_uint16_field_t rxpower_alarm_low;
  sfp_uint16_field_t rxpower_warn_high;
  sfp_uint16_field_t rxpower_warn_low;
  uint8_t unallocated[16];
} __attribute__((packed)) sfp_thresholds_fields_t;

typedef struct {
  sfp_uint32_field_t rx_pwr[5];
  sfp_uint16_field_t txi_slope;
  sfp_uint16_field_t txi_offset;
  sfp_uint16_field_t tx_pwr_slope;
  sfp_uint16_field_t tx_pwr_offset;
  sfp_uint16_field_t t_slope;
  sfp_uint16_field_t t_offset;
  sfp_uint16_field_t v_slope;
  sfp_uint16_field_t v_offset;
  uint8_t unallocated[3];
  uint8_t checksum;
} __attribute__((packed)) sfp_calibration_fields_t;

typedef struct {
  sfp_uint16_field_t temperature;
  sfp_uint16_field_t voltage;
  sfp_uint16_field_t bias_current;
  sfp_uint16_field_t tx_power;
  sfp_uint16_field_t rx_power;
  uint8_t unallocated[4];
  uint8_t status;
  uint8_t reserved;
  uint8_t alarms[2];
  uint8_t unallocated2[2];
  uint8_t warnings[2];
  uint8_t estatus;
  uint8_t unallocated3;
} __attribute__((packed)) sfp_rtdiagnostics_fields_t;

/** Struct to present bank at address A0 */
typedef struct {
  sfp_base_fields_t base;
  sfp_extended_fields_t ext;
} __attribute__((packed)) sfp_A0_t;

/** Struct to present bank at address A2 */
typedef struct {
  sfp_thresholds_fields_t th;        /** Threshold fields section */
  sfp_calibration_fields_t cl;       /** Calibration fields section */
  sfp_rtdiagnostics_fields_t dg;     /** Diagnostic fields section */  
  uint8_t vendor[8];
  uint8_t writable[120];
  uint8_t vendor2[8];
} __attribute__((packed)) sfp_A2_t;

typedef struct {
  sfp_A0_t a0;
  sfp_A2_t a2;
} __attribute__((packed)) sfp_dump_t;


/** Struct to store brief information for SFP module */
typedef struct {
  char vendor[17];      /** SFP module vendor name */
  char partnum[17];     /** SFP module part number */
  float txpower;        /** SFP module tx power (mW) */
  float rxpower;        /** SFP module rx power (mW) */
  uint32_t bitrate;     /** SFP module bitrate (bit/s)*/
  uint32_t spmode;      /** SFP module speed mode see LIBSFP_SPEED_MODE_* */
} sfp_brief_info_t;

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
typedef int(*sfp_readregs_fun_t)(void *udata, uint8_t addr,
                                 uint16_t start, uint16_t count, void *data);


#define LIBSFP_FLAGS_LONGOPT            1      /**< Output bit options as long list */
#define LIBSFP_FLAGS_HEXOUTPUT          2      /**< Output hex values */
#define LIBSFP_FLAGS_PRINT_UNKNOWN      4      /**< Print fields with unknown values */
#define LIBSFP_FLAGS_PRINT_CALIBRATIONS 0x08   /**< Print calibrations section info */
#define LIBSFP_FLAGS_PRINT_THRESHOLDS   0x10   /**< Print thresholds section info */
#define LIBSFP_FLAGS_PRINT_BITOPTIONS   0x20   /**< Print bit options fields */
#define LIBSFP_FLAGS_PRINT_LASERAUTO    0x40   /**< Automaticaly detect laser/copper module
                                                    and do not printing not used values */


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
int libsfp_set_readreg_callback(libsfp_t *h, sfp_readregs_fun_t readreg);

/**
 * @brief Assign userdata pointer for callback function
 * @param h - pointer to library handle
 * @param udata - pointer to user data
 * @return 0 on success
 */
int libsfp_set_user_data(libsfp_t *h, void *udata);

/**
 * @brief Assign file pointer used for text output
 * @param h - pointer to library handle
 * @param f - file pointer
 * @return 0 on success
 */
int libsfp_set_outfile(libsfp_t *h, FILE *f);

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
 * @brief Read full SFP module info to memory
 * @param h    - library handle
 * @param dump - pointer to memory to store information
 * @return 0 on success
 */
int libsfp_readinfo(libsfp_t *h, sfp_dump_t *d);

/**
 * @brief Output information selected by flags
 *        as text to specified file
 * @param h     - library handle
 * @param dump  - pointer to struct that store information
 * @return 0 on success
 */
void libsfp_printinfo(libsfp_t *h, sfp_dump_t *d);

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
int libsfp_readinfo_brief(libsfp_t *h, sfp_brief_info_t *info);

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


#endif
