This is a Fork from [Dosbox](www.dosbox.com)
DOSBox is a DOS-emulator that uses the SDL-library which makes DOSBox very easy to port to different platforms. DOSBox has already been ported to many different platforms, such as Windows, BeOS, Linux, MacOS X...

DOSBox also emulates CPU:286/386 realmode/protected mode, Directory FileSystem/XMS/EMS, Tandy/Hercules/CGA/EGA/VGA/VESA graphics, a SoundBlaster/Gravis Ultra Sound card for excellent sound compatibility with older games...

You can "re-live" the good old days with the help of DOSBox, it can run plenty of the old classics that don't run on your new computer!

DOSBox is totally free of charge and OpenSource.

DOSBox v0.74 Manual (always use the latest version from [Dosbox](www.dosbox.com))

NOTE:

While we are hoping that one day DOSBox will run all programs ever made for
the PC, we are not there yet.
At present, DOSBox running on a high-end machine will roughly be the equivalent
of a Pentium I PC. DOSBox can be configured to run a wide range of DOS games,
from CGA/Tandy/PCjr classics up to games from the Quake era.

This is an enhanced fork of DosBox. It is currently in sync with revision 4063.

Features Compiled: 32Bit DynamicX68 FpuCoreX68 OpenGL3 3DFX CGA-Extension 
                   PNG Fluidsynth MT32 SSI2001 M.A.M.E.-Sound Nuked-OPL3 
                   DirectSerial IPX Modem 

### Configuration

There are new configuration options, and some existing options have new defaults.
If you have a configuration file from a different version of DosBox, then I
recommend removing it and letting this version generate a replacement.

### OpenGL 3 Support

The default video output method, "texturenb", is equivalent to "openglnb" in
vanilla DosBox. Now, "openglnb" and "opengl" use OpenGL 3 and can take advantage
of external shaders.

### External Shaders

The shaders are stored in a separate project. Download them from here:

* [dosbox\_shaders](https://github.com/duganchen/dosbox_shaders)

### Credit

This fork uses, either directly or by building on, the following community contributions:

* [Patch for OpenGL fullscreen bug](http://www.vogons.org/viewtopic.php?f=32&t=27487&start=20#p276738)
* [FluidSynth soundfont patch](http://www.vogons.org/viewtopic.php?f=32&t=27831&start=20#p385413)
* [Basic GLSL shader support (prototype)](http://www.vogons.org/viewtopic.php?f=41&t=36342&start=20#p319636)
* [An adaptation to SDL 2.0](http://www.vogons.org/viewtopic.php?f=32&t=34770&start=40#p433097)

Thanks go to the DOSBox Team, dungan (for this Fork from Dosbox, i love the shader), nukeykt (Nuked OPL3 emulator), VileRancour (CGA w/ Monochrome Monitor Support), D_Skywalk (Multi-Floppy-Images), Michał Kępień (Volume Up/Down hotkeys ), TaeWoong Yoo (Various Patches), `Moe` (Speed Meter), Vasyl Tsvirkunov (Innovation SSI-2001 emulation ), kekko (3dfx voodoo emulation)

For more detais look at the ENHANCEMENTS.md/ OPTIONALS.md

### Compile Enviroment (Windows 7)
Used Mingw and addtional Extensions libs (Fluidsynth, MuntMT32 etc...)
[Static, I have explained at the vogons forum](http://www.vogons.org/viewtopic.php?f=41&t=43068&start=20#p624722)

Dosbox: This based on (r4006) the SDL2/OPENGL3/Fluidsynth Fork from dugan (<a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=43068" target="_blank" rel="noopener">Vogons</a>) 
Support shaders :)

- Features: In and i have done
- From r4006 to r4063, a<span id="result_box" class="short_text" lang="en"><span class="">ll updates have been added by hand.</span></span>
- Static/ Standalone 32Bit Single Exe
- SDL2 2.0.4/ - SDL2 Net

- Sound :
- [Mamesound Branch](https://sourceforge.net/p/dosbox/code-0/commit_browser)
- <a href="https://github.com/munt/munt/tree/master/DOSBox-mt32-patch" target="_blank" rel="noopener">Munt MT32</a>
- FM Sound Adjusting
- <a href="https://sourceforge.net/p/dosbox/patches/182/" target="_blank" rel="noopener">Innovation SSI-2001 reSID Emul.</a> (Tested with Ultima6)
- <a href="http://www.vogons.org/viewtopic.php?t=23371" target="_blank" rel="noopener">Added a second - PCSpeaker Emulation</a> (Starcontrol works)
- <a href="https://sourceforge.net/p/dosbox/patches/270/" target="_blank" rel="noopener">Nuked OPL3 Emulator</a>
- <a href="https://sourceforge.net/p/dosbox/patches/263/" target="_blank" rel="noopener">Mixer Volume Up/Down Keys</a>

- Graphic:
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=43068" target="_blank" rel="noopener">OpenGL3 (Look at ENHANCEMENTS.txt)</a>
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=41853" target="_blank" rel="noopener">3dfx Voodoo Emulation (SDL1)*</a>
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=29101" target="_blank" rel="noopener">CGA w/ Monochrome Monitor Support</a>
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=12319&amp;start=680" target="_blank" rel="noopener">CGA Composite Mode</a>
- Borderless in Window Mode

- Shell :
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=21213" target="_blank" rel="noopener">Multiple Image Mount</a> (with changes)
- <a href="https://sourceforge.net/p/dosbox/patches/194/" target="_blank" rel="noopener">Batch Bug Fix</a>
- <a href="https://sourceforge.net/p/dosbox/patches/111/" target="_blank" rel="noopener">Added Config.sys Commands</a>
- (<a href="https://sourceforge.net/p/dosbox/patches/57/" target="_blank" rel="noopener">ADDKEY</a>, LABEL, MORE, MOVE, PROMPT, VOL, VERO)
- Added Shell Commands
- (DEVICE.COM, BUFFERS.COM,FCBS.COM, FILES.COM)
- (LASTDRIV.COM, MOUSECAP, <a href="https://sourceforge.net/p/dosbox/patches/181/" target="_blank" rel="noopener">28.COM, 50.COM</a>)
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=18081" target="_blank" rel="noopener">Dir Sortswitches</a>
- Estended Prompt Command
- Extended &amp; Fixed the del command (*,*.*,*.,.*)
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=45688" target="_blank" rel="noopener">Mouse Capture</a> (with Code changes)
.</li>
 	<li>Various:
- Added a Shader Path to the Conf
- Config Create/Load where dosbox is (Appdata is gone)
- <a href="https://sourceforge.net/p/dosbox/patches/199/" target="_blank" rel="noopener">Unlock Speed with CTRL-ALT-F12</a>
- <a href="https://sourceforge.net/p/dosbox/patches/75/" target="_blank" rel="noopener">Speed Meter</a>, i added in the Conf a Variable for this
- Fixed a few *char Compile Warnings
- <a href="http://www.vogons.org/viewtopic.php?f=41&amp;t=48218" target="_blank" rel="noopener">Added Source Fixes by ntfshard</a>
- Mouse Capture Code Changed to work from the Autoexec
- Dosbox Splash Settings (on/offduration/fadetime)
- Dosbox Shudown Key change via conf
- Commandline -noconsole is now -console
- Commandline -version changed (add version.h/version.cpp)
.</li>
 	<li>Optionals Fixes
- Windows Taskbar height was on window mode not calculated.
- Fullscreen/Non Fullscreen Mouse Input
- Fixed 3DFX Voodoo, Crash with Output Method OpenGl3 (I hope)
- Fixed 3DFX Voodoo, Crash Texture Renderer Surface/Direct3d/Auto (i hope)
("Auto", On Windows -&gt; SDL take Direct3D first)
.</li>
 	<li>Features: Out
- GFX: Direct Draw
- GFX: Overlay</li>
</ul>
**<a href="https://sourceforge.net/p/dosbox/code-0/commit_browser" target="_blank" rel="noopener">Update Status.</a>
r4007 - Not needed because SDL2, OpenGL Fix in the sdlmain
r4052 - Not needed because SDL2, 2nothing to do with dangling comma
r4056 - Not Needed, ddraw is in the fork Obsolete

<strong>Voodoo Patch (SDL1)</strong>
<span id="result_box" class="" lang="en">That's the hardest part of the whole code. I changed the code from
SDL1 to SDL2. The seperate 3DFX window (window or full screen) is now processed in the DOSBox window. The resolution can be additionally set.
Important:
Games like Tomb Raider or Fatal Racing are running. But there are some games that are not working properly at a high resolution. 3 filters can
be set in the conf default, gl_linear, gl_nearest. If you use OpenGL/OpenGLnb (This is now OpenGL3) sometimes it can crash. If you have Problems
try compat_flag to true. To activate the Glide, Copy from the archive ".\! Extras\Extras Dosbox\FOR 3DFX" the glide file to you game directory.</span>

<strong>If Dosbox doesn't found the Dosbox.conf.</strong>
Config File wil be created in the Directory where Dosbox.exe is. Config will be not created in AppData Path. The search path is
- \DATA\Dosbox.conf
- \Dosbox.conf


Changelog SVN (Look "! Docs/Docs, DOSBox Optionals/CHANGELOG SVN.TXT")
r4007 -> r4101 

0.74
  - Several small game specific fixes/hacks/support. (Offensive,
    Roadhog, GTA installer, Kingdom O' Magic soundcard detection, 
    Pirate booter, Armored Fist installer)
  - Add the S3-specific 640x480 256 color mode. (fixes regression in "Wooden 
    Ships and Iron Men" and "I Have No Mouth  And I Must Scream")
  - Fix a stack overflow that could crash DOSBox.
  - Add fake microphone input. (fixes Talking Parrot)
  - Modify adlib turn off code, so that it doesn't turn off in 
    cases where the same sound is repeated over and over again.
  - Several small fixes to the CDROM audio code. (HOMM2, Redneck 
    Rampage and others)
  - Several improvements to the CDROM emulation code. (fixes Alpha
    Storm and  GT Racing 97)
  - Some small CPU fixes that might fix something.
  - Handle opcode 0xff subcode 7 as invalid instruction. (fixes dif-2 & others)
  - Some hercules fixes. (Testdrive)
  - Improve support for blanked parts that wrap around to the start of
    the screen. (fixes Magic Circle demo and Sid&Al)
  - Remove old OPL cores as the new ones seem to work very nicely.
  - Modify movie recording code so that the movies aren't corrupt when
    you exit dosbox without stopping the movie.
  - Change RGB3x scaler to look more pretty.
  - Improve initial register values compatility of the GUS.
  - Added autodetection for Gameblaster. (games can find it now)
  - Change render preferences a bit to be more compatible with Windows 7.
  - Add DOS fixes to terminate program. (fixes Fortune Teller) 
  - Add FFREEP. (fixes Trucks)
  - Improve FPU ST80 in C mode when writing zero. (fixes Antigok)
  - Add special int10 scanline function. (fixes mz700 and probably lots
    of games that mess with them)
  - Fix scrolling in rarely used video modes. (fixes Orphee)
  - Modify game specific hacks a bit so that Kick off 3 works again.
  - Lots of fixes to the INT10 video parameter table. (Seven spirits 
    of ra and others)
  - Add VGA blanking in machine=vgaonly. (used by Alien Carnage)
  - CGA, PCJr, Tandy: Add video blanking, change display start latch 
    timing, sync pulse width correction.
  - PCJr, Tandy: implement vertical retrace interrupt.
  - PCJr, CGA: line-by-line video emulation.
  - PCJr: support on-screen change of color modes 4medium to 16low. (used
    by Ghostbusters booter)
  - Hercules: Add green and amber monochrome support.
  - All machines: only update the video timing when needed. (Jungle Hunt,
    others that synchronize to the video screen might profit)
  - Several small DOS fixes.
  - Some UMB related fixes. (The Legacy without UMB)
  - Fix version number of DSP for SB 1.5. (fixes a few games)
  - Several VGA emulation improvements. (Allertone football manager)
  - Some Tandy fixes. (Mech Warrior)
  - Small improvements and fixes to the OPL emulation.
  - Add low level Tandy DAC emulation.
  - Some EMS fixes. (fixes Mortal Kombat and others)
  - Change SoundBlaster DSP reset mechanism, add sb irq acknowledge logic.
    (fixes stmik-based applications)
  - Some interrupt pointer location modifications. (fixes Tinker Tales)
  - Some fixes to the BOOT code. (fixes Last Mission)
  - Respect write-only file information. (fixes Champions of Zulala)
  - Some RTC fix. (fixes Tully Bodine and others)
  - Improve mouse emulation to work better with Water World.
  - Hopefully fix the translation of the configuration file.
  - Speed up and fixes for the recompiler core. (pitfall2 pcjr)
  - Change memory start location. (fixes 7th Guest installer)
  - Several fixes to the batch file handling. (Shift and 
    use the typed first %0 instead of the parsed %0)
  - Improve file redirection and redirected line ends. (fixes 
    Phantasmagoria 2 DOS installer)
  - Fix compilation with new MAC os X version.
  - Add 16C550A FIFO support to the serial port emulation.
  - Improve modem emulation to get higher speeds.
  - Change default samplerates to 44100, blocksize to 1024 and prebuffer to 20,
    so that hopefully certain soundcards produce more fluent sound playback.
  - Add some rarely used, but for some games critical flags to 
    the internal commands.
  - Add -userconf flag, so that the userspecific configuration can
    easily be used together with -conf configfile.
  - Improve internal timing with repeating timers (especially with 
    the dynamic core).

0.73
  - Add two new opl2+opl3 emulators. (better speed, different implementation
    approach)
  - Improved DRO recording/better file structure.
  - Add EGA emulation.
  - Add special vga machine mode. Supports more of the exotic tricks like
    changing the palette during screen updates, 9x16 fonts etc.
  - Added special machine modes for the following svga cards:
     - S3
     - Paradise
     - Tseng
  - Fix problems with the vga split line feature.
  - Improve vesa emulation.
  - Add optional selection of old vesa mode for games that don't work
    with certain vesa features.
  - Improve video BIOS emulation to behave more like a real bios.
  - Fixes for emulated 4bpp graphics modes.
  - Fixes to paging system.
  - Various fixes and improvements for the recompiling core.
  - Add arm backend for the recompiling core.
  - Add some mscdex quirks when dealing with files that are exactly 8.3 long.
  - Small fixes to batch file handling.
  - Small fixes to the XMS memory handling.
  - Various fixes for aligned memory on hosts that want it.
  - Various improvements to the mouse.
  - Fixes and small speed ups to the debugger.
  - Fix and improve lot's of compilation problems. (curses detection,
    GCC 3.4 and GCC 4.X fixes)
  - Added some basic auto keyboard layout handling. (windows only currently)
  - Add basic support for evdev keyboard driver.
  - Various fixes to the timer. (improve mode 2 timer changes,
    implement mode 1, improve gate2 handling)
  - Add audio extraction and mci audio support. Should enable CDROM audio
    for Vista and adds volume control.
  - Improve the directory cache speed a lot, especially with mounting slow
    media like network paths.
  - Various fixes to the create temporary file call.
  - Don't keep batchfiles open during execution. Allows rewriting of the
    active batchfile. (menu programs use this trick sometimes)
  - Fix problems with filenames with 2 extensions.
  - Add some more lowlevel dos tables.
  - Fixes to hercules emulation.
  - Fix flag handling for special case of ROR.
  - Make the batchfile handling in regard to IF more flexible.
  - Fixes to scrolling/panning feature.
  - Add prefetch queue emulation.
  - Make the emulated cpu type selectable. This is mainly the
    identification commands and the way paging works.
  - Some special EMS functionality added. (OS handles, zero-page handling)
  - Improve support for EMS when booting a different OS.
  - Improve cdrom speed detection by games.
  - Improve stability of cycle guessing code, when there is background
    activity.
  - Fix various mscdex and cdrom detection schemes.
  - Added Coremidi support on Mac OS X.
  - Improve support for DOS devices when used to detect the existance
    of directories in various ways.
  - Add IRQ 2 emulation on VRET. (ega only)
  - Added video parameter table and video state functionality.
  - Increase default freespace to 250 MB.
  - Some fixes to the fat filesystem handling for disk images.
  - Some soundblaster fixes and command additions.
  - Fix mixer 16bit direct transfers on bigendian hosts.
  
0.72
  - Fixed unitialized variable in joystick. (Fixes crashes on Vista and
    Mac OS X)
  - Some bugfixes and speedups to the 64 bit recompiling core.
  - Fixed sign flag on soundblaster dma transfers (Space Quest 6 intro)
  - Fixed a bug in keyboard layout processing code and fixed certain
    layouts.
  - Fixed Dreamweb.
  - Improved speed unlocking when running cycles=max.
  - Fixed a crash related to the tab completion in the shell.
  - Improved aspect correction code. Should now be like how a real monitor
    handles it.
  - Fixed a bug in the xms status report code. (Blake Stone 1.0 shareware)
  - Added a lot more keyboard layouts.
  - Fix crash related to changing the scaler before a screen was created.
  - Hopefully fixed compilation on *bsd.
  - Enabled auto cpu core selection for recompiling core as well.
  - Made the used joystick selectable when 4axis is specified.
  - Added some hints for inexperienced DOS users to the shell.

0.71
  - Add a new recompiling cpu core, which should be easier to port.
  - Add 64 bit version of the recompiling core.
  - Add mipsel 32 bit version of the recompiling core.
  - Fix a few small problems with FCBs. (fixes Jewels of darkness and
    cyrus chess)
  - Raise some more exceptions. (fixes vbdos)
  - Fix a few problems with the dynamic core. (fixes Inner Words,
    Archmimedean Dynasty and others)
  - Improve/Fix fallback code for certain graphics cards.
  - Fix a few cd audio related bugs.
  - Add an undocumented MSCDEX feature. (Fixes Ultimate Domain)
  - Fix some pcspeaker mode. (fixes Test Drive and similar games)
  - Improve dos keyinput handling. (fixes Wing Commander 3 exit dialog)
  - Remove Exit condition on fully nested mode. (fixes some demo)
  - Add image file size detection.
  - Add/Fix some ansi codes. (fixes PC Larn and certain versions of
    infocom games)
  - Several general DOS fixes. (fixes nba95, hexit and various other games)
  - Add some valid input checks. (fixes 3d body adventure and similar
    games)
  - Fix digital joystick centering problem.
  - Reenable textmode 54 and 55.
  - Fix a pelmask problem with univbe 5.0 lite. (fixes Panzer General)
  - Fix minor mixer underflow.
  - Some general image and bios disk emulation fixes.
  - Hopefully fix compilation on BSD and darwin.
  - Try using ioctl cdrom access by default if possible.
  - Fix some svga detection routine. (fixes Grandest Fleet 2 and Bobby Fischer
    Teaches Chess)
  - You can now close DOSBox using the status window in win32.
  - Add support for NX enabled systems.
  - Fix a casting error which only showed with certain compilers. (fixes
    various games under mac os x and 64 bit linux)
  - Improve timer and add gate 2 support. (fixes various games and
    joystick problems)
  - Improve mouse. Add undocumented backdoor. (fixes Last half of Darkness, 
    PC-BLOX and others)
  - Add/improve support for ~ and ~username in all commands.
  - Fix a font problem with the pcjr/tandy. (fixes personal deskmate 2)
  - Change dma routine a bit. (fixes ticks in sound in various games)
  - Allow read-only diskimages to be booted. (fixes various booter
    games)
  - Add basic hidden file support on cdrom images. (fixes Player
    Manager 2)
  - Add some rarely used functionality to the int10 mode setup. (fixes
    WW2 Battles of the South pacific)
  - Add ability to force scaler usage.
  - Speed up flag generation and make it more 386-like.
  - Some colourful feedback in the mapper.
  - General code cleanup.

0.70
  - Improve register handling and support with XMS.
  - Fix some issues with deleting open files.(windows only issue)
  - Add dummy LPT1 class. (windows only issue)
  - Improve some of the internal dos commands. (choice, copy and shift)
  - Improve ROM area. (for games that use it for random numbers or 
    overwrite it as some sort of detection thing)
  - Improve compatibility of dynamic core by making it handle certain
    pagefaults earlier.
  - Move internal dos tables around so we have more umb memory.
  - Add some dos tables.
  - Dynamic core supports io exceptions.
  - Move some interrupt handlers to XT Bios locations.
  - Add a dynamic fpu on x86.
  - Improve fpu on non-x86.
  - Trapflag gets strict priority over hardware IRQs.
  - Trapflag support for the dynamic core.
  - Add dummy TRx handling.
  - Fix a few rarely used character functions.
  - Improve auto cycle guessing code.
  - Improve and extend the joystick support.
  - Add autofire support.
  - Improve the mapper so you can map keys to the joystick and vice versa.
  - A few game specific video card fixes.
  - Fix some 64 bit cpu bugs.
  - Add support for certain cdrom detection schemes.
  - Improve HSG/Red Book support.
  - Improve MSCDEX.
  - Improve dynamic core support under intel macs.
  - Add basic support for clipper programs.
  - Add support for different keyboard layouts.
  - Add auto core guessing.
  - Fix a few flags bugs.
  - Fix a few small cpu bugs.
  - Improve soundblaster detection rate by various programs.
  - Improve EMS emulation. (allow mapping of non standard regions)
  - Improve keyboard input codes on various OS-es.
  - Fix problems with filenames having stackdata in them.
  - Changed a few basic operations in DOSBox so they take emulated time.
  - Improve dos ioctl functions.
  - Extend cpu core so they are capable of detecting and raising a few
    more exception types.
  - Improve DOS functions when dealing with virtual drive.
  - Improve FAT drives.
  - Better handling of volume-labels in file functions.
  - Image disk cycling capability. (prompt)
  - Try to reduce the impact of using an analog joystick.
  - Several measures to avoid code invalidation on certain types
    of self modification in the dynamic core.
  - Add dynamic core memory function inlining.
  - A few small mouse improvements. (some games are using things they
    shouldn't)
  - Add nullmodem emulation.(h-a-l-9000)
  - Some small cga and hercules fixes.
  - Add more scalers (hq2x/hq3x/sai). (Kronuz)
  - Change configuration file loading support. It now supports
    multiple configuration files.
  - Make dynamic core capable of running some win32s programs.
  - Fix and add some rare soundblaster modes. (Srecko)
  - Better soundblaster mixer controls. (Srecko)
  - Make soundblaster installation under windows much easier.
  - Add device control channel handling. 
  - GEMMIS support (ems under windows).
  - Support more colours in win 3. (vasyl)
  - Don't show unmounted drives in windows filemanager.
  - Fix some bugs in the int13 handler.
  - Simulate some side-effects of bios interrupt handlers on flags.
  - Add IPX functions needed by netbios.
  - Make ports take emulated time.
  - Tabcompletion is now aware of the CD command.
  - Add suppport for the dac pel mask.
  - Fixes to hercules emulation, better detection and bank switching.
  - Fixes to tandy emulation, 640x200x16 mode and different sizes bank.  
  - EGA/VGA memory changes detection for faster rendering.
  - Gus 16 bit fixes.
  - Many timer improvements. 
  - Some pcjr fixes.
  - Some booter fixes.
  - Many small fixes.

0.65
  - Fixed FAT writing.
  - Added some more missing DOS functions.
  - Improved PIC so that it actually honours irq 2/9.
  - Improved intelligent MPU-401 mode so that more games work with it.
  - Some mouse fixes.
  - Changed DMA transfers a bit so they bypass the paging tables.
  - Added S3 XGA functionality.
  - Improved paging so that read and write faults are handled differently.
  - Rewrote exception handling a bit (no exception 0x0B with dos4gw anymore).
  - Added IO exceptions in all but the dynamic core.
  - Some ems improvements.
  - Added midi-device selection code for the windows hosts.
  - Fix crashes/segfaults related to the disabling of the pcspeaker.
  - Added some more FILES=XX detection tricks.
  - Fixed some vga detection schemes.
  - Fixed screenshot corruption when using -noconsole in a read-only directory.
  - Fix wrong scaled screenshots.
  - Added some hidden file functions when using diskimages. (helps with cdrom 
    detection schemes)
  - Fixed a bug in the mixer code, that muted the music in certain games.
  - Added an assembly fpu core.
  - Made the shell more flexible for batch files.
  - Check for unaligned memory acces fixes hangups on ARM processors.
  - Some 64 bit fixes.
  - Added code to change configuration at runtime.
  - Improved ADPCM emulation.
  - Fixed a few cpu instructions.
  - Always report vesa 2.0 and fix some colour issues with vesa games.
  - Fix video mode 0x06 and 0x0a.
  - Improvements to the joystick emulation. 4 buttons are supported as well.
  - Add VCPI emulation for Origin games.
  - Fixed a lot of things in the boot code. Most booters work now.
  - Lots of improvements to the IPX emulation.
  - Rewritten modem emulation. Should work with more games.
  - Improvements to the dos memory managment routines.
  - Add UMB (upper memory blocks) support.
  - Emulate the pause key.
  - Improve Composite CGA mode emulation.
  - Lots of vga compatibility changes.
  - Improved support for chained video modes.
  - Improved mode and palette handling in cga modes.
  - Mount accepts ~ now.
  - Added a few of the EGA RIL functions.
  - Added TandyDAC emulation.
  - OS/2 support.
  - Improved and speed up the dynamic cpu core.
  - Fix some errors in the CD-ROM emulation layer.
  - Added an automatic work-around for some graphics chipsets.
  - Add PCjr support.
  - Allow mousedriver to be replaced. Fixes a few games that come with their
    own (internal) driver.
  - Improved dynamic cpu core so it can handle pagefaults and some obscure 
    types of self-modifying code.
  - Added -noautoexec switch to skip the contents of [autoexec] in the
    configuration file.
  - Improved v86 mode emulation (mainly for Strike Commander).
  - Improved timer behavior.
  - Improved extended keyboard support.
  - Enhanced and added several DOS tables.
  - Made core_full endian safe.
  - Made pagefaults endian safe.
  - Add support for moviecapturing
  - Add support for 15/16/32 bit videomodes.
  - Add some more VESA modi (4 bit).
  - Add 1024x768 output.
  - Changed screenrendering so it only draws changes to the screen.
  - Allow remapping of the EMS page when the dma transfer was started from 
    the page frame
  - Made EMS and DMA work together when playing from a mapped memory page.
  - Renamed several configuration options, so that they are unique.
  - Merged mpu and intelligent into one option.
  - Merged fullfixed and fullresolution.
  - Extended keys should be handled better.
  - F11 and F12 work.
  - Compilation fixes for various platforms.
  - Fix a few crashes when giving bad input.
  - Removed interp2x and added few new scalers.
  - Reintroduce the lockfree mouse. (autolock=false)
  - Add a larger cache for the dynamic cpu core.
  - Improved soundblaster DSP, so it gets detected by creative tools.
  - Lots of bugfixes.
  - Even more bugfixes.
  
0.63
  - Fixed crash with keymapper (ctrl-f1) and output=surface.
  - Added unmounting.
  - Fixed multiple issues with drive labels.
  - Fixed most if not all FILES=XX problems.
  - Added redirection in the shell.
  - Fixed crashes with subst.
  - Fixed multiple crashes with the drive images support.
  - Added a missing fpu instruction.
  - Fixed some cpu and fpu instructions.
  - Fixed a small bug related to font loading.
  - Rewrote the devices support.
  - Added capslock/numlock checks on startup.
  - Fixed wave writing.
  - A few internal DOS fixes.
  - Timer fixes for the hybrid loader.
  - Some small soundblaster fixes.
  - The drive cache can now be cleared by a keycombo. (CTRL-F4)
  - A few keyboard fixes.
  - Compilation fixes on various platforms.
  - Quite some debugger improvements.
  - Fixed dir only showing files after the first run on cdrom drives.
  - Added some cdrom detection checks.
  - Enabled insert in the shell. (Easier editing of commands)
  - Changed order in which executables appear with tab-completion.
  - Fixed some issues with raw opl recording and using a slightly different
    format
  
0.62
  - Added blinking support in the shell and some color fixes.
  - Fixed commandline parsing when .bat files involved (fixes -exit)
  - Fixed issues with tabs in commandline not being processed correctly.
  - Cleaned/improved shutdown sequence.
  - Added some more bios functions (wait and delay functions).
  - Made our XMS driver conform the specs better. (c2woody)
  - Added support for some more ems functions.
  - Added intelligent mpu401 emulation. (Srecko)
  - Added soundblaster 16 emulation.
  - Rewrote GUS emulation to sound more authentic.
  - Improved pc speaker emulation.
  - Added an internal (programmable) mixer.
  - Added support a few soundblaster/adlib detection routines.
  - Fixed lot's of bugs related to DMA transfers.
  - Added interpolating prebuffering mixer routines.
  - Added recording of OPL commands and raw midi.
  - Fixed some bugs with the wave recording.
  - Changed sensitivity settings of the mouse.
  - Added ps2 mouse-emulation in bios interrupts (c2woody).
  - Fixed some bugs with mouse emulation limits.
  - Fixed a bug with an unterminated string in the drivelabel.
  - Changed file search routines a bit to be more compatible.
  - Added support for attribute-searching with fcb's.
  - Added basic SDA.
  - Added TPA and DIB.
  - Added Lot's of missing dos tables (c2woody).
  - Changed psp and dta functions to use dta.
  - Returned filename in ds:dx in create-random-file (c2woody).
  - Fixed a bug with date and time used on open files.
  - Some mscdex fixes.
  - Added the -version switch, which makes dosbox report its version.
  - Added a keymapper.
  - Added basic IPX emulation.
  - Added cdrom iso support and floppy images support.
  - Added the possibity to boot another dos version.
  - Added Serial passthrough support (win32 only).
  - Added the possibility to pause dosbox.
  - Changed OpenGL so that it is initialized only when used.
  - Make dosbox run at higher priority when active and lower when inactive.
  - Added direct draw output support (win32 only).
  - Added current running program to title bar.
  - Rewrote video emulation to support new scalers.
  - Added new graphics scalers like advmame3x,tv2x.
  - Added a support for a few anti-debugger tricks.
  - Improved the handling of the tab-key.
  - Improved support for the numeric keyboard.
  - Fixed a few cpu opcodes.
  - Added cpu core simple (for lowerend machines)
  - Fixed some nasty bugs in the dynamic cpu core.
  - Added a few (rarely used) fpu opcodes.
  - Fixed various issues with GCC 3.4.
  - Many internal timer improvements (PIT and PIC).
  - Added some more PIC commands (c2woody).
  - Added BCD counting to the timers.
  - Fix some vesa functions.
  - Add some basic support for 132x25 and 132x45 textmodes.
  - Improved Tandy emulation a lot.
  - Lowered cpu usage when dosbox is idle.
  - Allow virtualisation of some basic IO-ports (c2woody).
  

0.61
  - Added a beta dynamic cpu for x86 hosts (very unstable)
  - Added opengl and hardware overlay display output
  - Rewrote the vga screen updates to go in lines
  - Added paging and v86 support to cpu emulation
  - Added a config option to simulate a certain type of machine
  - Added hercules graphics emulation
  - Made CGA/TANDY modes more compatible
  - Updated textmode drawing routines to support blinking colors
  - Fixed VESA set page function that was documented wrong
  - Fixed some wrongly emulated cpu opcodes.
  - improved exception handling
  - debugger: fixes; logging of gdt,lgt,idt, new commands(Fizzban)
  - fixed some mscdex issues (drive letter header error, added get directory entry)
  - added/fixed some bios funcs
  - added some rarely used xms functions (thanks c2woody!)
  - implemented GUS emulation
  - Added 16-bit DMA support (for GUS and eventually SB16)
  - Fixed many small bugs in filehandling routines
  - Many small FPU fixes (c2woody/Fizzban)
  - Some keyboard improvements (pharlab games)
  - Some Timer and cmos/rtc fixes (Mirek/Srecko/Others)
  - Lot's of mouse fixes (Help from various people)
  - Enabled internal modem
  - Made the DOS parsing routines a bit more flexible
  - Added Subst (Srecko)
  - Added cdrom ioctl support for linux (prompt)
  - Many internal DOS fixes: memory/files/datastructures.
  - Got some help from c2woody in allowing more than 1 irq being served
  - Disabled DPMI (not needed anymore. DOSBox handles almost every extender)
  - Search configfile in $HOME directory if none present in current directory
  - Added another way to switch to protected mode. (Thanks Morten Eriksen!)
  - Fixed some odd badly documented behaviour with PSP/DTA
  - Added some warnings on opening of readonly files in writemode(DOS default).
  - Many shell enhanchements
  - Fixed a win32 specific bug dealing with filenames starting with a "."
  - Fixed some bugs with the directory structure: not found/can't save errors
  
0.60
  - rewrote memory system for future paging support
  - fixed several EMS and XMS bugs and rewrite for new memory system
  - added some support for tandy video modes
  - added MAME Tandy 3 voice emulation
  - added MAME CMS/GameBlaster emulation
  - added serial port emulation with virtual tcp/ip modem (somewhat buggy)
  - sound blaster emulation is now sb pro 2.0 compatible
  - added basic support for 32-bit protected mode
  - VGA now tries to emulate an S3 Trio 64 card with 2 MB
  - VESA 2.0 support for some 256 color modes
  - rewrote large piece of video bios code for better compatibility
  - added support for the not inheritance flags.
  - created functions for creating child psp.
  - updated errorcodes of findfirst (thanks Mirek!)
  - rewrote loggingsystem to generate less warnings
  - added dos protected mode interface (dpmi)
  - added cdrom label support
  - improved cdrom audio playing
  - fixed and improved directory cache
  - debugger shows selector- and cpu mode info
  - added SELINFO (selector information) command to debugger
  - added reference counting for dos files
  - added tab-completion
  - added basic fpu support.
  - fixed several bugs with case sensitive filesystems.
  - added more shell commands and improved their behaviour.
  - mouse improvements.
  - real time clock improvements.
  - DMA fixes.
  - Improved .BAT file support.

0.58
  - fixed date and time issues with fcbs
  - added more commands to the internal Shell
  - corrected config system when a old configfile was used
  - fixed cga put and get pixel
  - fixed some vga register getting reset to wrong values
  - improved support for foreign keyboards
  - improved joystick support
  - made dosbox multithreaded again
  - lot's of soundblaster fixes
  - dma fixes
  - cdrom support
  - midi support
  - added scale2x
  - reenabled screenshot support
  - joystick support fixes
  - mouse improvements
  - support for writing wavefiles
  - added directory cache and longfilename support (longfilenames will be mangled)
  - mouse fixes
  - date and time updates at z:\
  - added (partial) direct disk support. (works probably only if directory is mounted under a:\)
  - added support for env variables. (must be set before starting dosbox: DOSBOX_SECTION_PROPERTY=value
    like DOSBOX_SBLASTER_IRQ=1)
0.57
  - added support for command /C
  - fixed all fcb-write functions
  - fixed fcb-parseline
  - added debugger under linux/freebsd 
  - added debugger memory breakpoints and autolog function (heavy debug)
  - added loadfix.com program that eats up memory (default 64kb)
    Usage  : loadfix [-option] [programname] [parameters]...
    Example: loadfix mm2      (Allocates 64kb and starts executable mm2)
             loadfix -32 mm2  (Allocates 32kb and starts executable mm2)
             loadfix -128     (Allocates 128kb)
             loadfix -f       (frees all previous allocated memory)
  - added echoing of characters for input function
  - added support for backspace for input function
  - added partial support for int10:01 set cursortype
  - fixed most of the problems/bugs with character input.
  - fixed allocationinfo call.(darksun series)
  - improved dos support for non-existant functions
  - Split screen support
  - prefix 66 67 support
  - rewrote timingscheme so 1000 hz timers don't cause problems anymore
  - update adlib emulation
  - fixed some isues with the mouse (double clicks and visible when it shouldn't be)
  - improved mouse behaviour (mickey/pixel rate) and detection routines.
  - basic ansi.sys support
  - Disney sound system emulation 
  - rewrote upcase/lowcase functions so they work fine with gcc3.2
  - SHELL: added rename and delete
  - added support for command /C. Fixed crashes in the shell
  - fixed various bugs when exiting dosbox
  - fixed a bug in XMS
  - fixed a bug with the joystick when pressing a button
  - create nicer configfiles.
  - bios_disk function improved. 
  - trapflag support
  - improved vertical retrace timing.
  - PIT Timer improvements and many bug fixes
  - Many many bug fixes to the DOS subsystem
  - Support for memory allocation strategy
  - rewrote cpu mainloop to act more like a real cpu
  
0.56
  - added support for a configclass/configfile
  - added support for writing out the configclass into a configfile
  - removed the language file and made it internal
  - added support for writing the language file (will override the internal one)
  - improved mousesupport
  - updated readme
  - support for screenshots
  - some cpu-bug fixes
  - dma changes
  - Real Sound support
  - EMM fixes and new functions.
  - VGA fixes
  - new wildcompare
  - support for size and disktype at mount.
  - added new debugger functionalities: start/trace into INTs, write processor status log, 
    step over rep and loop instructions, breakpoint support without using INT 03 (heavy debugging switch)
  - Added more cpu instructions and changed the string operations.
  - Added classes for most of the internal dos structures.
  - Rewrote most of the fcb calls to use normal dos calls.
  
0.55
  - fixed the errors/warnings in prefix_66.h and prefix_66_of.h (decimal too large becomming unsigned).
  - fixed compilation error on FreeBSD when #disable_joystick was defined
  - int10_writechar has been updated to move the cursor position.
  - changed the basedir routines to use the current working dir instead of argv[0]. This will fix and brake things :)
  - illegal command, now displays the command
  - wildcmp updated to be case insensitive
  - added fcb:open,close,findfirst, findnext.
  - fixed rename in drive_local
  - added new features to the debugger: breakpoint support / data view / command line
  - partial support of list of lists (dos info block)
  - full emm 3.2 support  
  - partial emm 4.0 support
  - fixes to graphics core fonts (text in sierra games is now correct)
  - improved support for user mousehandlers
  - fixed EGA graphics
  - fixed VGA graphics
  - fixed write with size 0
  - changed memory management.
  - fixed and cleaned up the cpu flags.
  - changed interrupt handler.
  - speeded up the graphics.
  - speeded up the cpu-core
  - changed dma 
  - improved dma streams from emm memory
  - added some cga videomodes
  - added more funtions to the keyboard handler

