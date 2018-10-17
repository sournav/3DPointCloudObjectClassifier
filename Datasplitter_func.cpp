/*
*Designed by Sournav Sekhar Bhattacharya
*Additional Help: Elise Koock, Anju Bimal
*SpaceCRAFT/ASTRO Lab
*Texas A&M University College Station
*GIG THEM AGRICULTURALISTS!!
*/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <Eigen>

using Eigen::ArrayXXd;
using namespace std;


class Data {
public:

	/*
		Static values
	*/

	// Minimum/maximum values
	static double min_x;
	static double min_y;
	static double min_z;
	static double max_x;
	static double max_y;
	static double max_z;

	// Averages
	static double avg_x;
	static double avg_y;
	static double avg_z;

	// Clouds
	static ArrayXXd yz_min;
	static ArrayXXd yz_max;
	static ArrayXXd xz_min;
	static ArrayXXd xz_max;
	static ArrayXXd xy_min;
	static ArrayXXd xy_max;

};



/*
	Constant values
*/

// Input file path
const string FILE_PATH = "specialsphr.pcd";

// Size of output image
const int IMAGE_SIZE = 250;

// Max number of elements
const int MAX_ELEMENTS = 2048;




/*
	Initialize static variables
*/

double Data::max_x = -200;
double Data::max_y = -200;
double Data::max_z = -200;
double Data::min_x = 200;
double Data::min_y = 200;
double Data::min_z = 200;


double Data::avg_x = 0;
double Data::avg_y = 0;
double Data::avg_z = 0;

ArrayXXd Data::yz_min = ArrayXXd::Zero(IMAGE_SIZE, IMAGE_SIZE);
ArrayXXd Data::yz_max = ArrayXXd::Zero(IMAGE_SIZE, IMAGE_SIZE);
ArrayXXd Data::xz_min = ArrayXXd::Zero(IMAGE_SIZE, IMAGE_SIZE);
ArrayXXd Data::xz_max = ArrayXXd::Zero(IMAGE_SIZE, IMAGE_SIZE);
ArrayXXd Data::xy_min = ArrayXXd::Zero(IMAGE_SIZE, IMAGE_SIZE);
ArrayXXd Data::xy_max = ArrayXXd::Zero(IMAGE_SIZE, IMAGE_SIZE);



/*
	Returns the number of elements in the file
*/
int getElementCount(string path)
{
	// Open input stream
	ifstream file(path);

	int count = 0;
	string temp;

	// Read all lines in the file
	while (!file.eof())
	{
		getline(file, temp);
		count++;
	}

	// Ignore the first line
	return count - 1;
}


/*
	Fill clouds & calculate metadata
*/
void populateArray(ifstream& file, double cloud[3][MAX_ELEMENTS], double sums[3], double maxvals[3])
{ 
	// Current element in the file
	double data;

	// Current index of the element
	int index = 0;

	// Temporary string to store data
	string str;



	// Ignore the first line
	getline(file, str);

	// Loop through the file
	while (!file.eof())
	{
		// X
		// Read element & convert to a double
		file >> str;
		data = atof(str.c_str());

		// Add to sums/cloud
		sums[0] += data;
		cloud[0][index] = data;

		// Set min/max
		if (Data::min_x > data) {
			Data::min_x = data;
		}
		if (Data::max_x < data) {
			Data::max_x = data;
		}

		// Y
		file >> str;
		data = atof(str.c_str());

		sums[1] += data;
		cloud[1][index] = data;

		if (Data::min_y > data) {
			Data::min_y = data;
		}
		if (Data::max_y < data) {
			Data::max_y = data;
		}


		// Z
		file >> str;
		data = atof(str.c_str());

		sums[2] += data;
		cloud[2][index] = data;

		if (Data::min_z > data) {
			Data::min_z = data;
		}
		if (Data::max_z < data) {
			Data::max_z = data;
		}

		// Increment element index
		index++;
	}

	// Adjust for negative max values
	maxvals[0] = Data::max_x + abs(Data::min_x);
	maxvals[1] = Data::max_y + abs(Data::min_y);
	maxvals[2] = Data::max_z + abs(Data::min_z);
}

/*
	Normalize values & fill clouds
*/
void normalize(double maxvals[3], double cloud[3][MAX_ELEMENTS], int elementCount)
{
	int temp_x;
	int temp_y;
	int temp_z;

	// This makes sure that none of the temp values are out of bounds
	for (int i = 0; i <= elementCount; i++)
	{
		// First round normalization to the rest of the point cloud (and making everything positive)
		temp_x = (int)(round(((cloud[0][i] + abs(Data::min_x)) / maxvals[0]) * elementCount));
		temp_y = (int)(round(((cloud[1][i] + abs(Data::min_y)) / maxvals[1]) * elementCount));
		temp_z = (int)(round(((cloud[2][i] + abs(Data::min_z)) / maxvals[2]) * elementCount));


		// Second round normalization for the rest of the dataset to the size of the image
		temp_x = (int)(round(((double)(temp_x) / (double)(elementCount)) * (IMAGE_SIZE - 1)));
		temp_y = (int)(round(((double)(temp_y) / (double)(elementCount)) * (IMAGE_SIZE - 1)));
		temp_z = (int)(round(((double)(temp_z) / (double)(elementCount)) * (IMAGE_SIZE - 1)));

		// Assignment of temp values to specific clouds 
		if (cloud[0][i] < Data::avg_x) 
		{
			Data::yz_min(temp_y, temp_z) = temp_x;
		}
		else 
		{
			Data::yz_max(temp_y, temp_z) = temp_x;
		}

		if (cloud[1][i] < Data::avg_y) 
		{
			Data::xz_min(temp_x, temp_z) = temp_y;
		}
		else 
		{
			Data::xz_max(temp_x, temp_z) = temp_y;
		}

		if (cloud[2][i] < Data::avg_z) 
		{
			Data::yz_min(temp_x, temp_y) = temp_z;
		}
		else 
		{
			Data::yz_max(temp_x, temp_y) = temp_z;
		}
	}
}



int main() 
{
	// Input stream
	ifstream file(FILE_PATH);

	// Load in the number of elements
	int elementCount = getElementCount(FILE_PATH);

	// Initialize meta data variables
	double sums[3] = { 0, 0, 0 };
	double cloud[3][MAX_ELEMENTS];
	double maxvals[3] = { 0, 0, 0 };

	// Loading in the points into cloud via input stream from file
	populateArray(file, cloud, sums, maxvals);
	
	// Averages
	Data::avg_x = sums[0] / (double)elementCount;
	Data::avg_y = sums[1] / (double)elementCount;
	Data::avg_z = sums[2] / (double)elementCount;

	// Normalize data & fill clouds
	normalize(maxvals, cloud, elementCount);

	return 0;
}