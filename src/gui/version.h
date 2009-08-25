#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.08;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 1160;
	static const long REVISION = 6351;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2143;
	#define RC_FILEVERSION 1,6,1160,6351
	#define RC_FILEVERSION_STRING "1, 6, 1160, 6351\0"
	static const char FULLVERSION_STRING[] = "1.6.1160.6351";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_h
