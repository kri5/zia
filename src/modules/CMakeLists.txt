INCLUDE(FindOpenSSL)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR} ../includes/ ./)

IF (UNIX)

INCLUDE(perl.cmake)
INCLUDE_DIRECTORIES(${PERL_INCLUDE_PATH})

#ADD_LIBRARY (helloworld MODULE helloworld.cpp)
#ADD_LIBRARY (rot13 MODULE rot13.cpp)
#ADD_LIBRARY (loutre MODULE loutre.cpp)
#ADD_LIBRARY (hooktests MODULE HookTests.cpp)
#ADD_LIBRARY (SSL MODULE SSL.cpp SSLClientSocket.cpp)
#ADD_LIBRARY (maintenance MODULE maintenance.cpp)
ADD_LIBRARY (ModPHP MODULE modphp.cpp)
ADD_LIBRARY (ModPerl MODULE modPerl.cpp)
#ADD_LIBRARY (AutoIndex MODULE autoindex.cpp)
#ADD_LIBRARY(PriorityTest1 MODULE priorityTest1.cpp)
#ADD_LIBRARY(PriorityTest2 MODULE priorityTest2.cpp)

ENDIF (UNIX)



IF (WIN32)

ADD_LIBRARY (SSL SHARED SSL.cpp SSLClientSocket.cpp)
TARGET_LINK_LIBRARIES(SSL ${OPENSSL_LIBRARIES} ${OPENSSL_EAY_LIBRARIES})

ENDIF (WIN32)
