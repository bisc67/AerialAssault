# Microsoft Developer Studio Project File - Name="x_files_mfc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=x_files_mfc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "x_files_mfc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "x_files_mfc.mak" CFG="x_files_mfc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "x_files_mfc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "x_files_mfc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/xCore/x_files", COGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "x_files_mfc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFC-Release"
# PROP BASE Intermediate_Dir "MFC-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFC-Release"
# PROP Intermediate_Dir "MFC-Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "TARGET_PC_MFC" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "x_files_mfc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFC-Debug"
# PROP BASE Intermediate_Dir "MFC-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFC-Debug"
# PROP Intermediate_Dir "MFC-Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "TARGET_PC_MFC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "x_files_mfc - Win32 Release"
# Name "x_files_mfc - Win32 Debug"
# Begin Group "Under Construction"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Implementation\x_flare.cpp
# End Source File
# Begin Source File

SOURCE=.\x_flare.hpp
# End Source File
# End Group
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
# Begin Source File

SOURCE=.\Implementation\x_bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_cmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_io.cpp
# End Source File
# Begin Source File

SOURCE=.\Implementation\x_bitmap_quant.cpp
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
