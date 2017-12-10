DOSBox SVN r4066 (Optionals v10) Build on (Dec 10 2017 18:41:05)


Features Compiled: 32Bit DynamicX68 FpuCoreX68 OpenGL3 3DFX CGA-Extension 
                   PNG Fluidsynth MT32 SSI2001 M.A.M.E.-Sound Nuked-OPL3 
                   DirectSerial IPX Modem 
				   

Features: In and i have done
- Added** Updates from 4006 to 4064 manual
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

	
Optionals Version: Fixes/ Changes / Additional
	- Windows Taskbar height was on Non Fullscreen not calculated.
	- Fullscreen/Non Fullscreen Mouse Input
	- Fixed 3DFX Voodoo, Crash with Output Method OpenGl3
	- Fixed 3DFX Voodoo, Crash Texture Renderer Surface/Direct3d/Auto
	  ("Auto", On Windows -> SDL take Direct3D first)
	- CGA Mono and Hercules Color can be set in the conf as default color
	
20.11	
	- stdout and stderr are dosbox_log, dosbox_err. If exists a sub 
           directory 'DATA'. save to the sub directory	
	  
25.11
    	- OpenGL3 Output Method, On 16 or 32Bit it jumps to Surface Method
	  Jump Label excluded. On Windows the shader works (Screen Test Windows 3.1)
	- OpenGl3 Output, VGAOnly/EGA Ratio Mismatch/Melt, Screen Height Doubled Fixed
	
27.11
	- Quick Fix for VGAonly and Aspect Ratio (Thanks to Diduz @ Vogons Forum)
30.11 
	- Rework Aspect Ratio Fix. This fixes Sleepwalker and Jazz Rabbit in vgaonly
	  additional Monolith Diffrent Screen Resolution Fixed. (Thanks to Diduz @ Vogons Forum)
32.11
	- Fixed Aspect Ration on EGA & VGAOnly. This fixed Resolution problems with Sleewalker
	  and Jazz Jack Rabbit.
	- VGA Multisync. Double Screen Resolution normalized (Scene Demo Monolith)
	- Update crt_lottes( & _mod). Added a Black mask for the Warp x and y. On my System
	  the Top and Left border was a glitch.
	- Added for svga_s3 a Mameory Option in the conf 'memsvga3'. A few Games or Scene Demos
	  brings a Message with 'not enough memory video'. You can Force them.
	- Added programs on Drive Z: (S3REFRESH, NOLFB.COM (to use with svga_s3), CHKVESA.COM)
        - Added programs on Drive Z: (Box, a echo command with a box) (This was only a test)!
        - Added programs on Drive Z: NOVERT.COM. This is eq. for 3D Shooter. This disabled the
          Forward Mode with the Vertical Mouse Movement.
        - Added Few Fixes from Dosbox-X. Eq. for Scene Demos.
        - Added in the conf 'CutVesaResolutionIndex'. You can cap the Resolution Index. This
          Fixed eg. Warcraft2
        - Reworked the generated Dosbox.conf. It is now Reader friendly.


03.12
	- Added and Update Dosbox Optionals to svn r4064
	  Allow for direct changing of 4op chaining without having to rewrite the algorithm mode.

	- Added a Missing Resolution Index 0x68, If you set in 'The Big Red Adventure' S3 Chip
	  and it founded. It saved in the bat the commandline '-24 (For S3) and the
	  Resolution Index 104'. this is Index 0x68. On Gamestart it fails because not founded.
    
	- Added from Freedos Project The memory Manager Programm 'JEMMEX'. I've searched the
	  Internet for the Tool. Now its permanent in Dosbox. I hate this 'searchen'
	  
06.12
	- Added and Update Dosbox Optionals to svn r4065
	  Fix bug where joysticks got disabled after a section restart (when changing parameters while running).
		Add code to map circularly restricted analogue input to be mapped to squares.
		Add deadzone support in both square and circular mode.
		Add deadzone=100 as fake digital device (idea by Hidden Asbestos)	
		
	- Added GETDIG.COM Program (Similar Choice but only with Digits Only)
    - Added Maschinentype/GFX Card to the Window Title
	
10.12
    - Added in the conf [SDL]. 'VoodooUseOwnFullScRes' and 'VoodooUseOwnWindowRes'.
	  On true the Voodoo 3DFX resolution is taken from the configuration settings windowresolution/
	  fullresolution. Set to false so you can change in the section	pci a own Resolution or if you
	  run with a game in problems and higher Resolution you can change this and use the Original Resolution.
	 
    - Added in the conf [SDL] 'VoodooDesktopFullScrn'. With this bool is controlled whether the
	  3DFX Fullscreen 'native' is or as 'Fullscreen Desktop' traded (SDL2). For 3DFX games that have no
	  problem with it (High Resolutions), such as Tomb Raider, Fatal Racing aka Screamer, D1 Enemy Zero
	  (Windows 95 with Voodoo driver) is recommended to set the value to true. For games like Pyl,
	  Blood etc .. put the value on false.
	
	- Added in the conf [SDL] WindowsTaskbarAdjust. With the switch you can adjust the fine tuning for
	  the centering of the window in the height if the window 'still' hangs a few pixels under the
	  Windows taskbar.
	
	- Update to SVN Code 4066
	  Allow an autoinit transfer to go over into a single cycle transfer. Fixes Paddlers.
	  
	- Added imgmake Patch. From HAL's Old Megabuild Version. Create HD images.
	- Dosbox Window Title show the Maschine/GFXCard.
	- Reworked the Template for the Autoexec
	- HD Nibblefix from Dobsox-X
	  NTS: DOSBox 0.74 mainline has these backwards: the upper nibble is the first hard disk,
		   the lower nibble is the second hard disk. It makes a big difference to stupid OS's like
		   Windows 95.   
    - Windows Keys (Win Left and Right added to SDL Mapper)
	- Reboot Message changed
	- Doshell Dir switch. More help to the Sort Order Command
	- Added in the conf [SDL] UseAspectHeight. If this true. Dosbox will be use Aspect Ratio
	  on the Screens (Aspect=true/false). if this set to false. Aspect Ratio has no 'heavy' effect
	  the screen hight's. I added this because my Resolution 1280x1024 was always shrink to 1820x960.
	  OK, this 640x480 *2. But it doesn't look good on Windows 3.x/ Windows 95.
	=======================================================================================================
	
	Adding and take a other few Fixes from Dosbox-X (Credits go to the DOSBox-X author(s))
	/* I have this taken and only adjusted for my DOSBox Source */
	 - VESA BIOS modelist a bit to place 320x200x256 at 0x153. 
			 Realtech's "Countdown" demo will crash if mode 0x153 is not 320x200x256
			 if you run it with S3 acceleration.

	- 	INT 10h bugfix: Before returning from INT 10h, make sure the VGA

			Attribute Controller is reset to the index state, not left in data
			state. Some demos (KUKOO2) call INT 10h and assume the AC is in the
			index phase.

			This also fixes jumpy scrolling emulation bug for KUKOO2.EXE. Video init
			code in KUKOO2.EXE does not read 3DAh to reset latch, and assumes that
			it can select register 10h, read from 3C1h, and write it back after
			having set bit 5 (0x20) in order to do it's panning effect at top while
			leaving the scrolling text+checkerboard stable at the bottom.
			 
	-  S3 Trio XGA bugfix: The XGA Bresenham line drawing routine sign-extended

			the parameters in a way that was perfectly fine for 32-bit builds, but
			causes the XGA emulation to hang on 64-bit builds. 13-bit sign extension
			has been corrected for both 32-bit and 64-bit builds. This fixes two
			known scenarios where the incorrect sign extension caused hangs: 1)
			Windows 95 with S3 emulation and an application that draws lines on the
			screen (the Mystify screen saver is a good test case) and 2) RealTech's
			1995 demo "Countdown" when S3 acceleration is enabled (at the wireframe
			hourglass part of the demo).
			
	- INT 10 VESA BIOS: new option added. If set, the VESA BIOS modelist is
			capped at the specified number of entries. You can use this option to
			cap the modelist for DOS applications that have problems with the long
			modelist DOSBox-X normally returns in enumeration. Specifically, setting
			this option to cap the modelist at 8-16 options fixes a problem with
			Warcraft II crashing with the DOS4/GW extender. For whatever reason,
			DOSBox-X's normal modelist overruns a buffer somewhere in the game (but
			only with DOS4/GW?) and the game will crash on startup.		 
		
    - Added More Resolution to INt10
	
	-  VGA sequencer emulation fix: when DOS programs write the sequencer index

			register (port 0x3C4) most VGA/SVGA cards actually decode fewer bits
			than the 8-bit value given. This can be seen through casual poking or
			from the Hackipedia VGA register dumps I've posted online, where
			Sequencer registers will repeat through indexes 0-255 depending on how
			many bits are decoded by the hardware. Currently it is known that ET4000
			cards decode only the lowest 3 bits, Trident TVGA cards decode the lowest
			4 bits, and S3 cards decode the lowest 6 bits. This behavior means that
			"aliases" of common Sequencer registers exist that function exactly the
			same as the normal registers.

			The reason for this fix, is the 1993 demo "Spiral" by Dataction, which
			has a flawed Mode X implementation. It sets up VGA 256-color Mode X as
			normal, but instead of writing directly to the Map Mask Register
			(Sequencer index 0x02) it writes to an alias of the Map Mask Register
			(Sequencer index 0x12) that exists on older VGA hardware (standard VGA,
			ET4000, Trident) but not on newer VGA hardware (S3). This is why, when
			the demo is run on newer hardware, the Mode X graphics don't render
			properly. Adding the sequencer masking behavior allows the demo to
			render Mode X properly with machine=svga_et4000 and the graphics no
			longer look like a low resolution blurry mess.
	
	  - S3 bugfix: INT 10h now uses I/O writes to clear & remove the hardware

			cursor. It uses I/O, not direct changes to VGA state, so that it works
			properly within Windows 95 I/O virtualization. This fixes the garbled
			cursor that would otherwise appear if a DOS game goes fullscreen and
			uses the VESA BIOS to switch into 640x480x256 or higher res SVGA modes.
			At the same time, allowing Windows 95 to virtualize I/O means bringing
			up a DOS box in a window will not kill the hardware cursor.
			Bugfix for GitHub issue #148: "Documentation: Windows 95 dos box and
			vesa mode with hardware mouse".
			
	  - S3 Trio emulation bugfix: On S3 cards, the "compatible VGA" vs "Extended

			SVGA" memory mapping is controlled by bit 3 in CRTC register 0x31. INT
			10h emulation already writes the bit via I/O, but manually set the
			"compatible chain 4" enable flag farther up. The problem with setting it
			manually is that the generic "S3" driver in Windows 95 (using bank
			switching and legacy VGA memory 0xA0000-0xAFFFF) expects to trap and
			virtualize that bit through I/O, which it can't when DOSBox just sets it
			internally like that. Prior to the fix, opening a DOS box in Windows 95
			would trigger "compatible chain 4" emulation which would then cause any
			non-accelerated graphics drawn after that point to come out garbled,
			until you forced a video mode reset by entering and leaving fullscreen
			mode in the DOS Box. As a reminder, Windows 95 calls INT 10h AX=0x0003
			(set 80x25 alphanumeric text mode) when opening a DOS box and it does
			so in a virtualized sense so that INT 10h affects the DOS virtual
			machine, not the actual Windows 95 desktop.
	
Features: Out
- GFX: Direct Draw
- GFX: Overlay

	  
NOTE: Fullscreen issues and todos.   
	I think you have already noticed. The whole thing with the fullscreen somehow not working properly.
	I therefore advise you to avoid the fullscreen. Why?
	- With the method Output = Surface, image remains black.
	- With the method Output = Texture (nb), Fullscreen does not work in combination with a specified
	  fullscreen resolution. Result: graphic error
	- With the method Output = OpenGL (nb), see above.
	- 3DFX. It may happen that crashes with the method OpenGL (nb) and Native Fullscreen Dosbox.
	- When you quit Dosbox in fullscreen mode. Keys entered or in the DosShell "exit". Dosbox crashes.
    I know. These are important things that need to be done.	
	
	Windows95 in Dosbox:
	- Shutting down does not always work.
	- The MsDOS Shell and a BPP 32Bit doesn't work
	

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