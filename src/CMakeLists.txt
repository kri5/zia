#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

ADD_DEFINITIONS(-DTIXML_USE_TICPP)

INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

SET(ZIAHTTPD_SRCS main.cpp IThreadbase.cpp Logger.cpp 
	Config.cpp Init.cpp SSLClientSocket.cpp Vhost.cpp
	Address.cpp Port.cpp NetworkID.cpp Wildcard.hpp Logger.hpp
	MemoryManager.hpp ErrorMsg.cpp Zerror.cpp ZException.hpp
	MutexLock.hpp AtExit.hpp Parser.cpp HttpParser.cpp HttpRequest.cpp
    HttpResponse.cpp Worker.cpp Server.cpp HttpError.cpp Date.cpp
    Buffer.cpp)

SET(ZIAHTTPD_HEADERS zia.h Singleton.hpp Socket_win32.h ClientSocket_win32.h MainSocket_win32.h IThreadbase.h IThread_win32.h IMutex.h Mutex_win32.h
	IClientSocket.h Init.h Config.h SSLClientSocket.h Vhost.h
	Address.h Port.h NetworkID.h Port.h Zerror.h IFile.h IFileSystem.h File_win32.h FileSystem_win32.h
	ITime.h Time_win32.h Parser.h HttpParser.h HttpRequest.h IThread.h
    Worker.h HttpResponse.h HttpError.h ISocket.h Date.h
    Buffer.h)

IF (WIN32)
	INCLUDE_DIRECTORIES(../libs/includes/ticpp)
	LINK_DIRECTORIES(../libs/win32/)
	SET(ZIAHTTPD_LIBS Ws2_32.lib ticppd.lib ${OPENSSL_LIBRARIES}
      ${OPENSSL_EAY_LIBRARIES})
	SET(ZIAHTTPD_SRCS ${ZIAHTTPD_HEADERS} ${ZIAHTTPD_SRCS}  IThread_win32.cpp
		Mutex_win32.cpp Socket_win32.cpp ClientSocket_win32.cpp
		MainSocket_win32.cpp File_win32.cpp Time_win32.cpp FileSystem_win32.cpp)
ELSE (WIN32)
	SET(CMAKE_CXX_FLAGS "-Wall -W -pedantic")
	INCLUDE_DIRECTORIES(../libs/includes/ticpp)
	LINK_DIRECTORIES(../libs/unix/)
	SET(ZIAHTTPD_LIBS pthread ticpp ${OPENSSL_LIBRARIES} ${OPENSSL_EAY_LIBRARIES})
ENDIF (WIN32)

IF (UNIX)
  SET(ZIAHTTPD_SRCS ${ZIAHTTPD_SRCS} Socket_unix.cpp ClientSocket_unix.cpp
	  MainSocket_unix.cpp IThread_unix.cpp Mutex_unix.cpp FileSystem_unix.cpp
	  File_unix.cpp Time_unix.cpp)
ENDIF (UNIX)

ADD_EXECUTABLE (ziahttpd ${ZIAHTTPD_SRCS})
TARGET_LINK_LIBRARIES(ziahttpd ${ZIAHTTPD_LIBS})
