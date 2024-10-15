# Microsoft Developer Studio Project File - Name="LabelSets" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LabelSets - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LabelSets.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LabelSets.mak" CFG="LabelSets - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LabelSets - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LabelSets - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LabelSets - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LabelSets - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "LabelSets"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LabelSets - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(x)\x_files" /I "$(x)\Entropy" /I "$(x)\Entropy\PS2" /I "$(x)\Auxiliary\Bitmap" /I "MCode" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LabelSets - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(x)\x_files" /I "$(x)\Entropy" /I "$(x)\Entropy\PS2" /I "$(x)\Auxiliary\Bitmap" /I "MCode" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LabelSets - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LabelSets___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "LabelSets___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(x)\x_files" /I "$(x)\Entropy" /I "$(x)\Entropy\PS2" /I "$(x)\Auxiliary\Bitmap" /I "MCode" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(x)\x_files" /I "$(x)\Entropy" /I "$(x)\Entropy\PS2" /I "$(x)\Auxiliary\Bitmap" /I "MCode" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "LabelSets - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LabelSets___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "LabelSets___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(x)\x_files" /I "$(x)\Entropy" /I "$(x)\Entropy\PS2" /I "$(x)\Auxiliary\Bitmap" /I "MCode" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /I "$(x)\x_files" /I "$(x)\Entropy" /I "$(x)\Entropy\PS2" /I "$(x)\Auxiliary\Bitmap" /I "MCode" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
# SUBTRACT CPP /O<none>
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /D:TARGET_LINUX
# ADD LIB32 /nologo /D:TARGET_LINUX

!ENDIF 

# Begin Target

# Name "LabelSets - Win32 Release"
# Name "LabelSets - Win32 Debug"
# Name "LabelSets - Win32 Linux Debug"
# Name "LabelSets - Win32 Linux Release"
# Begin Source File

SOURCE=.\AnimTypes.hpp
# End Source File
# Begin Source File

SOURCE=".\Copy To Drive P Art Tools.bat"
# End Source File
# Begin Source File

SOURCE=.\GUITypes.hpp
# End Source File
# Begin Source File

SOURCE=.\HotPointTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\LabelParse.cpp
# End Source File
# Begin Source File

SOURCE=.\LabelParse.hpp
# End Source File
# Begin Source File

SOURCE=.\ModelTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\musictypes.hpp
# End Source File
# Begin Source File

SOURCE=.\NodeTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\ParticleTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\ShapeTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\SoundTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\trainingtypes.hpp
# End Source File
# Begin Source File

SOURCE=.\Tribes2Types.hpp
# End Source File
# Begin Source File

SOURCE=.\Tribes2UserData.hpp
# End Source File
# Begin Source File

SOURCE=.\voicetypes.hpp
# End Source File
# End Target
# End Project
