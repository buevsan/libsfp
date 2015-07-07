#include "libsfp_int.h"
#include <stdarg.h>

static void libsfp_printf_value( libsfp_int_t *h, const char *format, ... );

#define SFPPRINT(h, format, ...) libsfp_printf_value(H(h), format, ##__VA_ARGS__)
#define SFPPRINTNAME(h, name) H(h)->printname(H(h)->udata, name)
#define SFPNEWLINE(h) H(h)->printnewline(H(h)->udata)
#define LIBSFP_VLFMT "%35s"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

char degree_s[]="C ";
char volts_s[]="V ";
char mAmps_s[]="mA ";
char mVats_s[]="mW ";
char uVats_s[]="uW ";
char uAmps_s[]="uA ";

typedef char *(*libsfp_u8_to_str_fun)(uint8_t);
typedef void (*libsfp_u8_to_str_fun2)(char *, uint8_t);
typedef void (*libsfp_u16_to_str_fun)(char *, libsfp_u16_field_t);

typedef struct  {
  uint8_t value;
  char *text;
} libsfp_u8_tbl_t;

typedef struct {
  char *name;
  char *units_name;
  libsfp_u8_to_str_fun2 v2s;
} libsfp_u8_tbl2_t;

typedef struct {
  uint8_t byte;
  uint8_t bit;
  char *longname;
  char *shortname;
} libsfp_bitoptions_table_t;

typedef struct {
  char *name;
} libsfp_floattbl_t;

typedef struct {
  char *name;
  char *units_name;
  libsfp_u16_to_str_fun v2s;
} libsfp_u16_tbl_t;

char *libsfp_u8_to_str(libsfp_u8_tbl_t *table, uint16_t count, uint8_t value)
{
  uint16_t i;
  for (i=0; i < count; ++i)
    if (table[i].value == value)
      return table[i].text;
  return 0;
}

void libsfp_print_u8_f(libsfp_t *h, char *value_str, char *name, uint8_t value)
{
  if (value_str) {
    SFPPRINTNAME(h, name);
    SFPPRINT(h, "%s",value_str);
  } else
    if (H(h)->flags & LIBSFP_FLAGS_PRINT_UNKNOWN) {
     SFPPRINTNAME(h, name);
     SFPPRINT(h, "Unknown");
    }

  if ( !((value_str) || (H(h)->flags & LIBSFP_FLAGS_PRINT_UNKNOWN)) )
    return;

  if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
    SFPPRINT(h, " (%02x)", (uint16_t)value);

  SFPNEWLINE(h);
}

int libsfp_print_ascii(libsfp_t *h, char *name, void *data, uint16_t count)
{
  int i;
  SFPPRINTNAME(h, name);
  for (i = 0;  i < count; ++i)
    SFPPRINT(h,  "%c", ((uint8_t*)data)[i]);
  SFPNEWLINE(h);
  return 0;
}

int libsfp_print_dump(libsfp_t *h, void *data, uint16_t count)
{
  int i;
  for (i = 0;  i < count; ++i)
    SFPPRINT(h, "%02X", (uint16_t)(((uint8_t*)data)[i]));
  return 0;
}

int libsfp_print_hex(libsfp_t *h, char *name, void *data, uint16_t count)
{
  SFPPRINTNAME(h, name);
  int i;
  for (i = 0;  i < count; ++i) {
    SFPPRINT(h, "%02X ", (uint16_t)(((uint8_t*)data)[i]));
    if (!((i+1)%16)) {
      SFPNEWLINE(h);
      SFPPRINT(h, LIBSFP_VLFMT," ");
    }
  }
  if (((i+1)%16))
     SFPNEWLINE(h);
  return 0;
}

void libsfp_print_uint8(libsfp_t *h, char *name, uint8_t v)
{
  SFPPRINTNAME(h, name);
  SFPPRINT(h, "%02xh", (uint16_t)v);
  SFPNEWLINE(h);
}

void libsfp_print_bitoptions(libsfp_t *h, char * name, libsfp_bitoptions_table_t *tbl, uint16_t count, uint8_t *data)
{
  uint8_t i, max=0, min=0xFF; int offset;

  if (!(H(h)->flags & LIBSFP_FLAGS_PRINT_BITOPTIONS))
    return;

  SFPPRINTNAME(h, name);

  if ((H(h)->flags&LIBSFP_FLAGS_PRINT_LONGOPT))
    SFPNEWLINE(h);

  for (i=0; i< count; ++i) {

    offset = tbl[i].byte-tbl[0].byte;

    if (max < offset)
       max = offset;
    if (min > offset)
       min = offset;

    if ((data[offset])&(0x1<<(tbl[i].bit))) {


      if (H(h)->flags&LIBSFP_FLAGS_PRINT_LONGOPT) {

        SFPPRINT(h, LIBSFP_VLFMT," ");
        /*SFPPRINTNAME(h, "-");*/
        if (tbl[i].longname[0]) {
          SFPPRINT(h, "%s", tbl[i].longname);
          SFPNEWLINE(h);
        } else {
          SFPPRINT(h, "(%u/%u)", (uint8_t)tbl[i].byte, (uint8_t)tbl[i].bit);
          SFPNEWLINE(h);
        }

      } else {
        if (tbl[i].shortname[0])
          SFPPRINT(h, "%s ", tbl[i].shortname);
      }

    }
  }

  if (H(h)->flags&LIBSFP_FLAGS_PRINT_HEXOUTPUT) {
    if (H(h)->flags&LIBSFP_FLAGS_PRINT_LONGOPT)
      SFPPRINT(h, LIBSFP_VLFMT," ");
    else
      SFPPRINT(h, " ");
    SFPPRINT(h, "(");
    libsfp_print_dump(h, &data[min], max-min+1);
    SFPPRINT(h, ")");
    SFPNEWLINE(h);
  } else
    if (!(H(h)->flags&LIBSFP_FLAGS_PRINT_LONGOPT))
      SFPNEWLINE(h);
}


void libsfp_print_float(libsfp_t *h, char *name, libsfp_u32_field_t f)
{
  uint32_t v;
  SFPPRINTNAME(h, name);
  v = ((f.d[0])<<24) | ((f.d[1])<<16) | ((f.d[2])<<8) | (f.d[3]);

  SFPPRINT(h, "%.2f", (float)v);
  if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
    SFPPRINT(h, " (%08X)",v);
  SFPNEWLINE(h);
}

void libsfp_print_float_table(libsfp_t *h, libsfp_floattbl_t *tbl, uint16_t cnt,  void *data)
{
  uint8_t i;
  libsfp_u32_field_t *f = (libsfp_u32_field_t *)data;

  for (i = 0; i < cnt; ++i, ++f )
    libsfp_print_float(h, tbl[i].name, *f);
}


/* Identifier */

libsfp_u8_tbl_t identifier_tbl[] = {
  {0x01, "GBIC"},
  {0x02, "SFF"},
  {0x03, "SFP or SFP+"},
};

char *libsfp_identifier2s(uint8_t id)
{
  return libsfp_u8_to_str(identifier_tbl,
                         ARRAY_SIZE(identifier_tbl), id);
}

void libsfp_print_identifier(libsfp_t *h, uint8_t id)
{
  libsfp_print_u8_f(h, libsfp_identifier2s(id), "Identifier", id);
}

/* Ext Identifier */

libsfp_u8_tbl_t extidentifier_tbl[] = {
  {0x00, "GBIC definition is not specified"},
  {0x01, "GBIC is compliant with MOD_DEF 1"},
  {0x02, "GBIC is compliant with MOD_DEF 2"},
  {0x03, "GBIC is compliant with MOD_DEF 3"},
  {0x04, "GBIC/SFP function is defined by two-wire interface ID only"},
  {0x05, "GBIC is compliant with MOD_DEF 5"},
  {0x06, "GBIC is compliant with MOD_DEF 6"},
  {0x07, "GBIC is compliant with MOD_DEF 7"}
};

char *libsfp_extidentifier2s(uint8_t id)
{
  return libsfp_u8_to_str(extidentifier_tbl,
                         ARRAY_SIZE(extidentifier_tbl), id);
}

void libsfp_print_extidentifier(libsfp_t *h, uint8_t id)
{
  libsfp_print_u8_f(h, libsfp_extidentifier2s(id), "Ext. identifier", id);
}

/* Connector */

libsfp_u8_tbl_t connector_tbl[] = {
    { 0x01,"SC"},
    { 0x02,"Fiber style 1"},
    { 0x03,"Fiber style 2"},
    { 0x04,"BNC/TNC"},
    { 0x05,"Fiber coaxial"},
    { 0x06,"FiberJack"},
    { 0x07,"LC"},
    { 0x08,"MT-RJ"},
    { 0x09,"MU"},
    { 0x0A,"SG"},
    { 0x0B,"Optical pigtail"},
    { 0x0C,"MPO Paralel opt"},
    { 0x20,"HSSDC 2"},
    { 0x21,"Copper"},
    { 0x22,"RJ45"}
};

char *libsfp_connector2s(uint8_t v)
{
  return libsfp_u8_to_str(connector_tbl,
                         ARRAY_SIZE(connector_tbl), v);
}

void libsfp_print_connector(libsfp_t *h, uint8_t v)
{
  libsfp_print_u8_f(h, libsfp_connector2s(v), "Connector", v);
}


libsfp_bitoptions_table_t trns_table[]= {
 {3, 7, "10G Base-ER","10G Base-ER"},
 {3, 6, "10G Base-LRM","10G Base-LRM"},
 {3, 5, "10G Base-LR","10G Base-LR"},
 {3, 4, "10G Base-SM","10G Base-SM"},
 {3, 3, "1X SX",""},
 {3, 2, "1X LX",""},
 {3, 1, "1X Copper Active",""},
 {3, 0, "1X Copper Passive",""},
 {4, 7, "",""},
 {4, 6, "",""},
 {4, 5, "",""},
 {4, 4, "",""},
 {4, 3, "",""},
 {4, 2, "",""},
 {4, 1, "",""},
 {4, 0, "",""},
 {5, 7, "",""},
 {5, 6, "",""},
 {5, 5, "",""},
 {5, 4, "",""},
 {5, 3, "",""},
 {5, 2, "",""},
 {5, 1, "",""},
 {5, 0, "",""},
 {6, 7, "BASE-PX","BASE-PX"},
 {6, 6, "BASE-BX10","BASE-BX10"},
 {6, 5, "100BASE-FX","100BASE-FX"},
 {6, 4, "100BASE-LX/LX10","100BASE-LX/LX10"},
 {6, 3, "1000BASE-T","1000BASE-T"},
 {6, 2, "1000BASE-CX","1000BASE-CX"},
 {6, 1, "1000BASE-LX","1000BASE-LX"},
 {6, 0, "1000BASE-SX","1000BASE-SX"},
 {7, 7, "Very long distance","V"},
 {7, 6, "Short distance","S"},
 {7, 5, "Intermediate distance","I"},
 {7, 4, "Long distance","L"},
 {7, 3, "mediaum distance","M"},
 {7, 2, "Shortwave laser linear RX","SA"},
 {7, 1, "Longwave laser","LC"},
 {7, 0, "EL",""},
 {8, 7, "",""},
 {8, 6, "Shortwave laser w/o OFC","SN"},
 {8, 5, "Shortwave laser with OFC","SL"},
 {8, 4, "Longwave laser","LL"},
 {8, 3, "Active Cable",""},
 {8, 2, "Passive Cable",""},
 {8, 1, "",""},
 {8, 0, "",""},
 {9, 7, "Twin axial pair","TW"},
 {9, 6, "Twisted pair","TP"},
 {9, 5, "Miniature","MI"},
 {9, 4, "Video Coax","TV"},
 {9, 3, "Multimode 62.6um","M6"},
 {9, 2, "Multimode 50um","M5"},
 {9, 1, "",""},
 {9, 0, "Single Mode","SM"},
 {10, 7, "1200 Mbyte/s",""},
 {10, 6, "800 Mbyte/s",""},
 {10, 5, "1600 Mbyte/s",""},
 {10, 4, "400 Mbyte/s",""},
 {10, 3, "",""},
 {10, 2, "200 Mbyte/s",""},
 {10, 1, "",""},
 {10, 0, "100 Mbyte/s",""}
};

void libsfp_print_transeiver(libsfp_t *h, uint8_t *data)
{
  libsfp_print_bitoptions(h,"Transeiver", trns_table,
                          ARRAY_SIZE(trns_table),
                         data);
}

/* Encoding */

libsfp_u8_tbl_t encoding_tbl[] = {
  {0x01, "8B/10B"},
  {0x02, "4B/5B"},
  {0x03, "NRZ"},
  {0x04, "Manchester"},
  {0x05, "Sonet Scrambled"},
  {0x06, "64B/66B"}
};

char *libsfp_encoding2s(uint8_t en)
{
  return libsfp_u8_to_str(encoding_tbl,
                         ARRAY_SIZE(encoding_tbl), en);
}

void libsfp_print_encoding(libsfp_t *h, uint8_t en)
{
  libsfp_print_u8_f(h, libsfp_encoding2s(en), "Encoding", en);
}

/* BR nomimal */

void libsfp_brnominal2s(char *s, uint8_t brn)
{
  sprintf(s, "%u", brn*100);
}

void libsfp_print_brnominal(libsfp_t *h, uint8_t brn)
{
  libsfp_brnominal2s(H(h)->sbuf, brn);
  SFPPRINTNAME(h, "Bit rate nominal");
  SFPPRINT(h, "%s MBits/s", H(h)->sbuf);
  if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
    SFPPRINT(h, " (%02X)", (uint16_t)brn);
  SFPNEWLINE(h);
}

/* Rate identifier */

libsfp_u8_tbl_t rate_identifier_tbl[] = {
  {0x01, "SFF-8079 (4/2/1G Rate_Select & AS0/AS1)"},
  {0x02, "SFF-8431 (8/4/2G Rx Rate_Select only)"},
  {0x04, "SFF-8431 (8/4/2G Tx Rate_Select only)"},
  {0x06, "SFF-8431 (8/4/2G Independent Rx & Tx Rate_select)"},
  {0x08, "FC-PI-5 (16/8/4G Rx Rate_select only) High=16G only, Low=8G/4G"},
  {0x0A, "FC-PI-5 (16/8/4G Independent Rx, Tx Rate_select) High=16G only, Low=8G/4G"},
  {0x0C, "FC-PI-6 (32/16/8G Independent Rx, Tx Rate_Select)"}
};

char *libsfp_rate_identifier2s(uint8_t rid)
{
  return libsfp_u8_to_str(rate_identifier_tbl,
                         ARRAY_SIZE(rate_identifier_tbl), rid);
}

void libsfp_print_rate_identifier(libsfp_t *h, uint8_t rid)
{
  libsfp_print_u8_f(h, libsfp_rate_identifier2s(rid), "Rate identifier",rid);
}

/* Length SM km */

void libsfp_length_km2s(char *s, uint8_t l)
{
  sprintf(s, "%u", l);
}

/* Length 100m */

void libsfp_length_100m2s(char *s, uint8_t l)
{
  sprintf(s, "%u", l*100);
}

/* Length 50um */

void libsfp_length_50um2s(char *s, uint8_t l)
{
  sprintf(s, "%u", l*10);
}

/* Length 62.5um */

void libsfp_length_625um2s(char *s, uint8_t l)
{
  sprintf(s, "%u", l*10);
}

/* Length active */

void libsfp_length_active2s(char *s, uint8_t l)
{
  sprintf(s, "%u", l);
}

/* Length active */

void libsfp_length_50um_om3_2s(char *s, uint8_t l)
{
  sprintf(s, "%u", l*10);
}

libsfp_u8_tbl2_t lengths_table[]= {
  {"Length SM-km", "km", libsfp_length_km2s},
  {"Length SM-100m", "m", libsfp_length_100m2s},
  {"Length MM (500MHz*km at 850nm)", "m", libsfp_length_50um2s},
  {"Length MM (200 MHz*km-850nm)", "m", libsfp_length_625um2s},
  {"Length Copper", "m", libsfp_length_active2s},
  {"Length MM (2000 Mhz*km)", "m", libsfp_length_50um_om3_2s}
};

void libsfp_print_lengths(libsfp_t *h, uint8_t *d, char laser)
{
  uint16_t i;

  for (i=0; i < ARRAY_SIZE(lengths_table); ++i, ++d) {

    if ( !( (*d) || (H(h)->flags & LIBSFP_FLAGS_PRINT_UNKNOWN)) )
      continue;

    if (H(h)->flags & LIBSFP_FLAGS_PRINT_LASERAUTO) {

      if ((laser) && (i==4))
        continue;
      if ((!laser) && (i!=4))
        continue;

    }

    lengths_table[i].v2s(H(h)->sbuf, *d);
    SFPPRINTNAME(h, lengths_table[i].name);
    SFPPRINT(h, "%s %s", H(h)->sbuf, lengths_table[i].units_name);
    if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
      SFPPRINT(h, " (%02X)", (uint16_t)(*d));
    SFPNEWLINE(h);
  }
}

/* Laser wave length */

void libsfp_wavelength2s(char *s, uint8_t *d)
{
  sprintf(s, "%u", (d[0]<<8)+d[1]);
}

void libsfp_print_wavelength(libsfp_t *h, uint8_t *d)
{
  if ( !( (*d) || (H(h)->flags & LIBSFP_FLAGS_PRINT_UNKNOWN)) )
    return;

  libsfp_wavelength2s(H(h)->sbuf, d);
  SFPPRINTNAME(h, "Laser wave length ");
  SFPPRINT(h, "%s nm", H(h)->sbuf);
  if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
    SFPPRINT(h, " (%04X)", *((uint16_t*)d));
  SFPNEWLINE(h);
}

/*---  Extended fields --- */
/*---  Options --- */

libsfp_bitoptions_table_t opts_table[]= {
 {64, 2, "Cooled Transceiver","CT"},
 {64, 1, "Power level 2","PW2"},
 {64, 0, "Linear Receiver Output","LRO"},
 {65, 5, "Rate Select","RS"},
 {65, 4, "TX Disable","TXD"},
 {65, 3, "TX Fault","TXF"},
 {65, 2, "Signal detect","SD"},
 {65, 1, "Lost of signal","LS"}
};

void libsfp_print_options(libsfp_t *h, uint8_t *data)
{
  libsfp_print_bitoptions(h,"Options", opts_table,
                          ARRAY_SIZE(opts_table),
                         data);
}

/* BR max BR min */

void libsfp_brminmax2s(char *s, uint8_t brnominal, uint8_t br)
{
  sprintf(s, "%u", brnominal/100*br);
}

void libsfp_print_brminmax(libsfp_t *h, char *name, uint8_t br_nominal, uint8_t br)
{
  if ( !( (br) || (H(h)->flags & LIBSFP_FLAGS_PRINT_UNKNOWN)) )
    return;

  libsfp_brminmax2s(H(h)->sbuf, br_nominal, br);
  SFPPRINTNAME(h, name);
  SFPPRINT(h, "%s Mbits/s", H(h)->sbuf);
  if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
    SFPPRINT(h, " (%02X)", (uint16_t)br);
  SFPNEWLINE(h);
}

/* Date code */

void libsfp_datecode2s(char *s, uint8_t *d)
{
  sprintf(s, "%c%c.%c%c.%c%c %c%c", d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]);
}

void libsfp_print_datecode(libsfp_t *h, uint8_t *d)
{
  libsfp_datecode2s(H(h)->sbuf, d);
  SFPPRINTNAME(h, "Date code");
  SFPPRINT(h, "%s", H(h)->sbuf);
  SFPNEWLINE(h);
}

libsfp_bitoptions_table_t montype_table[]= {
 {93, 7, "Legacy diagnostic","LDI"},
 {93, 6, "Monitoring implemented","MON"},
 {93, 5, "Internally calibrated","INC"},
 {93, 4, "Externally calibrated","EXC"},
 {93, 3, "Average power","APW"},
 {93, 2, "Address change required","ACH"},
};

void libsfp_print_montype(libsfp_t *h, uint8_t *data)
{
  libsfp_print_bitoptions(h,"Monitoring type", montype_table,
                          ARRAY_SIZE(montype_table),
                         data);
}

libsfp_bitoptions_table_t eoptions_table[]= {
 {93, 7, "Alarm/warning flags","AWF"},
 {93, 6, "Soft TX Disable","TXD"},
 {93, 5, "Soft TX Fault","TXF"},
 {93, 4, "Soft RX Los ","RXL"},
 {93, 3, "Soft Rate select","RS"},
 {93, 2, "Application Select SFF-8079","AS"},
 {93, 1, "Soft Rate select SFF-8431","RSF"}
};

void libsfp_print_eoptions(libsfp_t *h, uint8_t *data)
{
  libsfp_print_bitoptions(h,"Enhanced options", eoptions_table,
                          ARRAY_SIZE(eoptions_table),
                         data);
}

/* Rate identifier */

libsfp_u8_tbl_t sff8472compliance_tbl[] = {
  {0x00, "Functionality not included"},
  {0x01, "Rev 9.3"},
  {0x02, "Rev 9.5"},
  {0x03, "Rev 10.2"},
  {0x04, "Rev 10.4"},
  {0x05, "Rev 11.0"},
};

char *libsfp_sff8472compliance2s(uint8_t v)
{
  return libsfp_u8_to_str(sff8472compliance_tbl,
                         ARRAY_SIZE(sff8472compliance_tbl), v);
}

void libsfp_print_sff8472compliance(libsfp_t *h, uint8_t v)
{
  libsfp_print_u8_f(h, libsfp_sff8472compliance2s(v), "SFF-8472 compliance",v);
}

/**
 * @brief Print csum relay stored in memory and calculated by
 * @param h      Library handle
 * @param name   Name to print
 * @param data   Data to calc checksum
 * @param size   Data size
 * @param value  Checksum value stored in memory
 */
void libsfp_print_csum(libsfp_t *h, char *name, void *data, uint16_t size, uint8_t v)
{
  uint8_t calc_sum;
  calc_sum = libsfp_calc_csum(data, size);
  SFPPRINTNAME(h, name);
  if (v == calc_sum)
    snprintf(H(h)->sbuf, sizeof(H(h)->sbuf), "%02X", v);
  else
    snprintf(H(h)->sbuf, sizeof(H(h)->sbuf), "%02X (Expected: %02X)", v, calc_sum);
  SFPPRINT(h, "%s", H(h)->sbuf);
  SFPNEWLINE(h);
}

void libsfp_print_base_fields(libsfp_t *h, libsfp_base_fields_t *bf)
{
  int laser;

  laser = (H(h)->flags & LIBSFP_FLAGS_PRINT_LASERAUTO)?libsfp_is_laser_availble(bf):1;

  libsfp_print_identifier(h, bf->identifier);
  libsfp_print_extidentifier(h, bf->ext_identifier);
  libsfp_print_connector(h, bf->connector);
  libsfp_print_transeiver(h, bf->transceiver);
  libsfp_print_encoding(h, bf->encoding);
  libsfp_print_brnominal(h, bf->br_nominal);
  libsfp_print_rate_identifier(h, bf->rate_identifier);

  libsfp_print_lengths(h, &bf->length_smf_km, laser);

  libsfp_print_ascii(h, "Vendor",
                     bf->vendor_name,
                     sizeof(bf->vendor_name));

  libsfp_print_ascii(h, "Vendor PN",
                     bf->vendor_pn,
                     sizeof(bf->vendor_pn));

  libsfp_print_hex(h, "Vendor OUI",
                     bf->vendor_oui,
                     sizeof(bf->vendor_oui));
  if (laser)
    libsfp_print_wavelength(h, bf->wavelength.d);  

  if ( (H(h)->flags) & LIBSFP_FLAGS_PRINT_CSUM )
    libsfp_print_csum(h, "Checksum base", bf,
                      sizeof(*bf)- sizeof(bf->cc_base), bf->cc_base);

}

void libsfp_print_ext_fields(libsfp_t *h, libsfp_extended_fields_t *ef, uint8_t br_nominal)
{
  libsfp_print_options(h, ef->options.d);
  libsfp_print_brminmax(h, "Maximum bitrate", br_nominal, ef->br_max);
  libsfp_print_brminmax(h, "Minimum bitrate", br_nominal, ef->br_min);
  libsfp_print_ascii(h, "Vendor SN",
                     ef->vendor_sn,
                     sizeof(ef->vendor_sn));
  libsfp_print_datecode(h, ef->date_code);
  libsfp_print_montype(h, &ef->diag_mon_type);
  libsfp_print_eoptions(h, &ef->en_options);
  libsfp_print_sff8472compliance(h, ef->sff8472_comp);


  if ( (H(h)->flags) & LIBSFP_FLAGS_PRINT_CSUM )
    libsfp_print_csum(h, "Checksum ext",
                      ef, sizeof(*ef) - sizeof(ef->cc_ext), ef->cc_ext);


}

void libsfp_temp2s(char *s, libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  sprintf(s, "%.3f", libsfp_get_temp(v, cal));
}

void libsfp_voltage2s(char *s, libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  sprintf(s, "%.3f", libsfp_get_voltage(v, cal));
}

void libsfp_txpower2s(char *s, libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  sprintf(s, "%.3f", libsfp_get_txpower(v,
                                        (cal) ? &cal->tx_pwr_slope : 0,
                                        (cal) ? &cal->tx_pwr_offset : 0 ));
}

void libsfp_rxpower2s(char *s, libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  sprintf(s, "%.3f", libsfp_get_rxpower(v, (cal) ? cal->rx_pwr : 0));
}

void libsfp_biascurrent2s(char *s, libsfp_u16_field_t v, libsfp_calibration_fields_t *cal)
{
  sprintf(s, "%.3f", libsfp_get_biascurrent(v, cal));
}

typedef void (*libsfp_uint16cal2s_fun)(char *, libsfp_u16_field_t, libsfp_calibration_fields_t *cal);

typedef struct {
  char *name;
  char *units_name;
  libsfp_uint16cal2s_fun v2s;
} sfp_threshold_tbl_t;


sfp_threshold_tbl_t th_table[]={
  {"Temperature alarm", degree_s, libsfp_temp2s},
  {"Temperature warning", degree_s, libsfp_temp2s},
  {"Voltage alarm", volts_s, libsfp_voltage2s},
  {"Voltage warning", volts_s, libsfp_voltage2s},
  {"Bias current alarm", mAmps_s, libsfp_biascurrent2s},
  {"Bias current warning", mAmps_s, libsfp_biascurrent2s},
  {"TX power alarm", mVats_s, libsfp_txpower2s},
  {"TX power warning", mVats_s, libsfp_txpower2s},
  {"RX power alarm", mVats_s, libsfp_rxpower2s},
  {"RX power warning", mVats_s, libsfp_rxpower2s}
};

void libsfp_print_thresholds(libsfp_t *h, libsfp_u16_field_t * f, libsfp_calibration_fields_t *cal)
{
  uint8_t i;
  libsfp_u16_field_t *hf;

  if (!(H(h)->flags & LIBSFP_FLAGS_PRINT_THRESHOLDS))
    return;

  for (i = 0; i < ARRAY_SIZE(th_table); ++i) {
    SFPPRINTNAME(h, th_table[i].name);
    hf = (f+1);
    th_table[i].v2s(H(h)->sbuf, *(f+1), cal);
    SFPPRINT(h, "%s - ",H(h)->sbuf);
    th_table[i].v2s(H(h)->sbuf, *(f), cal);
    SFPPRINT(h, "%s %s", H(h)->sbuf, th_table[i].units_name);
    if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
      SFPPRINT(h, "(%04X %04X)",
               ((f->d[1]<<8) | f->d[0]),
               ((hf->d[1]<<8) | hf->d[0]));
    SFPNEWLINE(h);
    f+=2;
  }

}

void libsfp_calpwr2s(char *s, libsfp_u32_field_t f)
{
  sprintf(s, "%.2f", libsfp_get_rxpwr(f));
}

void libsfp_print_calpwr(libsfp_t *h, libsfp_u32_field_t * f)
{
  uint8_t i;

  SFPPRINTNAME(h, "RX_PWR 4/3/2/1/0");
  for (i = 0; i < 5; ++i, ++f ) {
    libsfp_calpwr2s(H(h)->sbuf, *f);
    SFPPRINT(h, "%s", H(h)->sbuf);
    if (i!=4)
      SFPPRINT(h, "/");
  }

  if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT) {
    SFPNEWLINE(h);
    SFPPRINT(h, LIBSFP_VLFMT," ");
    SFPPRINT(h, "(");
    for (i = 0; i < 5; ++i, ++f ) {
        libsfp_calpwr2s(H(h)->sbuf, *f);
        SFPPRINT(h, "%08X", f->u32);
        if (i!=4)
          SFPPRINT(h, "/");
    }
    SFPPRINT(h, ")");
  }

  SFPNEWLINE(h);
}

libsfp_u16_tbl_t slopeoffset_table[]={
  {"Bias current slope/offset", "", NULL},
  {"Power slope/offset", "", NULL},
  {"Temperature slope/offset", "", NULL},
  {"Voltage slope/offset", "", NULL},
};

void libsfp_slope2s(char *s, libsfp_u16_field_t f)
{
  sprintf(s, "%.4f", libsfp_get_slope(f));
}

void libsfp_offset2s(char *s, libsfp_u16_field_t f)
{
  sprintf(s, "%.0f", libsfp_get_offset(f));
}

void libsfp_print_slopeoffset(libsfp_t *h, libsfp_u16_field_t *f)
{
  uint8_t i;
  libsfp_u16_field_t *nf;

  for (i = 0; i < ARRAY_SIZE(slopeoffset_table); ++i, f+=2 ) {
    nf = (f+1);
    libsfp_slope2s(H(h)->sbuf, *f);

    SFPPRINTNAME(h, slopeoffset_table[i].name);
    SFPPRINT(h, "%s / ", H(h)->sbuf);
    libsfp_offset2s(H(h)->sbuf, *nf);

    SFPPRINT(h, "%s", H(h)->sbuf);
    if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
      SFPPRINT(h, " (%04X %04X)",((f->d[0])<<8) | (f->d[1]), ((nf->d[0])<<8) | (nf->d[1]));
    SFPNEWLINE(h);
  }
}

void libsfp_print_calibrations(libsfp_t *h, libsfp_calibration_fields_t *data)
{
  if (H(h)->flags & LIBSFP_FLAGS_PRINT_CALIBRATIONS) {
    libsfp_print_calpwr(h, data->rx_pwr);
    libsfp_print_slopeoffset(h, &data->txi_slope);
  }
}

typedef struct {
  char *name;
  char *units_name;
  libsfp_uint16cal2s_fun v2s;
} sfp_analog_tbl_t;

sfp_analog_tbl_t analogvalues_table[]={
  {"Temperature", degree_s, libsfp_temp2s},
  {"Voltage", volts_s, libsfp_voltage2s},
  {"Bias current", mAmps_s, libsfp_biascurrent2s},
  {"TX power", mVats_s, libsfp_txpower2s},
  {"RX power", mVats_s, libsfp_rxpower2s}
};

typedef struct  {
  uint8_t abyte;
  uint8_t ahbit;
  uint8_t albit;
  uint8_t wbyte;
  uint8_t whbit;
  uint8_t wlbit;
} analogvalues_aw_table_t;

analogvalues_aw_table_t analogvalues_aw_table[]={
  {112, 7, 6, 116, 7, 6},
  {112, 5, 4, 116, 5, 4},
  {112, 3, 2, 116, 3, 2},
  {112, 1, 0, 116, 1, 0},
  {113, 7, 6, 117, 7, 6}
};

void libsfp_analogvalue2s(char *s, libsfp_u16_field_t v)
{
  sprintf(s, "%u", ((v.d[0]<<8) | v.d[1]));
}

void libsfp_print_analog_values(libsfp_t *h, sfp_analog_tbl_t *tbl, analogvalues_aw_table_t *tbl2, uint16_t cnt,
                                libsfp_rtdiagnostics_fields_t *rt, libsfp_calibration_fields_t *cal)
{
  uint8_t i;
  libsfp_u16_field_t *f = &rt->temperature;
  uint8_t *data = (uint8_t*)rt;

  for (i = 0; i < cnt; ++i, ++f ) {

    /* print values */
    tbl[i].v2s(H(h)->sbuf, *f, cal);
    SFPPRINTNAME(h, tbl[i].name);
    SFPPRINT(h, "%s %s", H(h)->sbuf, tbl[i].units_name);
    if (H(h)->flags & LIBSFP_FLAGS_PRINT_HEXOUTPUT)
      SFPPRINT(h, "(%04X)", ((f->d[0])<<8) | (f->d[1]));

    /* print alarm warning status */
    if (tbl2) {
      if ( (data[tbl2->abyte]&(1<<(tbl2->ahbit))) ||
           (data[tbl2->abyte]&(1<<(tbl2->albit))) )
        SFPPRINT(h, "Alarm!");
      else {
       if ( (data[tbl2->wbyte]&(1<<(tbl2->whbit))) ||
            (data[tbl2->wbyte]&(1<<(tbl2->wlbit))) )
         SFPPRINT(h, "Warning!");
      }
    }
    SFPNEWLINE(h);
  }
}

libsfp_bitoptions_table_t status_control_table[]= {
 {110, 7, "TX Disable","TXD"},
 {110, 5, "Rate select 1","RS1"},
 {110, 4, "Rate select 0","RS0"},
 {110, 2, "TX fault state","TXF"},
 {110, 1, "RX loss","RXL"},
 {110, 0, "Data_Ready_Bar","DR"},
 {118, 1, "Power level 2","PW2"}
};

void libsfp_print_status_control(libsfp_t *h, uint8_t *data)
{
  libsfp_print_bitoptions(h,"Status/Control", status_control_table,
                          ARRAY_SIZE(status_control_table),
                         data);
}

void libsfp_print_rtdiagnostics(libsfp_t *h, libsfp_rtdiagnostics_fields_t *rt, libsfp_extended_fields_t *ext, libsfp_calibration_fields_t *cal)
{
  libsfp_print_analog_values(h, analogvalues_table,
                             (ext->en_options&0x80)?analogvalues_aw_table:0,
                            ARRAY_SIZE(analogvalues_table),
                            rt,
                            (ext->diag_mon_type & 0x10)?cal:0);
  libsfp_print_status_control(h, &rt->status);
}

void libsfp_print_vendor_specific(libsfp_t *h, libsfp_A2_t *a2)
{
  libsfp_print_hex(h, "Vendor Specific",
                   a2->vendor_specific, sizeof(a2->vendor_specific));

  libsfp_print_hex(h, "User EEPROM",
                   a2->user_eeprom, sizeof(a2->user_eeprom));

  libsfp_print_hex(h, "Vendor Control",
                   a2->vendor_control, sizeof(a2->vendor_control));
}


/**
 * @brief Output information selected by flags
 *        as text to specified file
 * @param h     - library handle
 * @param dump  - pointer to struct that store information
 * @return 0 on success
 */
void libsfp_printinfo(libsfp_t *h, libsfp_dump_t *dump)
{
  libsfp_print_base_fields(h, &dump->a0.base);
  libsfp_print_ext_fields(h, &dump->a0.ext, dump->a0.base.br_nominal);

  if (!(dump->a0.ext.diag_mon_type & LIBSFP_A0_DIAGMON_TYPE_DDM))
    return;

  /* Print thresholds */
  if (dump->a0.ext.diag_mon_type & LIBSFP_A0_DIAGMON_TYPE_DDM)
    libsfp_print_thresholds(h, &dump->a2.th.temp_alarm_high, &dump->a2.cl);
  else
    libsfp_print_thresholds(h, &dump->a2.th.temp_alarm_high, 0);

  libsfp_print_calibrations(h, &dump->a2.cl);

  if ( (H(h)->flags) & LIBSFP_FLAGS_PRINT_CSUM )
    libsfp_print_csum(h, "Checksum dmi",
                      &dump->a2, 94, dump->a2.cc_dmi);

  libsfp_print_rtdiagnostics(h, &dump->a2.dg, &dump->a0.ext, &dump->a2.cl);

  if ( (H(h)->flags) & LIBSFP_FLAGS_PRINT_VENDOR )
    libsfp_print_vendor_specific(h, &dump->a2);
}


/**
 * The value buffer size --- 1K.
 */
#define VALBUFSIZE 1024

/**
 * @brief Calls the printvalue callback.
 *
 * @param h the     the internal library handle
 * @param format    the string format (tempalte)
 */
static void libsfp_printf_value( libsfp_int_t *h, const char *format, ... )
{
  va_list args;
  char buf[VALBUFSIZE];

  va_start( args, format );
  vsnprintf( buf, sizeof( buf ), format, args );
  va_end( args );

  h->printvalue( h->udata, buf );
}
