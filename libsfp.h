#ifndef LIBSFP_H
#define LIBSFP_H

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

/* Bank at address A0 */
typedef struct {
  sfp_base_fields_t base;
  sfp_extended_fields_t ext;
} __attribute__((packed)) sfp_A0_t;

/* Bank at address A2 */
typedef struct {
  sfp_thresholds_fields_t th;
  sfp_calibration_fields_t cl;
  sfp_rtdiagnostics_fields_t dg;
  uint8_t vendor[8];
  uint8_t writable[120];
  uint8_t vendor2[8];
} __attribute__((packed)) sfp_A2_t;

typedef struct {
  sfp_A0_t a0;
  sfp_A2_t a2;
} __attribute__((packed)) sfp_dump_t;


typedef struct {
  char vendor[17];      /* SFP module vendor name */
  char partnum[17];     /* SFP module part number */
  float txpower;        /* SFP module tx power */
  float rxpower;        /* SFP module rx power */
  uint32_t bitrate;     /* SFP module rx power */
  uint32_t spmode;      /* SFP module speed mode see LIBSFP_SPEED_MODE_* */
} sfp_brief_info_t;

typedef int(*sfp_readregs_fun_t)(void *udata, uint8_t addr,
                                 uint16_t start, uint16_t count, void *data);


#define LIBSFP_FLAGS_LONGOPT            1      /* Output options as long list */
#define LIBSFP_FLAGS_HEXOUTPUT          2      /* Output hex values */
#define LIBSFP_FLAGS_PRINT_UNKNOWN      4      /* Print fields with unknown values */
#define LIBSFP_FLAGS_PRINT_CALIBRATIONS 0x08   /* Print calibrations section info */
#define LIBSFP_FLAGS_PRINT_THRESHOLDS   0x10   /* Print thresholds section info */
#define LIBSFP_FLAGS_PRINT_BITOPTIONS   0x20   /* Print bit options fields */
#define LIBSFP_FLAGS_PRINT_LASERAUTO    0x40   /* Automaticaly detect laser/copper module
                                                  and do not printing not used values */


#define LIBSFP_SPEED_MODE_UNKNOWN   0
#define LIBSFP_SPEED_MODE_1G        1000
#define LIBSFP_SPEED_MODE_10G       10000
#define LIBSFP_SPEED_MODE_20G       20000

#define LIBSFP_DEF_A0_ADDRESS (0xA0>>1)       /* Default A0 Bank address on I2C bus */
#define LIBSFP_DEF_A2_ADDRESS (0xA2>>1)       /* Default A2 Bank address on I2C bus */

typedef struct {
  char sbuf[16];                 /** Internal string buffer */
  uint32_t flags;                /** Library flags  */
  FILE *file;                    /** File for text output */
  void *udata;                   /** User data pointer */
  uint8_t a0addr, a2addr;        /** SFP Bank addresses to use */
  sfp_readregs_fun_t readregs;   /** Callback for read information */
} libsfp_t;

int libsfp_init(libsfp_t **h);
int libsfp_free(libsfp_t *h);

int libsfp_set_readreg_callback(libsfp_t *h, sfp_readregs_fun_t readreg);
int libsfp_set_user_data(libsfp_t *h, void *udata);
int libsfp_set_outfile(libsfp_t *h, FILE *f);
int libsfp_set_flags(libsfp_t *h, uint32_t flags);
int libsfp_set_addresses(libsfp_t *h, uint8_t a0addr, uint8_t a2addr);

int libsfp_readinfo(libsfp_t *h, sfp_dump_t *d);
void libsfp_printinfo(libsfp_t *h, sfp_dump_t *d);
int libsfp_showinfo(libsfp_t *h);

int libsfp_readinfo_brief(libsfp_t *h, sfp_brief_info_t *info);

int libsfp_get_speed_mode(libsfp_t *h, uint32_t *smode);
int libsfp_is_copper_eth(libsfp_t *h, uint8_t *ans);
int libsfp_get_copper_length(libsfp_t *h, uint8_t *ans);
int libsfp_is_directattach(libsfp_t *h, uint8_t *ans);


#endif
