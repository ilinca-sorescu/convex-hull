#include <vector>

struct point
{
	double x,y,z;
};

struct equation
{
	double a, b, c, d;
	equation(point p1, point p2, point p3)
	{
		a=p1.y*(p2.z - p3.z) + p2.y*(p3.z - p1.z) + p3.y*(p1.z - p2.z);
		b=p1.z*(p2.x - p3.x) + p2.z*(p3.x - p1.x) + p3.z*(p1.x - p2.x);
		c=p1.x*(p2.y - p3.y) + p2.x*(p3.y - p1.y) + p3.x*(p1.y - p2.y);
		d=p1.x*(p2.y2*p3.z - p3.y*p2.z) + p2.x*(p3.y*p1.z - p1.y*p3.z) + p3.x*(p1.y*p2.z - p2.y*p1.z);
		d*=-1;
	}
};

struct edge;
struct vertex;

struct face
{
	equation *equ;
	edge *e;
	face ()
	{
		equ=e=NULL;
	}
};

struct edge
{
	vertex *origin;
	edge *next,*prev,*twin;
	face *f;
	edge ()
	{
		origin=next=prev=tewin=face=NULL;
	}
};

struct vertex
{
	point *p;
	edge *e;
	vertex ()
	{
		p=e=NULL;
	}
};

class doublyConnectedEdgeList
{
	public:
		doublyConnectedEdgeList();
		~doublyConnectedEdgeList();
		std::vector<face*> f;
		std::vector<vertex*> v;	
};

class ConvexHull
{
	public:
		ConvexHull();
		ConvexHull(int, const point*);
		~ConvexHull();
		void setPoints(int , const point*);
		doublyConnectedEdgeList getConvexHull();
	private:
		void computeConvexHull();
		const point* p;		
		int nrPoints;
		doublyConnectedEdgeList ch;
}

