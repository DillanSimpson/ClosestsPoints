#include <iostream>
#include<math.h>
#include<vector>
#include<string>
#include<fstream>
#include<tuple>
#include "Point.h"

using namespace std;

//Holds points
vector<int> close;

//Merge 
void Merge(vector<Point> & a, int low, int high, int mid)
{
	// We have low to mid and mid+1 to high already sorted.
	int i, j, k;
	Point *temp = new Point[high - low + 1];
	i = low;
	k = 0;
	j = mid + 1;
	Point::CompareYCoordinate comY;

	// Merge the two parts into temp[].
	while (i <= mid && j <= high)
	{
			if (comY(a[i], a[j]))
			{
				temp[k] = a[i];
				k++;
				i++;
			}
			else
			{
				temp[k] = a[j];
				k++;
				j++;
			}
		}
	
	// Insert all the remaining values from i to mid into temp[].
	while (i <= mid)
	{
		temp[k] = a[i];
		k++;
		i++;
	}
	// Insert all the remaining values from j to high into temp[].
	while (j <= high)
	{
		temp[k] = a[j];
		k++;
		j++;
	}
	// Assign sorted data stored in temp[] to a[].
	for (i = low; i <= high; i++)
	{
		a[i] = temp[i - low];
	}
}

// A function to split array into two parts.
void MergeSort(vector<Point> & a, int low, int high)
{

	int mid;
	if (low < high)
	{
			mid = (low + high) / 2;
			// Split the data into two half.
			MergeSort(a, low, mid);
			MergeSort(a, mid + 1, high);

			// Merge them to get sorted output.
			Merge(a, low, high, mid);
		
	}

}

//find the smallest Distance
double smallestDist(vector<Point> &p, unsigned int n, vector<Point> c) {
	double min = DBL_MAX;
	close.resize(2);
	while (n > 0) {
		if (min > p[n].distance(p[n - 1])) {
			min = p[n].distance(p[n - 1]);

			close[0] = n;
			close[1] = n - 1;

		}
		n--;
	}
	return min;
}

//finds the smallest point with in center strip
double closestAcrossStrip(vector<Point> & strip, unsigned size, double d, vector<Point> c)
{
	MergeSort(strip, strip.size()-1,0);
	double min = d;
	while (size > 0) {
		if (strip[size].distance(strip[size - 1]) < min) {
			min = strip[size].distance(strip[size - 1]);
			close[0] = size;
			close[1] = size - 1;
		}
		size--;
	}
	return min;
}

//Reads in file and builds vector
vector<Point> readPoints(string file) {
	ifstream in;
	string line;
	int count = 0;
	Point p;
	vector<Point> v;
	double x, y;
	in.open(file);

	if (in.is_open()) {
		while (in >> line) {
			if (count % 2 == 0) {
				x = stod(line);
			}
			else {
				y = stod(line);
			}
			if (count % 2 == 1) {
				p.setX(x);
				p.setY(y);
				v.push_back(p);
			}
			count++;
		}
	}
	else {
		cerr << "Error opening file" << endl;
		exit(1);
	}
	return v;
}

int main(int argv, char* argc[]) {
	double upperBound;

	vector<Point> strip;
	vector<Point> arr1;
	vector<Point> clos;

	//Get file
	//arr1 = readPoints(argc[1]);

	//for testing
	//arr1 = readPoints("C:\\Users\\Dillanandjori\\Desktop\\CS320P02Simpson\\Debug\\test.txt");

	cout << "Calcuating smallest distance..." << endl;

	//Sort points
	MergeSort(arr1, 0, arr1.size() - 1);

	//construct 2 arrays from sorted array size
	vector<Point> larray;//from smallest to middle point
	vector<Point> rarray;//from middle point to largest point

	//Allocate vectors
	larray.resize((arr1.size() + 1) / 2);
	rarray.resize((arr1.size() + 1) / 2);

	//Set right sub array
	for (unsigned int i = 0; i < larray.size(); i++) {
		larray[i] = arr1[i];
	}

	//set left sub array
	for (unsigned int i = 0; i < rarray.size(); i++) {
		rarray[i] = arr1.at((arr1.size() / 2) + i);
	}

	//get smallest of left and right sub-array
	double dl, dr;
	dl = smallestDist(larray, larray.size() - 1, clos);
	dr = smallestDist(larray, rarray.size() - 1, clos);

	//get the upperbound
	upperBound = (dl < dr) ? dl : dr;


	//allocate for strip
	int j = arr1.size() / 4 + 1;

	//copy into strip vector
	for (unsigned int i = 0; i < ((arr1.size() / 4) + 1) - (arr1.size() * (3 / 4)); i++) {
		strip.push_back(arr1[j]);
		j++;
	}

	//output
	cout << "Closest Points are: " << "and " << arr1[close[0]] << " and " << arr1[close[1]] << " with a distance of " <<
		closestAcrossStrip(strip, strip.size() - 1, upperBound, clos) << endl;
	return 0;
}
