#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.04;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 1337;
	static const long REVISION = 7342;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2494;
	#define RC_FILEVERSION 1,6,1337,7342
	#define RC_FILEVERSION_STRING "1, 6, 1337, 7342\0"
	static const char FULLVERSION_STRING[] = "1.6.1337.7342";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_h
