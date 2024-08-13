
#include <cmath>
#include <iostream>
#include "defaultDataSource.h"
#include "fileDataSource.h"
#include "arrayDataSource.h"
#include "alternateDataSource.h"
#include "generatorDataSource.h"

char* generateRandomString() {
	const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
	char* result=new char[11];
	for (int i = 0; i < 10; ++i) {
		int randomIndex = rand() % 26;
		result[i] = alphabet[randomIndex];
	}
	result[10] = '\0';
	return result;
}


unsigned nextPrime() {
	static unsigned current = 2;
	unsigned prime = current;

	bool found = false;
	while (!found) {
		++current;
		found = true;
		for (unsigned i = 2; i <= std::sqrt(current); ++i) {
			if (current % i == 0) 
			{
				found = false;
				break;
			}
		}
	}
	return prime;
}


unsigned simpleRandom() {
	static unsigned seed = 123456789;
	seed = (seed * 1103515245 + 12345) % 71274175175;
	return seed;
}


unsigned nextFibonacci() {
	static unsigned prev1 = 1, prev2 = 0, count = 1;
	if (count >= 25)
	{
		prev1 = 1;
		prev2 = 0;
		count = 1;
	}
	unsigned result = prev1 + prev2;
	prev2 = prev1;
	prev1 = result;
	count++;
	return result;
}


int main()
{
	try
	{
		GeneratorDataSource<unsigned> primeSource(nextPrime);
		GeneratorDataSource<unsigned> randomSource(simpleRandom);
		GeneratorDataSource<unsigned> fibonacciSource(nextFibonacci);

		const DataSource<unsigned>* sources[] = { &primeSource, &randomSource, &fibonacciSource };
		AlternateDataSource<unsigned> alternateSource(sources, 3);

		//Write on binary file 1000 elements
		std::ofstream binaryFile("output.dat", std::ios::binary);
		for (int i = 0; i < 1000; ++i) {
			unsigned num = alternateSource.getNextElement();
			binaryFile.write(reinterpret_cast<const char*>(&num), sizeof(num));
		}
		binaryFile.close();

		// Transfer to text file
		std::ifstream binaryInput("output.dat", std::ios::binary);
		std::ofstream textFile("output.txt");
		unsigned num;
		while (binaryInput.read(reinterpret_cast<char*>(&num), sizeof(num)))
		{
			textFile << num << std::endl;
		}

		binaryInput.close();
		textFile.close();

		// Read from the text file and print
		FileDataSource<unsigned> fileSource("output.txt");
		while (fileSource) {
			unsigned num;
			fileSource >> num;
			std::cout << num << std::endl;
		}

	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}


	// Generate the 25 strings with length 10
	GeneratorDataSource<char*> generator(&generateRandomString);
	for (size_t i = 0; i < 25; i++)
	{
		char* string = generator.getNextElement();
		std::cout << string << std::endl;
	}
}




