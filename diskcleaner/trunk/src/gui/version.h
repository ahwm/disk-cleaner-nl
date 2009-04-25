#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.04;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 728;
	static const long REVISION = 3966;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1269;
	#define RC_FILEVERSION 1,6,728,3966
	#define RC_FILEVERSION_STRING "1, 6, 728, 3966\0"
	static const char FULLVERSION_STRING[] = "1.6.728.3966";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_h
