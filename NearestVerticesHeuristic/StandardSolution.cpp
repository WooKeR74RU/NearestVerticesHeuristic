#include <vector>
#include <set>
#define sqr(x) ((x) * (x))
using namespace std;

namespace stan
{
	struct Point
	{
		int x;
		int y;
		bool used;
		double dist;
		Point(int x, int y) : x(x), y(y), used(false), dist(FLT_MAX)
		{ }
	};
	typedef Point* PntPtr;
	double distance(PntPtr a, PntPtr b)
	{
		return sqrt(sqr(a->x - b->x) + sqr(a->y - b->y));
	}

	struct Edge
	{
		double weight;
		PntPtr ver;
		Edge(double weight, PntPtr ver) : weight(weight), ver(ver)
		{ }
		bool operator<(const Edge& other) const
		{
			if (weight != other.weight)
				return weight < other.weight;
			return ver < other.ver;
		}
	};

}

using namespace stan;

double standardSolution(int n, const vector<pair<int, int>>& points)
{
	vector<Point> graph;
	graph.reserve(n);
	for (int i = 0; i < n; i++)
		graph.emplace_back(points[i].first, points[i].second);

	double answer = 0;
	graph[0].dist = 0;
	set<pair<double, PntPtr>> setDists;
	setDists.insert(make_pair(0, &graph[0]));
	for (int i = 1; i < n; i++)
		setDists.insert(make_pair(FLT_MAX, &graph[i]));
	for (int k = 0; k < n; k++)
	{
		PntPtr ver = setDists.begin()->second;
		answer += setDists.begin()->first;
		setDists.erase(setDists.begin());
		ver->used = true;
		for (int i = 0; i < n; i++)
		{
			if (!graph[i].used)
			{
				double curDist = distance(ver, &graph[i]);
				if (graph[i].dist > curDist)
				{
					setDists.erase(make_pair(graph[i].dist, &graph[i]));
					setDists.insert(make_pair(curDist, &graph[i]));
					graph[i].dist = curDist;
				}
			}
		}
	}

	return answer;
}
