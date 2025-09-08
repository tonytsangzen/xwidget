#include <stdint.h>
#include <sys/time.h>

#ifdef _MSC_VER
#include <windows.h>
int32_t kernel_tic(uint32_t* sec, uint64_t* usec){
	FILETIME ft;
    LARGE_INTEGER li;

    GetSystemTimeAsFileTime(&ft);

    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    *us = (li.QuadPart - 116444736000000000ULL)/ 10;
	*sec = *us / 10000000;

    return us;
}
#else
int32_t kernel_tic(uint32_t* sec, uint64_t* usec){
   struct timeval tv;
    gettimeofday(&tv, NULL); // Get current time

	if(sec)
		*sec = (uint32_t)tv.tv_sec;
	if(usec)
		*usec = tv.tv_usec + tv.tv_sec * 1000000;
    return 0;
}
#endif

uint64_t kernel_tic_ms(int zone){
	uint32_t sec = 0;
	uint64_t usec = 0;
	kernel_tic(&sec, &usec);
	return usec / 1000;
}
