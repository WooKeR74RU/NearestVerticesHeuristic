#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <numeric>
using namespace std;

int HEUR_VER = 1; //Values: 1 - 2
int AXES_COUNT = 32; //Values > 0
int THICKNESS = 3; //Values > 0

const int MIN_N = 1;
const int MAX_N = 1e3;
const int MIN_COORD = -1e4;
const int MAX_COORD = 1e4;
const int TEST_COUNT = 1e3;

extern double standardSolution(int n, const vector<pair<int, int>>& points);
extern double heuristic(int n, const vector<pair<int, int>>& points);

const double EPS = FLT_EPSILON;
bool doubleEqual(double a, double b)
{
	return abs(a - b) < EPS;
}

int rand(int l, int r)
{
	return rand() % (r - l + 1) + l;
}

int main()
{
	FILE* filePtr;
	freopen_s(&filePtr, "input.txt", "r", stdin);
	freopen_s(&filePtr, "output.txt", "w", stdout);

	srand((uint32_t)time(nullptr));

	vector<double> deviation(TEST_COUNT);
	for (int k = 0; k < TEST_COUNT; k++)
	{
		int n = rand(MIN_N, MAX_N);

		set<pair<int, int>> setPoints;
		while (setPoints.size() < n)
		{
			int x = rand(MIN_COORD, MAX_COORD);
			int y = rand(MIN_COORD, MAX_COORD);
			setPoints.insert(make_pair(x, y));
		}

		vector<pair<int, int>> points(setPoints.begin(), setPoints.end());

		double standardAns = standardSolution(n, points);
		double heuristicAns = heuristic(n, points);
		
		if (!doubleEqual(standardAns, 0))
			deviation[k] = abs(standardAns - heuristicAns) / standardAns;
	}

	double avgDeviation = accumulate<vector<double>::iterator, double>(deviation.begin(), deviation.end(), 0) / TEST_COUNT;
	double minDeviation = *min_element(deviation.begin(), deviation.end());
	double maxDeviation = *max_element(deviation.begin(), deviation.end());

	cout.setf(ios::fixed);
	cout.precision(10);

	cout << "avgDeviation = " << avgDeviation << endl;
	cout << "minDeviation = " << minDeviation << endl;
	cout << "maxDeviation = " << maxDeviation << endl;

	return 0;
}

