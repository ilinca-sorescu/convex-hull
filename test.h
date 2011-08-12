#include "ch.h"

class TestConvexHull : ConvexHull
{
  public:
    void test_collinear();
    void test_coplanar();
    void test_computeTetrahedon();
    void run_tests();
};

