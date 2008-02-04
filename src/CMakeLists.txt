SET(ZIAHTTPD_SRCS main.cpp test.cpp IThreadbase.cpp Logger.cpp 
	Config.cpp)

SET(ZIAHTTPD_HEADERS zia.h Singleton.hpp Socket_win32.h ClientSocket_win32.h MainSocket_win32.h test.h IThreadbase.h IThread_win32.h IMutex.h Mutex_win32.h
	IClientSocket.h Logger.hpp Config.h)

IF (WIN32)
	INCLUDE_DIRECTORIES(../libs/libxml/win32/includes ../libs/iconv/win32/include "C:/Program Files/Common Files/GTK/2.0/include/libxml++-2.6/"
	"C:/Program Files/Common Files/GTK/2.0/include/glibmm-2.4/"
	"C:/Program Files/Common Files/glib/include/glib-2.0/"
	"C:/Program Files/Common Files/glib/lib/glib-2.0/include/"
	"C:/Program Files/Common Files/GTK/2.0/lib/gtkmm-2.4/include/"
	"C:/Program Files/Common Files/GTK/2.0/lib/glibmm-2.4/include"
	"C:/Program Files/Common Files/GTK/2.0/lib/libxml++-2.6/include"
	)
	LINK_DIRECTORIES("C:/Program Files/Common Files/GTK/2.0/lib/")
	SET(ZIAHTTPD_LIBS Ws2_32.lib xml++-2.6.lib)
	SET(ZIAHTTPD_SRCS ${ZIAHTTPD_HEADERS} ${ZIAHTTPD_SRCS}  IThread_win32.cpp Mutex_win32.cpp Socket_win32.cpp ClientSocket_win32.cpp MainSocket_win32.cpp)
ELSE (WIN32)
	INCLUDE_DIRECTORIES(
	/usr/include/libxml++-2.6 /usr/include/libxml2 /usr/include/glibmm-2.4 /usr/lib/glibmm-2.4/include /usr/include/sigc++-2.0 /usr/lib/sigc++-2.0/include /usr/include/glib-2.0	
	/usr/lib/glib-2.0/include
	)	
	SET(ZIAHTTPD_LIBS pthread xml2 xml++-2.6 glibmm-2.4
		gobject-2.0 sigc-2.0 glib-2.0)
ENDIF (WIN32)

IF (UNIX)
  SET(ZIAHTTPD_SRCS ${ZIAHTTPD_SRCS} Socket_unix.cpp ClientSocket_unix.cpp
	  MainSocket_unix.cpp IThread_unix.cpp Mutex_unix.cpp)
ENDIF (UNIX)

ADD_EXECUTABLE (ziahttpd ${ZIAHTTPD_SRCS})
TARGET_LINK_LIBRARIES(ziahttpd ${ZIAHTTPD_LIBS})
