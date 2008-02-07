#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

SET(ZIAHTTPD_SRCS main.cpp IThreadbase.cpp Logger.cpp 
	Config.cpp Init.cpp SSLClientSocket.cpp Vhost.cpp XMLDocument.cpp
	XMLNode.cpp Address.cpp Port.cpp NetworkID.cpp Wildcard.hpp Logger.hpp)

SET(ZIAHTTPD_HEADERS zia.h Singleton.hpp Socket_win32.h ClientSocket_win32.h MainSocket_win32.h IThreadbase.h IThread_win32.h IMutex.h Mutex_win32.h
	IClientSocket.h Init.h Config.h SSLClientSocket.h Vhost.h
	Address.h Port.h NetworkID.h XMLDocument.h XMLNode.h Port.h
	MemoryManager.h)

IF (WIN32)
	INCLUDE_DIRECTORIES(../libs/libxml/win32/includes ../libs/iconv/win32/include)
	LINK_DIRECTORIES(../libs/libxml/win32/lib ../libs/iconv/win32/lib)
	SET(ZIAHTTPD_LIBS Ws2_32.lib libxml2.lib iconv.lib ${OPENSSL_LIBRARIES}
      ${OPENSSL_EAY_LIBRARIES})
	SET(ZIAHTTPD_SRCS ${ZIAHTTPD_HEADERS} ${ZIAHTTPD_SRCS}  IThread_win32.cpp Mutex_win32.cpp Socket_win32.cpp ClientSocket_win32.cpp MainSocket_win32.cpp)
ELSE (WIN32)
	INCLUDE_DIRECTORIES(/usr/include/libxml2/)
	SET(ZIAHTTPD_LIBS pthread xml2 ${OPENSSL_LIBRARIES} ${OPENSSL_EAY_LIBRARIES})
ENDIF (WIN32)

IF (UNIX)
  SET(ZIAHTTPD_SRCS ${ZIAHTTPD_SRCS} Socket_unix.cpp ClientSocket_unix.cpp
	  MainSocket_unix.cpp IThread_unix.cpp Mutex_unix.cpp)
ENDIF (UNIX)

ADD_EXECUTABLE (ziahttpd ${ZIAHTTPD_SRCS})
TARGET_LINK_LIBRARIES(ziahttpd ${ZIAHTTPD_LIBS})
