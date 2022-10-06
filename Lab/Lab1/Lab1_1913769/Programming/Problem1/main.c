
#include <stdio.h>
#include "factorial.h"

int main(int argc, char** argv){
	int n;
	scanf("%d",&n);
	printf("factorial of %d is: %d",n,factorial(n));
	return 0;
}
