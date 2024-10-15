# Microsoft Developer Studio Project File - Name="BotObject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BotObject - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BotObject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BotObject.mak" CFG="BotObject - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BotObject - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 PS2 DVD Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 PS2 DVD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 PS2 Client Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 PS2 Client Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 PS2 DevKit Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BotObject - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Support/Objects/Bot", QLJAAAAA"
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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "$(USERNAME)" /YX /FD /c
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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BotObject - Win32 PS2 DVD Release"

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
# ADD CPP /O2 /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "BotObject - Win32 PS2 DVD Debug"

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
# ADD CPP /Zi /Od /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "BotObject - Win32 PS2 Client Release"

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
# ADD CPP /O2 /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "BotObject - Win32 PS2 Client Debug"

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
# ADD CPP /Zi /Od /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "BotObject - Win32 PS2 DevKit Release"

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
# ADD CPP /O2 /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "BotObject - Win32 PS2 DevKit Debug"

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
# ADD CPP /Zi /O2 /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "$(Tribes)\Demo1" /I "$(Tribes)\Support\Objects\Projectiles" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
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

# Name "BotObject - Win32 Release"
# Name "BotObject - Win32 Debug"
# Name "BotObject - Win32 PS2 DVD Release"
# Name "BotObject - Win32 PS2 DVD Debug"
# Name "BotObject - Win32 PS2 Client Release"
# Name "BotObject - Win32 PS2 Client Debug"
# Name "BotObject - Win32 PS2 DevKit Release"
# Name "BotObject - Win32 PS2 DevKit Debug"
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
