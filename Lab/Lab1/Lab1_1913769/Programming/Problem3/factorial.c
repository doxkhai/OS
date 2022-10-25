#include "factorial.h"

long long int factorial(const long int aNumber) {
	if(aNumber == 1) return 1;
	return aNumber * factorial(aNumber-1);
}
