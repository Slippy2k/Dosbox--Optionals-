DOSBox SVN r4063 (Optionals) Build on (Nov 18 2017 02:40:08)


Features Compiled: 32Bit DynamicX68 FpuCoreX68 OpenGL3 3DFX CGA-Extension 
                   PNG Fluidsynth MT32 SSI2001 M.A.M.E.-Sound Nuked-OPL3 
                   DirectSerial IPX Modem 
				   

Features: In and i have done
- Added** Updates from 4006 to 4063 manual
- static (Single Exe)
- SDL2 2.0.4
- SDL2 Net

Sound  :
	- Mamesound Branch
	- Munt MT32	
	- FM Sound Adjusting
	- Innovation SSI-2001 reSID Emul. (Tested with Ultima6)
	- Added a second PCSpeaker Emulation (Starcontrol works)
	- Mixer Volume Up/Down Keys

Graphic:
	- OpenGL3 (Look at ENHANCEMENTS.txt)
	- 3dfx Voodoo Emulation*
	- CGA w/ Monochrome Monitor Support
	- CGA Composite Mode
	- Borderless in Window Mode
	
Shell  :
	- Multiple Image Mount (with changes)
	- Batch Bug Fix
	- Added Config.sys Commands
	- Added Shell Commands
	- Estended Prompt Command
	- Extended & Fixed the del command (*,*.*,*.,.*)
	- Mouse Capture (with Code changes)
	
Various:
	- Added a Shader Path to the Conf
	- Config Create/Load where dosbox is (Appdata is gone)
	- Unlock Speed with CTRL-ALT-F12
	- Speed Meter, i added in the Conf a Variable for this
	- Fixed a few *char Compile Warnings
	- Added Source Fixes by ntfshard
	- Mouse Capture Code Changed to work from the Autoexec
	- Dosbox Splash Settings (on/offduration/fadetime)
	- Dosbox Shudown Key change via conf
	- Commandline -noconsole is now -console
	- Commandline -version changed (add version.h/version.cpp)

	
Optionals Fixes
	- Windows Taskbar height was on Non Fullscreen not calculated.
	- Fullscreen/Non Fullscreen Mouse Input
	- Fixed 3DFX Voodoo, Crash with Output Method OpenGl3
	- Fixed 3DFX Voodoo, Crash Texture Renderer Surface/Direct3d/Auto
	  ("Auto", On Windows -> SDL take Direct3D first)
	  
	
	
Features: Out
- GFX: Direct Draw
- GFX: Overlay


**Manually Update Status.
r4007 - Not needed because SDL2, OpenGL Fix in the sdlmain
r4052 - Not needed because SDL2, 2nothing to do with dangling comma
r4056 - Not Needed, ddraw is in the fork Obsolete
https://sourceforge.net/p/dosbox/code-0/commit_browser


******
Added ADDKEY, LABEL, MORE, MOVE, PROMPT, VOL

Added DEVICE.COM
      BUFFERS.COM
      FCBS.COM
      FILES.COM
      LASTDRIV.COM
      28.COM, 50.COM

	  *
- Voodoo Patch (SDL1)
I have now convertet the SDL1 code to SDL2.
The entire 3DFX screen is now in DOSBox. No a Extra Seperate
Screen/Window. You change the Window and FullScreen 3DFX
Resolution. However, it might works on some games and on 
other not yet. Note: Best Settings is texture/ texturenb
as Output Method an Texture Render set to OpenGL.
If you use OpenGL/OpenGLnb (This is OpenGL3) sometimes it
can crash. If you have Problems try compat_flag to true.
To activate the Glide, Copy from the archive
".\! Extras\Extras Dosbox\FOR 3DFX" the glide file
to you game directory.



**
If Dosbox doesn't found the Dosbox.conf.
Config File wil be created in the Directory where Dosbox.exe is.
Config will be not created in AppData Path. The search path is
- \DATA\Dosbox.conf
- \Dosbox.conf

***
Change the Shaderpath
Change: Dobsox.conf-> sdl -> gl.shpath

****
Make window Borderless
Change: Dobsox.conf-> sdl -> windowborderless

*****
I added the fetaure for the Mixer Volume hotkeys.
You can change the Mixer Volume via ALT-F9/ALT-F10.


Patches:
Based on the DosBox fork. OpenGL 3 GLSL shaders, FluidSynth (SDL2)
http://www.vogons.org/viewtopic.php?f=41&t=43068

AUDIO - Nuked OPL3 Patch for DOSBox(+Windows build)
https://sourceforge.net/p/dosbox/patches/270/
http://www.vogons.org/viewtopic.php?f=41&t=49327

VIDEO - CGA w/ Monochrome Monitor Support
http://www.vogons.org/viewtopic.php?f=41&t=29101

Mounting multiple floppies to same drive (CTRL+F4 Switch)
http://www.vogons.org/viewtopic.php?f=41&t=21213

#263 Volume Up/Down hotkeys
https://sourceforge.net/p/dosbox/patches/263/

#219 Add support for user-defined prompts 
https://sourceforge.net/p/dosbox/patches/219/

#199 fix for unlock speed + cycles=max/auto 
https://sourceforge.net/p/dosbox/patches/199/

#194 bug fix for batch file
https://sourceforge.net/p/dosbox/patches/194/

#181 28.com/50.com & config.sys commands 
https://sourceforge.net/p/dosbox/patches/181/

#111 More parameters support in internal commands 
https://sourceforge.net/p/dosbox/patches/111/
https://sourceforge.net/p/dosbox/patches/57/ < Addkey Info

#75 speed meter (Patch without Titlebar support)
https://sourceforge.net/p/dosbox/patches/75/ 

#182 Innovation SSI-2001 emulation 
https://sourceforge.net/p/dosbox/patches/182/ (Makes Compile Errors)
Using -> reSID from https://github.com/leafo/goattracker2

Patch with various fixes
http://www.vogons.org/viewtopic.php?f=41&t=48218

CLI - DIR switches
http://www.vogons.org/viewtopic.php?f=41&t=18081

INPUT - Capture/release mouse from the command line
http://www.vogons.org/viewtopic.php?f=41&t=45688

CGA Composite Mode under DOSBOX (Commited r3804)
http://www.vogons.org/viewtopic.php?f=41&t=12319&start=680


Marty

My thanks go to the DOSBox Team, dungan (for this Fork from Dosbox, i love the shader), nukeykt (Nuked OPL3 emulator), VileRancour (CGA w/ Monochrome Monitor Support), D_Skywalk (Multi-Floppy-Images), Michał Kępień (Volume Up/Down hotkeys ), TaeWoong Yoo (Various Patches), `Moe` (Speed Meter), Vasyl Tsvirkunov (Innovation SSI-2001 emulation ), kekko (3dfx voodoo emulation)