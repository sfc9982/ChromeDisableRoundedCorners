##############################################################################
##
##  API Extention to Measure time slept.
##
##  Microsoft Research Detours Package
##
##  Copyright (c) Microsoft Corporation.  All rights reserved.
##

!include ..\common.mak

LIBS=$(LIBS) kernel32.lib

##############################################################################

all: dirs \
    $(BIND)\winver_hook$(DETOURS_BITS).dll \
    $(BIND)\GetVersion.exe \
    \
!IF $(DETOURS_SOURCE_BROWSING)==1
    $(OBJD)\winver_hook$(DETOURS_BITS).bsc \
    $(OBJD)\GetVersion.bsc \
!ENDIF
    option

##############################################################################

dirs:
    @if not exist $(BIND) mkdir $(BIND) && echo.   Created $(BIND)
    @if not exist $(OBJD) mkdir $(OBJD) && echo.   Created $(OBJD)

$(OBJD)\winver_hook.obj : winver_hook.cpp

$(OBJD)\winver_hook.res : winver_hook.rc

$(BIND)\winver_hook$(DETOURS_BITS).dll $(BIND)\winver_hook$(DETOURS_BITS).lib: \
        $(OBJD)\winver_hook.obj $(OBJD)\winver_hook.res $(DEPS)
    cl /LD $(CFLAGS) /Fe$(@R).dll /Fd$(@R).pdb \
        $(OBJD)\winver_hook.obj $(OBJD)\winver_hook.res \
        /link $(LINKFLAGS) /subsystem:console \
        /export:DetourFinishHelperProcess,@1,NONAME \
        /export:W10GetVersionEx \
        $(LIBS)

$(OBJD)\winver_hook$(DETOURS_BITS).bsc : $(OBJD)\winver_hook.obj
    bscmake /v /n /o $@ $(OBJD)\winver_hook.sbr

$(OBJD)\GetVersion.obj : GetVersion.cpp

$(BIND)\GetVersion.exe : $(OBJD)\GetVersion.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb $(OBJD)\GetVersion.obj \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console
	mt /manifest GetVersion.manifest -outputresource:$(BIND)\GetVersion.exe

$(OBJD)\GetVersion.bsc : $(OBJD)\GetVersion.obj
    bscmake /v /n /o $@ $(OBJD)\GetVersion.sbr

##############################################################################

clean:
    -del *~ 2>nul
    -del $(BIND)\winver_hook*.* 2>nul
    -del $(BIND)\GetVersion.* 2>nul
    -rmdir /q /s $(OBJD) 2>nul

realclean: clean
    -rmdir /q /s $(OBJDS) 2>nul

############################################### Install non-bit-size binaries.

!IF "$(DETOURS_OPTION_PROCESSOR)" != ""

$(OPTD)\winver_hook$(DETOURS_OPTION_BITS).dll:
$(OPTD)\winver_hook$(DETOURS_OPTION_BITS).pdb:

$(BIND)\winver_hook$(DETOURS_OPTION_BITS).dll : $(OPTD)\winver_hook$(DETOURS_OPTION_BITS).dll
    @if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\winver_hook$(DETOURS_OPTION_BITS).pdb : $(OPTD)\winver_hook$(DETOURS_OPTION_BITS).pdb
    @if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).

option: \
    $(BIND)\winver_hook$(DETOURS_OPTION_BITS).dll \
    $(BIND)\winver_hook$(DETOURS_OPTION_BITS).pdb \

!ELSE

option:

!ENDIF

##############################################################################

test: all
    @echo -------- Reseting test binaries to initial state. ---------------------
    $(BIND)\setdll.exe -r $(BIND)\GetVersion.exe
    @echo.
    @echo -------- Should not load winver_hook$(DETOURS_BITS).dll -----------------------------------
    $(BIND)\GetVersion.exe
    @echo.
    @echo -------- Adding winver_hook$(DETOURS_BITS).dll to GetVersion.exe ------------------------------
    $(BIND)\setdll.exe -d:$(BIND)\winver_hook$(DETOURS_BITS).dll $(BIND)\GetVersion.exe
    @echo.
    @echo -------- Should load winver_hook$(DETOURS_BITS).dll statically ----------------------------
    $(BIND)\GetVersion.exe
    @echo.
    @echo -------- Removing winver_hook$(DETOURS_BITS).dll from GetVersion.exe --------------------------
    $(BIND)\setdll.exe -r $(BIND)\GetVersion.exe
    @echo.
    @echo -------- Should not load winver_hook$(DETOURS_BITS).dll -----------------------------------
    $(BIND)\GetVersion.exe
    @echo.
    @echo -------- Should load winver_hook$(DETOURS_BITS).dll dynamically using withdll.exe----------
    $(BIND)\withdll.exe -d:$(BIND)\winver_hook$(DETOURS_BITS).dll $(BIND)\GetVersion.exe
    @echo.

debug: all
    windbg -o $(BIND)\withdll.exe -d:$(BIND)\winver_hook$(DETOURS_BITS).dll $(BIND)\GetVersion.exe


################################################################# End of File.
