/*
    Ollydbg Plugin  - Anti Anti Hardware Breakpoint v0.1
    Copyright (C) 2005 Mattwood^FRET <mattwood9@gmail.com>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place, Suite 330, Boston, MA 02111-1307 USA
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {

  strcpy(shortname,"Anti Anti BPM");       // Name of plugin

  return PLUGIN_VERSION;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) {

	if (ollydbgversion<PLUGIN_VERSION)
		return -1;

	olly_add_to_list(0,0,"Anti Anti Hardware Breakpoint // Public Version // Compiled on "__DATE__);
	olly_add_to_list(0,-1," Copyright (c) 2005 - Mattwood^FRET <mattwood9@gmail.com>");

return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) {



}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {

	switch (origin) {

	case PM_MAIN:
		strcpy(data,"0 &Force to hook|1 &Restore ntdll|2 &About");
		return 1;
	default: 
		break;
	}

return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
//
	if (origin==PM_MAIN) {
		switch (action) {
			case 0:
			if(failed) {
				switch(SetDownHook()) {
				case 0:
					olly_add_to_list(0,1," Wtf i can write in ntll !");
					break;
				}

				switch(SetUpHook()) {
					case -3:
						olly_add_to_list(0,1," I can't load the magic dll !");
						failed = 1;
						break;			
					case -2:
						olly_add_to_list(0,1," I can't read in ntdll =(");
						failed = 1;
						break;
					case -1:
						olly_add_to_list(0,1," Hey I can't find the dll ! Try to do F7/F8/F9 and Plugins> SEH Hooker> Force to hook");
						failed = 1;
						break;
					case 0:
						olly_add_to_list(0,1," I can't write in ntdll :(");
						failed = 1;
						break;
					case 1:
						olly_add_to_list(0,-1, " -> Installed");
						failed = 0;
						break;
				}
			}
			break;
			case 1:
			if(!failed) {
				switch(SetDownHook()) {
				case 0:
					olly_add_to_list(0,1," Wtf i can write in ntll !");
				}
			}
			break;
			case 2:
				MessageBox(hWMain,
							"Anti Anti Hardware Breakpoint v0.1 // PUBLiC RELEASE\n"
							"Mattwood^FRET\n"
							"contact: mattwood9@gmail.com\n"
							"WebSite : www.pastapolis.info\n\n"

							"Thanks: Robert Freeman, Pedram Amini\n\n"

							"Greets: Kaine, Kharneth, Gbillou, Snio, RaX|,\n"
							"Neista, eedy31, Beatrix2oo4, Bigbang, jB, wtbw\n"
							"edcba, SeVeN, elooo, Lucky, Silvere, blaCk_eye, bluffer\n"
							"Pulsar, Genaytyk, CoaxCable, feddex, blueownl, dref\n"
							"kralor, tony, Christal, +theAnalyst, [n00b], u_32, Kephren\n"
							"FRET, nGen, FFF, CPH, TITAN, RET\n"
							"all pasta's eater\n"
							"and all funny guys who i forget enfin voila\n\n"
							"And you Oleh :) We love you :p",
							"SEH Hooker - About",
						MB_OK|MB_ICONINFORMATION);
			break;
			default: 
			break;
		}
	}

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Pluginreset(void) {
	memset(buffer,0x0,12);
	ishooked=0;
	failed=1;
	injected=0;
	return;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Pluginclose(void) {
	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Plugindestroy(void) {
//	olly_unregister_plugin_class(dllclass);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export ODBG_Pausedex(int reason, int extdata, t_reg *reg, DEBUG_EVENT *debugevent) {

		if(!ishooked) {
			olly_add_to_list(0,-1," Installing the hook...");
			switch(SetUpHook()) {
				case -3:
					olly_add_to_list(0,1," I can't load the magic dll !");
					failed = 1;
					break;		
				case -2:
					olly_add_to_list(0,1," I can't read in ntdll =(");
					failed = 1;
					break;
				case -1:
					olly_add_to_list(0,1," Hey I can't find the dll ! Try to do F7/F8/F9 and Plugins> SEH Hooker> Force to hook");
					failed = 1;
					break;
				case 0:
					olly_add_to_list(0,1," I can't write in ntdll :/");
					failed = 1;
					break;
				case 1:
					olly_add_to_list(0,-1, " -> Installed");
					failed = 0;
					break;
			}
			ishooked=1;
		}

	return 1;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 