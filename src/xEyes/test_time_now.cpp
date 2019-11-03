
#include <stdio.h>
#include "libUtil/util.h"

int test_time_now(int argc, char *argv[])
{
	while (1) {
		std::cout << xeyes::getPrettyTimeStamp() << std::endl;
		THREAD_SLEEP(100);
	}
}