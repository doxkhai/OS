#include "readline.h"
#include "factorial.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//Implement factorial

	char str[50];
	scanf("%50[^\n]",str);

	if(read_line(str)==0) printf("-1");
	else printf("%d", factorial(atoi(str)));
	return 0;
}
