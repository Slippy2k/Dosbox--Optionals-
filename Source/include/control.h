/*
 *  Copyright (C) 2002-2018  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef DOSBOX_CONTROL_H
#define DOSBOX_CONTROL_H

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4290 )
#endif

#ifndef DOSBOX_PROGRAMS_H
#include "programs.h"
#endif
#ifndef DOSBOX_SETUP_H
#include "setup.h"
#endif

#ifndef CH_LIST
#define CH_LIST
#include <list>
#endif

#ifndef CH_VECTOR
#define CH_VECTOR
#include <vector>
#endif

#ifndef CH_STRING
#define CH_STRING
#include <string>
#endif




class Config{
public:
	CommandLine * cmdline;
private:
	std::list<Section*> sectionlist;
	typedef std::list<Section*>::iterator it;
	typedef std::list<Section*>::reverse_iterator reverse_it;
	typedef std::list<Section*>::const_iterator const_it;
	typedef std::list<Section*>::const_reverse_iterator const_reverse_it;
	void (* _start_function)(void);
	bool secure_mode; //Sandbox mode
public:
	bool initialised;
	std::vector<std::string> startup_params;
	std::vector<std::string> configfiles;
	Config(CommandLine * cmd):cmdline(cmd),secure_mode(false) {
		startup_params.push_back(cmdline->GetFileName());
		cmdline->FillVector(startup_params);
		opt_exit = false;
		opt_debug = false;
		opt_nogui = false;
		opt_nomenu = false;
        opt_showrt = false;
		opt_startui = false;
		initialised = false;
		opt_console = false;
		opt_userconf = false;
		opt_noconsole = false;
		opt_eraseconf = false;
		opt_resetconf = false;
		opt_printconf = false;
		opt_noautoexec = false;
		opt_securemode = false;
		opt_fullscreen = false;
		opt_showcycles = false;
		opt_earlydebug = false;
		opt_break_start = false;
		opt_erasemapper = false;
		opt_resetmapper = false;
		opt_startmapper = false;
		opt_date_host_forced = false;
		opt_disable_numlock_check = false;
		opt_disable_dpi_awareness = false;
	}
	~Config();

	Section_line * AddSection_line(char const * const _name,void (*_initfunction)(Section*));
	Section_prop * AddSection_prop(char const * const _name,void (*_initfunction)(Section*),bool canchange=false);
	
	Section* GetSection(int index);
	Section* GetSection(std::string const&_sectionname) const;
	Section* GetSectionFromProperty(char const * const prop) const;

	void SetStartUp(void (*_function)(void));
	void Init();
	void ShutDown();
	void StartUp();
	bool PrintConfig(char const * const configfilename) const;
	bool ParseConfigFile(char const * const configfilename);
	void ParseEnv(char ** envp);
	bool SecureMode() const { return secure_mode; }
	void SwitchToSecureMode() { secure_mode = true; }//can't be undone
public:
	std::string opt_editconf,opt_opensaves,opt_opencaptures,opt_lang;
	std::vector<std::string> config_file_list;
	std::vector<std::string> opt_c;
	bool opt_disable_dpi_awareness;
	bool opt_disable_numlock_check;
	bool opt_date_host_forced;
	bool opt_break_start;
	bool opt_erasemapper;
	bool opt_resetmapper;
	bool opt_startmapper;
	bool opt_noautoexec;
	bool opt_securemode;
	bool opt_fullscreen;
	bool opt_showcycles;
	bool opt_earlydebug;
	bool opt_noconsole;
	bool opt_eraseconf;
	bool opt_resetconf;
	bool opt_printconf;
	bool opt_userconf;
	bool opt_console;
	bool opt_startui;
    bool opt_showrt;
	bool opt_nomenu;
	bool opt_debug;
	bool opt_nogui;
	bool opt_exit;
};

#endif
