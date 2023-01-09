#include <iostream>
#include "Case.h"

using namespace std;

int main() {
	Repository repo("Warehouse", 100);
	cout << Case::count() << endl; //0
	try {
		repo.add(new BrickCase("books", 0x000000, 5, 3.5, 4)); //5 x 3.5 x 4
		repo.add(new TubeCase("posters", 0x00ff00, 1, 3)); //radius=1, height=3
		repo.add(new PrismCase("toys", 0x800080, 3, 2)); //base=3, height=2
		repo.add(new BrickCase("jewels", 0xffff00, 2, 2, 2)); //2 x 2 x 2
		repo.add(new BrickCase("trinkets", 0x000080, 5, 1, 1)); //5 x 1 x 1
	}
	catch (Repository::CapacityError& e) {
		cout << e.what() << endl; //trinkets too large - only 4.8 free space
	}
	cout << repo;
	//Warehouse, total capacity: 100.0, free space: 4.8:
	//1. books, 0x000000, volume: 70.0
	//2. posters, 0x00ff00, volume: 9.4
	//3. toys, 0x800080, volume: 7.8
	//4. jewels, 0xffff00, volume: 8.0
	cout << Case::count() << endl; //4
	cout << repo.summaryVolume() << endl; //95.2
	repo.remove("toys");
	cout << Case::count() << endl; //3
	cout << repo.summaryVolume() << endl; //87.4
	try {
		cout << repo[1]->getCapacity() << endl; //70.0
		cout << repo[5]->getCapacity() << endl; //IndexError exception
	}
	catch (Repository::IndexError& e) {
		cout << e.what() << endl; //item no. 5 not found
	}
	repo.clear();
	cout << Case::count() << endl; //0
	
	return 0;
}