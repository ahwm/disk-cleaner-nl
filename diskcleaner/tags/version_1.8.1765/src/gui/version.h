#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.08";
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 8;
	static const long BUILD = 1765;
	static const long REVISION = 9581;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3187;
	#define RC_FILEVERSION 1,8,1765,9581
	#define RC_FILEVERSION_STRING "1, 8, 1765, 9581\0"
	static const char FULLVERSION_STRING[] = "1.8.1765.9581";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_H