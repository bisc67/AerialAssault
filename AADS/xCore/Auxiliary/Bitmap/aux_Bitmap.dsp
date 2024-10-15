# Microsoft Developer Studio Project File - Name="aux_Bitmap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=aux_Bitmap - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aux_Bitmap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aux_Bitmap.mak" CFG="aux_Bitmap - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aux_Bitmap - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "aux_Bitmap - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "aux_Bitmap - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "aux_Bitmap - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "aux_Bitmap"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\x_files" /I "PNG" /I "PNG\ZLib" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "PNG" /I "PNG\ZLib" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "aux_Bitmap___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "aux_Bitmap___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "PNG" /I "PNG\ZLib" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "PNG" /I "PNG\ZLib" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "aux_Bitmap___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "aux_Bitmap___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "PNG" /I "PNG\ZLib" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /O2 /I "$(X)\x_files" /I "PNG" /I "PNG\ZLib" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ENDIF 

# Begin Target

# Name "aux_Bitmap - Win32 Release"
# Name "aux_Bitmap - Win32 Debug"
# Name "aux_Bitmap - Win32 Linux Debug"
# Name "aux_Bitmap - Win32 Linux Release"
# Begin Group "Implementation"

# PROP Default_Filter ""
# Begin Group "External"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\x_files\x_bitmap.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_bitmap_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_color.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_color_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_debug.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_math.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_bb_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_m4_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_p_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_q_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_r3_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_v2_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_v3_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_math_v4_inline.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_memory.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\Implementation\x_memory_private.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_plus.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_stdio.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_target.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\x_files\x_types.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "PNG"

# PROP Default_Filter ""
# Begin Group "ZLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PNG\ZLib\adler32.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\compress.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\crc32.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\deflate.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\deflate.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\gzio.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\infblock.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\infblock.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\infcodes.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\infcodes.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\inffast.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\inffast.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\inffixed.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\inflate.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\inftrees.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\inftrees.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\infutil.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\infutil.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\trees.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\trees.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\uncompr.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\zconf.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\zlib.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\zutil.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\ZLib\zutil.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\PNG\png.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\png.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngasmrd.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngconf.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngerror.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pnggccrd.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngget.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngmem.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngpread.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngread.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngrio.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngrtran.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngrutil.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngset.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngtrans.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngtypes.h

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngvcrd.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngwio.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngwrite.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngwtran.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PNG\pngwutil.c

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\aux_Bitmap.cpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DefaultBMP.cpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Load_BMP.cpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Load_PNG.cpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Load_PSD.cpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Load_TGA.cpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\aux_Bitmap.hpp

!IF  "$(CFG)" == "aux_Bitmap - Win32 Release"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Debug"

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "aux_Bitmap - Win32 Linux Release"

!ENDIF 

# End Source File
# End Target
# End Project
