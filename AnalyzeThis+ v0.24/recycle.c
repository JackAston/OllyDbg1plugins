
        /*
        //find all modules currently present, list the first item as the main module
        t_table *modules=(t_table*)Plugingetvalue(VAL_MODULES);
        //record module names and their structs
        sprintf(tmpstr,"%08x",modules[0].data.data);
        MessageBox(hwmain,NULL,tmpstr,NULL);
        */

        //test-analyze
        t_module *currModule=Findmodule(0x401000);
        ulong currBase,currSize;
        Getdisassemblerrange(&currBase,&currSize);
        currModule->codebase=currBase;
        currModule->codesize=currSize;

        //add custom section
        //sections do not include PE header
        //currModule->nsect=9;
        IMAGE_SECTION_HEADER *newSects=(IMAGE_SECTION_HEADER*)malloc(sizeof(IMAGE_SECTION_HEADER)*currModule->nsect);
        memcpy(newSects,currModule->sect,sizeof(IMAGE_SECTION_HEADER)*currModule->nsect);
        IMAGE_SECTION_HEADER addedSect;
        addedSect.Name[0]=0;
        addedSect.Misc.PhysicalAddress=0;
        addedSect.Misc.VirtualSize=0x6000;
        addedSect.VirtualAddress=0x160000;
        addedSect.SizeOfRawData=0;
        addedSect.PointerToRawData=NULL;
        addedSect.PointerToRelocations=NULL;
        addedSect.PointerToLinenumbers=NULL;
        addedSect.NumberOfRelocations=0;
        addedSect.NumberOfLinenumbers=0;
        addedSect.Characteristics=IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE|IMAGE_SCN_MEM_EXECUTE;
        //newSects[7]=addedSect;

        currModule->sect=newSects;
        currModule->size+=0x1000;
        currModule->sect[7].Misc.VirtualSize+=0x1000;
        Addtolist(0,0,"%x",currModule->sect[0].VirtualAddress);

        //Analysecode(currModule);
