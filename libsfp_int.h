#ifndef LIBSFP_INT_H__
#define LIBSFP_INT_H__

/**
   @file
   @brief libsfp internal header file\n
          (for internal components only)
*/

#include "libsfp.h"

typedef struct {
  libsfp_A0_t a0;
  libsfp_A2_t a2;
} __attribute__((packed)) libsfp_dump_t;

typedef struct {
  char sbuf[16];                 /** Internal string buffer */
  uint32_t flags;                /** Library flags  */
  void *udata;                   /** User data pointer */
  uint8_t a0addr, a2addr;        /** SFP Bank addresses to use */
  libsfp_readregs_cb_t readregs;   /** Callback to read information */
  libsfp_writeregs_cb_t writeregs;  /** Callback to write information */
  libsfp_printname_cb_t printname;   /** Callback to print parameter name */
  libsfp_printvalue_cb_t printvalue;  /** Callback to print parameter value */
  libsfp_printnewline_cb_t printnewline;  /** Callback to print parameter newline */
} libsfp_int_t;

#define H(ptr) ((libsfp_int_t*)(ptr))

#define READREG(h, bank_addr, reg_offset, count, dest) \
    ((H(h)->readregs) ? \
       H(h)->readregs(H(h)->udata, bank_addr, \
                      reg_offset, count, dest) : -1)

#define READREG_A0(h, reg_offset, count, dest) \
    READREG(h, H(h)->a0addr, reg_offset, count, dest)
#define READREG_A2(h, reg_offset, count, dest) \
    READREG(h, H(h)->a2addr, reg_offset, count, dest)

#define WRITEREG(h, bank_addr, reg_offset, count, dest) \
    ((H(h)->writeregs) ? \
       H(h)->writeregs(H(h)->udata, bank_addr, \
                      reg_offset, count, dest) : -1)

#define WRITEREG_A0(h, reg_offset, count, dest) \
    WRITEREG(h, H(h)->a0addr, reg_offset, count, dest)
#define WRITEREG_A2(h, reg_offset, count, dest) \
    WRITEREG(h, H(h)->a2addr, reg_offset, count, dest)


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


/**
 * @brief Calc simple check sum that used in SFP memory banks
 * @param d     data pointer
 * @param size  data size
 * @return Checksum
 */
uint8_t libsfp_calc_csum(void *d, uint16_t size);




#endif
