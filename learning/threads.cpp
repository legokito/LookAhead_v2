#include <cstdlib>
#include <iostream>
#include <thread>


void foobar(std::string x){
	printf("%s", x.c_str());
}

int main(){
	std::thread threadA(foobar, "hallo");
	
//	threadA.join();
	
	return 0;
}
