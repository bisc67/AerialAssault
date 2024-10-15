# Microsoft Developer Studio Project File - Name="x_files" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=x_files - Win32 Linux Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "x_files.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "x_files.mak" CFG="x_files - Win32 Linux Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "x_files - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "x_files - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "x_files - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "x_files - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "x_files"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "x_files - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "NDEBUG" /D "_LIB" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x_files___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "x_files___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD CPP /Zi /Od /I "$(X)\x_files" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x_files___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "x_files___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /D "NDEBUG" /D "_LIB" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ENDIF 

# Begin Target

# Name "x_files - Win32 Release"
# Name "x_files - Win32 Debug"
# Name "x_files - Win32 Linux Debug"
# Name "x_files - Win32 Linux Release"
# Begin Group "Implementation"

# PROP Default_Filter ""
# Begin Group "Inline"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Implementation\x_bitmap_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_color_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_bb_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_m4_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_p_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_q_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_r3_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_rect_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_v2_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_v3_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_v4_inline.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_string_inline.hpp
# End Source File
# End Group
# Begin Group "Private"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Implementation\x_array_private.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_files_private.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_memory_private.hpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_stdio_private.hpp
# End Source File
# End Group
# Begin Group "External"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\_ansi.h
# End Source File
# Begin Source File

SOURCE=".\PS2-DevKit-Debug\_SN_in_VC.h"
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\sys\config.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\machine\ieeefp.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\sys\reent.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\sifdev.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Implementation\x_bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_cmap.cpp

!IF  "$(CFG)" == "x_files - Win32 Release"

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# ADD CPP /Zi /O2 /Op

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# ADD BASE CPP /Zi /O2 /Op
# ADD CPP /Zi /O2 /Op

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_convert.cpp

!IF  "$(CFG)" == "x_files - Win32 Release"

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# ADD CPP /Zi /O2

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# ADD BASE CPP /Zi /O2
# ADD CPP /Zi /O2

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_io.cpp

!IF  "$(CFG)" == "x_files - Win32 Release"

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# ADD CPP /Zi /O2

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# ADD BASE CPP /Zi /O2
# ADD CPP /Zi /O2

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_quant.cpp

!IF  "$(CFG)" == "x_files - Win32 Release"

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# ADD CPP /Zi /O2 /Op

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# ADD BASE CPP /Zi /O2 /Op
# ADD CPP /Zi /O2 /Op

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bytestream.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_files.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_math_basic.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_memory.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_mqueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_plus.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_profile.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_stdio.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_string.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_threads.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_time.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_vsprintf.cpp
# End Source File
# End Group
# Begin Group "Under Construction"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Implementation\x_flare.cpp

!IF  "$(CFG)" == "x_files - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\x_flare.hpp

!IF  "$(CFG)" == "x_files - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "x_files - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "x_files - Win32 Linux Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\x_array.hpp
# End Source File
# Begin Source File

SOURCE=.\x_bitmap.hpp
# End Source File
# Begin Source File

SOURCE=.\x_bytestream.hpp
# End Source File
# Begin Source File

SOURCE=.\x_color.hpp
# End Source File
# Begin Source File

SOURCE=.\x_debug.hpp
# End Source File
# Begin Source File

SOURCE=.\x_files.hpp
# End Source File
# Begin Source File

SOURCE=.\x_math.hpp
# End Source File
# Begin Source File

SOURCE=.\x_memory.hpp
# End Source File
# Begin Source File

SOURCE=.\x_plus.hpp
# End Source File
# Begin Source File

SOURCE=.\x_profile.hpp
# End Source File
# Begin Source File

SOURCE=.\x_stdio.hpp
# End Source File
# Begin Source File

SOURCE=.\x_string.hpp
# End Source File
# Begin Source File

SOURCE=.\x_target.hpp
# End Source File
# Begin Source File

SOURCE=.\x_threads.hpp
# End Source File
# Begin Source File

SOURCE=.\x_time.hpp
# End Source File
# Begin Source File

SOURCE=.\x_types.hpp
# End Source File
# End Target
# End Project
