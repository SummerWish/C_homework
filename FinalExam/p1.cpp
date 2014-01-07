#include <iostream>
#include <fstream>

#define INPUT_FILE		"input1.txt"
#define SIZE			10

int binary_search(int rec[], int length, int find)
{
    
    int l = 0, r = length - 1, mid;
    
    while (l <= r) {
        
        mid = (l + r) >> 1;
        
        if (find == rec[mid]) {
            return mid;
        } else if (find < rec[mid]) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
        
    }
    
    return -1;
    
}

int main()
{
	
	// 1. Read data from file
	std::ifstream fin;
    fin.open(INPUT_FILE);
        
    if (!fin.good()) {
        std::cout << "Error: Cannot open file: " << INPUT_FILE << std::endl;
        return 1;
    }
    
    int n[SIZE];
    for (int i = 0; i < SIZE; ++i) {
    	fin >> n[i];
    }
    
    fin.close();
    
	// 2. Read target number from the terminal
	int to_search;
	std::cout << "Please input the number to search: ";
	std::cin >> to_search;
    
    // 3. Search!
    int result = binary_search(n, SIZE, to_search);
    if (result == -1) {
    	std::cout << "NO" << std::endl;
    } else {
    	std::cout << result + 1 << std::endl;
    }
    
    return 0;
    
}
