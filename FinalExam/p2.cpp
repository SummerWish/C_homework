#include <iostream>
#include <fstream>
#include <vector>

#define INPUT_FILE		"input2.txt"
#define OUTPUT_FILE		"1352978_output2.txt"

int main()
{
	
	// 1. Read data from file
	std::ifstream fin;
    fin.open(INPUT_FILE);
        
    if (!fin.good()) {
        std::cout << "Error: Cannot open file: " << INPUT_FILE << std::endl;
        return 1;
    }
    
    std::vector<int> data;
    
    while (fin.good()) {
    	int d; fin >> d;
    	data.push_back(d);
    }
    
    fin.close();
    
    // 2. Insert unique data into new vector
    std::vector<int> result;
    if (data.size() > 0) {
    	
    	result.push_back(data[0]);
    	
    	for (int i = 1; i < data.size(); ++i) {
    		if (data[i] != data[i - 1]) {
    			result.push_back(data[i]);
    		}
    	}
    	
    }
    
    // 3. Output data
    std::ofstream fout;
    fout.open(OUTPUT_FILE);
    if (!fout.good()) {
        std::cout << "Error: Cannot open file: " << OUTPUT_FILE << std::endl;
        return 1;
    }
    // Possible output:
    // There are X numbers left in the list: A B C D.
    // There are 1 number left in the list: A.
    // There are 0 number left in the list.
    fout << "There are " << result.size() << " number";
	if (result.size() > 1) {
		fout << "s";	
	}
	fout << " left in the list";
	if (result.size() > 0) {
		fout << ":";
		for (int i = 0; i < result.size(); ++i) {
			fout << " " << result[i];
		}
	}
	fout << ".";
	fout.close();
    
    return 0;
    
}
