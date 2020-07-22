#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#define sqr(x) ((x) * (x))
using namespace std;

extern int AXES_COUNT;
extern int THICKNESS;
extern int HEUR_VER;
namespace heur
{
	class DisjointSet
	{
	private:
		vector<int> par;
		vector<int> setRank;
	public:
		void initialize(int count)
		{
			par.resize(count);
			for (int i = 0; i < count; i++)
				par[i] = i;
			setRank.assign(count, 0);
		}
		int get(int v)
		{
			if (par[v] != v)
			{
				int tmp = get(par[v]);
				par[v] = tmp;
			}
			return par[v];
		}
		void join(int a, int b)
		{
			a = get(a);
			b = get(b);
			if (setRank[a] > setRank[b])
			{
				par[b] = a;
			}
			else
			{
				par[a] = b;
				if (setRank[a] == setRank[b])
					setRank[b]++;
			}
		}
	};

	const double EPS = FLT_EPSILON;
	bool doubleEqual(double a, double b)
	{
		return abs(a - b) < EPS;
	}

	const double PI = M_PI;

	struct Point
	{
		double x;
		double y;
		int id;
		double curInterval;
		Point(int id, double x, double y) : id(id), x(x), y(y)
		{ }
	};
	typedef Point* PntPtr;
	double distance(PntPtr a, PntPtr b)
	{
		return sqrt(sqr(a->x - b->x) + sqr(a->y - b->y));
	}
	bool intervalCmp(PntPtr a, PntPtr b)
	{
		return a->curInterval < b->curInterval;
	}

	struct Line
	{
		double a;
		double b;
		double c;
		Line() = default;
		Line(double a, double b, double c) : a(a), b(b), c(c)
		{ }
		void moveTo(Point point)
		{
			c = -(a * point.x + b * point.y);
		}
	};
	double orientedDistance(Line line, Point point)
	{
		return -(line.a * point.x + line.b * point.y + line.c) / sqrt(sqr(line.a) + sqr(line.b));
	}
	double getInterval(Line line, Point point)
	{
		line.moveTo(point);
		return orientedDistance(line, Point(0, 0, 0));
	}

	struct Edge
	{
		double weight;
		PntPtr v;
		PntPtr u;
		Edge(double weight, PntPtr v, PntPtr u) : weight(weight), v(v), u(u)
		{ }
	};
	bool weightCmp(const Edge& a, const Edge& b)
	{
		return a.weight < b.weight;
	}

	double ver1(int n, const vector<pair<int, int>>& points)
	{
		vector<Point> graph;
		graph.reserve(n);
		vector<PntPtr> curAxis(n);
		for (int i = 0; i < n; i++)
		{
			graph.emplace_back(i, points[i].first, points[i].second);
			curAxis[i] = &graph[i];
		}

		vector<Line> lines(AXES_COUNT);
		double angle = 0;
		for (int i = 0; i < AXES_COUNT; i++)
		{
			double mod = fmod(angle, PI);
			if (doubleEqual(mod, PI / 2))
			{
				lines[i] = Line(1, 0, 0);
			}
			else
			{
				if (doubleEqual(mod, 0))
					lines[i] = Line(0, 1, 0);
				else
					lines[i] = Line(tan(angle), 1, 0);
			}
			angle += PI / AXES_COUNT;
		}

		vector<Edge> edges;
		for (int k = 0; k < AXES_COUNT; k++)
		{
			for (int i = 0; i < n; i++)
				graph[i].curInterval = getInterval(lines[k], graph[i]);
			sort(curAxis.begin(), curAxis.end(), intervalCmp);
			const int DIRECTIONS = 2;
			for (int l = 0; l < DIRECTIONS; l++)
			{
				list<PntPtr> prev;
				prev.push_back(curAxis[0]);
				int diffValues = 0;
				for (int i = 1; i < n; i++)
				{
					Edge minEdge(FLT_MAX, nullptr, nullptr);
					for (auto it = prev.begin(); it != prev.end(); it++)
					{
						double dist = distance(curAxis[i], *it);
						if (dist < minEdge.weight)
							minEdge = Edge(dist, curAxis[i], *it);
					}
					edges.push_back(minEdge);
					if (!doubleEqual(prev.back()->curInterval, curAxis[i]->curInterval))
						diffValues++;
					prev.push_back(curAxis[i]);
					if (diffValues == THICKNESS)
					{
						PntPtr first = *prev.begin();
						while (doubleEqual(first->curInterval, (*prev.begin())->curInterval))
							prev.erase(prev.begin());
						diffValues--;
					}
				}
				if (l == 0)
					reverse(curAxis.begin(), curAxis.end());
			}
		}
		sort(edges.begin(), edges.end(), weightCmp);

		double answer = 0;
		DisjointSet dSet;
		dSet.initialize(n);
		for (int i = 0; i < edges.size(); i++)
		{
			int getV = dSet.get(edges[i].v->id);
			int getU = dSet.get(edges[i].u->id);
			if (getV != getU)
			{
				answer += edges[i].weight;
				dSet.join(getV, getU);
			}
		}
		return answer;
	}

	double ver2(int n, const vector<pair<int, int>>& points)
	{
		vector<Point> graph;
		graph.reserve(n);
		vector<PntPtr> curAxis(n);
		for (int i = 0; i < n; i++)
		{
			graph.emplace_back(i, points[i].first, points[i].second);
			curAxis[i] = &graph[i];
		}

		vector<Line> lines(AXES_COUNT);
		double angle = PI / AXES_COUNT / 3;
		for (int i = 0; i < AXES_COUNT; i++)
		{
			double mod = fmod(angle, PI);
			if (doubleEqual(mod, PI / 2))
			{
				lines[i] = Line(1, 0, 0);
			}
			else
			{
				if (doubleEqual(mod, 0))
					lines[i] = Line(0, 1, 0);
				else
					lines[i] = Line(tan(angle), 1, 0);
			}
			angle += PI / AXES_COUNT;
		}

		vector<Edge> edges;
		for (int k = 0; k < AXES_COUNT; k++)
		{
			for (int i = 0; i < n; i++)
				graph[i].curInterval = getInterval(lines[k], graph[i]);
			sort(curAxis.begin(), curAxis.end(), intervalCmp);
			list<PntPtr> prev;
			prev.push_back(curAxis[0]);
			int diffValues = 0;
			for (int i = 1; i < n; i++)
			{
				for (auto it = prev.begin(); it != prev.end(); it++)
					edges.push_back(Edge(distance(curAxis[i], *it), curAxis[i], *it));
				if (!doubleEqual(prev.back()->curInterval, curAxis[i]->curInterval))
					diffValues++;
				prev.push_back(curAxis[i]);
				if (diffValues == THICKNESS)
				{
					PntPtr first = *prev.begin();
					while (doubleEqual(first->curInterval, (*prev.begin())->curInterval))
						prev.erase(prev.begin());
					diffValues--;
				}
			}
		}
		sort(edges.begin(), edges.end(), weightCmp);

		double answer = 0;
		DisjointSet dSet;
		dSet.initialize(n);
		for (int i = 0; i < edges.size(); i++)
		{
			int getV = dSet.get(edges[i].v->id);
			int getU = dSet.get(edges[i].u->id);
			if (getV != getU)
			{
				answer += edges[i].weight;
				dSet.join(getV, getU);
			}
		}

		return answer;
	}
}

using namespace heur;

double heuristic(int n, const vector<pair<int, int>>& points)
{
	if (HEUR_VER == 1)
		return ver1(n, points);
	return ver2(n, points);
}
