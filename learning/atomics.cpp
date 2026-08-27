#include <cstdlib>
#include <iostream>
#include <thread>


void foobar(std::string x){
	printf("%s", x.c_str());
}

void incMany(int& x){
	for (int i = 0; i < 100000; i++){
		x++;
	}
}

void incManyAtomic(std::atomic<int>& x){
	for (int i = 0; i < 100000; i++){
		x++;
	}
}

int main(){
	//int a = 0;
	std::atomic<int> a(0);

	std::thread threadA(incManyAtomic, std::ref(a));
	std::thread threadB(incManyAtomic, std::ref(a));
	
	//std::cout << a;
	std::cout << a.load();
	std::cout << "\n";

	threadA.join();
	threadB.join();

	//std::cout << a;
	std::cout << a.load();
	
	return 0;
}
