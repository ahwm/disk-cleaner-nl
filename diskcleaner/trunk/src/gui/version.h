#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "16";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.08;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 828;
	static const long REVISION = 4489;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1428;
	#define RC_FILEVERSION 1,6,828,4489
	#define RC_FILEVERSION_STRING "1, 6, 828, 4489\0"
	static const char FULLVERSION_STRING[] = "1.6.828.4489";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_h
