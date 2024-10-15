# Microsoft Developer Studio Project File - Name="BotObject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BotObject - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BotObject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BotObject.mak" CFG="BotObject - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BotObject - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BotObject"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BotObject - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "NDEBUG" /D "_LIB" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BotObject - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BotObject - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BotObject___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "BotObject___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "BotObject - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BotObject___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "BotObject___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /O2 /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
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

# Name "BotObject - Win32 Release"
# Name "BotObject - Win32 Debug"
# Name "BotObject - Win32 Linux Debug"
# Name "BotObject - Win32 Linux Release"
# Begin Group "MasterAI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CNH_MasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\CNH_MasterAI.hpp
# End Source File
# Begin Source File

SOURCE=.\CTF_MasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\CTF_MasterAI.hpp
# End Source File
# Begin Source File

SOURCE=.\Deployable.cpp
# End Source File
# Begin Source File

SOURCE=.\Deployable.hpp
# End Source File
# Begin Source File

SOURCE=.\DM_MasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\DM_MasterAI.hpp
# End Source File
# Begin Source File

SOURCE=.\HunterMasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\HunterMasterAI.hpp
# End Source File
# Begin Source File

SOURCE=.\MAI_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\MAI_Manager.hpp
# End Source File
# Begin Source File

SOURCE=.\MasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterAI.hpp
# End Source File
# Begin Source File

SOURCE=.\SPM_MasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\SPM_MasterAI.hpp
# End Source File
# Begin Source File

SOURCE=.\Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Task.hpp
# End Source File
# Begin Source File

SOURCE=.\TDM_MasterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\TDM_MasterAI.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BotCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\BotGoal.cpp
# End Source File
# Begin Source File

SOURCE=.\BotGoal.hpp
# End Source File
# Begin Source File

SOURCE=.\BotInput.cpp
# End Source File
# Begin Source File

SOURCE=.\BotLog.cpp
# End Source File
# Begin Source File

SOURCE=.\BotLog.hpp
# End Source File
# Begin Source File

SOURCE=.\BotObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BotObject.hpp
# End Source File
# Begin Source File

SOURCE=.\BotStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\CellRef.hpp
# End Source File
# Begin Source File

SOURCE=.\FibHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\FibHeap.hpp
# End Source File
# Begin Source File

SOURCE=.\FibHeapAC.cpp
# End Source File
# Begin Source File

SOURCE=.\FibHeapAC.hpp
# End Source File
# Begin Source File

SOURCE=.\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph.hpp
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\Path.hpp
# End Source File
# Begin Source File

SOURCE=.\PathFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinder.hpp
# End Source File
# Begin Source File

SOURCE=.\PathGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\PathGenerator.hpp
# End Source File
# Begin Source File

SOURCE=.\PathKeeper.cpp
# End Source File
# Begin Source File

SOURCE=.\PathKeeper.hpp
# End Source File
# Begin Source File

SOURCE=.\PathManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PathManager.hpp
# End Source File
# End Target
# End Project
