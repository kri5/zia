SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

SET(ZIAHTTPD_SRCS main.cpp Test1.cpp Test2.cpp ZUnitManager.cpp
    ZUnitStdOutput.cpp ZUnitSuite.hpp)

SET(ZIAHTTPD_HEADERS IZUnitSuite.h ZUnitManager.h ZUnitOutput.h
    ZUnitStdOutput.h)


ADD_EXECUTABLE (ziatest ${ZIAHTTPD_SRCS})
