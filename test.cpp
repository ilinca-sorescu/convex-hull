#include "test.h"
#include <cassert>
#include <cstdio>

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
    if ((expectedResult == 'y' && result == true) || (expectedResult == 'n' && result == false)) 
    {
      fprintf (stderr, "#%d: ok!\n", i);
      fprintf (out, "#%d: ok!\n", i);
    }
    else
    {
      fprintf (stderr, "#%d: wrong!\n", i);
      fprintf (out, "#%d: wrong!\n", i);
    }

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
    if ((expectedResult == 'y' && result == true) || (expectedResult == 'n' && result == false)) 
    {
      fprintf (stderr, "#%d: ok!\n", i);
      fprintf (out, "#%d: ok!\n", i);
    }
    else
    {
      fprintf (stderr, "#%d: wrong!\n", i);
      fprintf (out, "#%d: wrong!\n", i);
    }

  }

  fclose(in);
  fclose(out);

  fprintf (stderr, "--------------\n");
}

void TestConvexHull::test_computeTetrahedon()
{
  fprintf (stderr, "ComputeTetrahedon:\n");

	FILE *in, *out;
  bool ok;
	int T, i, j, n, ord[5], res;
  point* p;

	in = fopen("computeTetrahedon/tests.txt", "r");
	out = fopen("computeTetrahedon/log.txt", "w");

  assert (fscanf(in, "%d", &T) != EOF);
	for (i=1; i <= T; i++)
	{
      fprintf(stderr, "#%d: ", i);
      fprintf(out, "#%d: ", i); 

			assert (fscanf(in, "%d", &n) != EOF);
      p=new point [n+1];

      for (j=1; j <= n; ++j)
        assert (fscanf(in, "%lf%lf%lf", &p[j].x, &p[j].y, &p[j].z) != EOF);

      this->setPoints(n, p);
      this->computeTetrahedon();

      ord[0]=0;
      for (j=1; j <= n; ++j)
      {
        if (this->viz[j] == false) continue;
       
        ord[++ord[0]]=j;
        
        fprintf(stderr, "%d ", j);
        fprintf(out, "%d ", j);
      }
      
      ok=true;
      for (j=1; j <= 4 && ok; ++j)
      {
        assert (fscanf(in, "%d", &res) != EOF);
        if (res != ord [j]) ok=false;
      }

      if (ok == true)
      {
        fprintf(stderr, " -- ok!\n");
        fprintf(out, " -- ok!\n");
      }
      else
      {
        fprintf(stderr, " -- wrong!\n");
        fprintf(out, " -- wrong!\n");
      }

      delete [] this->p; 
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
}

int main()
{
  TestConvexHull t;
  t.run_tests();
  return 0;
}
