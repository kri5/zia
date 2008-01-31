PROJECT (ziahttpd)

SET(ZIAHTTPD_SRCS main.cpp test.cpp IThreadbase.cpp)
SET(ZIAHTTPD_HEADERS zia.h Singleton.hpp ISocket.h Socket_win32.h ClientSocket_win32.h MainSocket_win32.h test.h IThreadbase.h IThread_win32.h IMutex.h Mutex_win32.h)

IF (WIN32)
	SET(ZIAHTTPD_LIBS Ws2_32.lib)
	SET(ZIAHTTPD_SRCS ${ZIAHTTPD_HEADERS} ${ZIAHTTPD_SRCS}  IThread_win32.cpp Mutex_win32.cpp Socket_win32.cpp ClientSocket_win32.cpp MainSocket_win32.cpp)
ELSE (WIN32)
	SET(ZIAHTTPD_LIBS pthread)
ENDIF (WIN32)

IF (UNIX)
  SET(ZIAHTTPD_SRCS ${ZIAHTTPD_SRCS} Socket_unix.cpp ClientSocket_unix.cpp
	  MainSocket_unix.cpp IThread_unix.cpp Mutex_unix.cpp)
ENDIF (UNIX)

ADD_EXECUTABLE (ziahttpd ${ZIAHTTPD_SRCS})
TARGET_LINK_LIBRARIES(ziahttpd ${ZIAHTTPD_LIBS})
