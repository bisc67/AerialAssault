# Microsoft Developer Studio Project File - Name="Shape" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Shape - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Shape.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Shape.mak" CFG="Shape - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Shape - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 PS2 DVD Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 PS2 DVD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 PS2 Client Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 PS2 Client Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 PS2 DevKit Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Shape - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Support/Shape", CXGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Shape - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Shape - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Release"

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
# ADD BASE CPP /O2 /D "TARGET_PS2_DVD" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Debug"

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
# ADD BASE CPP /Zi /Od /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Release"

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
# ADD BASE CPP /O2 /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Debug"

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
# ADD BASE CPP /Zi /Od /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Release"

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
# ADD BASE CPP /O2 /D "TARGET_PS2_DEV" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Debug"

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
# ADD BASE CPP /Zi /Od /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /O2 /I "$(X)\Auxiliary\Bitmap" /I "$(X)\x_files" /I "$(Tribes)\Support" /I "$(X)\Entropy" /I "$(X)" /I "$(Tribes)" /I "MCode" /I "Shaders" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
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

# Name "Shape - Win32 Release"
# Name "Shape - Win32 Debug"
# Name "Shape - Win32 PS2 DVD Release"
# Name "Shape - Win32 PS2 DVD Debug"
# Name "Shape - Win32 PS2 Client Release"
# Name "Shape - Win32 PS2 Client Debug"
# Name "Shape - Win32 PS2 DevKit Release"
# Name "Shape - Win32 PS2 DevKit Debug"
# Begin Group "MCode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MCode\Kernal.vu

!IF  "$(CFG)" == "Shape - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MCode\PS2_Shape.cpp

!IF  "$(CFG)" == "Shape - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MCode\PS2_Shape.hpp

!IF  "$(CFG)" == "Shape - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MCode\TexDifClip_Clipper.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MCode\TexDifClip_Transform.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MCode\TexDifNoClip_Transform.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MCode\TexDifRefClip_Clipper.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MCode\TexDifRefClip_Transform.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MCode\TexDifRefNoClip_Transform.vu
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Shaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\D3D_Shape.cpp

!IF  "$(CFG)" == "Shape - Win32 Release"

!ELSEIF  "$(CFG)" == "Shape - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DVD Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 Client Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Shape - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shaders\D3D_Shape.hpp
# End Source File
# Begin Source File

SOURCE=.\Shaders\Dif.psh
# End Source File
# Begin Source File

SOURCE=.\Shaders\Dif.vsh
# End Source File
# Begin Source File

SOURCE=.\Shaders\Make.bat
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDif.psh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDif.vsh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDifDot3.psh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDifDot3.vsh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDifRef.psh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDifRef.vsh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDifRefBump.psh
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Shaders\TexDifRefBump.vsh
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\Anim.cpp
# End Source File
# Begin Source File

SOURCE=.\Anim.hpp
# End Source File
# Begin Source File

SOURCE=.\DebugUtils.hpp
# End Source File
# Begin Source File

SOURCE=.\HotPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\HotPoint.hpp
# End Source File
# Begin Source File

SOURCE=.\LabelSet.cpp
# End Source File
# Begin Source File

SOURCE=.\LabelSet.hpp
# End Source File
# Begin Source File

SOURCE=.\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\Material.hpp
# End Source File
# Begin Source File

SOURCE=.\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\Model.hpp
# End Source File
# Begin Source File

SOURCE=.\Node.cpp
# End Source File
# Begin Source File

SOURCE=.\Node.hpp
# End Source File
# Begin Source File

SOURCE=.\Poly.cpp
# End Source File
# Begin Source File

SOURCE=.\Poly.hpp
# End Source File
# Begin Source File

SOURCE=.\SelectionList.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionList.hpp
# End Source File
# Begin Source File

SOURCE=.\Shape.cpp
# End Source File
# Begin Source File

SOURCE=.\Shape.hpp
# End Source File
# Begin Source File

SOURCE=.\ShapeBinFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeBinFile.hpp
# End Source File
# Begin Source File

SOURCE=.\ShapeInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeInstance.hpp
# End Source File
# Begin Source File

SOURCE=.\ShapeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeManager.hpp
# End Source File
# Begin Source File

SOURCE=.\ShapeReadAsc.cpp
# End Source File
# Begin Source File

SOURCE=.\Strip.cpp
# End Source File
# Begin Source File

SOURCE=.\Strip.hpp
# End Source File
# Begin Source File

SOURCE=.\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Texture.hpp
# End Source File
# Begin Source File

SOURCE=.\TextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureManager.hpp
# End Source File
# Begin Source File

SOURCE=.\UserData.hpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.hpp
# End Source File
# Begin Source File

SOURCE=.\Vert.cpp
# End Source File
# Begin Source File

SOURCE=.\Vert.hpp
# End Source File
# End Target
# End Project
