# - Find the IRRKLANG library
#
# Usage:
#   find_package(IRRKLANG [REQUIRED] [QUIET] )
#     
# It sets the following variables:
#   IRRKLANG_FOUND               ... true if IRRKLANG is found on the system
#   IRRKLANG_LIBRARIES           ... full path to IRRKLANG library
#   IRRKLANG_INCLUDES            ... IRRKLANG include directory
#
# The following variables will be checked by the function
#   IRRKLANG_USE_STATIC_LIBS    ... if true, only static libraries are found
#   IRRKLANG_ROOT               ... if set, the libraries are exclusively searched
#                               under this path
#   IRRKLANG_LIBRARY            ... IRRKLANG library to use
#   IRRKLANG_INCLUDE_DIR        ... IRRKLANG include directory
#

#If environment variable IRRKLANGDIR is specified, it has same effect as IRRKLANG_ROOT
if( NOT IRRKLANG_ROOT AND ENV{IRRKLANGDIR} )
  set( IRRKLANG_ROOT $ENV{IRRKLANGDIR} )
endif()

# Check if we can use PkgConfig
find_package(PkgConfig)

#Determine from PKG
if( PKG_CONFIG_FOUND AND NOT IRRKLANG_ROOT )
  pkg_check_modules( PKG_IRRKLANG QUIET "IRRKLANG3" )
endif()

#Check whether to search static or dynamic libs
set( CMAKE_FIND_LIBRARY_SUFFIXES_SAV ${CMAKE_FIND_LIBRARY_SUFFIXES} )

if( ${IRRKLANG_USE_STATIC_LIBS} )
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX} )
else()
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX} )
endif()

if( IRRKLANG_ROOT )
   message(${IRRKLANG_ROOT})
  #find libs
  find_library(
    IRRKLANG_LIB
    NAMES "IrrKlang"
    PATHS ${IRRKLANG_ROOT} 
    PATH_SUFFIXES "lib" "lib64" "bin" "bin/linux-gcc-64"
    NO_DEFAULT_PATH
  )

  find_library(
    IRRKLANG_MP3_LIB
    NAMES "IrrKlangMP3"
    PATHS ${IRRKLANG_ROOT} 
    PATH_SUFFIXES "lib" "lib64" "bin" "bin/linux-gcc-64"
    NO_DEFAULT_PATH
  )

  find_library(
    IRRKLANG_FLAC_LIB
    NAMES "IrrKlangFLAC"
    PATHS ${IRRKLANG_ROOT} 
    PATH_SUFFIXES "lib" "lib64" "bin" "bin/linux-gcc-64"
    NO_DEFAULT_PATH
  )


  #find includes
  find_path(
    IRRKLANG_INCLUDES
    NAMES "irrKlang.h"
    PATHS ${IRRKLANG_ROOT}
    PATH_SUFFIXES "include"
    NO_DEFAULT_PATH
  )

else()

    find_library(
        IRRKLANG_LIB
        NAMES "IRRKLANG"
        PATHS ${PKG_IRRKLANG_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
    )

    find_library(
        IRRKLANG_MP3_LIB
        NAMES "IRRKLANGMP3"
        PATHS ${PKG_IRRKLANG_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
    )

    find_library(
        IRRKLANG_FLAC_LIB
        NAMES "IRRKLANGFLAC"
        PATHS ${PKG_IRRKLANG_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
    )


    find_path(
       IRRKLANG_INCLUDES
       NAMES "irrKlang.h"
       PATHS ${PKG_IRRKLANG_INCLUDE_DIRS} ${INCLUDE_INSTALL_DIR}
    )

endif( IRRKLANG_ROOT )

set(IRRKLANG_LIBRARIES ${IRRKLANG_LIB})

if (IRRKLANG_MP3_LIB)
	set(IRRKLANG_LIBRARIES ${IRRKLANG_MP3_LIB})
endif(IRRKLANG_MP3_LIB)

if (IRRKLANG_FLAC_LIB)
	set(IRRKLANG_LIBRARIES ${IRRKLANG_FLAC_LIB})
endif(IRRKLANG_FLAC_LIB)

set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IRRKLANG DEFAULT_MSG
                                  IRRKLANG_INCLUDES IRRKLANG_LIBRARIES)

mark_as_advanced(IRRKLANG_INCLUDES IRRKLANG_LIBRARIES)

