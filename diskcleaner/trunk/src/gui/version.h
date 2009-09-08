#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.09;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 1273;
	static const long REVISION = 6980;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2371;
	#define RC_FILEVERSION 1,6,1273,6980
	#define RC_FILEVERSION_STRING "1, 6, 1273, 6980\0"
	static const char FULLVERSION_STRING[] = "1.6.1273.6980";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_h
