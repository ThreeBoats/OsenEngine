#include "Logging/Logger.h"
#include "Logging/internal/coreLogger.h"


//first define OSEN_BREAK, because the compiler does
//not like #ifdef's in #define
///do not use this standalone
#ifdef OSEN_RELEASE
    #define OSEN_ASSERT(condition, message) do {} while(false)
#else
    //I am not sure if WIN32 is the correct thing to check.
    //Might also be compiler related
    #ifdef _WIN32
        #define OSEN_BREAK() __debugbreak()
    #else
        #define OSEN_BREAK() __builtin_trap()
#endif


#define OSEN_ASSERT(condition, message)                                                     \
        do {                                                                                \
            if (!condition)                                                                 \
            {                                                                               \
                LOG(osen::coreLogger, osen::Logger::LogSeverity::FATAL, message);           \
                OSEN_BREAK();                                                               \
            }                                                                               \
        } while(false)
#endif
