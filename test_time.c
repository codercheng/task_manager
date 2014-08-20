#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
	time_t timer;
	int n = 10;
	while(n--) {
		sleep(1);
		timer = time(NULL);
		printf("%ld\n", timer);
	}
	return 0;
}
