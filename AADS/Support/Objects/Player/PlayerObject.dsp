# Microsoft Developer Studio Project File - Name="PlayerObject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PlayerObject - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PlayerObject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PlayerObject.mak" CFG="PlayerObject - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PlayerObject - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PlayerObject - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PlayerObject - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PlayerObject - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "PlayerObject"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PlayerObject - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /D "NDEBUG" /D "_LIB" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PlayerObject - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PlayerObject - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PlayerObject___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "PlayerObject___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "PlayerObject - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PlayerObject___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "PlayerObject___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /O2 /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
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

# Name "PlayerObject - Win32 Release"
# Name "PlayerObject - Win32 Debug"
# Name "PlayerObject - Win32 Linux Debug"
# Name "PlayerObject - Win32 Linux Release"
# Begin Source File

SOURCE=.\Blaster.cpp
# End Source File
# Begin Source File

SOURCE=.\Chaingun.cpp
# End Source File
# Begin Source File

SOURCE=.\CorpseObject.cpp
# End Source File
# Begin Source File

SOURCE=.\CorpseObject.hpp
# End Source File
# Begin Source File

SOURCE=.\DefaultLoadouts.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultLoadouts.hpp
# End Source File
# Begin Source File

SOURCE=.\DiskLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\Elf.cpp
# End Source File
# Begin Source File

SOURCE=.\GrenadeLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\LaserRifle.cpp
# End Source File
# Begin Source File

SOURCE=.\missilelauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\MortarGun.cpp
# End Source File
# Begin Source File

SOURCE=.\PlasmaGun.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerCommonInfo.hpp
# End Source File
# Begin Source File

SOURCE=.\PlayerControlInfo.hpp
# End Source File
# Begin Source File

SOURCE=.\PlayerDefines.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerDefines.hpp
# End Source File
# Begin Source File

SOURCE=.\PlayerLogic.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerMoveInfo.hpp
# End Source File
# Begin Source File

SOURCE=.\PlayerNet.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerObject.hpp
# End Source File
# Begin Source File

SOURCE=.\PlayerRender.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerStates.cpp
# End Source File
# Begin Source File

SOURCE=.\RepairGun.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetingLaser.cpp
# End Source File
# Begin Source File

SOURCE=.\WeaponStates.cpp
# End Source File
# End Target
# End Project
