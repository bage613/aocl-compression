AOCL COMPRESSION
================

AOCL Compression is a software framework of various lossless compression and
decompression methods tuned and optimized for AMD Zen based CPUs.
The framework offers a single set of unified APIs for all the supported
compression and decompression methods which facilitate the applications to
easily integrate and use them.
AOCL Compression supports lz4, zlib/deflate, lzma, zstd, bzip2, snappy, and lz4hc
based compression and decompression methods along with their native APIs.
It supports a dynamic dispatcher feature that executes the most optimal
function variant implemented using Function Multi-versioning thereby offering
a single optimized library portable across different x86 CPU architectures.
AOCL Compression framework is developed in C for Unix and Windows based systems.
A test suite is provided for validation and performance benchmarking
of the supported compression and decompression methods. The test suite also
supports the benchmarking of IPP compression methods like lz4, lz4hc, zlib and bzip2.
The library build framework offers CTest based testing of the test cases that are
implemented using GTest and the library test suite.


INSTALLATION
------------

Download the latest stable release from the Github repository:\
https://github.amd.com/AOCL/aocl-compression \
Install cmake on the machine where the sources are to be compiled.\
Make available any one of the compilers GCC or Clang on the machine.\
Then, use the cmake based build system to compile and generate AOCL Compression\
library and testsuite binary as explained below for Linux and Windows platforms.

BUILDING ON LINUX
-----------------
1. To create a build directory and configure the build system in it,\
   run the below command:\
   "cmake -B \<build directory\> \<CMakeList.txt filepath\>"\
   Additional options that can be specified for build configuration are:\
   "cmake -B \<build directory\> \<CMakeList.txt filepath\> \
   -DCMAKE_INSTALL_PREFIX=\<install path\>\
   -DCMAKE_BUILD_TYPE=\<Debug or Release\>\
   -DBUILD_STATIC_LIBS=ON"\
   To use clang compiler for the build, specify -DCMAKE_C_COMPILER=clang as the option.
2. Compile using the below command:\
   "cmake --build \<build directory\> --target install -j"\
   The library will be generated in the "lib" directory.\
   The test bench executable will be generated inside "build" directory itself.\
   The additional option "--target install" will install the library, binary\
   and interface header files in the installation path as specified with\
   -DCMAKE_INSTALL_PREFIX option or in the local system path otherwise.\
   The option "-j" will run the compilation process using multiple cores.
3. To uninstall the installed files, run the below custom command provided:\
   "cmake --build \<build directory\> --target uninstall"\
   To uninstall and then install the build package, run the below command:\
   "cmake --build \<build directory\> --target uninstall --target install -j -v\
   The option "-v" will print verbose build logs on the console.
4. To clear or delete the build folder or files, manually remove the
   build directory or its files.

Note: When using cmake with version lesser than 3.15, "-B" option is not supported,\
so the build folder needs to be created manually.\
The option "-v" is also not supported in cmake with version lesser than 3.15.


BUILDING ON WINDOWS
-------------------
As the prerequisites, make available Microsoft Visual Studio along with its\
"Desktop development with C++" toolset that includes the Clang compiler.

BUILDING WITH VISUAL STUDIO IDE (GUI)
-------------------------------------
1. Launch CMake GUI and set the locations for source package and build output.\
   Click the configure option and choose:\
   Generator as the installed Visual Studio version,\
   Platform as x64,\
   Optional toolset as ClangCl.\
   Choose additional library config and build options.\
   Configure CMAKE_INSTALL_PREFIX appropriately.\
   Click the Generate option.\
   After Microsoft Visual Studio project is generated, click "Open Project".\
   This will launch the Microsoft Visual Studio project for the source package.
2. Build the entire solution or the required projects accordingly.

BUILDING WITH VISUAL STUDIO IDE (command line)
----------------------------------------------
1. Go to aocl-compression source package and create a folder named build.
2. Go to the build folder.
3. Use the below command to configure and build the library and test bench executable.\
   cmake .. -T ClangCl -G \<installed Visual Studio version\> && cmake --build . --config Release --target INSTALL\
   Additional library config and build options can be passed to the above command.


ADDITIONAL LIBRARY BUILD OPTIONS
--------------------------------
You can use the following additional options for configuring your build.

Option                              |  Description
------------------------------------|----------------------------------------------------------------------------------------
AOCL_LZ4_OPT_PREFETCH_BACKWARDS     |  Enable LZ4 optimizations related to backward prefetching of data (Disabled by default)
SNAPPY_MATCH_SKIP_OPT               |  Enable Snappy match skipping optimization (Disabled by default)
LZ4_FRAME_FORMAT_SUPPORT            |  Enable building LZ4 with Frame format and API support (Enabled by default)
AOCL_LZ4HC_DISABLE_PATTERN_ANALYSIS |  Disable Pattern Analysis in LZ4HC for level 9 (Enabled by default)
AOCL_ZSTD_4BYTE_LAZY2_MATCH_FINDER  |  Enable 4-byte comparison for finding a potential better match candidate with Lazy2 compressor (Disabled by default)
AOCL_TEST_COVERAGE                  |  Enable GTest and AOCL test bench based CTest suite (Disabled by default)
BUILD_DOC                           |  Build documentation for this library (Disabled by default)
ZLIB_DEFLATE_FAST_MODE_2            |  Enable optimization for deflate fast using Z_FIXED strategy. Do not combine with ZLIB_DEFLATE_FAST_MODE_3 (Disabled by default)
ZLIB_DEFLATE_FAST_MODE_3            |  Enable ZLIB deflate quick strategy. Do not combine with ZLIB_DEFLATE_FAST_MODE_2 (Disabled by default)
AOCL_LZ4_MATCH_SKIP_OPT_LDS_STRAT1  |  Enable LZ4 match skipping optimization strategy-1 based on a larger base step size applied for long distance search (Disabled by default)
AOCL_LZ4_MATCH_SKIP_OPT_LDS_STRAT2  |  Enable LZ4 match skipping optimization strategy-2 by aggressively setting search distance on top of strategy-1. Preferred to be used with Silesia corpus (Disabled by default)
AOCL_EXCLUDE_BZIP2                  |  Exclude BZIP2 compression method from the library build (Disabled by default)
AOCL_EXCLUDE_LZ4                    |  Exclude LZ4 compression method from the library build. LZ4HC also gets excluded (Disabled by default)
AOCL_EXCLUDE_LZ4HC                  |  Exclude LZ4HC compression method from the library build (Disabled by default)
AOCL_EXCLUDE_LZMA                   |  Exclude LZMA compression method from the library build (Disabled by default)
AOCL_EXCLUDE_SNAPPY                 |  Exclude SNAPPY compression method from the library build (Disabled by default)
AOCL_EXCLUDE_ZLIB                   |  Exclude ZLIB compression method from the library build (Disabled by default)
AOCL_EXCLUDE_ZSTD                   |  Exclude ZSTD compression method from the library build (Disabled by default)


RUNNING AOCL COMPRESSION TEST BENCH ON LINUX
--------------------------------------------

Test bench supports several options to validate, benchmark or debug the supported
compression methods.\
It uses the unified API set to invoke the compression methods supported by AOCL Compression.\
Test bench can invoke and benchmark some of the IPP's compression methods as well.

To check various options supported by the test bench, use the command:\
	aocl_compression_bench -h\
   Or, aocl_compression_bench --help
	
To check all the supported compression methods, use the command:\
	aocl_compression_bench -l
	
To run the test bench with requested number of iterations, use the command:\
	aocl_compression_bench -i

To run the test bench and check the performance of all the supported
compression and decompression methods for a given input file, use the command:\
	aocl_compression_bench -a -p \<input filename\>

To run the test bench and validate the outputs from all the supported
compression and decompression methods for a given input file, use the command:\
	aocl_compression_bench -a -t \<input filename\>

To run the test bench and check the performance of a particular
compression and decompression method for a given input file, use the command:\
	aocl_compression_bench -ezstd:5:0 -p \<input filename\>\
Here, 5 is the level and 0 is the additional parameter passed to ZSTD method.

To run the test bench and validate the output of a particular
compression and decompression method for a given input file, use the command:\
	aocl_compression_bench -ezstd:5:0 -t \<input filename\>\
Here, 5 is the level and 0 is the additional parameter passed to ZSTD method.

To run the test bench with error/debug/trace/info logs, use the command:\
	aocl_compression_bench -a -t -v \<input filename\>\
Here, -v can be passed with a number like v\<n\> that can take values: 
	1 for Error (default), 2 for Info, 3 for Debug, 4 for Trace.

To test and benchmark the performance of IPP's compression methods, use the
test bench option "-c" along with other relevant options (as explained above).\
IPP's lz4, lz4hc, zlib and bzip2 methods are supported by the test bench.\
Check the following details for the exact steps:
1. Set the library path environment variable (export LD_LIBRARY_PATH on
   Linux) to point to the installed IPP library path.\
   Alternatively, one can also run vars.sh that comes along with the
   IPP installation to setup the environment variable.
2. Download lz4-1.9.3, zlib-1.2.11 and bzip2-1.0.8 source packages.
3. Apply IPP's patch files as per the below command:\
   patch -p1 < "path to corresponding patch file"
4. Build the patched IPP lz4, zlib and bzip2 libraries as per the steps given
   in IPP's readme files present in the corresponding patch file
   locations for these compression methods.
5. Set the library path environment variable (export LD_LIBRARY_PATH on
   Linux) to point to patched IPP lz4, zlib and bzip2 libraries.
6. Run the test bench as given below to benchmark IPP library methods:\
   aocl_compression_bench -a -p -c \<input filename\>\
   aocl_compression_bench -elz4 -p -c \<input filename\>\
   aocl_compression_bench -elz4hc -p -c \<input filename\>\
   aocl_compression_bench -ezlib -p -c \<input filename\>\
   aocl_compression_bench -ebzip2 -p -c \<input filename\>

RUNNING AOCL COMPRESSION TEST BENCH ON WINDOWS
----------------------------------------------

Test bench on Windows supports all the user options as supported on Linux\
except for the "-c" option to link and test IPP's compression methods.\
Refer the previous section on Linux to learn about the various user options.\
To set and launch the test bench with a specific user option,\
go to project aocl\_compression\_bench -> Properties -> Debugging and\
specify the user options and the input test file.

GENERATING DOCUMENTATION
------------------------
- To generate documentation, specify the `-DBUILD_DOC=ON` option while building.
- Documents will be generated in HTML format inside the folder __docs/html__ , you can open the index.html file with any browser to view the documentation.
- CMake will use the existing Doxygen if available or else it will download the respective Doxygen binaries according to the OS you are using to build the documentation.


CONTACTS
--------
AOCL Compression is developed and maintained by AMD.
You can contact us on the email-id toolchainsupport@amd.com.
