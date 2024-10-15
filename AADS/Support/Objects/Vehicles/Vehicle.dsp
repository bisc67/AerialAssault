# Microsoft Developer Studio Project File - Name="Vehicle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Vehicle - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Vehicle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Vehicle.mak" CFG="Vehicle - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Vehicle - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Vehicle - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Vehicle - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Vehicle - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Vehicle"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Vehicle - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Vehicle - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Vehicle - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Vehicle___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "Vehicle___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "Vehicle - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Vehicle___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "Vehicle___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(AADS)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(AADS)" /I "$(AADS)\AADS" /I "$(AADS)\Support\Objects\Projectiles" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
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

# Name "Vehicle - Win32 Release"
# Name "Vehicle - Win32 Debug"
# Name "Vehicle - Win32 Linux Debug"
# Name "Vehicle - Win32 Linux Release"
# Begin Source File

SOURCE=.\bomber.cpp
# End Source File
# Begin Source File

SOURCE=.\bomber.hpp
# End Source File
# Begin Source File

SOURCE=.\gndeffect.cpp
# End Source File
# Begin Source File

SOURCE=.\gndeffect.hpp
# End Source File
# Begin Source File

SOURCE=.\GravCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\GravCycle.hpp
# End Source File
# Begin Source File

SOURCE=.\shrike.cpp
# End Source File
# Begin Source File

SOURCE=.\shrike.hpp
# End Source File
# Begin Source File

SOURCE=.\transport.cpp
# End Source File
# Begin Source File

SOURCE=.\transport.hpp
# End Source File
# Begin Source File

SOURCE=.\Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\Vehicle.hpp
# End Source File
# End Target
# End Project
