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

 PRENZL_LIB = $(DISTDIR)/prenzl.dll
 PRENZL_BLIB = $(DISTDIR)/libprenzl.a
 STDRULES_LIB = $(DISTDIR)/stdrules.dll

 PLUGINS_DIR = ..
 PRENZL_INSTALL = $(PLUGINS_DIR)/net.sf.prenzl/prenzl.dll
 STDRULES_INSTALL = $(PLUGINS_DIR)/net.sf.prenzl.stdrules/stdrules.dll
else 
endif

ifeq ($(PLATFORM),macos)
 CC = /usr/bin/g++
 CFLAGS = -D_REENTRANT -D_GNU_SOURCE -D__int64='long long' -O3 -Wall -c -fmessage-length=0
 INCLUDES = -I/System/Library/Frameworks/JavaVM.framework/Headers -I$(SRCDIR)
 LIB_FLAGS = -dynamiclib -flat_namespace -undefined warning -framework JavaVM -L/usr/local/lib
 
 PRENZL_LIB = $(DISTDIR)/libprenzl.jnilib
 PRENZL_BLIB = $(DISTDIR)/libprenzl.dylib
 STDRULES_LIB = $(DISTDIR)/libstdrules.jnilib

 JAVA_EXTDIR = /Library/Java/Extensions
 PRENZL_INSTALL = $(JAVA_EXTDIR)/libprenzl.jnilib
 STDRULES_INSTALL = $(JAVA_EXTDIR)/libstdrules.jnilib
else
endif



all : init $(PRENZL_LIB) $(STDRULES_LIB)

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
	copy /Y $(PRENZL_LIB) $(PRENZL_INSTALL)
	copy /Y $(STDRULES_LIB) $(STDRULES_INSTALL)
else
	cp $(PRENZL_LIB) $(PRENZL_INSTALL)
	cp $(STDRULES_LIB) $(STDRULES_INSTALL)
endif

clean :
	rm -rf $(OUTDIR)
	rm -rf $(DISTDIR)

# PRENZL LIBRARY
$(PRENZL_LIB) : $(OUTDIR)/JNIComputation.o
ifeq ($(PLATFORM),win)
	$(CC) $(LIB_FLAGS) -Wl,--out-implib=$(PRENZL_BLIB) $? -o $@
else 
endif

ifeq ($(PLATFORM),macos)
	$(CC) $(LIB_FLAGS) -Wl,-install_name,$(PRENZL_INSTALL) $? -o $@
else
endif

$(PRENZL_BLIB) : $(PRENZL_LIB)
ifeq ($(PLATFORM),macos)
	cp $(PRENZL_LIB) $(PRENZL_BLIB)
else
 # do nothing
endif

$(OUTDIR)/JNIComputation.o : $(SRCDIR)/JNI/prenzl/src/JNIComputation.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o$@ $?

# STANDARD RULES
$(STDRULES_LIB) : $(PRENZL_BLIB) $(OUTDIR)/StdRules.o
ifeq ($(PLATFORM),macos)
	$(CC) $(LIB_FLAGS) -L$(DISTDIR) -lprenzl $? -Wl,-install_name,$(STDRULES_INSTALL) -o $@
else
	$(CC) $(LIB_FLAGS) -L$(DISTDIR) -lprenzl $(OUTDIR)/StdRules.o -o$@
endif
	
$(OUTDIR)/StdRules.o : $(SRCDIR)/JNI/stdrules/src/StdRules.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o$@ $?
