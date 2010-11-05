#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.08";
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 9;
	static const long BUILD = 1845;
	static const long REVISION = 10012;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3313;
	#define RC_FILEVERSION 1,9,1845,10012
	#define RC_FILEVERSION_STRING "1, 9, 1845, 10012\0"
	static const char FULLVERSION_STRING[] = "1.9.1845.10012";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
