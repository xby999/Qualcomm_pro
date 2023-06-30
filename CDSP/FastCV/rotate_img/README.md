#  Corner detection example
##  Overview

The corner_detect example illustrates a basic use case with DSP acceleration, including the FastCV library.

  * inc/corner_detect.idl defines the FastRPC interface between the Apps Processor and DSP portions of the customer application.

  * src/corner_detect.c is the source for the executable to test the DSP corner detection algorithm. It can be built for Android to run on target, or built for Hexagon to link with the DSP libs and run on the Hexagon Simulator. It has an image compiled into memory, which it sends to the DSP and reads back the list of detected corners.

  * src/corner_detect_imp.c is the DSP portion of the example. It implements the methods defined in corner_detect.idl, and invokes FastCV API's to perform the corner detection.

  * src/verify.h contains macros for result checking, useful in corner_detect.c.

  * android.min contains information required to build the Android executable. It may need to be modified if source files and/or dependencies are changed. DEPENDENCIES to build the example need to be mentioned in hexagon_deps.min and android_deps.min.

  * hexagon.min contains information required to build the DSP libraries and test executable. It may need to be modified if source files and/or dependencies are changed.

  * Makefile should not need to change.

  * See the base Hexagon SDK documentation for more information on build system.

  * qprof.cfg is an input configuration file to generate graphical profiling when running the example on the Hexagon simulator.

##  corner_detect example simulation

Following are the steps to build the corner_detect example for Hexagon simulation, using the unit test framework described in base Hexagon SDK documentation.
From ${HEXAGON_SDK_ROOT}/addons/compute/examples/corner_detect:

    make hexagon BUILD=ReleaseG DSP_ARCH=v65

Note that the "tree" in the make command is necessary to build all the example's dependencies for the specified variant (hexagon_ReleaseG).
Subsequent rebuilds of the example for the same variant can omit the "tree" as follows.

    make hexagon BUILD=ReleaseG DSP_ARCH=v65

Note that this make command builds the Hexagon static library version of the example, and also builds/links the test application in corner_detect.c, and finally triggers the application to run in the Hexagon simulator. Note the simulation command line is echoed at the beginning of the run.

The ` --timing --buspenalty 75` options need to be added to simulation command to model the effects of cache misses in the profiling. This setting causes the simulation to take longer, but more closely predicts the performance that would be seen on a Hexagon target. The full simulation command is:

 `$HEXAGON_SDK_ROOT/tools/HEXAGON_Tools/8.4.03/Tools/bin/hexagon-sim -mv65 --simulated_returnval --usefs hexagon_ReleaseG_toolv84_v65 --pmu_statsfile hexagon_ReleaseG_toolv84_v65/pmu_stats.txt --cosim_file hexagon_ReleaseG_toolv84_v65/q6ss.cfg --l2tcm_base 0xd800 --rtos hexagon_ReleaseG_toolv84_v65/osam.cfg $HEXAGON_SDK_ROOT/rtos/qurt//computev65/sdksim_bin/runelf.pbn -- $HEXAGON_SDK_ROOT/libs/run_main_on_hexagon/ship/hexagon_toolv84_v65/run_main_on_hexagon_sim -- corner_detect_q.so  --timing --buspenalty 75 `

Without these additional options, the simulator does not take into account any memory stalls when reporting performance.  This is referred as an ideal-cache simulation.  The `--timing --buspenalty 75` options force the simulator to simulate memory latencies and assume a DDR read latency of 75 cycles and thus lead to a higher cycle count.

One of the goals of code optimization is to bring the cycle count that the simulator reports when taking memory delays into account as close as possible to the ideal-cache simulation. For more details on optimization techniques, see the [benchmark example](../benchmark/README.md) or read the DSP optimization guidelines provided in the base SDK documentation.


###  corner_detect example on-target

The corner_detect example comes equipped with a walkthrough script `corner_detect_walkthrough.py` that builds and runs the example.

If you want to reproduce these steps manually, please follow the instructions below.


In order to test dynamic modules on the device, the device needs to be prepared with a testsig. . These steps are described in the base Hexagon SDK documentation and not repeated here.

Following are the steps to build the corner_detect example for the target:

    cd ${HEXAGON_SDK_ROOT}/addons/compute/examples/corner_detect
    make hexagon BUILD=ReleaseG DSP_ARCH=v65
    make android BUILD=ReleaseG

Following are the steps to load the binaries onto the device via adb.

    adb root
    adb remount
    adb shell mkdir -p /vendor/bin
    adb push android_ReleaseG/ship/libcorner_detect.so /vendor/lib
    adb push android_ReleaseG/ship/corner_detect /vendor/bin
    adb shell chmod 755 /vendor/bin/corner_detect
    adb push hexagon_ReleaseG_dynamic_toolv84_v65/ship/libcorner_detect_skel.so /vendor/lib/rfsa/adsp

Following are the steps to execute the test on the device via adb.

    adb shell /vendor/bin/corner_detect

The output should look as follows (profiling number may vary):

    initq6 done...
    Num corners detected: 60. Expected: 60
    run time of corner detection: 349501 microseconds for 100 iterations
    deinit done....  Num corners detected: 60
