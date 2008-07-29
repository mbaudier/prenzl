PLATFORM = linux

SRCDIR = .
OUTDIR = out
DISTDIR = dist

ifeq ($(PLATFORM),win)
 JDK_HOME=C:/Program Files/Java/jdk1.5.0_09
 CC = g++
 COPY = copy /Y
 MKDIR = mkdir
 DELDIR = delete
 
 CFLAGS = -D_REENTRANT -D_GNU_SOURCE -D__int64='long long' -O3 -Wall -c -fmessage-length=0  -mno-cygwin
 INCLUDES = -I'$(JDK_HOME)/include' -I'$(JDK_HOME)/include/win32' -I$(SRCDIR)
 LIB_FLAGS = -mno-cygwin -Wl,--add-stdcall-alias -shared

 STDRULES_LIB = $(DISTDIR)\stdrules.dll
 TEST_STDRULES_DEPLOY = ..\net.sf.prenzl.stdrules.win32\stdrules.dll
 
else ifeq ($(PLATFORM),macos)
 CC = /usr/bin/g++
 COPY = cp
 MKDIR = mkdir -p
 DELDIR = rm -rf
 
 CFLAGS = -D_REENTRANT -D_GNU_SOURCE -D__int64='long long' -O3 -Wall -c -fmessage-length=0
 INCLUDES = -I/System/Library/Frameworks/JavaVM.framework/Headers -I$(SRCDIR)
 LIB_FLAGS = -dynamiclib -flat_namespace -undefined warning -framework JavaVM -L/usr/local/lib
 
 STDRULES_LIB = $(DISTDIR)/libstdrules.jnilib
 STDRULES_MAC_INSTALL = @executable_path/plugins/net.sf.prenzl.stdrules.macosx/libstdrules.jnilib
 TEST_STDRULES_DEPLOY = ../net.sf.prenzl.stdrules.macosx/libstdrules.jnilib
 
else ifeq ($(PLATFORM),linux)
 CC = /usr/bin/g++
 COPY = cp
 MKDIR = mkdir -p
 DELDIR = rm -rf
 
 CFLAGS = -D_REENTRANT -O3 -Wall -c -fmessage-length=0
 INCLUDES = -I$(SRCDIR)
 LIB_FLAGS = -shared    
 
 STDRULES_LIB = $(DISTDIR)/libstdrules.so
 TEST_STDRULES_DEPLOY = ../net.sf.prenzl.stdrules.linux/libstdrules.so
else
endif


all : init $(STDRULES_LIB) deploy

# STANDARD RULES
$(STDRULES_LIB) : $(OUTDIR)/StdRules.o $(OUTDIR)/StdRulesFactories.o
ifeq ($(PLATFORM),macos)
	$(CC) $(LIB_FLAGS) $? -Wl,-install_name,$(STDRULES_MAC_INSTALL) -o $@
else ifeq ($(PLATFORM),linux)
	$(CC) $(LIB_FLAGS) $? -Wl,-soname,libstdrules.so -static -lc -o $@
else
	$(CC) $(LIB_FLAGS) $? -o$@
endif
	
$(OUTDIR)/StdRules.o : $(SRCDIR)/JNI/Rules/stdrules/StdRules.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o$@ $?
	
$(OUTDIR)/StdRulesFactories.o : $(SRCDIR)/JNI/Rules/stdrules/StdRulesFactories.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o$@ $?
	
# UTILS
init :
ifeq ($(PLATFORM),macos)
	$(MKDIR) $(OUTDIR)
	$(MKDIR) $(DISTDIR)
else ifeq ($(PLATFORM),linux)
	$(MKDIR) $(OUTDIR)
	$(MKDIR) $(DISTDIR)
endif

deploy :
	$(COPY) $(STDRULES_LIB) $(TEST_STDRULES_DEPLOY)

clean :
	$(DELDIR) $(OUTDIR)
	$(DELDIR) $(DISTDIR)

