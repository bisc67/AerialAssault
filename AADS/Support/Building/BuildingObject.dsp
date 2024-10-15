# Microsoft Developer Studio Project File - Name="BuildingObject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BuildingObject - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BuildingObject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BuildingObject.mak" CFG="BuildingObject - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BuildingObject - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BuildingObject - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BuildingObject - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BuildingObject - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BuildingObject"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BuildingObject - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Auxiliary\bitmap" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /D "NDEBUG" /D "_LIB" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Auxiliary\bitmap" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BuildingObject___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "BuildingObject___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Auxiliary\bitmap" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Auxiliary\bitmap" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BuildingObject___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "BuildingObject___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Auxiliary\bitmap" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Auxiliary\bitmap" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
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

# Name "BuildingObject - Win32 Release"
# Name "BuildingObject - Win32 Debug"
# Name "BuildingObject - Win32 Linux Debug"
# Name "BuildingObject - Win32 Linux Release"
# Begin Group "Implementation"

# PROP Default_Filter ""
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BldRenderStub.cpp

!IF  "$(CFG)" == "BuildingObject - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DBldRender.cpp

!IF  "$(CFG)" == "BuildingObject - Win32 Release"

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Debug"

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BuildingObject - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\BldFileSystem.hpp
# End Source File
# Begin Source File

SOURCE=.\BldInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\BldManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BldRender.hpp
# End Source File
# Begin Source File

SOURCE=.\Building.cpp
# End Source File
# Begin Source File

SOURCE=.\Building.hpp
# End Source File
# Begin Source File

SOURCE=.\portal_clipper.cpp
# End Source File
# Begin Source File

SOURCE=.\portal_clipper.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BldInstance.hpp
# End Source File
# Begin Source File

SOURCE=.\BldManager.hpp
# End Source File
# Begin Source File

SOURCE=.\BuildingOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildingOBJ.hpp
# End Source File
# End Target
# End Project
