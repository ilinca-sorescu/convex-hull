#include "../ch.h"

class TestConvexHull : ConvexHull
{
  public:
    inline void readPoints(FILE*);
    inline void print(FILE*, bool, int);
    void test_collinear();
    void test_coplanar();
    void test_computeTetrahedon();
    void test_conflictTetrahedon();
    void test_getConvexHull();
    void run_tests();
};

