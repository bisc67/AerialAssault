# Microsoft Developer Studio Project File - Name="GameMgr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GameMgr - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameMgr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameMgr.mak" CFG="GameMgr - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameMgr - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 PS2 DVD Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 PS2 DVD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 PS2 Client Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 PS2 Client Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 PS2 DevKit Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GameMgr - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Support/GameMgr", YFLAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameMgr - Win32 Release"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DVD Release"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DVD Debug"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 Client Release"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 Client Debug"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DevKit Release"

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

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DevKit Debug"

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

# Name "GameMgr - Win32 Release"
# Name "GameMgr - Win32 Debug"
# Name "GameMgr - Win32 PS2 DVD Release"
# Name "GameMgr - Win32 PS2 DVD Debug"
# Name "GameMgr - Win32 PS2 Client Release"
# Name "GameMgr - Win32 PS2 Client Debug"
# Name "GameMgr - Win32 PS2 DevKit Release"
# Name "GameMgr - Win32 PS2 DevKit Debug"
# Begin Group "Campaign Missions 1-5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C01_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C01_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C02_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C02_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C03_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C03_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C04_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C04_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C05_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C05_Logic.hpp
# End Source File
# End Group
# Begin Group "Campaign Missions 6-13"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C06_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C06_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C07_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C07_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C08_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C08_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C09_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C09_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C10_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C10_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C11_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C11_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C12_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C12_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\C13_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\C13_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\Campaign_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\Campaign_Logic.hpp
# End Source File
# End Group
# Begin Group "Game Types"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CNH_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CNH_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\CTF_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CTF_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\DM_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\DM_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\HUNT_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\HUNT_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\TDM_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\TDM_Logic.hpp
# End Source File
# End Group
# Begin Group "Training Missions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CT1_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CT1_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\CT2_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CT2_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\CT3_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CT3_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\CT4_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CT4_Logic.hpp
# End Source File
# Begin Source File

SOURCE=.\CT5_Logic.cpp
# End Source File
# Begin Source File

SOURCE=.\CT5_Logic.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\GameLogic.cpp
# End Source File
# Begin Source File

SOURCE=.\GameLogic.hpp
# End Source File
# Begin Source File

SOURCE=.\GameMgr.cpp

!IF  "$(CFG)" == "GameMgr - Win32 Release"

!ELSEIF  "$(CFG)" == "GameMgr - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DVD Release"

# ADD CPP /O1

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 Client Release"

# ADD CPP /O1

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DevKit Release"

# ADD CPP /O1

!ELSEIF  "$(CFG)" == "GameMgr - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\demo1\Data\UI\Messages.h
# End Source File
# Begin Source File

SOURCE=.\MsgMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgMgr.hpp
# End Source File
# End Target
# End Project
