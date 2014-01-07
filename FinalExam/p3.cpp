#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#define MIN_PRECISION	0.000000001
#define MAX_DEPTH		1000

#define INPUT_FILE		"input3.txt"
#define OUTPUT_FILE		"1352978_output3.txt"

// abs(x)
double fabs(double x)
{
	return (x < 0) ? -x : x;
}

// calculate f(x)
double f(std::vector<double>& c, double x)
{
	double r = 0.0;
	double xc = 1.0;
	
	for (int i = 0; i < c.size(); ++i) {
		r += c[i]*xc;
		xc *= x;
	}
	
	return r;
}

// calculate df(x)
std::vector<double> df(std::vector<double>& c)
{
	std::vector<double> c2;
	
	for (int i = 1; i < c.size(); ++i) {
		c2.push_back(c[i]*i);
	}
	
	return c2;
}

// newton algorithm
double newton(std::vector<double>& fx, std::vector<double>& dfx, double x)
{
	int depth = 0;
	double xn = x;
	
	while (depth < MAX_DEPTH) {
		double t = f(fx, xn);
		if (fabs(t) < MIN_PRECISION) break;
		xn = xn - t / f(dfx, xn);
		depth++;
	}
	
	return xn;
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
    
    std::vector<double> fx;
    
    while (fin.good()) {
    	double d; fin >> d;
    	fx.push_back(d);
    }
    
    fin.close();
    
    // 2. for f(x) = 9x^3+7x^2+5x+3.6,
    // fx  = [3.6, 5, 7, 9] === 3.6 +   5*x +   7*x^2 + 9*x^3
    // dfx = [     5, 7, 9] === 5   + 7*2*x + 9*3*x^2
    std::reverse(fx.begin(), fx.end());
    std::vector<double> dfx = df(fx);
    
    // 3. Calculate! (let x0 = 0)
    double result = newton(fx, dfx, 0.0);
    
    // 4. Output solution
    std::ofstream fout;
    fout.open(OUTPUT_FILE);
    if (!fout.good()) {
        std::cout << "Error: Cannot open file: " << OUTPUT_FILE << std::endl;
        return 1;
    }
    fout << result << std::endl;
	fout.close();
	
    return 0;
    
}
