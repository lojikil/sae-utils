/* simple terminal title command, using VT commands. Not portable, but
 * works on multiple terminal emulators.
 * Copy 2011 Stefan Edwards, under the zlib/png license.
 * See LICENSE for details
 */
#include <stdio.h>
int
main(int ac, char **al)
{
	if(ac != 2)
	{
		printf("usage: tt <terminal title>\n");
		return 1;
	}
	printf("\033]0;%s\007\n",al[1]);
	return 0;
}
