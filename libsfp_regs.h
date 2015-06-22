#ifndef LIBSFP_REGS_H__
#define LIBSFP_REGS_H__

/**
   @file
   @brief libsfp register constants header file

*/

/* A0 bank */

/* Offsets constants */

#define LIBSFP_OFS_A0_IDENTIFIER            0
#define LIBSFP_OFS_A0_EXTIDENTIFIER         1
#define LIBSFP_OFS_A0_CONNECTOR             2
#define LIBSFP_OFS_A0_TRANSCEIVER           3
#define LIBSFP_OFS_A0_ENCODING              11
#define LIBSFP_OFS_A0_BR_NOMINAL            12
#define LIBSFP_OFS_A0_RATE_IDENTIFIER       13
#define LIBSFP_OFS_A0_LENGTH_SMF_KM         14
#define LIBSFP_OFS_A0_LENGTH_SMF            15
#define LIBSFP_OFS_A0_LENGTH_50UM           16
#define LIBSFP_OFS_A0_LENGTH_625UM          17
#define LIBSFP_OFS_A0_LENGTH_CABLE          18
#define LIBSFP_OFS_A0_LENGTH_OM3            19
#define LIBSFP_OFS_A0_VENDOR_NAME           20
#define LIBSFP_OFS_A0_TRANSCEIVER2          36
#define LIBSFP_OFS_A0_VENDOR_OUI            37
#define LIBSFP_OFS_A0_VENDOR_PN             40
#define LIBSFP_OFS_A0_VENDOR_REV            56
#define LIBSFP_OFS_A0_WAVELENGTH            60
#define LIBSFP_OFS_A0_UNALLOCATED           62
#define LIBSFP_OFS_A0_CC_BASE               63
#define LIBSFP_OFS_A0_OPTIONS               64
#define LIBSFP_OFS_A0_BR_MAX                66
#define LIBSFP_OFS_A0_BR_MIN                67
#define LIBSFP_OFS_A0_VENDOR_SN             68
#define LIBSFP_OFS_A0_DATE_CODE             84
#define LIBSFP_OFS_A0_DIAGMON_TYPE          92
#define LIBSFP_OFS_A0_ENHANCED_OPTIONS      93
#define LIBSFP_OFS_A0_SFF_8472_COMPLIANCE   94
#define LIBSFP_OFS_A0_CC_EXT                95

/* Lengths constants */

#define LIBSFP_LEN_A0_IDENTIFIER             1
#define LIBSFP_LEN_A0_EXTIDENTIFIER          1
#define LIBSFP_LEN_A0_CONNECTOR              1
#define LIBSFP_LEN_A0_TRANSCEIVER            8
#define LIBSFP_LEN_A0_ENCODING               1
#define LIBSFP_LEN_A0_BR_NOMINAL             1
#define LIBSFP_LEN_A0_RATE_IDENTIFIER        1
#define LIBSFP_LEN_A0_LENGTH_SMF_KM          1
#define LIBSFP_LEN_A0_LENGTH_SMF             1
#define LIBSFP_LEN_A0_LENGTH_50UM            1
#define LIBSFP_LEN_A0_LENGTH_625UM           1
#define LIBSFP_LEN_A0_LENGTH_CABLE           1
#define LIBSFP_LEN_A0_LENGTH_OM3             1
#define LIBSFP_LEN_A0_VENDOR_NAME            16
#define LIBSFP_LEN_A0_TRANSCEIVER2           1
#define LIBSFP_LEN_A0_VENDOR_OUI             3
#define LIBSFP_LEN_A0_VENDOR_PN              16
#define LIBSFP_LEN_A0_VENDOR_REV             4
#define LIBSFP_LEN_A0_WAVELENGTH             2
#define LIBSFP_LEN_A0_UNALLOCATED            1
#define LIBSFP_LEN_A0_CC_BASE                1
#define LIBSFP_LEN_A0_OPTIONS                2
#define LIBSFP_LEN_A0_BR_MAX                 1
#define LIBSFP_LEN_A0_BR_MIN                 1
#define LIBSFP_LEN_A0_VENDOR_SN              16
#define LIBSFP_LEN_A0_DATE_CODE              8
#define LIBSFP_LEN_A0_DIAGMON_TYPE           1
#define LIBSFP_LEN_A0_ENHANCED_OPTIONS       1
#define LIBSFP_LEN_A0_SFF_8472_COMPLIANCE    1
#define LIBSFP_LEN_A0_CC_EXT                 1


#define LIBSFP_A0_CONNECTOR_UNKNOWN         0x00
#define LIBSFP_A0_CONNECTOR_SC              0x01
#define LIBSFP_A0_CONNECTOR_FC_STYLE1       0x02
#define LIBSFP_A0_CONNECTOR_FC_STYLE2       0x03
#define LIBSFP_A0_CONNECTOR_BNC_TNC         0x04
#define LIBSFP_A0_CONNECTOR_FC_COAXIAL      0x05
#define LIBSFP_A0_CONNECTOR_FIBERJACK       0x06
#define LIBSFP_A0_CONNECTOR_LC              0x07
#define LIBSFP_A0_CONNECTOR_MTRJ            0x08
#define LIBSFP_A0_CONNECTOR_MU              0x09
#define LIBSFP_A0_CONNECTOR_SG              0x0A
#define LIBSFP_A0_CONNECTOR_OPTICAL         0x0B
#define LIBSFP_A0_CONNECTOR_MPO_PARALLEL    0x0C
#define LIBSFP_A0_CONNECTOR_HSSDC2          0x20
#define LIBSFP_A0_CONNECTOR_COPPER          0x21
#define LIBSFP_A0_CONNECTOR_RJ45            0x22

#define LIBSFP_A0_DIAGMON_TYPE_LDM      (1<<7)
#define LIBSFP_A0_DIAGMON_TYPE_DDM      (1<<6)
#define LIBSFP_A0_DIAGMON_TYPE_INCAL    (1<<5)
#define LIBSFP_A0_DIAGMON_TYPE_EXCAL    (1<<4)
#define LIBSFP_A0_DIAGMON_TYPE_OMA      (1<<3)
#define LIBSFP_A0_DIAGMON_TYPE_ADDRCH   (1<<2)

#define LIBSFP_A0_ENHANCED_OPTIONS_AWFLAGS          (1<<7)
#define LIBSFP_A0_ENHANCED_OPTIONS_TXDIS            (1<<6)
#define LIBSFP_A0_ENHANCED_OPTIONS_TXFAULT          (1<<5)
#define LIBSFP_A0_ENHANCED_OPTIONS_RXLOS            (1<<4)
#define LIBSFP_A0_ENHANCED_OPTIONS_RATESEL          (1<<3)
#define LIBSFP_A0_ENHANCED_OPTIONS_APPSEL_SFF8079   (1<<2)
#define LIBSFP_A0_ENHANCED_OPTIONS_RATESEL_SFF8431  (1<<1)




/* A2 bank */

/* Standart offsets constants */

#define LIBSFP_OFS_A2_AW_THRESHOLDS          0
#define LIBSFP_OFS_A2_UNALLOCATED0           40
#define LIBSFP_OFS_A2_EXT_CAL_CONSTANTS      56
#define LIBSFP_OFS_A2_UNALLOCATED1           92
#define LIBSFP_OFS_A2_CC_DMI                 95
#define LIBSFP_OFS_A2_DIAGNOSTICS            96
#define LIBSFP_OFS_A2_UNALLOCATED2           106
#define LIBSFP_OFS_A2_STATUSCONTROL          110
#define LIBSFP_OFS_A2_RESERVED               111
#define LIBSFP_OFS_A2_ALARM_FLAGS            112
#define LIBSFP_OFS_A2_UNALLOCATED3           114
#define LIBSFP_OFS_A2_WARNING_FLAGS          116
#define LIBSFP_OFS_A2_EXT_STATUS_CONTROL     118
#define LIBSFP_OFS_A2_VENDOR_SPECIFIC        120
#define LIBSFP_OFS_A2_USER_EEPROM            128
#define LIBSFP_OFS_A2_VENDOR_CONTROL         248

/* Additional offests constansts */

#define LIBSFP_OFS_A2_EXT_CAL_RXPWR          56
#define LIBSFP_OFS_A2_EXT_CAL_TXPWR_SLOPE    80
#define LIBSFP_OFS_A2_EXT_CAL_TXPWR_OFFSET   82

#define LIBSFP_OFS_A2_DIAGNOSTICS_TXPOWER    102
#define LIBSFP_OFS_A2_DIAGNOSTICS_RXPOWER    104

#define LIBSFP_OFS_A2_DIAGNOSTICS_STSCTRL    110


/* Lengths constants */

#define LIBSFP_LEN_A2_AW_THRESHOLDS          40
#define LIBSFP_LEN_A2_UNALLOCATED0           16
#define LIBSFP_LEN_A2_EXT_CAL_CONSTANTS      36
#define LIBSFP_LEN_A2_UNALLOCATED1           3
#define LIBSFP_LEN_A2_CC_DMI                 1
#define LIBSFP_LEN_A2_DIAGNOSTICS            10
#define LIBSFP_LEN_A2_UNALLOCATED2           4
#define LIBSFP_LEN_A2_STATUSCONTROL          1
#define LIBSFP_LEN_A2_RESERVED               1
#define LIBSFP_LEN_A2_ALARM_FLAGS            2
#define LIBSFP_LEN_A2_UNALLOCATED3           2
#define LIBSFP_LEN_A2_WARNING_FLAGS          2
#define LIBSFP_LEN_A2_EXT_STATUS_CONTROL     2
#define LIBSFP_LEN_A2_VENDOR_SPECIFIC        8
#define LIBSFP_LEN_A2_USER_EEPROM            120
#define LIBSFP_LEN_A2_VENDOR_CONTROL         8


/* Additional Length constansts */

#define LIBSFP_LEN_A2_EXT_CAL_RXPWR         20
#define LIBSFP_LEN_A2_EXT_CAL_TXPWR_SLOPE    2
#define LIBSFP_LEN_A2_EXT_CAL_TXPWR_OFFSET   2


#define LIBSFP_LEN_A2_DIAGNOSTICS_TXPOWER    2
#define LIBSFP_LEN_A2_DIAGNOSTICS_RXPOWER    2

#define LIBSFP_LEN_A2_DIAGNOSTICS_STSCTRL    1



/* Register bits constants */

#define LIBSFP_A2_STATUSCONTROL_TXD     0x80
#define LIBSFP_A2_STATUSCONTROL_TXD_SET 0x40
#define LIBSFP_A2_STATUSCONTROL_RS1     0x20
#define LIBSFP_A2_STATUSCONTROL_RS0     0x10
#define LIBSFP_A2_STATUSCONTROL_RS0_SET 0x08
#define LIBSFP_A2_STATUSCONTROL_TXFAULT 0x04
#define LIBSFP_A2_STATUSCONTROL_RXLOS   0x02
#define LIBSFP_A2_STATUSCONTROL_DR      0x01



#endif
