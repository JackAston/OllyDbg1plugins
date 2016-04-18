#ifndef __MAPIMP_H__
#define __MAPIMP_H__

typedef struct
{
	LPVOID next;
	PCHAR buffer;
	PCHAR repl;
	int type;
	regex_t regex;
} mask_t;

typedef struct
{
	LPVOID next;
	size_t size;
	PCHAR buffer;
	ULONG offset;
	BYTE segment;
} name_t;

typedef struct
{
	ULONG count;
	LPVOID first;
	LPVOID last;
} list_t;

typedef struct
{
	BOOL comments;
	BOOL labels;
	BOOL collisionchecks;
	ULONG aimport;
	BOOL applytodebuggee;
	BOOL demangle;
	BOOL usemasks;
	list_t* masks;
} config_t;

/* mask_compile output definitions */
typedef enum
{
	FILTER_NOERROR = 0,
	FILTER_INVALID_MASK = REG_NOMATCH + 1,
	FILTER_INVALID_KEY,
	FILTER_INVALID_REPLACEMENT
} __filter_error;

/* .UDD tag */
#define TAG_MAPIMP			  0x504D694D

#define ERRORBUFLEN           TEXTLEN
#define MAPBUFLEN             1024

/* Plugin action identifiers */
#define ACTION_IMPORT         0
#define ACTION_OPTIONS        1
#define ACTION_ABOUT          3

/* mask_filter output definitions */
#define FILTER_NOTHING        0
#define FILTER_SKIP           1
#define FILTER_CUT            2
#define FILTER_REPLACE        4

/* Autoimport option definitions */
#define AUTOIMPORT_DISABLED   0
#define AUTOIMPORT_ASK        1
#define AUTOIMPORT_ALWAYS     2

/* Configuration file parameters */
#define CONFIG_STR_COMMENTS   "comments"
#define CONFIG_STR_LABELS     "labels"
#define CONFIG_STR_COLLISIONS "collisionchecks"
#define CONFIG_STR_APPLYTO    "applytodebuggee"
#define CONFIG_STR_AUTOIMPORT "autoimport"
#define CONFIG_STR_DEMANGLE   "demangle"
#define CONFIG_STR_USEMASKS   "usemasks"
#define CONFIG_STR_MASKS      "customfilter"

/* Option window definitions */
#define OPTWND_CLASS_NAME     "mapimp_wnd"
#define OPTWND_WINDOW_NAME    "Options"
#define OPTWND_FONT_NAME      "Verdana"
#define OPTWND_ICON_NAME      "ICO_OPTIONS"
#define OPTWND_WINDOW_WIDTH   354
#define OPTWND_WINDOW_HEIGHT  312

#define INPUTWND_TYPE         196

#define ACCEL_COUNT           23

#define ID_CANCEL             2000
#define ID_APPLY              2001
#define ID_ADD                2002
#define ID_DELETE             2003
#define ID_USEMASKS           2004
#define ID_COLLISIONS         2005
#define ID_IMPORT             2006
#define ID_MASKS              2007
#define ID_FILTER             2008
#define ID_OVERWRITE          2009
#define ID_SKIP               2010
#define ID_LABELS             2011
#define ID_COMMENTS           2012
#define ID_EDIT               2013
#define ID_AUTOIMPORT         2014
#define ID_ASKTOIMPORT        2015
#define ID_IMPORTALWAYS       2016
#define ID_DONOTHING          2017
#define ID_APPLYTO            2018
#define ID_MODULE             2019
#define ID_DEBUGGEE           2020
#define ID_DEMANGLE           2021
#define ID_SAVE               2022
#define ID_LOAD               2023
#define ID_INCREASE           2024
#define ID_DECREASE           2025
#define ID_MAXINCREASE        2026
#define ID_MAXDECREASE        2027
#define ID_INSERT             2028

#endif