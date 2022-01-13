#ifndef _VERSION_H_
#define _VERSION_H_

// https://semver.org/
#define VERSION_MAJOR   0
#define VERSION_MINOR   1
#define VERSION_PATCH   0

// If applicable a DEBUG version can be used
#ifdef DEBUG_BUILD
    #define VERSION_DEBUG   1
#else
    #define VERSION_DEBUG   0
#endif

#endif // _VERSION_H_