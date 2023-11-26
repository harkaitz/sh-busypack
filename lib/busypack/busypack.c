#define INCBIN_PREFIX b_
#define INCBIN_STYLE  INCBIN_STYLE_SNAKE
#include "incbin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#ifdef FLYCHECK
#  define SCRIPT_FILE "script.sh"
#  define SCRIPT_NAME "script.sh"
#  define INSDIR      "Microsoft/WindowsApps"
#endif
#ifndef SCRIPT_FILE
#  error "SCRIPT_FILE not defined."
#endif
#ifndef SCRIPT_NAME
#  error "SCRIPT_NAME not defined."
#endif
#ifndef BUSYPACK_DIR
#  define BUSYPACK_DIR "."
#endif

static void
extract(
   const char *_dir,
   const char *_name,
   const unsigned char _data[],
   unsigned int        _datasz
);

INCBIN(script,SCRIPT_FILE);
INCBIN(sh    ,BUSYPACK_DIR "/sh.exe");
#ifdef EXTRA_BINARIES
#  include <busypack-extra.h>
#else
static void extract_binaries(void) {}
#endif

int main(int _argc, char *_argv[]) {
   char   cmd[1024*5];
   size_t cmdsz;

   /* Calculate installation directory. */
   size_t inspathsz = strlen(getenv("LOCALAPPDATA"))+sizeof(INSDIR)+2;
   char   inspath[inspathsz];
   sprintf(inspath, "%s\\%s", getenv("LOCALAPPDATA"), INSDIR);
   
   /* Create directory if it does not exist. */
   CreateDirectory(inspath, NULL);
   
   /* Calculate shell path. */
   char shpath[inspathsz + 1 + strlen("/sh.exe")+1];
   sprintf(shpath, "%s\\sh.exe", inspath);

   /* Calculate new path. */
   char npath[strlen(getenv("Path"))+inspathsz+2];
   sprintf(npath, "%s;%s", inspath, getenv("Path"));
   SetEnvironmentVariable("Path", npath);

   /* Extract files. */
   extract(inspath, "sh.exe"    , b_sh_data    , b_sh_size);
   extract(inspath, SCRIPT_NAME , b_script_data, b_script_size);
   extract_binaries();

   /* Save command in a environment variable. */
   SetEnvironmentVariable("BUSYPACK_SH" , shpath);
   SetEnvironmentVariable("BUSYPACK_CMD", SCRIPT_NAME);

   /* Prepare command. */
   cmdsz = snprintf(cmd, sizeof(cmd), "%%BUSYPACK_SH%% -e \"%s\\%%BUSYPACK_CMD%%\"", inspath);
   for (int i=1; i<_argc; ++i) {
      char varname[64];
      sprintf(varname, "ARG%d", i);
      SetEnvironmentVariable(varname, _argv[i]);
      cmdsz += snprintf(cmd+cmdsz, sizeof(cmd)-cmdsz, " \"%%ARG%d%%\"", i);
   }
   cmd[cmdsz++] = '\0';
   
   /* Execute the command. */
   return system(cmd);
}

static void
extract(
   const char *_dir,
   const char *_name,
   const unsigned char _data[],
   unsigned int        _datasz
) {

   FILE *ofp;
   char  opath[strlen(_dir)+3+strlen(_name)];
   
   /* Calculate filename. */
   sprintf(opath, "%s/%s", _dir, _name);

   /* Skip if exists. */
   ofp = fopen(opath, "r");
   if (ofp) { fclose(ofp); return; } 
   
   /* Create file. */
   ofp = fopen(opath, "wb");
   if (!ofp) return;
   fwrite(_data, 1, _datasz, ofp);
   fclose(ofp); ofp = NULL;
   
}
