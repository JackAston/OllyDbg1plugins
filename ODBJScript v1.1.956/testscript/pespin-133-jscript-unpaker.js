/* Unpacking script for PESpin 1.33b
This Script fix all but:
- nanomites (use spinano to fix them)
- iat alignement (use PESpin 1.33c version or UIF to fix it).

###########################################################
UPDATED FROM PREVIOUS VERSION (1.32) BUT DOESN'T SUPPORT IT
###########################################################

Tested on WINXP SP2 + ODBJScript 1.0.899 build 2005 alpha + OLLYDBG1.10
Script done by Zool@nder of AT4RE
Please for all remarks and suggestions, contact me at anderzool@hotmail.fr or at WWW.AT4RE.COM
23/01/2012
Regards */

// this is a mandatory function
function jsmain() {
	var val0
	var oep
	var ip
	var cb_packer;
	
	cb_packer = GMEMI(eip.v, MEMORYBASE);
	modulebase = GMI(eip.v, MODULEBASE);
	
	var response = MSGYN("Is DebugBlocker enabled");
	if (response == IDCANCEL)
		return;
	else if (response == IDNO) {
		val0 = cb_packer + 0x296C;
		bphws(val0, "x")
		esto();
		bphwc(val0);
		val0 = dd(cb_packer + 0x29E7); // dword [val0]
		log(val0 + "");
		bp(val0)
		sto();
		bc(val0);
	} else {
		setoption(IGNORE_ALL);
		var ddCreateMutex = gpa("CreateMutexA", "Kernel32.dll");
		if (!ddCreateMutex) {
			log("Can'te get CreateMutexA address");
			return;
		}
		
		bphws(ddCreateMutex, "x");
		
		// a dumb but working method as a workarround to win7 kernelbase/kernel32 stuff
		esto();
		bphwc();
		for (i = 0; i < 7; i++)
			sto();
		// dumbness ends here
		
		if (!find(eip.v, "C20C00")) {
			log("Can'te get CreateMutexA return address");
			return;
		}
		bp($RESULT)
		esto();
		bc($RESULT)
		sti();
		
		if (!find(eip.v, "3BC39C")) {
			log("Can'te get usefull pattern");
			return;
		}
		
		// You can always use the global variable $RESULT as in ODBGScript language :)
		$RESULT += 2;
		
		bp($RESULT);
		esto();
		ZF.v = true;
		bc($RESULT);
		
		bp($RESULT + 0x47);
		esto();
		bc($RESULT + 0x47);
		
		eip.v = $RESULT + 0x47 + 0x1F;
		
		var pattern = "F187DF57C3558BECC745FC00000000C745F800000000";
		if (!find(eip.v, pattern)) {
			log("can't find pattern : " + pattern);
			return;
		}
		bp($RESULT);
		esto();
		bc($RESULT);
		
		eip.v = dd(esp.v);
		esp.v += 4;
	}
	
	// NOP ; NOP ; NOP ; NOP ; NOP ; NOP ; NOP
	// XOR     EDI, EDI
	// OR      EDI, EDI
	mov(cb_packer + 0x19A1, "9090909090909033FF0BFF");
	
	// MOV     WORD PTR DS:[EDI-1], 25FF
	// MOV     DWORD PTR DS:[EDI+1], EDX
	// MOV     DWORD PTR DS:[EDX], EAX
	// NOP ; NOP ; NOP ; NOP ; NOP ; NOP ; NOP ; NOP ; NOP
	mov(cb_packer + 0x114B, "66C747FFFF258957018902909090909090909090");
	
	oep = cb_packer + 0x1CF9;
	repl(oep, "EB01??", "909090", 0x100);
	repl(oep, "68????????E9", "9090909090E8", 0x100);
	
	val0 = oep;
	var tmp = 0;
	
	// here you can also use findo() instead of looping with find. See example further
	while (find(val0, "68????????81??24")) {
		tmp = dd($RESULT + 1);
		
		b = db($RESULT + 6);
		if (b == 4 || b == 0x2c) {
			if (b == 4)
				tmp += dd($RESULT + 8);
			else
				tmp -= dd($RESULT + 8);
			
			tmp &= 0xFFFFFFFF; // problem with javascript arithmetics, see readme.pdf for more help
			mov($RESULT + 1, tmp);
			mov($RESULT + 5, "90909090909090");
		}
		val0 += 0xC;
	}
	
	bp(oep);
	erun();
	bc(oep);
	
	if (find(oep, "909090")) {
		preop($RESULT);
		eip.v = $RESULT;
		log("FAKE OEP AT : ", $RESULT);
		
		ip = eip.v;
		wrt("stoleb.bytes", "");
		while (opcode(ip) && db(ip) != 0xE9) {
			if (db(ip) != NOP)
				wrta("stoleb.bytes", $RESULT_1);
			ip += $RESULT_2;
		}
		
		jmpdest = gci(ip, DESTINATION);
		if (!jmpdest) {
			log("can't get jump destination from : ", ip);
			return;
		}
		while (preop(jmpdest)) {
			if (dw($RESULT) != 0)
				break;
			jmpdest = $RESULT;
		}
		
		// asmtxt(jmpdest, "stoleb.bytes");
		eip.v = jmpdest;
		log("real OEP AT : ", jmpdest);
		oep = jmpdest;
	}
	
	if (!gmi(oep, CODEBASE)) {
		log("The script can't continue fixing the target ... Please do it by hand or send me an(Email");
		return;
	}
	codebase = $RESULT;
	
	tmp1 = [[],[]];
	
	counter = findo(codebase, "E9??????FF", 0, jmp_callback, modulebase, codebase - 1);
	log(counter, " jumps to header cleaned");
	
	counter = findo(codebase, "E8??????FF", 0, call_callback, modulebase, codebase-1);
	asmb(tmp1[0], tmp1[1]); // batch assemble is very fast compaired to asm in loops
	log(counter, " calls to header cleaned");
	
	response = msgyn("Do you wan't to let script fix iat mis-alignement?");
	if(response == IDYES) {
		tmp1 = [[],[]];
		tmp2 = [[],[]];
		tmp0 = alloc(0x2000); // tmp0 to tmp4 and TMP0 to TMP4 are global variables that can be used when needed (this is a temporary fix)
		if(tmp0) {
			counter = findo(codebase, "FF?5??????00", 0, imp_callback, modulebase, modulebase+gmi(codebase, MODULESIZE)-1);
			log(counter, " imp jumps/call fixed");
			movb(tmp1[0], tmp1[1]);
			movb(tmp2[0], tmp2[1]);
		}
	}
	
	log("Open 'stolen.bytes' file and choose the valid commands, then assemble them.");
	log("If target crashes, try to fix it with UIF as this script doesn't scan the iat variant: mov R32, addr -> call R32");
	return
}

/*
 * callback that checks if it's a jump to header
 * I kept assmebling is loop to let you see the difference between calling asm in loops and asmb
 */
function jmp_callback(addr, min, max) {
	var dest = gci(addr, DESTINATION);
	if (dest < max && dest > min) {
		opcode(dest);
		asm(addr, $RESULT_1);
		return true;
	}
	return false;
}

/*
 * callback that checks if it's a call to header
 * I kept assmebling is loop to let you see the difference between calling asm in loops and asmb
 */
function call_callback(addr, min, max) {
	var dest = gci(addr, DESTINATION);
	if (dest < max && dest > min) {
		dest = gci(dest, DESTINATION);
		if (dest) {
			// You would use 'asm(addr, "call " + dest.toString(16));'
			tmp1[0].push(addr);
			tmp1[1].push("call " + dest.toString(16));
			return true;
		}
	}
	return false;
}

/*
 * callback that checks if it's a call/jump to iat
 */
function imp_callback(addr, min, max) {
	var dest;
	var b = db(addr+1);
	if(b == 0x15 || b == 0x25) {
		var ddapi = dd(addr+2);
		if (ddapi && ddapi < max && ddapi > min) {
			dest = dd(ddapi);
			if (dest > max || dest< min) {
				tmp1[0].push(addr+2);
				tmp1[1].push(tmp0);
				tmp2[0].push(tmp0);
				tmp2[1].push(dest);
				tmp0+=8;
				return true;
			}
		}
	}
	return false;
}