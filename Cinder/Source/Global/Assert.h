#ifdef CFG_DEBUG
#	define ASSERT(condition, message) Assert(condition, message, __FILE__, __LINE__)
#else
#	define ASSERT(condition, message)
#endif

void Assert(bool condition, char* message, char* file, int line);
