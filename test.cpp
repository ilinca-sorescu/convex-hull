#include "test.h"
#include <cassert>
#include <cstdio>

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
      
      delete [] this->p; 
  }
  fclose(in);
  fclose(out);

  fprintf (stderr, "--------------\n");

}

void TestConvexHull::test_conflictTetrahedon()
{
  fprintf (stderr, "ConflictTetrahedon:\n");

	FILE *in, *out;
	int T, i, j, k, pp, ff, n;
  bool ok; 
	
  in = fopen("conflictTetrahedon/tests.txt", "r");
	out = fopen("conflictTetrahedon/log.txt", "w");

  assert (fscanf(in, "%d", &T) != EOF);
	for (i=1; i <= T; i++)
	{
    readPoints(in);

    fprintf(stderr, "#%d:\n", i);
    fprintf(out, "#%d: ", i);
   
    fscanf(in, "%d", &n);

    ok=true; 
    for(j=1; ok && j <= this->nrPoints; ++j)
      for(k=0; ok && k != (int)this->conflictP[j].size(); ++k, --n)
      {
        fscanf(in, "%d %d", &pp, &ff);
        
        if (n < 0)
        {
          ok=false;
          continue;
        }

        fprintf(stderr, "point: %d face: %d\n", j, k);

        if (pp != j || ff != this->conflictP[j][k])
          ok=false;
      }  

    print(out, ok);
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
