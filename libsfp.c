/**
   @file
   @brief main libsfp implementation source file
*/

#include <stdlib.h>
#include <string.h>
#include "libsfp_int.h"
#include "libsfp_print.h"

/**
 * @brief Create library handle with default parameters
 * @param h - pointer to address of library handle
 * @return 0 on success
 */
int libsfp_init(libsfp_t **h)
{
  (*h) = malloc(sizeof(libsfp_int_t));
  if (!H(*h))
    return -1;
  memset(H(*h), 0, sizeof(libsfp_int_t));

  H(*h)->flags = LIBSFP_FLAGS_PRINT_LONGOPT;

  H(*h)->a0addr = LIBSFP_DEF_A0_ADDRESS;
  H(*h)->a2addr = LIBSFP_DEF_A2_ADDRESS;

  /* Assign default print callbacks */
  libsfp_print_callbacks_t *cbks = &(H(*h)->print_cb);
  cbks->name = libsfp_printname_default;
  cbks->value = libsfp_printvalue_default;
  cbks->newline = libsfp_printnewline_default;

  return 0;
}

/**
 * @brief Create library handle with default parameters
 * @return libsfp handle or 0 if error occured
 */
libsfp_t *libsfp_create()
{
  libsfp_t *h;
  if (libsfp_init(&h))
    return 0;
  return h;
}


/**
 * @brief Free library handle and its memory
 * @param h - pointer to library handle
 * @return 0 on success
 */
int libsfp_free(libsfp_t *h)
{
  free(h);
  return 0;
}

/**
 * @brief Assign callback function address for reading access to SFP
 * @param h - pointer to library handle
 * @param readregs - address of callback function
 * @return 0 on success
 */
int libsfp_set_readreg_callback(libsfp_t *h, libsfp_readregs_cb_t readregs)
{
  H(h)->readregs = readregs;
  return 0;
}

/**
 * @brief Assign name print callbacks function address
 *
 * @param h - pointer to library handle
 * @param clbk - address struct that contain callbacks addresses
 *               if any of addresses is NULL then this address not changed
 * @return 0 on success
 */
int libsfp_set_print_callbacks(libsfp_t *h, const libsfp_print_callbacks_t *clbk)
{
  if (clbk->name)
    H(h)->print_cb.name = clbk->name;

  if (clbk->newline)
    H(h)->print_cb.newline = clbk->newline;

  if (clbk->value)
    H(h)->print_cb.value = clbk->value;

  return 0;
}

/**
 * @brief Assign userdata pointer for callback functions
 * @param h - pointer to library handle
 * @param udata - pointer to user data
 * @return 0 on success
 */
int libsfp_set_user_data(libsfp_t *h, void *udata)
{
  H(h)->udata = udata;
  return 0;
}

/**
 * @brief Assign some option flags used for modify output text information
 * @param h - pointer to library handle
 * @param flags - file pointer
 * @return 0 on success
 */
int libsfp_set_flags(libsfp_t *h, uint32_t flags)
{
  H(h)->flags = flags;
  return 0;
}

/**
 * @brief Assign bus addresses of two SFP memory banks
 * @param h - pointer to library handle
 * @param a0addr - address of first bank  A0
 * @param a2addr - address of second bank A2
 * @return 0 on success
 */
int libsfp_set_addresses(libsfp_t *h, uint8_t a0addr, uint8_t a2addr)
{
  H(h)->a0addr = a0addr;
  H(h)->a2addr = a2addr;
  return 0;
}

int libsfp_is_laser_availble(libsfp_base_fields_t *bf)
{
  if ( ((bf->connector >= 0x20) && (bf->connector <= 0x22)) ||
       ((bf->connector >= 0x2) && (bf->connector <= 0x6)) )
    return 0;

  if ((bf->transceiver[3] & 0x08) || (bf->transceiver[6] & 0xF0))
    return 0;

  return 1;
}

/* A2 address */

float libsfp_get_slope(libsfp_u16_field_t f)
{
  return (float)f.d[0]+(float)f.d[1]/(float)256.0;
}

float libsfp_get_offset(libsfp_u16_field_t f)
{
  return ((int16_t)((f.d[0]<<8) |f.d[1]));
}

float libsfp_get_rxpwr(libsfp_u32_field_t f)
{
  uint32_t v = ((f.d[0])<<24) | ((f.d[1])<<16) | ((f.d[2])<<8) | (f.d[3]);
  return (float)v;
}

float libsfp_get_temp(libsfp_u16_field_t tf, libsfp_calibration_fields_t *cal)
{
  float f;

  f = ((float)((tf.d[0])&0x7F)) + ((float)tf.d[1])/(float)256.0;
  f = f*(float)((tf.d[0]&0x80)?-1.0:1.0);

  if (cal)
    f = (libsfp_get_slope(cal->t_slope)*f + libsfp_get_offset(cal->t_offset))/(float)1000.0;

  return f;
}

float libsfp_get_voltage(libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  float f;
  f = ((float)( (v.d[0] << 8) | v.d[1]))/(float)10000.0;

  if (cal)
    f = (libsfp_get_slope(cal->v_slope)*f + libsfp_get_offset(cal->v_offset))/(float)10.0;

  return f;
}

float libsfp_get_biascurrent(libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  float f;
  f = ((float)( (v.d[0] << 8) | v.d[1])*(float)0.002);

  if (cal) {
    f = (libsfp_get_slope(cal->txi_slope)*f +
         libsfp_get_offset(cal->txi_offset))*(float)0.002;
  }

  return f;
}

/**
 * @brief Return tx power level in mWatt
 *
 * @param v             corresponding field value
 * @param slope         pointer for calibration slope  field or 0 if not present
 * @param ofs           pointer for calibration offset field or 0 if not present
 *
 * @return tx level (mWatt)
 */
float libsfp_get_txpower(libsfp_u16_field_t v,
                         libsfp_u16_field_t *slope, libsfp_u16_field_t *ofs)
{
  float f;
  f = (((float)( (v.d[0] << 8) | v.d[1]))/10000.0f);

  if ((slope) && (ofs))
    f = (libsfp_get_slope(*slope)*f + libsfp_get_offset(*ofs))/10.0f;

  return f;
}

/**
 * @brief Get RX power level in mWatt's
 *
 * @param v       coresponding field value
 * @param rx_pwr  pointer to calibration array value [5] or null if not present
 *
 * @return RX power level (mWatts)
 *
 */
float libsfp_get_rxpower(libsfp_u16_field_t v, libsfp_u32_field_t *rx_pwr)
{
  int i;
  float f, sum = 0.0;
  f = (float)((float)( (v.d[0] << 8) | v.d[1])/10000.0);

  if (rx_pwr) {
    for (i=0; i<4; ++i)
      sum += libsfp_get_rxpwr(rx_pwr[i])*f;
    sum += libsfp_get_rxpwr(rx_pwr[4]);
    return sum;
  }

  return f;
}

/**
 * @brief Calc simple check sum that used in SFP memory banks
 * @param d    data pointer
 * @param size data size
 * @return Checksum;
 */
uint8_t libsfp_calc_csum(void *d, uint16_t size)
{
  uint16_t i;
  uint32_t csum=0;
  for (i=0; i<size; ++i)
    csum += ((uint8_t*)d)[i];
  return csum & 0xFF;
}

int libsfp_is_csums_correct(libsfp_t *h, libsfp_A0_t *a0, libsfp_A2_t *a2)
{
  if ( !(H(h)->flags & LIBSFP_FLAGS_CSUM_CHECK) )
    return 0;

  if (a0) {
    if (a0->base.cc_base != libsfp_calc_csum(&a0->base, sizeof(a0->base) - 1))
       return 1;

    if (a0->ext.cc_ext != libsfp_calc_csum(&a0->ext, sizeof(a0->ext) - 1))
       return 1;
  }

  if (a2) {
    if (a2->cc_dmi != libsfp_calc_csum(a2, sizeof(a2->th) + sizeof(a2->cl)-1))
      return 1;
  }

  return 0;
}

/**
 * @brief Read full SFP module info to memory
 * @param h    - library handle
 * @param dump - pointer to memory to store information
 * @return 0 on success
 */
int libsfp_readinfo(libsfp_t *h, libsfp_dump_t *dump)
{
  if (READREG_A0(h, 0, sizeof(libsfp_A0_t), &dump->a0))
    return -1;

  if (dump->a0.ext.diag_mon_type & LIBSFP_A0_DIAGMON_TYPE_DDM) {

    if (READREG_A2(h, 0, sizeof(libsfp_A2_t), &dump->a2))
      return -1;

    if (libsfp_is_csums_correct(h, &dump->a0, &dump->a2))
      return -1;

  } else {

    if (libsfp_is_csums_correct(h, &dump->a0, 0))
      return -1;

  }

  return 0;
}

/**
 * @brief Read and output information selected by flags
 *        as text to specified file
 * @param h     - library handle
 * @return 0 on success
 */
int libsfp_showinfo(libsfp_t *h)
{
  libsfp_dump_t *dump;

  dump = malloc(sizeof(libsfp_dump_t));

  if (!dump)
    return -1;

  if (libsfp_readinfo(h, dump)) {
    free(dump);
    return -1;
  }

  libsfp_printinfo(h, dump);

  free(dump);

  return 0;
}

uint32_t libsfp_bitrate2speed_mode(uint8_t br)
{
  uint32_t smode;
  smode = LIBSFP_SPEED_MODE_UNKNOWN;

  if ((br >= 10) && (br < 100))
    smode = LIBSFP_SPEED_MODE_1G;

  if ((br >= 100 ) && (br < 200))
    smode = LIBSFP_SPEED_MODE_10G;

  if (br >= 200)
    smode = LIBSFP_SPEED_MODE_20G;

  return smode;
};

/**
 * @brief Read brief information for SFP module an store it to
 *        specified place
 * @param h    - library handle
 * @param info - struct to store information
 * @return
 */
int libsfp_readinfo_brief(libsfp_t *h, libsfp_brief_info_t *info)
{
  uint8_t d[2], dmtype;
  libsfp_u16_field_t tp, rp;
  libsfp_u32_field_t rx_pwr[5];
  libsfp_u16_field_t txpwr_slope, txpwr_offset;

  info->txpower = -1;
  info->rxpower = -1;

  if (READREG_A0(h, LIBSFP_OFS_A0_BR_NOMINAL, 1, d))
    return -1;
  info->bitrate = d[0]*100;  
  if (libsfp_get_speed_mode(h, &info->spmode))
    return -1;

  if (READREG_A0(h, LIBSFP_OFS_A0_VENDOR_NAME,
                    LIBSFP_LEN_A0_VENDOR_NAME, &info->vendor))
    return -1;
  info->vendor[16] = 0;

  if (READREG_A0(h, LIBSFP_OFS_A0_VENDOR_PN,
                    LIBSFP_LEN_A0_VENDOR_PN, &info->partnum))
    return -1;
  info->partnum[16] = 0;

  if (READREG_A0(h, LIBSFP_OFS_A0_DIAGMON_TYPE, 1, &dmtype))
    return -1;

  if (!(dmtype & LIBSFP_A0_DIAGMON_TYPE_DDM))
    return 0;  

  if (READREG_A2(h, LIBSFP_OFS_A2_DIAGNOSTICS_TXPOWER, 2, &tp))
    return -1;

  if (READREG_A2(h, LIBSFP_OFS_A2_DIAGNOSTICS_RXPOWER, 2, &rp))
    return -1;

  if (dmtype & LIBSFP_A0_DIAGMON_TYPE_EXCAL) {

    /* Module power Externally calibrated
     * read calibration values */

    if (READREG_A2(h, LIBSFP_OFS_A2_EXT_CAL_RXPWR,
                   sizeof(rx_pwr), rx_pwr))
      return -1;

    if (READREG_A2(h, LIBSFP_OFS_A2_EXT_CAL_TXPWR_SLOPE,
                   sizeof(txpwr_slope), &txpwr_slope))
      return -1;

    if (READREG_A2(h, LIBSFP_OFS_A2_EXT_CAL_TXPWR_OFFSET,
                   sizeof(txpwr_offset), &txpwr_offset))
      return -1;

    info->txpower = libsfp_get_txpower(tp, &txpwr_slope, &txpwr_slope);
    info->rxpower = libsfp_get_rxpower(rp, rx_pwr);

  } else {
    info->txpower = libsfp_get_txpower(tp, 0, 0);
    info->rxpower = libsfp_get_rxpower(rp, 0);
  }

  return 0;
}

/**
 * @brief Get SFP module max speed (See LIBSFP_SPEED_MODE_* constants)
 * @param h      - library handle
 * @param smode  - speed
 * @return 0 on success
 */
int libsfp_get_speed_mode(libsfp_t *h, uint32_t *smode)
{
  uint8_t br, tr[8];
  if (READREG_A0(h, LIBSFP_OFS_A0_BR_NOMINAL, 1, &br))
    return -1;

  (*smode) = libsfp_bitrate2speed_mode(br);    

  if ((*smode) == LIBSFP_SPEED_MODE_UNKNOWN) {

    if (READREG_A0(h, LIBSFP_OFS_A0_TRANSCEIVER,
                      LIBSFP_LEN_A0_TRANSCEIVER, tr))
      return -1;

    if ((tr[0]&0xF0)) {
      (*smode) = LIBSFP_SPEED_MODE_10G;
      return 0;
    }

    if ((tr[3]&0x0F))
      (*smode) = LIBSFP_SPEED_MODE_1G;
  }

  return 0;
}

/**
 * @brief Check that SFP module is a copper ethernet module
 * @param h   - library handle
 * @param ans - answer (0/1 - Other/Copper ethernet)
 * @return 0 on success
 */
int libsfp_is_copper_eth(libsfp_t *h, uint8_t *ans)
{
  if (READREG_A0(h, LIBSFP_OFS_A0_TRANSCEIVER+3, 1, ans))
    return -1;

  (*ans) = ((*ans) & 0x08) ? 1 : 0;
  return 0;
}

/**
 * @brief Check that SFP module is Dirrect Attach cable module
 * @param h   - library handle
 * @param ans - answer (0/1 - Normal/Dirrect Attach)
 * @return 0 on success
 */
int libsfp_is_directattach(libsfp_t *h, uint8_t *ans)
{
  (*ans) = 0;
  uint8_t v;

  if (READREG_A0(h, LIBSFP_OFS_A0_CONNECTOR, 1, &v))
    return -1;

  if (v != LIBSFP_A0_CONNECTOR_COPPER)  /* Cooper */
    return 0;

  if (READREG_A0(h, LIBSFP_OFS_A0_TRANSCEIVER+5, 1, &v)) /* Passive cable */
    return -1;

  if (!(v & 4))
    return 0;

  (*ans) = 1;
  return 0;
}

/**
 * @brief Get copper length in case of copper ethernet module
 * @param h    -  library handle
 * @param ans  -  length of cable (m)
 * @return 0 on success
 */
int libsfp_get_copper_length(libsfp_t *h, uint8_t *ans)
{
  if (READREG_A0(h, LIBSFP_OFS_A0_LENGTH_CABLE, 1, ans))
    return -1;

  return 0;
}


/**
 * @brief Get SFP module pins state (if supported)
 * @param h      library handle
 * @param value  pointer to bit value\n
 *               see LIBSFP_A2_STATUSCONTROL_* constants
 * @return 0 on success
 */
int libsfp_get_pins_state(libsfp_t *h, uint8_t *value)
{
  uint8_t dmtype;

  if (READREG_A0(h, LIBSFP_OFS_A0_DIAGMON_TYPE, 1, &dmtype))
    return -1;

  if ((!(dmtype & LIBSFP_A0_DIAGMON_TYPE_DDM)))
    return -1;

  if (READREG_A2(h, LIBSFP_OFS_A2_STATUSCONTROL, 1, value))
    return -1;

  /* Clear bits not corresponding for pin states */
  (*value) &= ~(LIBSFP_A2_STATUSCONTROL_TXD_SET | LIBSFP_A2_STATUSCONTROL_RS0_SET);

  return 0;
}

/**
 * @brief Set SFP module soft pins (if supported)
 * @param h      library handle
 * @param mask   bit mask to set \n
 *               see LIBSFP_A2_STATUSCONTROL_*_SET constants
 * @param value  bit value\n
 *               see LIBSFP_A2_STATUSCONTROL_*_SET constants
 * @return 0 on success
 */
int libsfp_set_soft_pins_state(libsfp_t *h, uint8_t mask, uint8_t value)
{
  uint8_t v, m;

  if (READREG_A0(h, LIBSFP_OFS_A0_DIAGMON_TYPE, 1, &v))
    return -1;

  if (!(v & LIBSFP_A0_DIAGMON_TYPE_DDM))
    return -1;

  if (READREG_A0(h, LIBSFP_OFS_A0_ENHANCED_OPTIONS, 1, &v))
    return -1;

  if (v & LIBSFP_A0_ENHANCED_OPTIONS_TXDIS)
    m |= LIBSFP_A2_STATUSCONTROL_TXD_SET;

  if (v & LIBSFP_A0_ENHANCED_OPTIONS_RATESEL)
    m |= LIBSFP_A2_STATUSCONTROL_RS0_SET;

  /* Check that operation supported */
  if (!m)
    return -1;

  /* only this bits can be set */
  mask &= m;
  value &= m;

  /* if nothing to do  exit */
  if (!mask)
    return 0;

  if (READREG_A2(h, LIBSFP_OFS_A2_STATUSCONTROL, 1, &v))
    return -1;

  v &= ~mask;
  v |= value;

  if (WRITEREG_A2(h, LIBSFP_OFS_A2_STATUSCONTROL, 1, &v))
    return 1;

  return 0;
}

