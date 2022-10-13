#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int glo_init_data = 99; //In data segment
int glo_noninit_data; //In BSS segment

void print_func() { //In program code
    int local_data = 9; //In stack
    printf("Process ID = %d\n", getpid());
    printf("Addresses of the process: \n");
    printf("1. glo_init_data = %p\n", &glo_init_data);
    printf("2. glo_noninit_data = %p\n", &glo_noninit_data);
    printf("3. print_func() = %p\n", &print_func);
    printf("4. local_data = %p\n", &local_data);
}

int main(int argc, char** argv) {
    print_func();
    return 0;
}
