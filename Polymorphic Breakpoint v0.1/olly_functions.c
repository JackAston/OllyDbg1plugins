/*
    Ollydbg Plugin  - Polymorphic Breakpoint Manager
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

  strcpy(shortname,"PolyMorphic BreakPoint");       // Name of plugin

  return PLUGIN_VERSION;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) {

	if (ollydbgversion<PLUGIN_VERSION)
		return -1;

	if (olly_create_sorted_data(&(breakpoint.data),"PolyMorphic Breakpoint List", 
		sizeof(t_polymorphicbreakpoint),10, (SORTFUNC *)polybp_sort_function,NULL)!=0)
			return -1;

	if (olly_register_plugin_class(dllclass,NULL,hInstance,polybp_winproc)<0) {
		olly_destroy_sorted_data(&(breakpoint.data));
			return -1;
	}

	olly_add_to_list(0,0,"PolyMorphic Breakpoint // Public Version // Compiled on "__DATE__);
	olly_add_to_list(0,-1," Copyright (c) 2005 - Mattwood^FRET <mattwood9@gmail.com>");

	BreakpointManager();

return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) {

	if(olly_get_status() == STAT_RUNNING)
		ispaused = 0;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {

	switch (origin) {

	case PM_MAIN:
		strcpy(data,"0 &List of PolyMorphic Breakpoint|1 &About");
		return 1;
	case PM_DISASM:
		sprintf(data, "PolyMorphic BreakPoint{"
							"0 Set PolyMorphic Int 3 Breakpoint,"
							"1 Set PolyMorphic EB FE Breakpoint,"
							"2 Set Classical EB FE BreakPoint,"
							"3 Set Stack PolyMorphic EB FE Breakpoint,"
							"|"
							"4 PolyMorphic Breakpoint Manager"
						"}");
		return 1;
	default: 
		break;
	}

return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
	t_dump *cpuasm;
//
	if (origin==PM_MAIN) {
		switch (action) {
			case 0:
				BreakpointManager();
			break;
			case 1:
				MessageBox(hWMain,
							"PolyMorphic Breakpoint v0.1 // PUBLiC RELEASE\n"
							"Idea and plugin by Mattwood^FRET\n"
							"contact: mattwood9@gmail.com\n"
							"email: reverseengineering.online.fr\n"
							"Greets: Kaine, Kharneth, Gbillou, Snio, RaX|,\n"
							"Neista, eedy31, Beatrix2oo4, blaCk_eye, Bigbang,\n"
							"edcba, SeVeN, elooo, Lucky, Silvere, jB\n"
							"Pulsar, Genaytyk, CoaxCable, feddex, blueownl, dref\n"
							"kralor, tony, Christal, +theAnalyst,\n"
							"all pasta's eater, FRET, nGen, FFF, \n"
							"and all guys who i forget enfin voila\n\n"
							"And you Oleh :) We love you :p",
							"PolyMorphic BreakPoint - About",
						MB_OK|MB_ICONINFORMATION);
			break;
			default: 
			break;
		}
	}
  // = = 
	else if (origin==PM_DISASM) {
		switch (action) {
			case 0:
			// Set PolyMorphic Int 3 Breakpoint
			case 1:
			// Set PolyMorphic EB FE Breakpoint
			case 2:
			// Set Classical EB FE BreakPoint
			case 3:
			// Set Stack PolyMorphic EB FE Breakpoint
				cpuasm = (t_dump *)olly_plugin_get_value(VAL_CPUDASM);
				AddBreakpoint(cpuasm->sel0,action);
			break;
			case 4:
			// Run the manager
				BreakpointManager();
			break;
		}
	}

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Pausedex(int reason, int extdata, t_reg *reg, DEBUG_EVENT *debugevent) {

		ispaused = 1;

	return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Pluginreset(void) {
	olly_delete_sorted_data_range(&(breakpoint.data),0,0xFFFFFFFF);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc int _export cdecl ODBG_Pluginclose(void) {
	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

extc void _export cdecl ODBG_Plugindestroy(void) {
	olly_unregister_plugin_class(dllclass);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

