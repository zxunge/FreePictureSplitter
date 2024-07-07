#ifndef FPS_VERSION_H
#define FPS_VERSION_H

namespace FPSVersion{

	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2024";
	static const char UBUNTU_VERSION_STYLE[] =  "24.07";

	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";

	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 3;
	static const long REVISION  = 3;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 41;
	#define RC_FILEVERSION 1,0,3,3
	#define RC_FILEVERSION_STRING "1, 0, 3, 3\0"
	static const char FULLVERSION_STRING [] = "1.0.3.3";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;

}
#endif //FPS_VERSION_H
