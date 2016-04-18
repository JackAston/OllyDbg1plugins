
//Change Here Plugin Version

#define VERSIONHI      1               // High plugin version
#define VERSIONLO      82              // Low plugin version
#define VERSIONST      6               // plugin state (1svn 2beta 3official VC6 release 4hacked, 6:VC9 WDK, 9:VC9)


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define VERSIONSTR  TOSTRING(VERSIONHI) "." TOSTRING(VERSIONLO) "." TOSTRING(VERSIONST)


#if _MSC_VER >= 1600
#  define VERSIONCOMPILED "i686 VC2010 WDK7.1 CRT/STL60"
#elif _MSC_VER >= 1500
#  define VERSIONCOMPILED "i686 VC2008 WDK7.1 CRT/STL60"
#elif _MSC_VER >= 1400
#  define VERSIONCOMPILED "i686 VC2005 WDK7.1 CRT/STL60"
#elif _MSC_VER >= 1310
#  define VERSIONCOMPILED "i686 VC2003 WDK7.1 CRT/STL60"
#elif _MSC_VER > 1300
#  define VERSIONCOMPILED "i686 VC2002 WDK7.1 CRT/STL60"
#else
#  define VERSIONCOMPILED "i686 VC6"
#endif
