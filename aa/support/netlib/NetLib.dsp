# Microsoft Developer Studio Project File - Name="NetLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=NetLib - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NetLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NetLib.mak" CFG="NetLib - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NetLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 PS2 DVD Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 PS2 DVD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 PS2 Client Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 PS2 Client Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 PS2 DevKit Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NetLib - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Support/NetLib", OQHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NetLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PC-Release"
# PROP BASE Intermediate_Dir "PC-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PC-Release"
# PROP Intermediate_Dir "PC-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(X)\x_files" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "$(USERNAME)" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "NetLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PC-Debug"
# PROP BASE Intermediate_Dir "PC-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PC-Debug"
# PROP Intermediate_Dir "PC-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-DVD-Release"
# PROP BASE Intermediate_Dir "PS2-DVD-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-DVD-Release"
# PROP Intermediate_Dir "PS2-DVD-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /O2 /I "$(X)\x_files" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-DVD-Debug"
# PROP BASE Intermediate_Dir "PS2-DVD-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-DVD-Debug"
# PROP Intermediate_Dir "PS2-DVD-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /Zi /Od /I "$(X)\x_files" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-Client-Release"
# PROP BASE Intermediate_Dir "PS2-Client-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-Client-Release"
# PROP Intermediate_Dir "PS2-Client-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /O2 /I "$(X)\x_files" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-Client-Debug"
# PROP BASE Intermediate_Dir "PS2-Client-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-Client-Debug"
# PROP Intermediate_Dir "PS2-Client-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /Zi /Od /I "$(X)\x_files" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-DevKit-Release"
# PROP BASE Intermediate_Dir "PS2-DevKit-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-DevKit-Release"
# PROP Intermediate_Dir "PS2-DevKit-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /O2 /I "$(X)\x_files" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-DevKit-Debug"
# PROP BASE Intermediate_Dir "PS2-DevKit-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-DevKit-Debug"
# PROP Intermediate_Dir "PS2-DevKit-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /Zi /Od /I "$(X)\x_files" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN

!ENDIF 

# Begin Target

# Name "NetLib - Win32 Release"
# Name "NetLib - Win32 Debug"
# Name "NetLib - Win32 PS2 DVD Release"
# Name "NetLib - Win32 PS2 DVD Debug"
# Name "NetLib - Win32 PS2 Client Release"
# Name "NetLib - Win32 PS2 Client Debug"
# Name "NetLib - Win32 PS2 DevKit Release"
# Name "NetLib - Win32 PS2 DevKit Debug"
# Begin Group "Implementation"

# PROP Default_Filter ""
# Begin Group "External"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\PS2-DevKit-Debug\_SN_in_VC.h"
# End Source File
# Begin Source File

SOURCE="..\..\xCore\x_files\PS2-DevKit-Debug\_SN_in_VC.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\eekernel.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\eetypes.h
# End Source File
# Begin Source File

SOURCE=.\ps2\iop.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\IOP\iop.hpp
# End Source File
# Begin Source File

SOURCE=.\ps2\mqueue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\IOP\mqueue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\sifrpc.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_array.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_array_private.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_bitmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_bitmap_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_color.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_color_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_debug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_files.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_math.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_bb_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_m4_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_p_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_q_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_r3_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_rect_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_v2_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_v3_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_math_v4_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_memory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_memory_private.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_plus.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_stdio.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_string.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\Implementation\x_string_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_target.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_time.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\x_files\x_types.hpp
# End Source File
# End Group
# Begin Group "PC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pc\NetLib.cpp

!IF  "$(CFG)" == "NetLib - Win32 Release"

!ELSEIF  "$(CFG)" == "NetLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PC\WS2_32.LIB

!IF  "$(CFG)" == "NetLib - Win32 Release"

!ELSEIF  "$(CFG)" == "NetLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "PS2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ps2\net.cpp

!IF  "$(CFG)" == "NetLib - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "NetLib - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\IOP\netdefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\IOP\netvars.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\bitstream.cpp
# End Source File
# Begin Source File

SOURCE=.\NetCommon.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\bitstream.hpp
# End Source File
# Begin Source File

SOURCE=.\NetLib.hpp
# End Source File
# End Target
# End Project
