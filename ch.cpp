#include "ch.h"
#include <cstdlib>


ConvexHull::ConvexHull()
{
}

void ConvexHull::setPoints(int n, point* p)
{
	this->p=p;
	this->nrPoints=n;
  this->viz.resize(n+1, 0);
  this->conflictP.resize(n+1);
}

ConvexHull::ConvexHull(int n, point* p)
{
	setPoints(n, p);
}

bool ConvexHull::collinear(point p1, point p2, point p3)
{
  double eq, t;
  int i;
  
  for (i=0; (i != 3) && (p2.coord[i] == p1.coord[i]); ++i) ;
  if (i == 3)//duplicates!
  {
    fprintf(stderr, "ERROR: Duplicates!");
    exit(1);
  }
  t=(p3.coord[i]-p1.coord[i])/(p2.coord[i]-p1.coord[i]);

  for (i=0; i != 3; ++i)  
  {
    eq=p1.coord[i]+(p2.coord[i]-p1.coord[i])*t;
    eq -= p3.coord[i];
    if (!(eq >= -eps && eq <= eps)) return false;
  }
  return true;
}

bool ConvexHull::coplanar(point p1, point p2, point p3, point p4)
{
  equation coef(p1, p2, p3);
  double eq=coef.a*p4.x+coef.b*p4.y+coef.c*p4.z+coef.d;
  if (eq >= -eps && eq <= eps) return true;
  return false;
}

void ConvexHull::addFace(edge *e, vertex *v)
{
  e->twin=new edge;
  e->twin->twin=e;
  e->twin->origin=e->next->origin;

  face *newface=new face;
  newface->equ=new equation(*(e->next->origin->p), *(e->origin->p), *(v->p));
  newface->e=e->twin;
  e->twin->f=newface;
  this->ch.f.push_back(newface);

  edge *e1=new edge, *e2=new edge;
  e1->prev=e->twin;
  e2->prev=e1;
  e->twin->prev=e2;

  e1->next=e2;
  e2->next=e->twin;
  e->twin->next=e1;

  e1->f=e2->f=newface;

  e1->origin=e->origin;
  e2->origin=v;

  v->e=e2; 

  if(e->prev->twin != NULL)
    e1->twin=e->prev->twin->prev;
  if(e->next->twin != NULL)
    e2->twin=e->next->twin->next;
}

void ConvexHull::computeTetrahedon()
{
	vertex *v=new vertex[5];
	v[1].p=&this->p[1];
	v[2].p=&this->p[2];
	this->viz[1]=viz[2]=true;
	
	int i=3;
	while (collinear(this->p[1], this->p[2], this->p[i])) ++i;
	v[3].p=&this->p[i];
	this->viz[i]=true;

	int j=i+1;
	while (coplanar(this->p[1], this->p[2], this->p[i], this->p[j])) ++j;
	v[4].p=&this->p[j];
	this->viz[j]=true;

	edge *e=new edge [4];
	face *newface=new face;

	e[1].next=&e[2];
	e[2].next=&e[3];
	e[3].next=&e[1];

	e[1].prev=&e[3];
	e[2].prev=&e[1];
	e[3].prev=&e[2];

	for (i=1; i <= 3; ++i)
	{
		e[i].origin=&v[i];
		v[i].e=&e[i];
		e[i].f=newface;
	}

	newface->equ=new equation(*v[1].p, *v[2].p, *v[3].p);
	newface->e=&e[1];
	this->ch.f.push_back(newface);

	for (i=1; i <= 4; ++i)
		this->ch.v.push_back(&v[i]);

  addFace(&e[3], &v[4]);
  addFace(&e[2], &v[4]);
  addFace(&e[1], &v[4]);

  delete &v[0];
	delete &e[0];

}

inline int ConvexHull::sgn(double v)
{
  if (v >= -eps && v <= eps)
  {
    if (this->exteriorSgn == -1) return 1;
    return -1;
  }
  if (v < 0) return -1;
  return 1;
}

int ConvexHull::computeInteriorSgn()
{
  long double s[3];
  point m;
  int i, j;
  
  s[0]=s[1]=s[2]=0;
  
  for (i=1; i <= this->nrPoints; ++i)
    if (this->viz[i] == true)
      for (j=0; j != 3; ++j)
        s[j]+=this->p[i].coord[j];

  for (i=0; i != 3; ++i)
    m.coord[i]=s[i]/this->nrPoints;

  equation coef=*this->ch.f[0]->equ;
  return sgn(coef.a*m.x+coef.b*m.y+coef.c*m.z+coef.d);
}

void ConvexHull::conflictTetrahedon()
{
  this->exteriorSgn = computeInteriorSgn()*(-1);
  
  int i, j;
  equation e;

  for (i=1; i <= this->nrPoints; ++i)
  {
    if (this->viz[i] == true) continue;
    for (j=0; j != (int)this->ch.f.size(); ++j)
    {
       e=*this->ch.f[j]->equ;
       if (sgn (e.a*this->p[i].x + e.b*this->p[i].y + e.c*this->p[i].z + e.d) == this->exteriorSgn) 
       {
         //face j conflicts with point i
         this->ch.f[j]->conflict.push_back(i); 
         this->conflictP[i].push_back(j);
       }
    }
  }
}

/*ConvexHull::addPoint(point P)
{
	deleteConflicts();
	addAllFaces();
}*/

void ConvexHull::computeConvexHull()
{
	computeTetrahedon();
	conflictTetrahedon();
/*	int i;
	for(i=1; i <= this->nrPoints; ++i)
	{
		if(this->viz[i] == true) continue;
		addPoint(this->p[i]);
	}*/
}

/*int main()
{
	
	return 0;
}*/
