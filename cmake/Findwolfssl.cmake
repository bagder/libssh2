# Copyright (C) The libssh2 project and its contributors.
# SPDX-License-Identifier: BSD-3-Clause

# - Try to find wolfSSL
# Output variables:
#  WOLFSSL_FOUND           - If wolfSSL headers and library were found
#  WOLFSSL_INCLUDE_DIR     - The wolfSSL include directories
#  WOLFSSL_LIBRARIES       - The libraries needed to use wolfSSL

find_path(WOLFSSL_INCLUDE_DIR NAMES "wolfssl/options.h")
find_library(WOLFSSL_LIBRARY NAMES "wolfssl")

if(WOLFSSL_INCLUDE_DIR)
  set(_version_regex "^#define[ \t]+LIBWOLFSSL_VERSION_STRING[ \t]+\"([^\"]+)\".*")
  file(STRINGS "${WOLFSSL_INCLUDE_DIR}/wolfssl/version.h"
    WOLFSSL_VERSION REGEX "${_version_regex}")
  string(REGEX REPLACE "${_version_regex}" "\\1"
    WOLFSSL_VERSION "${WOLFSSL_VERSION}")
  unset(_version_regex)
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set WOLFSSL_FOUND
# to TRUE if all listed variables are TRUE and the requested version
# matches.
find_package_handle_standard_args("wolfssl" VERSION_VAR WOLFSSL_VERSION)

mark_as_advanced(WOLFSSL_INCLUDE_DIR WOLFSSL_LIBRARY)

if(WOLFSSL_FOUND)
  set(WOLFSSL_INCLUDE_DIRS ${WOLFSSL_INCLUDE_DIR})
  set(WOLFSSL_LIBRARIES    ${WOLFSSL_LIBRARY})
else()
  find_package(PkgConfig QUIET)
  pkg_check_modules(WOLFSSL "wolfssl")

  if(WOLFSSL_FOUND)
    set(WOLFSSL_LIBRARIES ${WOLFSSL_LINK_LIBRARIES})
  endif()
endif()
