#ifndef LIBSFP_INT_H__
#define LIBSFP_INT_H__

/**
   @file
   @brief libsfp internal header file\n
          (for internal components only)
*/

#include "libsfp.h"

typedef struct {
  char sbuf[16];                 /** Internal string buffer */
  uint32_t flags;                /** Library flags  */
  FILE *file;                    /** File for text output */
  void *udata;                   /** User data pointer */
  uint8_t a0addr, a2addr;        /** SFP Bank addresses to use */
  libsfp_readregs_cb_t readregs;   /** Callback for read information */
} libsfp_int_t;

#define H(ptr) ((libsfp_int_t*)(ptr))

#define READREG_A0(h, reg, count, data) \
    H(h)->readregs(H(h)->udata, H(h)->a0addr, reg, count, data)
#define READREG_A2(h, reg, count, data) \
    H(h)->readregs(H(h)->udata, H(h)->a2addr, reg, count, data)



int libsfp_is_laser_availble(libsfp_base_fields_t *bf);
float libsfp_get_slope(libsfp_u16_field_t f);
float libsfp_get_offset(libsfp_u16_field_t f);
float libsfp_get_rxpwr(libsfp_u32_field_t f);

float libsfp_get_temp(libsfp_u16_field_t tf, libsfp_calibration_fields_t *cal);
float libsfp_get_voltage(libsfp_u16_field_t v, libsfp_calibration_fields_t *cal);
float libsfp_get_biascurrent(libsfp_u16_field_t v, libsfp_calibration_fields_t *cal);
float libsfp_get_rxpower(libsfp_u16_field_t v, libsfp_u32_field_t *rx_pwr);
float libsfp_get_txpower(libsfp_u16_field_t v,
                         libsfp_u16_field_t *slope, libsfp_u16_field_t *ofs);




#endif
