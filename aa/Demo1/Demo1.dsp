# Microsoft Developer Studio Project File - Name="*Demo1*" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=*Demo1* - Win32 PS2 DevKit Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Demo1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Demo1.mak" CFG="*Demo1* - Win32 PS2 DevKit Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "*Demo1* - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 PS2 DVD Release" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 PS2 DVD Debug" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 PS2 Client Release" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 PS2 Client Debug" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 PS2 DevKit Release" (based on "Win32 (x86) Application")
!MESSAGE "*Demo1* - Win32 PS2 DevKit Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T2/Demo1", URHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "*Demo1* - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(X)\x_files" /I "$(X)\Entropy" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "." /I "$(Tribes)\Support\BotEditor" /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "$(USERNAME)" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /fixed:no
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)\x_files" /I "$(X)\Entropy" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "X_DEBUG" /D "X_ASSERT" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(Tribes)\Support\BotEditor" /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-DVD-Release"
# PROP BASE Intermediate_Dir "PS2-DVD-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-DVD-Release"
# PROP Intermediate_Dir "PS2-DVD-Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /O2 /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-DVD-Release/Demo1.elf" /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LINK32 libscedemo.a /machine:IX86 /out:"PS2-DVD-Release/Demo1.elf" /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-DVD-Debug"
# PROP BASE Intermediate_Dir "PS2-DVD-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-DVD-Debug"
# PROP Intermediate_Dir "PS2-DVD-Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /Zi /Od /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DVD" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /debug /machine:IX86 /out:"PS2-DVD-Debug/Demo1.elf" /D:TARGET_PS2_DVD /D:VENDOR_SN
# ADD LINK32 libscedemo.a /debug /machine:IX86 /out:"PS2-DVD-Debug/Demo1.elf" /D:TARGET_PS2_DVD /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-Client-Release"
# PROP BASE Intermediate_Dir "PS2-Client-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-Client-Release"
# PROP Intermediate_Dir "PS2-Client-Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /O2 /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-Client-Release/Demo1.elf" /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LINK32 sntty.lib /machine:IX86 /out:"PS2-Client-Release/Demo1.elf" /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-Client-Debug"
# PROP BASE Intermediate_Dir "PS2-Client-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-Client-Debug"
# PROP Intermediate_Dir "PS2-Client-Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /Zi /Od /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_CLIENT" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /debug /machine:IX86 /out:"PS2-Client-Debug/Demo1.elf" /D:TARGET_PS2_CLIENT /D:VENDOR_SN
# ADD LINK32 sntty.lib /debug /machine:IX86 /out:"PS2-Client-Debug/Demo1.elf" /D:TARGET_PS2_CLIENT /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PS2-DevKit-Release"
# PROP BASE Intermediate_Dir "PS2-DevKit-Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PS2-DevKit-Release"
# PROP Intermediate_Dir "PS2-DevKit-Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /O2 /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /FD
# ADD CPP /O2 /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86 /out:"PS2-DevKit-Release/Demo1.elf" /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LINK32 sntty.lib /machine:IX86 /out:"PS2-DevKit-Release/Demo1.elf" /D:TARGET_PS2_DEV /D:VENDOR_SN

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PS2-DevKit-Debug"
# PROP BASE Intermediate_Dir "PS2-DevKit-Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2-DevKit-Debug"
# PROP Intermediate_Dir "PS2-DevKit-Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /Zi /Od /I "$(X)\x_files" /I "$(X)\Entropy" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /FD
# ADD CPP /Zi /Od /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(Tribes)\Support" /I "$(Tribes)" /D "TARGET_PS2_DEV" /D "VENDOR_SN" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /FD
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /debug /machine:IX86 /out:"PS2-DevKit-Debug/Demo1.elf" /D:TARGET_PS2_DEV /D:VENDOR_SN
# ADD LINK32 sntty.lib /debug /machine:IX86 /out:"PS2-DevKit-Debug/Demo1.elf" /D:TARGET_PS2_DEV /D:VENDOR_SN

!ENDIF 

# Begin Target

# Name "*Demo1* - Win32 Release"
# Name "*Demo1* - Win32 Debug"
# Name "*Demo1* - Win32 PS2 DVD Release"
# Name "*Demo1* - Win32 PS2 DVD Debug"
# Name "*Demo1* - Win32 PS2 Client Release"
# Name "*Demo1* - Win32 PS2 Client Debug"
# Name "*Demo1* - Win32 PS2 DevKit Release"
# Name "*Demo1* - Win32 PS2 DevKit Debug"
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Group "Shapes"

# PROP Default_Filter ""
# Begin Group "Skel"

# PROP Default_Filter ""
# Begin Group "LightFemale"

# PROP Default_Filter ""
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_back.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_celbow.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_celdance.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_celsalute.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_celwave.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_dieback.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_diechest.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_dieforward.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_diehead.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_dieknees.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_dieleglf.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_dielegrt.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_diesidelf.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_diesidert.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_dieslump.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_diespin.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_fall.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_forward.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_head.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_headside.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_idlepda.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_jet.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_jumpup.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_land.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_lookde.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_lookms.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_looksn.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_recoilde.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_root.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\light_female_side.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_sitting.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_tauntbest.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_tauntbutt.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_tauntimp.skel"
# End Source File
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Player_models\female\light\FullBody\light_female_tauntkiss.skel"
# End Source File
# End Group
# Begin Group "Weapons"

# PROP Default_Filter ""
# Begin Source File

SOURCE="P:\Art\Tribes2_Art WORKING\base\shapes\Weapons\Spinfusor\weapon_disc.skel"
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\Data\Shapes\DesertShapes.log
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\DesertShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\GameShapes.log
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\GameShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\IceShapes.log
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\IceShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\LavaShapes.log
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\LavaShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\LushShapes.log
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\LushShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\XBmpToolScript1.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\XBmpToolScript2.txt
# End Source File
# End Group
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Data\particles\particles.txt
# End Source File
# End Group
# Begin Group "Characters"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Data\Characters\CommonDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\ControlDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\DefaultHeavyDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\DefaultLightDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\DefaultMediumDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\HeavyBioDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\HeavyFemaleDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\HeavyMaleDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\LightBioDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\LightFemaleDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\LightMaleDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\MediumBioDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\MediumFemaleDefines.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Characters\MediumMaleDefines.txt
# End Source File
# End Group
# Begin Group "Vehicles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Data\Vehicles\VehiclePhysics.txt
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\fe_colors.cpp
# End Source File
# Begin Source File

SOURCE=.\fe_colors.hpp
# End Source File
# Begin Source File

SOURCE=.\fe_Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\fe_Globals.hpp
# End Source File
# Begin Source File

SOURCE=.\FrontEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FrontEnd.hpp
# End Source File
# Begin Source File

SOURCE=.\GameUser.cpp
# End Source File
# Begin Source File

SOURCE=.\GameUser.hpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.hpp
# End Source File
# Begin Source File

SOURCE=.\MissionIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SavedGame.hpp
# End Source File
# Begin Source File

SOURCE=.\serverman.cpp
# End Source File
# Begin Source File

SOURCE=.\serverman.hpp
# End Source File
# End Group
# Begin Group "Events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\event_Input.cpp
# End Source File
# Begin Source File

SOURCE=.\event_Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\event_Render.cpp
# End Source File
# Begin Source File

SOURCE=.\event_Time.cpp
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Support\PointLight\PointLight.cpp
# End Source File
# Begin Source File

SOURCE=..\Support\PointLight\PointLight.hpp
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# End Group
# Begin Group "BotEditor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Support\BotEditor\BotEditorScrip.rc

!IF  "$(CFG)" == "*Demo1* - Win32 Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 Debug"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg_Admin.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Admin.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_AudioOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_AudioOptions.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Brief.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Brief.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Buddies.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Buddies.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Campaign.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Campaign.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_ClientLimits.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_ClientLimits.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_ControlConfig2.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_ControlConfig2.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Controls.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Controls.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_DeBrief.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_DeBrief.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Debug.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Inventory.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_LoadSave.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_LoadSave.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_MainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_MainMenu.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Message.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_MissionEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_MissionEnd.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_MissionLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_MissionLoad.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_NetworkOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_NetworkOptions.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Objectives.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Objectives.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Online.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Online.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_OnlineHost.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_OnlineHost.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_OnlineJoin.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_OnlineJoin.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Player.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Player.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Score.cpp

!IF  "$(CFG)" == "*Demo1* - Win32 Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 Debug"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dlg_Score.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_soundtest.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_SoundTest.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_util_RenderController.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_util_RenderController.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_Vehicle.hpp
# End Source File
# Begin Source File

SOURCE=.\dlg_WarriorSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_WarriorSetup.hpp
# End Source File
# Begin Source File

SOURCE=.\ui_joinlist.cpp
# End Source File
# Begin Source File

SOURCE=.\ui_joinlist.hpp
# End Source File
# End Group
# Begin Group "ClientServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClientServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientServer.hpp
# End Source File
# Begin Source File

SOURCE=.\ConnManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnManager.hpp
# End Source File
# Begin Source File

SOURCE=.\GameClient.cpp
# End Source File
# Begin Source File

SOURCE=.\GameClient.hpp
# End Source File
# Begin Source File

SOURCE=.\GameEventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GameEventManager.hpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.hpp
# End Source File
# Begin Source File

SOURCE=.\MoveManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveManager.hpp
# End Source File
# Begin Source File

SOURCE=.\ServerVersion.hpp
# End Source File
# Begin Source File

SOURCE=.\sm_campaign.cpp
# End Source File
# Begin Source File

SOURCE=.\sm_client.cpp
# End Source File
# Begin Source File

SOURCE=.\sm_common.cpp
# End Source File
# Begin Source File

SOURCE=.\sm_common.hpp
# End Source File
# Begin Source File

SOURCE=.\sm_server.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateManager.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\crt0.s

!IF  "$(CFG)" == "*Demo1* - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Debug"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Debug"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Support\Damage\Damage.cpp
# End Source File
# Begin Source File

SOURCE=..\Support\Damage\Damage.hpp
# End Source File
# Begin Source File

SOURCE=..\Support\Fog\Fog.cpp

!IF  "$(CFG)" == "*Demo1* - Win32 Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 Debug"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DVD Debug"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 Client Debug"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Release"

!ELSEIF  "$(CFG)" == "*Demo1* - Win32 PS2 DevKit Debug"

# ADD CPP /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Support\Fog\Fog.hpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\PS2.lk
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SpecialVersion.hpp
# End Source File
# Begin Source File

SOURCE=.\titles.cpp
# End Source File
# Begin Source File

SOURCE=.\titles.hpp
# End Source File
# End Target
# End Project
