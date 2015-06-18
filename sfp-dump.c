#include <stdio.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "libsfp.h"

#define ERR(format, ...) \
   fprintf(stderr, "ERR: "format"\n", ##__VA_ARGS__)

#define ERRCALL(s) \
   ERR("%s: %s", s, strerror(errno))

typedef struct {
  char *file1, *file2;
  uint32_t flags;
} prm_t;

int read_sfp_dump(void *udata, uint8_t bank_addr, uint16_t start,
              uint16_t count, void *data)
{
  uint16_t offset;
  FILE *f;
  char *filename=0;
  prm_t *prm = (prm_t *)udata;

  /* Select filename to open
   * If only filename1 provided
   * we attempt to read bank A2 from filename1
  */
  filename = prm->file1;
  offset = 0;

  if (bank_addr == LIBSFP_DEF_A2_ADDRESS) {
    if (prm->file2)
      filename = prm->file2;
    else
      offset = 0x100;
  }

  /*printf("a:%x s:%u c:%u f:%s o:%u\n", bank_addr, start, count, filename, offset);*/

  /* Open  file */
  f = fopen(filename, "rb");
  if (!f) {
    ERRCALL("fopen");
    return -1;
  }

  /* Read data portion */
  if (fseek(f, start+offset, SEEK_SET)) {
    ERRCALL("fseek");
    fclose(f);
    return -1;
  }

  if (fread(data, 1, count, f) != count) {
    ERR("Data read failed");
    fclose(f);
    return -1;
  }

  fclose(f);

  return 0;
}

int print_help()
{
  printf("\nDisplay SFP module dump information\n\n");
  printf("sfp-dump <options> [file1.bin] <file2.bin>\n\n");
  printf("-h -- show help\n");
  printf("-v -- show verbose info (same as '-uctb')'\n");
  printf("-x -- show hex data\n");
  printf("-s -- show bit fields in short format\n");
  printf("-u -- show fields with unknown/undefined values\n");
  printf("-c -- show calibration parameters\n");
  printf("-t -- show thresholds parameters\n");
  printf("-b -- show bit fields\n\n");
}

int parse_args(int argc, char **argv, prm_t *prm)
{
   int opt;
   while ((opt = getopt(argc, argv, "hvxuctbs")) != -1) {
     switch (opt) {
       case 'h':
         print_help();
         return 2;
       break;
       case 'v':
         prm->flags = LIBSFP_FLAGS_LONGOPT |
                       LIBSFP_FLAGS_PRINT_UNKNOWN |
                       LIBSFP_FLAGS_PRINT_CALIBRATIONS |
                       LIBSFP_FLAGS_PRINT_THRESHOLDS |
                       LIBSFP_FLAGS_PRINT_BITOPTIONS;
       break;
       case 'x':
         prm->flags |= LIBSFP_FLAGS_HEXOUTPUT;
       break;
       case 'u':
         prm->flags |= LIBSFP_FLAGS_PRINT_UNKNOWN;
       break;
       case 'c':
         prm->flags |= LIBSFP_FLAGS_PRINT_CALIBRATIONS;
       break;
       case 't':
         prm->flags |= LIBSFP_FLAGS_PRINT_THRESHOLDS;
       break;
       case 'b':
         prm->flags |= LIBSFP_FLAGS_PRINT_BITOPTIONS;
       break;
       case 's':
         prm->flags &= ~LIBSFP_FLAGS_LONGOPT;
       break;
       default:
         ERR("Wrong option");
         print_help();
         return 1;
       break;
     }
  }

  if (optind == argc) {
    ERR("No file name specified");
    return 1;
  }
  prm->file1 = argv[optind];

  prm->file2 = (optind+1 > argc) ? 0 : argv[optind+1];

  return 0;
}

int main(int argc, char **argv)
{
  libsfp_t *handle = 0;
  int ret = 0 ;
  prm_t prm;

  /* Set default parameters */
  memset(&prm, 0, sizeof(prm));
  prm.flags = LIBSFP_FLAGS_LONGOPT;

  /* Parser CLI args */
  if (parse_args(argc, argv, &prm))
    goto exit;

  /* Init library and obtain handle */
  if (libsfp_init(&handle)) {
    ERR("Can't init SFP lib!");
    ret = -1;
    goto exit;
  }

  /* Set parameters */
  libsfp_set_readreg_callback(handle, read_sfp_dump);
  libsfp_set_flags(handle,prm.flags);
  libsfp_set_user_data(handle, &prm);

  /* Normaly we do not need set this (it's a default values)
  libsfp_set_outfile(handle, stdout);
  libsfp_set_addresses(handle,
                         LIBSFP_DEF_A0_ADDRESS, LIBSFP_DEF_A2_ADDRESS);
  */

  /* Read and show information */
  if (libsfp_showinfo(handle)) {
    /*ERR("Can't read information!");*/
    ret = -2;
    goto exit;
  }


exit:

  /* Close lib handle */
  if (handle)
    libsfp_free(handle);

  return ret;
}
