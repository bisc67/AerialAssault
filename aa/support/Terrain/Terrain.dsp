# Microsoft Developer Studio Project File - Name="Terrain" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Terrain - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Terrain.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Terrain.mak" CFG="Terrain - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Terrain - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 PS2 DVD Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 PS2 DVD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 PS2 Client Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 PS2 Client Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 PS2 DevKit Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Terrain - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Support/Terrain", QFHAAAAA"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "Terrain - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Terrain - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "X_DEBUG" /D "X_ASSERT" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DVD Release"

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
# ADD CPP /O2 /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DVD Debug"

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
# ADD CPP /Zi /Od /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 Client Release"

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
# ADD CPP /O2 /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 Client Debug"

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
# ADD CPP /Zi /Od /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DevKit Release"

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
# ADD CPP /O2 /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DevKit Debug"

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
# ADD CPP /Zi /Od /I "$(X)" /I "$(Tribes)\Support" /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LIB32 /D:TARGET_PS2_DEV /D:VENDOR_SN

!ENDIF 

# Begin Target

# Name "Terrain - Win32 Release"
# Name "Terrain - Win32 Debug"
# Name "Terrain - Win32 PS2 DVD Release"
# Name "Terrain - Win32 PS2 DVD Debug"
# Name "Terrain - Win32 PS2 Client Release"
# Name "Terrain - Win32 PS2 Client Debug"
# Name "Terrain - Win32 PS2 DevKit Release"
# Name "Terrain - Win32 PS2 DevKit Debug"
# Begin Group "Implementation"

# PROP Default_Filter ""
# Begin Group "external"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\_ansi.h
# End Source File
# Begin Source File

SOURCE=".\PS2-DevKit-Debug\_SN_in_VC.h"
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Auxiliary\bitmap\aux_Bitmap.hpp
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\sys\config.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3d8.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3d8caps.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3d8types.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\D3DEngine\d3deng_private.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8core.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8effect.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8math.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8math.inl
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8shape.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\d3dx8tex.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\devfont.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\devgif.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\devvif0.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\devvif1.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\devvu0.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\devvu1.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\dpaddr.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\dplay8.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\DirectX8\dxfile.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_Draw.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_Engine.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_Input.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_Input_Gadgets.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_ScratchMem.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_View.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\Common\e_View_inline.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\e_VRAM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\eekernel.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\eeregs.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\eestruct.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\eetypes.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\Entropy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\common\include\libcdvd.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libdev.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libdma.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libdmapk.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libgifpk.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libgraph.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libpad.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libpkt.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\libvifpk.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_dlist.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_dma.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_dmaman.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_font.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_framebuf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_gsreg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_misc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_vifgif.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\Entropy\PS2\ps2_vram.hpp
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\gcc\ee\include\sys\reent.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\sifdev.h
# End Source File
# Begin Source File

SOURCE=..\..\xCore\3rdParty\PS2\Sony\sce\ee\include\sifrpc.h
# End Source File
# Begin Source File

SOURCE=..\Tokenizer\tok.hpp
# End Source File
# Begin Source File

SOURCE=..\Tokenizer\tokenizer.hpp
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
# Begin Group "mcode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mcode\block0_clip.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\block0_noclip.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\boot.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\clipper.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\fog.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\kernel.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\matrix.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\memlayout.vu
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mcode\terrvu.vu

!IF  "$(CFG)" == "Terrain - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Terrain - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mcode\tris_noclip.vu
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\Terr.cpp

!IF  "$(CFG)" == "Terrain - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Terrain - Win32 Debug"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DVD Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DVD Debug"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 Client Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 Client Debug"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DevKit Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Terrain - Win32 PS2 DevKit Debug"

# ADD CPP /O2

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\Terr.hpp
# End Source File
# End Target
# End Project
