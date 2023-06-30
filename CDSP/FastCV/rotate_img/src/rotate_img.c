#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "verify.h"
#include <stdio.h>
#include <stdbool.h>
#include "fastcv.h"
#include "corner_detect.h"
#include "AEEStdErr.h"
#include "rpcmem.h"
#include "remote.h"
#include "os_defines.h"
#include "dsp_capabilities_utils.h"     // available under <HEXAGON_SDK_ROOT>/utils/examples

#ifdef __hexagon__     // some defs so app can build for Android or Hexagon simulation
#include "hexagon_sim_timer.h"
#define GetTime hexagon_sim_read_pcycles        // For Hexagon sim, use PCycles for profiling
#define LOOPS 1

#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#define LOOPS 100          // reduce the profiling impact of dynamic loading. Load once, and run FastRPC+processing many times.

unsigned long long GetTime( void )
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return tv.tv_sec * 1000000ULL + tv.tv_usec;
}

#endif

static void print_usage()
{
    printf( "Usage:\n"
            "    corner_detect [-d domain] [-U unsigned_pd]\n\n"
            "Options:\n"
            "-d domain: Run on a specific domain.\n"
            "    3: run the example on CDSP\n"
            "        Default Value: 3(CDSP)\n"
            "-U unsigned_pd: Run on signed or unsigned PD.\n"
            "    0: Run on signed PD.\n"
            "    1: Run on unsigned PD.\n"
            "        Default Value: 1\n"
            );
}


int test_main_start(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    return test_main_start(argc, argv);
}