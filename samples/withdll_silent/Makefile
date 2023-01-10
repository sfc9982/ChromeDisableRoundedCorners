##############################################################################
##
##  Makefile for Detours Test Programs.
##
##  Microsoft Research Detours Package
##
##  Copyright (c) Microsoft Corporation.  All rights reserved.
##

!include ..\common.mak

LIBS=$(LIBS) kernel32.lib

##############################################################################

all: dirs \
    $(BIND)\withdll_silent.exe \
!IF $(DETOURS_SOURCE_BROWSING)==1
    $(OBJD)\withdll_silent.bsc \
!ENDIF
    option

clean:
    -del *~ 2>nul
    -del $(BIND)\withdll_silent.* 2>nul
    -rmdir /q /s $(OBJD) 2>nul

realclean: clean
    -rmdir /q /s $(OBJDS) 2>nul

##############################################################################

dirs:
    @if not exist $(BIND) mkdir $(BIND) && echo.   Created $(BIND)
    @if not exist $(OBJD) mkdir $(OBJD) && echo.   Created $(OBJD)

$(OBJD)\withdll_silent.obj : withdll_silent.cpp

$(BIND)\withdll_silent.exe : $(OBJD)\withdll_silent.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb $(OBJD)\withdll_silent.obj \
        /link $(LINKFLAGS) $(LIBS) /subsystem:windows /entry:mainCRTStartup

$(OBJD)\withdll_silent.bsc : $(OBJD)\withdll_silent.obj
    bscmake /v /n /o $@ $(OBJD)\withdll_silent.sbr

############################################### Install non-bit-size binaries.

option:

##############################################################################

test: all
    $(BIND)\withdll_silent.exe -d:$(BIND)\slept$(DETOURS_BITS).dll $(BIND)\sleepold.exe
    $(BIND)\withdll_silent.exe -v -d:$(BIND)\slept$(DETOURS_BITS).dll $(BIND)\sleepold.exe

debug: all
    windbg  -c ".srcfix;l+s;l+t" -o \
        $(BIND)\withdll_silent.exe -d:$(BIND)\slept$(DETOURS_BITS).dll $(BIND)\sleepold.exe

################################################################# End of File.
