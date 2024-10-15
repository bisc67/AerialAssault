# Microsoft Developer Studio Project File - Name="Projectiles" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Projectiles - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Projectiles.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Projectiles.mak" CFG="Projectiles - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Projectiles - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 PS2 DVD Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 PS2 DVD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 PS2 Client Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 PS2 Client Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 PS2 DevKit Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Projectiles - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Support/Objects/Projectiles", DDIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Projectiles - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 PS2 DVD Release"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 PS2 DVD Debug"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 PS2 Client Release"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 PS2 Client Debug"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 PS2 DevKit Release"

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

!ELSEIF  "$(CFG)" == "Projectiles - Win32 PS2 DevKit Debug"

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

# Name "Projectiles - Win32 Release"
# Name "Projectiles - Win32 Debug"
# Name "Projectiles - Win32 PS2 DVD Release"
# Name "Projectiles - Win32 PS2 DVD Debug"
# Name "Projectiles - Win32 PS2 Client Release"
# Name "Projectiles - Win32 PS2 Client Debug"
# Name "Projectiles - Win32 PS2 DevKit Release"
# Name "Projectiles - Win32 PS2 DevKit Debug"
# Begin Group "Projectiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\blaster.cpp
# End Source File
# Begin Source File

SOURCE=.\blaster.hpp
# End Source File
# Begin Source File

SOURCE=.\bomb.cpp
# End Source File
# Begin Source File

SOURCE=.\bomb.hpp
# End Source File
# Begin Source File

SOURCE=.\bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\bullet.hpp
# End Source File
# Begin Source File

SOURCE=.\Disk.cpp
# End Source File
# Begin Source File

SOURCE=.\Disk.hpp
# End Source File
# Begin Source File

SOURCE=.\generic.cpp
# End Source File
# Begin Source File

SOURCE=.\generic.hpp
# End Source File
# Begin Source File

SOURCE=.\Grenade.cpp
# End Source File
# Begin Source File

SOURCE=.\Grenade.hpp
# End Source File
# Begin Source File

SOURCE=.\Laser.cpp
# End Source File
# Begin Source File

SOURCE=.\Laser.hpp
# End Source File
# Begin Source File

SOURCE=.\Mine.cpp
# End Source File
# Begin Source File

SOURCE=.\Mine.hpp
# End Source File
# Begin Source File

SOURCE=.\missile.cpp
# End Source File
# Begin Source File

SOURCE=.\missile.hpp
# End Source File
# Begin Source File

SOURCE=.\Mortar.cpp
# End Source File
# Begin Source File

SOURCE=.\Mortar.hpp
# End Source File
# Begin Source File

SOURCE=.\plasma.cpp
# End Source File
# Begin Source File

SOURCE=.\plasma.hpp
# End Source File
# Begin Source File

SOURCE=.\SatchelCharge.cpp
# End Source File
# Begin Source File

SOURCE=.\SatchelCharge.hpp
# End Source File
# Begin Source File

SOURCE=.\ShrikeShot.cpp
# End Source File
# Begin Source File

SOURCE=.\ShrikeShot.hpp
# End Source File
# End Group
# Begin Group "Support"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Aimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Aimer.hpp
# End Source File
# Begin Source File

SOURCE=.\Arcing.cpp
# End Source File
# Begin Source File

SOURCE=.\Arcing.hpp
# End Source File
# Begin Source File

SOURCE=.\AutoAim.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoAim.hpp
# End Source File
# Begin Source File

SOURCE=.\linear.cpp
# End Source File
# Begin Source File

SOURCE=.\linear.hpp
# End Source File
# Begin Source File

SOURCE=.\textures.hpp
# End Source File
# End Group
# Begin Group "Assets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Asset.cpp
# End Source File
# Begin Source File

SOURCE=.\Asset.hpp
# End Source File
# Begin Source File

SOURCE=.\Generator.cpp
# End Source File
# Begin Source File

SOURCE=.\Generator.hpp
# End Source File
# Begin Source File

SOURCE=.\Sensor.cpp
# End Source File
# Begin Source File

SOURCE=.\Sensor.hpp
# End Source File
# Begin Source File

SOURCE=.\Station.cpp
# End Source File
# Begin Source File

SOURCE=.\Station.hpp
# End Source File
# Begin Source File

SOURCE=.\Turret.cpp
# End Source File
# Begin Source File

SOURCE=.\Turret.hpp
# End Source File
# Begin Source File

SOURCE=.\VehiclePad.cpp
# End Source File
# Begin Source File

SOURCE=.\VehiclePad.hpp
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Beacon.cpp
# End Source File
# Begin Source File

SOURCE=.\Beacon.hpp
# End Source File
# Begin Source File

SOURCE=.\Bounds.cpp
# End Source File
# Begin Source File

SOURCE=.\Bounds.hpp
# End Source File
# Begin Source File

SOURCE=.\Bubble.cpp
# End Source File
# Begin Source File

SOURCE=.\Bubble.hpp
# End Source File
# Begin Source File

SOURCE=.\debris.cpp
# End Source File
# Begin Source File

SOURCE=.\debris.hpp
# End Source File
# Begin Source File

SOURCE=.\Flag.cpp
# End Source File
# Begin Source File

SOURCE=.\Flag.hpp
# End Source File
# Begin Source File

SOURCE=.\FlipFlop.cpp
# End Source File
# Begin Source File

SOURCE=.\FlipFlop.hpp
# End Source File
# Begin Source File

SOURCE=.\ForceField.cpp
# End Source File
# Begin Source File

SOURCE=.\ForceField.hpp
# End Source File
# Begin Source File

SOURCE=.\Nexus.cpp
# End Source File
# Begin Source File

SOURCE=.\Nexus.hpp
# End Source File
# Begin Source File

SOURCE=.\ParticleObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleObject.hpp
# End Source File
# Begin Source File

SOURCE=..\PlaceHolder\PlaceHolder.cpp
# End Source File
# Begin Source File

SOURCE=..\PlaceHolder\PlaceHolder.hpp
# End Source File
# Begin Source File

SOURCE=.\Projector.cpp
# End Source File
# Begin Source File

SOURCE=.\Projector.hpp
# End Source File
# Begin Source File

SOURCE=..\Scenic\scenic.cpp
# End Source File
# Begin Source File

SOURCE=..\Scenic\scenic.hpp
# End Source File
# Begin Source File

SOURCE=.\textures.cpp
# End Source File
# Begin Source File

SOURCE=.\WayPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\WayPoint.hpp
# End Source File
# End Group
# End Target
# End Project
