#ifndef LIBSFP_TYPES_H__
#define LIBSFP_TYPES_H__

/**
   @file
   @brief libsfp public header file \n
          (basic types definitions)

*/

#include <inttypes.h>

typedef struct {
  union {
    uint16_t u16;
    uint8_t d[2];
  };
} __attribute__((packed)) libsfp_u16_field_t;

typedef struct {
  union {
    uint32_t u32;
    uint8_t d[4];
  };
} __attribute__((packed)) libsfp_u32_field_t;

typedef struct {
  uint8_t identifier;
  uint8_t ext_identifier;
  uint8_t connector;
  uint8_t transceiver[8];
  uint8_t encoding;
  uint8_t br_nominal;
  uint8_t rate_identifier;
  uint8_t length_smf_km;
  uint8_t length_smf;
  uint8_t length_50um;
  uint8_t length_625um;
  uint8_t length_cable;
  uint8_t length_om3;
  uint8_t vendor_name[16];
  uint8_t transceiver2;
  uint8_t vendor_oui[3];
  uint8_t vendor_pn[16];
  uint8_t vendor_rev[4];
  libsfp_u16_field_t wavelength;
  uint8_t unallocated;
  uint8_t cc_base;
} __attribute__((packed)) libsfp_base_fields_t;

typedef struct {
  libsfp_u16_field_t options;
  uint8_t br_max;
  uint8_t br_min;
  uint8_t vendor_sn[16];
  uint8_t date_code[8];
  uint8_t diag_mon_type;
  uint8_t en_options;
  uint8_t sff8472_comp;
  uint8_t cc_ext;
} __attribute__((packed)) libsfp_extended_fields_t;

typedef struct {
  libsfp_u16_field_t temp_alarm_high;
  libsfp_u16_field_t temp_alarm_low;
  libsfp_u16_field_t temp_warn_high;
  libsfp_u16_field_t temp_warn_low;
  libsfp_u16_field_t voltage_alarm_high;
  libsfp_u16_field_t voltage_alarm_low;
  libsfp_u16_field_t voltage_warn_high;
  libsfp_u16_field_t voltage_warn_low;
  libsfp_u16_field_t bias_alarm_high;
  libsfp_u16_field_t bias_alarm_low;
  libsfp_u16_field_t bias_warn_high;
  libsfp_u16_field_t bias_warn_low;
  libsfp_u16_field_t txpower_alarm_high;
  libsfp_u16_field_t txpower_alarm_low;
  libsfp_u16_field_t txpower_warn_high;
  libsfp_u16_field_t txpower_warn_low;
  libsfp_u16_field_t rxpower_alarm_high;
  libsfp_u16_field_t rxpower_alarm_low;
  libsfp_u16_field_t rxpower_warn_high;
  libsfp_u16_field_t rxpower_warn_low;
  uint8_t unallocated[16];
} __attribute__((packed)) libsfp_thresholds_fields_t;

typedef struct {
  libsfp_u32_field_t rx_pwr[5];
  libsfp_u16_field_t txi_slope;
  libsfp_u16_field_t txi_offset;
  libsfp_u16_field_t tx_pwr_slope;
  libsfp_u16_field_t tx_pwr_offset;
  libsfp_u16_field_t t_slope;
  libsfp_u16_field_t t_offset;
  libsfp_u16_field_t v_slope;
  libsfp_u16_field_t v_offset;
  uint8_t unallocated[3];
  uint8_t checksum;
} __attribute__((packed)) libsfp_calibration_fields_t;

typedef struct {
  libsfp_u16_field_t temperature;
  libsfp_u16_field_t voltage;
  libsfp_u16_field_t bias_current;
  libsfp_u16_field_t tx_power;
  libsfp_u16_field_t rx_power;
  uint8_t unallocated[4];
  uint8_t status;
  uint8_t reserved;
  uint8_t alarms[2];
  uint8_t unallocated2[2];
  uint8_t warnings[2];
  uint8_t estatus;
  uint8_t unallocated3;
} __attribute__((packed)) libsfp_rtdiagnostics_fields_t;

/** Struct to present bank at address A0 */
typedef struct {
  libsfp_base_fields_t base;
  libsfp_extended_fields_t ext;
} __attribute__((packed)) libsfp_A0_t;

/** Struct to present bank at address A2 */
typedef struct {
  libsfp_thresholds_fields_t th;        /** Threshold fields section */
  libsfp_calibration_fields_t cl;       /** Calibration fields section */
  libsfp_rtdiagnostics_fields_t dg;     /** Diagnostic fields section */
  uint8_t vendor[8];
  uint8_t writable[120];
  uint8_t vendor2[8];
} __attribute__((packed)) libsfp_A2_t;



#endif
