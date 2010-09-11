#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.09";
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 8;
	static const long BUILD = 1792;
	static const long REVISION = 9728;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3216;
	#define RC_FILEVERSION 1,8,1792,9728
	#define RC_FILEVERSION_STRING "1, 8, 1792, 9728\0"
	static const char FULLVERSION_STRING[] = "1.8.1792.9728";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
