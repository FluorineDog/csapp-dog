#include <iostream>
int main(){
    char str[] = "59b997fa";
    for(auto ch:str){
        std::cout << std::hex << int(ch) << ' ';
    }
}
