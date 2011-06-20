#include <stdio.h>
int
main(int ac, char **al)
{
	int val = 1, lvl = 0, incr = 1;
	if(ac < 2 || ac > 4)
	{
		printf("usage: seq [first [incr]] last\n");
		return 1;
	}
	lvl = atoi(al[ac - 1]);
	if(ac >= 3)
		val = atoi(al[1]);
	if(ac == 4)
		incr = atoi(al[2]);
	if(val <= lvl)
		for(;val <= lvl;val += incr)
			printf("%2.2d\n",val);
	else
		for(;val >= lvl;val += incr)
			printf("%2.2d\n",val);
	return 0;
}
