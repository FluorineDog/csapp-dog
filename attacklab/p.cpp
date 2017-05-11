#include <iostream>
#include <stdio.h>
int main(){
    unsigned int x = 0x59b997fa;
    char str[100];
    sprintf(str, "%.8x", x);
    for(auto ch:str){
        std::cout << std::hex << int(ch) << ' ';
        if(!ch)
            break;
    }
}
