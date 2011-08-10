#include "ch.h"

void ConvexHull::setPoints(int n, const point* p)
{
	this->p=p;
	this->nrPoints=n;
}

ConvexHull::ConvexHull(int n, const point* p)
{
	setPoints(n, p);
}

ConvexHull::computeTetrahedon()
{
	vertex *v=new vertex[5];
	v[1].p=this->p[1];
	v[2].p=this->p[2];
	this->viz[1]=viz[2]=true;
	
	int i=3;
	while (collinear(this->p[1], this->p[2], this->p[i])) ++i;
	v[3].p=this->p[i];
	this->viz[i]=true;

	int j=i+1;
	while (coplanar(this->p[1], this->p[2], this->p[i], this->p[j])) ++j;
	v[4].p=this->p[j];
	this->viz[j]=true;

	edge *e=new edge [4];
	face *newface=new face;

	e[1].next=e[2];
	e[2].next=e[3];
	e[3].next=e[1];

	e[1].prev=e[3];
	e[2].prev=e[1];
	e[3].prev=e[2];

	for (i=1; i <= 3; ++i)
	{
		e[i].origin=v[i];
		v[i].e=e[i];
		e[i].f=newface;
	}

	newface.equ=new equation(v[1].p, v[2].p, v[3].p);
	newface.e=e1;
	this->ch.f.push_back(newface);

	for (i=1; i <= 4; ++i)
		this->ch.v.push_back(v[i]);

	delete v[0];
	delete e[0];
}

ConvexHull::addPoint(point P)
{
	deleteConflicts();
	addFaces();
}

ConvexHull::computeConvexHull()
{
	computeTetrahedon();
	conflictTetrahedon();
	int i;
	for(i=1; i <= this->nrPoints; ++i)
	{
		if(this->viz[i] == true) continue;
		addPoint(this->p[i]);
	}
}

int main(int argc, const char *argv[])
{
	
	return 0;
}
