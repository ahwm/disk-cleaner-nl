#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.05;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 7;
	static const long BUILD = 1440;
	static const long REVISION = 7868;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2668;
	#define RC_FILEVERSION 1,7,1440,7868
	#define RC_FILEVERSION_STRING "1, 7, 1440, 7868\0"
	static const char FULLVERSION_STRING[] = "1.7.1440.7868";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_h
