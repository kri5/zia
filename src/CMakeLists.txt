#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

#doesn't work on win32 :
ADD_SUBDIRECTORY(modules)

ADD_DEFINITIONS(-DTIXML_USE_TICPP)

INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR} includes/ ./)

SET(ZIAHTTPD_SRCS main.cpp IThreadbase.cpp Logger.cpp 
	Config.cpp Init.cpp SSLClientSocket.cpp Vhost.cpp
	Address.cpp Port.cpp NetworkID.cpp Wildcard.hpp Logger.hpp
	MemoryManager.hpp ErrorMsg.cpp ZException.hpp
	MutexLock.hpp AtExit.hpp Parser.cpp HttpParser.cpp HttpRequest.cpp
    HttpResponse.cpp Server.cpp ErrorResponseStream.cpp
    Buffer.cpp DirectoryBrowser.cpp ResponseStreamFile.cpp ResponseStreamDir.cpp
    Task.cpp Worker.cpp Pool.cpp PoolManager.cpp HttpTransaction.cpp
    ModuleManager.cpp UrlString.cpp NastyCast.hpp ModuleInfo.cpp ResponseStreamGeneric.cpp
    AbstractModule.cpp)

SET(ZIAHTTPD_HEADERS includes/zia.h Singleton.hpp includes/Sockets/Socket_win32.h includes/Sockets/ClientSocket_win32.h includes/Sockets/MainSocket_win32.h includes/Threads/IThreadbase.h 
	includes/Threads/IThread_win32.h includes/Mutex/IMutex.h includes/Mutex/Mutex_win32.h includes/Mutex/IRWMutex.h includes/Mutex/RWMutex.h includes/Mutex/RWMutex_win32.h
	includes/API/IClientSocket.h includes/Init.h includes/Config.h includes/Sockets/SSLClientSocket.h includes/Network/Vhost.h
	includes/Network/Address.h includes/Network/Port.h includes/Network/NetworkID.h includes/File/IFile.h includes/FileSystem/IFileSystem.h includes/File/File_win32.h includes/FileSystem/FileSystem_win32.h
	includes/Time/ITime.h includes/Time/Time_win32.h includes/Workflow/Parser.h includes/Http/HttpParser.h includes/Http/HttpRequest.h includes/Threads/IThread.h
    includes/API/IHttpTransaction.h includes/API/IHttpRequest.h includes/API/IHttpResponse.h includes/API/IServerEvent.h includes/API/IModuleEvent.h
    includes/API/IWorkflow.h includes/API/INetwork.h includes/API/IReceiveRequest.h includes/API/IBuildResponse.h includes/API/ISendResponse.h
    includes/Http/HttpResponse.h includes/Stream/ErrorResponseStream.h includes/Sockets/ISocket.h includes/Server.h
    includes/Utils/Buffer.h RootConfig.hpp includes/DirectoryBrowser.h
    includes/Workflow/Task.h includes/Stream/ResponseStreamFile.h
    includes/Stream/ResponseStreamDir.h includes/Workflow/Worker.h includes/Workflow/Pool.h includes/Http/HttpTransaction.h
    includes/API/IHttpTransaction.h includes/API/IHttpRequest.h includes/API/IHttpResponse.h includes/Utils/UrlString.h
    includes/Modules/ModuleManager.h includes/Modules/IDynLib.h includes/Modules/DynLib.h includes/Modules/DynLib_win32.h
    includes/API/IModuleInfo.h includes/Stream/ResponseStreamGeneric.h
    includes/Modules/AbstractModule.h)


IF (WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	INCLUDE_DIRECTORIES(../libs/includes/ticpp)
	LINK_DIRECTORIES(../libs/win32/)
	SET(ZIAHTTPD_LIBS Ws2_32.lib ticppd.lib ${OPENSSL_LIBRARIES}
      ${OPENSSL_EAY_LIBRARIES})
	SET(ZIAHTTPD_SRCS ${ZIAHTTPD_HEADERS} ${ZIAHTTPD_SRCS}  IThread_win32.cpp
		Mutex_win32.cpp RWMutex_win32.cpp Socket_win32.cpp ClientSocket_win32.cpp
		MainSocket_win32.cpp File_win32.cpp Time_win32.cpp FileSystem_win32.cpp
		DynLib_win32.cpp)
ELSE (WIN32)
	SET(CMAKE_CXX_FLAGS "-O0 -Wall -W -pedantic -ggdb")
	INCLUDE_DIRECTORIES(../libs/includes/ticpp)
	LINK_DIRECTORIES(../libs/unix/)
	SET(ZIAHTTPD_LIBS pthread ticpp dl ${OPENSSL_LIBRARIES} ${OPENSSL_EAY_LIBRARIES})
    #SET(ZIAHTTPD_LIBS ${ZIAHTTPD_LIBS} -lefence)
ENDIF (WIN32)

IF (UNIX)
  SET(ZIAHTTPD_SRCS ${ZIAHTTPD_SRCS} Socket_unix.cpp ClientSocket_unix.cpp
	  MainSocket_unix.cpp IThread_unix.cpp Mutex_unix.cpp RWMutex_unix.cpp
      FileSystem_unix.cpp File_unix.cpp Time_unix.cpp DynLib_unix.cpp)
ENDIF (UNIX)

ADD_EXECUTABLE (ziahttpd ${ZIAHTTPD_SRCS})
TARGET_LINK_LIBRARIES(ziahttpd ${ZIAHTTPD_LIBS})
