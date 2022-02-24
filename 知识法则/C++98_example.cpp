#include <iostream>
using namespace std;

double betsy(int);
double pam(int);

// second argument is pointer to a type double fucntion that
// taeks a type int argument
void estimate(int lines, double (*ptr)(int)); 

int main()
{
	int code = 30;
	cout << "How many lines of code do you need? ";
	cout << "Here's Besty's estimate:\n";
	estimate(code, betsy);
	cout << "Here's Pam's estimate:\n";
	estimate(code, pam);
	return 0;
}

double betsy(int Ins)
{
	return 0.05 * Ins;
}

double pam(int Ins)
{
	return 0.03 * Ins + 0.0004 * Ins * Ins; 
}

void estimate(int lines, double (*pf)(int))
{
	cout << lines << " lines will take ";
	cout << (*pf)(lines) << " hour(s)\n";
}