# Microsoft Developer Studio Project File - Name="Entropy" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Entropy - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Entropy.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Entropy.mak" CFG="Entropy - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Entropy - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Entropy - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Entropy - Win32 Linux Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Entropy - Win32 Linux Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Entropy"
# PROP Scc_LocalPath "..\..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Entropy - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(X)\..\support" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(X)\..\support" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Entropy___Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "Entropy___Win32_Linux_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(X)\..\support" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(X)\..\support" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /D:TARGET_LINUX

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Entropy___Win32_Linux_Release"
# PROP BASE Intermediate_Dir "Entropy___Win32_Linux_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(X)\..\support" /D "_DEBUG" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /O2 /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(X)\..\support" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
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

# Name "Entropy - Win32 Release"
# Name "Entropy - Win32 Debug"
# Name "Entropy - Win32 Linux Debug"
# Name "Entropy - Win32 Linux Release"
# Begin Group "Implementation - Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Common\e_cdfs.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_cdfs.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_Input.cpp
# End Source File
# Begin Source File

SOURCE=.\e_netfs.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\e_netfs.hpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\e_netfsdefs.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_ScratchMem.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_Text.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_View.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\e_View_inline.hpp
# End Source File
# End Group
# Begin Group "Implementation - PC"

# PROP Default_Filter ""
# Begin Group "Libs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\3rdParty\DirectX8\dsound.lib

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8.lib

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3d8.lib

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\dxguid.lib

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\dinput8.lib

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\D3DEngine\d3deng.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DEngine\d3deng_draw.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DEngine\d3deng_input.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DEngine\d3deng_private.hpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DEngine\d3deng_vram.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "External"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\_ansi.h
# End Source File
# Begin Source File

SOURCE="..\x_files\PS2-DevKit-Debug\_SN_in_VC.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\sys\config.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3d8.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3d8caps.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3d8types.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8core.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8effect.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8math.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8math.inl
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8mesh.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8shape.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\d3dx8tex.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\devfont.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\devgif.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\devvif0.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\devvif1.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\devvu0.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\devvu1.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\dpaddr.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\dplay8.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\DirectX8\dxfile.h
# End Source File
# Begin Source File

SOURCE=.\e_Engine.hpp
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\eekernel.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\eeregs.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\eestruct.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\eetypes.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\common\include\libcdvd.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libdev.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libdma.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libdmapk.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libgifpk.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libgraph.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libpad.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libpkt.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\libvifpk.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\sys\reent.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\sifdev.h
# End Source File
# Begin Source File

SOURCE=..\3rdParty\PS2\Sony\sce\ee\include\sifrpc.h
# End Source File
# Begin Source File

SOURCE=..\x_files\x_array.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_array_private.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_bitmap.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_bitmap_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_color.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_color_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_debug.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_files.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_math.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_bb_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_m4_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_p_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_q_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_r3_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_v2_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_v3_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_math_v4_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_memory.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_memory_private.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_plus.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_stdio.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_string.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\Implementation\x_string_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_target.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_time.hpp
# End Source File
# Begin Source File

SOURCE=..\x_files\x_types.hpp
# End Source File
# End Group
# Begin Group "Implementation - Linux"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Linux\linux_Draw.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Linux\Linux_Misc.hpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Linux\linux_startup.cpp

!IF  "$(CFG)" == "Entropy - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Debug"

!ELSEIF  "$(CFG)" == "Entropy - Win32 Linux Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\e_Draw.hpp
# End Source File
# Begin Source File

SOURCE=.\e_Input.hpp
# End Source File
# Begin Source File

SOURCE=.\e_Input_Gadget_Defines.hpp
# End Source File
# Begin Source File

SOURCE=.\e_Input_Gadgets.hpp
# End Source File
# Begin Source File

SOURCE=.\e_ScratchMem.hpp
# End Source File
# Begin Source File

SOURCE=.\e_Text.hpp
# End Source File
# Begin Source File

SOURCE=.\e_View.hpp
# End Source File
# Begin Source File

SOURCE=.\e_VRAM.hpp
# End Source File
# Begin Source File

SOURCE=.\Entropy.hpp
# End Source File
# End Target
# End Project