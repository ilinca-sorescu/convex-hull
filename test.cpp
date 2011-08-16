#include "test.h"
#include <cassert>
#include <cstdio>

#define x coord[0]
#define y coord[1]
#define z coord[2]

#define a coefficient[0]
#define b coefficient[1]
#define c coefficient[2]
#define d coefficient[3]

inline void TestConvexHull::print(FILE* out, bool value, int k=0)
{
  if (k != 0)
  {
    fprintf (stderr, "#%d: ", k);
    fprintf (out, "#%d: ", k);
  }
  if (value == true)
  {
    fprintf (stderr, " ok!\n");
    fprintf (out, " ok!\n");
  }
  else
  {
    fprintf (stderr, " wrong!\n");
    fprintf (out, " wrong!\n");
  }
}

void TestConvexHull::test_collinear()
{
  fprintf (stderr, "Collinear:\n");

	FILE *in, *out;
	int T, i, j;
  char expectedResult;
  bool result;
  point p[4];

	in = fopen("collinear/tests.txt", "r");
	out = fopen("collinear/log.txt", "w");

  assert (fscanf(in, "%d", &T) != EOF);
	for (i=1; i <= T; i++)
	{
		
    for (j=1; j <= 3; j++)
			assert (fscanf(in, "%lf%lf%lf", &p[j].x, &p[j].y, &p[j].z) != EOF);
    
    assert (fscanf(in, " %c", &expectedResult) != EOF);
    result=ConvexHull::collinear(p[1], p[2], p[3]);
    
    print (out, (expectedResult == 'y' && result == true) || (expectedResult == 'n' && result == false), i);
  }

  fclose(in);
  fclose(out);

  fprintf (stderr, "--------------\n");
}

void TestConvexHull::test_coplanar()
{
  fprintf (stderr, "Coplanar:\n");

	FILE *in, *out;
	int T, i, j;
  char expectedResult;
  bool result;
  point p[5];

	in = fopen("coplanar/tests.txt", "r");
	out = fopen("coplanar/log.txt", "w");

  assert (fscanf(in, "%d", &T) != EOF);
	for (i=1; i <= T; i++)
	{
		
    for (j=1; j <= 4; j++)
			assert (fscanf(in, "%lf%lf%lf", &p[j].x, &p[j].y, &p[j].z) != EOF);
    
    assert (fscanf(in, " %c", &expectedResult) != EOF);
    result=ConvexHull::coplanar(p[1], p[2], p[3], p[4]);
    
    print (out, (expectedResult == 'y' && result == true) || (expectedResult == 'n' && result == false), i); 
  }

  fclose(in);
  fclose(out);

  fprintf (stderr, "--------------\n");
}

inline void TestConvexHull::readPoints(FILE* in)
{
      int n, j;
      point *p;
			assert (fscanf(in, "%d", &n) != EOF);
      p=new point [n+1];

      for (j=1; j <= n; ++j)
        assert (fscanf(in, "%lf%lf%lf", &p[j].x, &p[j].y, &p[j].z) != EOF);
      this->setPoints(n, p);
}

void TestConvexHull::test_computeTetrahedon()
{
  fprintf (stderr, "ComputeTetrahedon:\n");

	FILE *in, *out;
  bool ok;
	int T, i, j, ord[5], res;

	in = fopen("computeTetrahedon/tests.txt", "r");
	out = fopen("computeTetrahedon/log.txt", "w");

  assert (fscanf(in, "%d", &T) != EOF);
	for (i=1; i <= T; i++)
	{
      fprintf(stderr, "#%d: ", i);
      fprintf(out, "#%d: ", i);

      readPoints(in);
     
      this->computeTetrahedon();

      ord[0]=0;
      for (j=1; j <= this->nrPoints; ++j)
      {
        if (this->viz[j] == false) continue;
       
        ord[++ord[0]]=j;
        
        fprintf(out, "%d ", j);
      }
      
      ok=true;
      for (j=1; j <= 4 && ok; ++j)
      {
        assert (fscanf(in, "%d", &res) != EOF);
        if (res != ord [j]) ok=false;
      }

      fprintf (out, " -- ");

      print (out, ok == true);
      
      this->clean_up();
 /*     delete [] this->p; 
      this->viz.resize(0);
      this->ch.v.resize(0);
      this->ch.f.resize(0);*/
  }
  fclose(in);
  fclose(out);

  fprintf (stderr, "--------------\n");

}

void TestConvexHull::test_conflictTetrahedon()
{
  fprintf (stderr, "ConflictTetrahedon:\n");

	FILE *in, *out;
	int T, i, j, k, n, num, pj, fj;
  bool ok; 

  std::vector<int>pp;
  std::vector<int>ff;
	
  in = fopen("conflictTetrahedon/tests.txt", "r");
	out = fopen("conflictTetrahedon/log.txt", "w");

  assert (fscanf(in, "%d", &T) != EOF);
	for (i=1; i <= T; i++)
	{
    readPoints(in);

    this->computeTetrahedon();
    this->conflictTetrahedon();

    fprintf(stderr, "#%d: ", i);
    fprintf(out, "#%d:\n", i);
   
    assert(fscanf(in, "%d", &n) != EOF);

    for(j=1; j <= n; ++j)
    {
      assert(fscanf(in, "%d %d", &pj, &fj) != EOF);
      pp.push_back(pj);
      ff.push_back(fj);
    }

    ok=true; 
    num=0;
    for(j=1; j <= this->nrPoints; ++j)
      for(k=0; k != (int)this->conflictP[j].size(); ++k, ++num)
      {
       
        fprintf(out, "point: %d face: %d\n", j, this->conflictP[j][k]);

        if (num >= n)
        {
          ok=false;
          continue;
        }

        if (pp[num] != j || ff[num] != this->conflictP[j][k])
          ok=false;
      } 

    if (n != num) 
        ok=false; 

    print(out, ok);
 
    pp.resize(0);
    ff.resize(0);

    this->clean_up();
/*    delete [] this->p;
    this->viz.resize(0);
    this->ch.v.resize(0);
    this->ch.f.resize(0);
    this->conflictP.resize(0);*/
  }

  fclose(in);
  fclose(out);

  fprintf (stderr, "--------------\n");
}


void TestConvexHull::run_tests()
{
  TestConvexHull t;
  t.test_collinear();
  t.test_coplanar();
  t.test_computeTetrahedon();
  t.test_conflictTetrahedon();
}

int main()
{
  TestConvexHull t;
  t.run_tests();
  return 0;
}

#undef x
#undef y
#undef z

#undef a
#undef b
#undef c
#undef d

