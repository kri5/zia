# - Find Perl libraries
  # This module finds if PERL is installed and determines where the include files
  # and libraries are. It also determines what the name of the library is. This
  # code sets the following variables:
  #
  #  PERL_INCLUDE_PATH = path to where perl.h is found
  #  PERL_EXECUTABLE   = full path to the perl binary
  #
  
  SET(PERL_POSSIBLE_INCLUDE_PATHS
    /usr/lib/perl/5.8.8/CORE
    /usr/lib/perl/5.8.3/CORE
    /usr/lib/perl/5.8.2/CORE
    /usr/lib/perl/5.8.1/CORE
    /usr/lib/perl/5.8.0/CORE
    /usr/lib/perl/5.8/CORE
    /usr/lib/perl/*/CORE
    /usr/lib/perl5/5.8.8/x86_64-linux/CORE
    /usr/lib/perl5/5.8.8/i686-linux/CORE
    /usr/lib/perl5/*/x86_64-linux/CORE
    /usr/lib/perl5/*/i686-linux/CORE
    $ENV{ProgramFiles}/Perl/lib/CORE
    $ENV{SystemDrive}/Perl/lib/CORE
    )
  
  SET(PERL_POSSIBLE_LIB_PATHS
    /usr/lib
    $ENV{ProgramFiles}/Perl/lib/CORE
    $ENV{SystemDrive}/Perl/lib/CORE
    )
  
  SET(PERL_POSSIBLE_LIBRARY_NAME
    perl5.8.0
    perl5.8
    perl58
    perl
    )
  
  FIND_PATH(PERL_INCLUDE_PATH perl.h
    ${PERL_POSSIBLE_INCLUDE_PATHS})
  
  
  FIND_PROGRAM(PERL_EXECUTABLE
    NAMES perl
    PATHS
    /usr/bin
    /usr/local/bin
    $ENV{ProgramFiles}/Perl/bin
    $ENV{SystemDrive}/Perl/bin
    )
  
  IF(PERL_EXECUTABLE)
    EXEC_PROGRAM(${PERL_EXECUTABLE}
      ARGS -MExtUtils::Embed -e ldopts
      OUTPUT_VARIABLE PERL_LIBRARY_OUTPUT_VARIABLE
      RETURN_VALUE PERL_LIBRARY_RETURN_VALUE
      )
    IF(PERL_LIBRARY_OUTPUT_VARIABLE)
      SET(PERL_EXTRA_LD_FLAGS ${PERL_LIBRARY_OUTPUT_VARIABLE})
  #    MESSAGE(STATUS "Perl LD flags: ${PERL_EXTRA_LD_FLAGS}")
    ELSE(PERL_LIBRARY_OUTPUT_VARIABLE)
  #    MESSAGE(STATUS "Perl: no LD flags")
    ENDIF(PERL_LIBRARY_OUTPUT_VARIABLE)
  
    SET(PERL_LIBDIR_EXTRACT_REGEX "([-_./a-zA-Z0-9]*[.][a])")
    IF("${PERL_EXTRA_LD_FLAGS}" MATCHES "${PERL_LIBDIR_EXTRACT_REGEX}")
      STRING(REGEX MATCH "([-_./a-zA-Z0-9]*[.][a])" PERL_DYNALOADER "${PERL_EXTRA_LD_FLAGS}")
  #    MESSAGE(DEBUG ": found .a file: ${PERL_DYNALOADER}")
    ENDIF("${PERL_EXTRA_LD_FLAGS}" MATCHES "${PERL_LIBDIR_EXTRACT_REGEX}")
  
    IF(NOT PERL_LIBRARY_RETURN_VALUE)
      FOREACH(path ${PERL_POSSIBLE_LIB_PATHS})
        SET(PERL_POSSIBLE_LIBRARY_NAME ${PERL_POSSIBLE_LIBRARY_NAME} "${path}/${PERL_LIBRARY_OUTPUT_VARIABLE}")
      ENDFOREACH(path ${PERL_POSSIBLE_LIB_PATHS})
    ENDIF(NOT PERL_LIBRARY_RETURN_VALUE)
  
    EXECUTE_PROCESS(COMMAND ${PERL_EXECUTABLE} -MExtUtils::Embed -e ccopts
      OUTPUT_VARIABLE PERL_CPPFLAGS_OUTPUT_VARIABLE
      RESULT_VARIABLE PERL_CPPFLAGS_RETURN_VALUE
      )
  #  MESSAGE("PERL_CPPFLAGS_OUTPUT_VARIABLE: ${PERL_CPPFLAGS_OUTPUT_VARIABLE}")
  
    IF(NOT PERL_CPPFLAGS_RETURN_VALUE)
      SET(PERL_EXTRA_C_FLAGS "${PERL_CPPFLAGS_OUTPUT_VARIABLE}")
      # on windows, suppress the -O1, it conflicts with other options
      STRING(REPLACE "-O1" "" PERL_EXTRA_C_FLAGS ${PERL_EXTRA_C_FLAGS})
  #    SEPARATE_ARGUMENTS(PERL_EXTRA_C_FLAGS)
  #    MESSAGE(STATUS "Perl C flags: ${PERL_EXTRA_C_FLAGS}")
    ENDIF(NOT PERL_CPPFLAGS_RETURN_VALUE)
  
    IF (NOT PERL_INCLUDE_PATH)
      EXECUTE_PROCESS(COMMAND ${PERL_EXECUTABLE} -MExtUtils::Embed -e perl_inc
        OUTPUT_VARIABLE PERL_INCLUDE_OUTPUT_VARIABLE
        RESULT_VARIABLE PERL_INCLUDE_RETURN_VALUE
        )
  #    MESSAGE(STATUS "Perl include: ${PERL_INCLUDE_OUTPUT_VARIABLE}")
      IF (PERL_INCLUDE_OUTPUT_VARIABLE)
        STRING(REGEX REPLACE "[ ]*-I([-_./a-zA-Z0-9]*)[ ]*" "\\1" PERL_POSSIBLE_INCLUDE_PATHS "${PERL_INCLUDE_OUTPUT_VARIABLE}")
  #      MESSAGE(STATUS "Perl possible include path: [${PERL_POSSIBLE_INCLUDE_PATHS}]")
           FIND_PATH(PERL_INCLUDE_PATH perl.h ${PERL_POSSIBLE_INCLUDE_PATHS})
     #      MESSAGE(STATUS "Perl include path: ${PERL_INCLUDE_PATH}")
         ENDIF (PERL_INCLUDE_OUTPUT_VARIABLE)
       ENDIF (NOT PERL_INCLUDE_PATH)
     ENDIF(PERL_EXECUTABLE)
     
     FIND_LIBRARY(PERL_LIBRARY
       NAMES ${PERL_POSSIBLE_LIBRARY_NAME}
       PATHS ${PERL_POSSIBLE_LIB_PATHS}
       )
     
     IF(PERL_LIBRARY)
       SET(PERL_FOUND TRUE)
       #message(STATUS "Found Perl: ${PERL_INCLUDE_PATH}, ${PERL_LIBRARY}")
     ENDIF(PERL_LIBRARY)
     
     MARK_AS_ADVANCED(
       PERL_INCLUDE_PATH
       PERL_EXECUTABLE
       PERL_LIBRARY
       )
