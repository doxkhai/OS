#include "readline.h"

int read_line(char* str) {
	while(str != 0 && *str != '\0' ){
		if(*str > '9' || *str < '0') 
			return 0;
		str++;	
	}
	return 1;
}
