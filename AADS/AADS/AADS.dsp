# Microsoft Developer Studio Project File - Name="*AADS*" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=*AADS* - Win32 Linux Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AADS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AADS.mak" CFG="*AADS* - Win32 Linux Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "*AADS* - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "*AADS* - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "*AADS* - Win32 Linux Debug" (based on "Win32 (x86) Application")
!MESSAGE "*AADS* - Win32 Linux Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "AADS"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "*AADS* - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "." /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /I "$(AADS)\Support\BotEditor" /D "NDEBUG" /D "_WINDOWS" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /fixed:no
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "*AADS* - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /I "$(AADS)\Support\BotEditor" /D "_DEBUG" /D "_WINDOWS" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "*AADS* - Win32 Linux Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "_AADS____Win32_Linux_Debug"
# PROP BASE Intermediate_Dir "_AADS____Win32_Linux_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Debug"
# PROP Intermediate_Dir "Linux-Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /I "$(AADS)\Support\BotEditor" /D "_DEBUG" /D "_WINDOWS" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /I "$(AADS)\Support\BotEditor" /D "TARGET_LINUX" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"Linux-Debug/AADS.elf" /pdbtype:sept /D:TARGET_LINUX
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Linux-Debug\AADS.elf
SOURCE="$(InputPath)"
PostBuild_Cmds=..\xcore\3rdparty\linux\bin\objcopy --strip-debug  $(TargetPath) t:\aads.elf 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "*AADS* - Win32 Linux Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "_AADS____Win32_Linux_Release"
# PROP BASE Intermediate_Dir "_AADS____Win32_Linux_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Linux-Release"
# PROP Intermediate_Dir "Linux-Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /I "$(AADS)\Support\BotEditor" /D "_DEBUG" /D "_WINDOWS" /D "X_DEBUG" /D "X_ASSERT" /D "$(USERNAME)" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /O2 /I "$(X)" /I "$(X)\x_files" /I "$(X)\Entropy" /I "$(AADS)\Support" /I "$(AADS)" /I "$(AADS)\Support\BotEditor" /D "TARGET_LINUX" /D "$(USERNAME)" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /machine:I386 /out:"t:\aads-release.elf" /pdbtype:sept /D:TARGET_LINUX
# SUBTRACT LINK32 /pdb:none /debug
# Begin Special Build Tool
TargetPath=t:\aads-release.elf
SOURCE="$(InputPath)"
PostBuild_Cmds=..\xcore\3rdparty\linux\bin\objcopy --strip-debug  $(TargetPath) t:\aads
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "*AADS* - Win32 Release"
# Name "*AADS* - Win32 Debug"
# Name "*AADS* - Win32 Linux Debug"
# Name "*AADS* - Win32 Linux Release"
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

SOURCE=.\Data\Shapes\DesertShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\GameShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\IceShapes.txt
# End Source File
# Begin Source File

SOURCE=.\Data\Shapes\LavaShapes.txt
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

SOURCE=.\event_Time.cpp
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Support\PointLight\PointLight.hpp
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

SOURCE=..\Support\Damage\Damage.cpp
# End Source File
# Begin Source File

SOURCE=..\Support\Damage\Damage.hpp
# End Source File
# Begin Source File

SOURCE=..\Support\Fog\Fog.cpp
# End Source File
# Begin Source File

SOURCE=..\Support\Fog\Fog.hpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=..\Support\Sky\Sky.cpp
# End Source File
# Begin Source File

SOURCE=..\Support\Sky\Sky.hpp
# End Source File
# Begin Source File

SOURCE=.\SpecialVersion.hpp
# End Source File
# End Target
# End Project
