#include <stdio.h>

void func(unsigned long number){
    unsigned long local = number;

    printf("%2lu -> %p\n", local, &local);
    if(--local > 0){
        func(local);
    }
}

int main() {
    func(10);
}