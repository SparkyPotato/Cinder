#define OUT_RED "\x1b[31m"
#define OUT_YELLOW "\x1b[93m"
#define OUT_COLOR "\x1b[38;2;255;140;0m"
#define OUT_RESET "\x1b[0m"

#ifdef PLATFORM_MAC

inline void Output(const char* format)
{
	printf("%s\n", format);
}
template<typename... Args>
void Output(const char* format, Args&&... args)
{
	printf(format, std::forward<Args...>(args...));
	printf("\n");
}

inline void Error(const char* format)
{
	printf(OUT_RED "Error: ");
	Output(format);
	printf(OUT_RESET);
	
	throw -1;
}
template<typename... Args>
void Error(const char* format, Args&&... args)
{
	printf(OUT_RED "Error: ");
	Output(format, std::forward<Args...>(args...));
	printf(OUT_RESET);
	
	throw -1;
}

inline void Warning(const char* format)
{
	printf(OUT_YELLOW "Warning: ");
	Output(format);
	printf(OUT_RESET);
}
template<typename... Args>
void Warning(const char* format, Args&&... args)
{
	printf(OUT_YELLOW "Warning: ");
	Output(format, std::forward<Args...>(args...));
	printf(OUT_RESET);
}

#endif
