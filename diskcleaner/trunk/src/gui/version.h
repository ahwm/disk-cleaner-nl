#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.08";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 8;
	static const long BUILD = 1753;
	static const long REVISION = 9498;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3164;
	#define RC_FILEVERSION 1,8,1753,9498
	#define RC_FILEVERSION_STRING "1, 8, 1753, 9498\0"
	static const char FULLVERSION_STRING[] = "1.8.1753.9498";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
