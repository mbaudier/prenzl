PLATFORM = macos


SRCDIR = .
OUTDIR = out
DISTDIR = dist

ifeq ($(PLATFORM),win)
 JDK_HOME=C:/Program Files/Java/jdk1.5.0_09
 CC = g++
 CFLAGS = -D_REENTRANT -D_GNU_SOURCE -D__int64='long long' -O3 -Wall -c -fmessage-length=0  -mno-cygwin
 INCLUDES = -I'$(JDK_HOME)/include' -I'$(JDK_HOME)/include/win32' -I$(SRCDIR)
 LIB_FLAGS = -mno-cygwin -Wl,--add-stdcall-alias -shared

 STDRULES_LIB = $(DISTDIR)/stdrules.dll

 PLUGINS_DIR = ..
 STDRULES_INSTALL = $(PLUGINS_DIR)/net.sf.prenzl.stdrules/stdrules.dll
else 
endif

ifeq ($(PLATFORM),macos)
 CC = /usr/bin/g++
 CFLAGS = -D_REENTRANT -D_GNU_SOURCE -D__int64='long long' -O3 -Wall -c -fmessage-length=0
 INCLUDES = -I/System/Library/Frameworks/JavaVM.framework/Headers -I$(SRCDIR)
 LIB_FLAGS = -dynamiclib -flat_namespace -undefined warning -framework JavaVM -L/usr/local/lib
 
 STDRULES_LIB = $(DISTDIR)/libstdrules.jnilib

 STDRULES_INSTALL = ../net.sf.prenzl.stdrules/libstdrules.jnilib
else
endif



all : init $(STDRULES_LIB) deploy

init :
ifeq ($(PLATFORM),win)
	mkdir $(OUTDIR)
	mkdir $(DISTDIR)
else 
	mkdir -p $(OUTDIR)
	mkdir -p $(DISTDIR)
endif

deploy :
ifeq ($(PLATFORM),win)
	copy /Y $(STDRULES_LIB) $(STDRULES_INSTALL)
else
	cp $(STDRULES_LIB) $(STDRULES_INSTALL)
endif

clean :
	rm -rf $(OUTDIR)
	rm -rf $(DISTDIR)

# STANDARD RULES
$(STDRULES_LIB) : $(OUTDIR)/StdRules.o
ifeq ($(PLATFORM),macos)
	$(CC) $(LIB_FLAGS) $? -Wl,-install_name,$(STDRULES_INSTALL) -o $@
else
	$(CC) $(LIB_FLAGS) -L$(DISTDIR) -lprenzl $(OUTDIR)/StdRules.o -o$@
endif
	
$(OUTDIR)/StdRules.o : $(SRCDIR)/JNI/Rules/stdrules/StdRules.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o$@ $?
