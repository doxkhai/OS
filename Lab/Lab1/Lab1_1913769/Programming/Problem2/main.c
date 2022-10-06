#include "readline.h"
#include <stdio.h>

int main(){
	char str[50];
	scanf("%50[^\n]",str);
	printf("Return %d.\n",read_line(str));
	return 0;
}
