
lib_LTLIBRARIES = libs_fwk.la



AM_CPPFLAGS = $(PTHREAD_CFLAGS) 
AM_CFLAGS = -Wall
AM_CXXFLAGS = $(AM_CFLAGS)




libs_fwk_la_SOURCES =  s_list.c  s_loop.c  s_queue.c  s_socket.c  s_thread.c  s_time.c  s_udp.c 
if !WINNT
libs_fwk_la_SOURCES += s_async_queue.c
endif
libs_fwk_la_LDFLAGS =   -no-undefined -version-info 1:0:0
libs_fwk_la_CFLAGS = $(AM_CFLAGS)
if WINNT
libs_fwk_la_CFLAGS += -D_WIN32_WINNT=0x0600 
endif

if WINNT
libs_fwk_la_LDFLAGS += -lws2_32  
else
libs_fwk_la_LDFLAGS += -pthread
endif
