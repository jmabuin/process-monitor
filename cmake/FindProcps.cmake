   # Try to find Procps headers and libraries.
   #
   # Usage of this module as follows:
   #
   #     find_package(Procps)
   #
   # Variables used by this module, they can change the default behaviour and need
   # to be set before calling find_package:
   #
   #  PROCPS_PREFIX         Set this variable to the root installation of
   #                      libpapi if the module has problems finding the
   #                      proper installation path.
   #
   # Variables defined by this module:
   #
   #  PROCPS_FOUND              System has PROCPS libraries and headers
   #  PROCPS_LIBRARIES          The PROCPS library
   #  PROCPS_INCLUDE_DIRS       The location of PROCPS headers

   find_path(PROCPS_PREFIX_DIRS
           NAMES include/libproc2
   )

   find_library(PROCPS_LIBRARIES
           # Pick the static library first for easier run-time linking.
           NAMES libproc2.so libproc2.a proc2
   )

   find_library(PROCPS_STATIC_LIBRARIES
           # Pick the static library first for easier run-time linking.
           NAMES libproc2.a
   )

   find_path(PROCPS_INCLUDE_DIRS
           NAMES slabinfo.h
           HINTS ${PROCPS_PREFIX_DIRS}/include/libproc2 ${HILTIDEPS}/include/libproc2
   )

   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(Procps DEFAULT_MSG
           PROCPS_LIBRARIES
           PROCPS_INCLUDE_DIRS
   )

   mark_as_advanced(
           PROCPS_PREFIX_DIRS
           PROCPS_LIBRARIES
           PROCPS_STATIC_LIBRARIES
           PROCPS_INCLUDE_DIRS
   )