#include"../src/All.h"

using namespace std;
using namespace Eigen;

int main(int, char **)
{
  Matrix<double,2,2> m; // 2x2 fixed-size matrix with uninitialized entries
  m(0,0) = 1;
  m(0,1) = 2;
  m(1,0) = 3;
  m(1,1) = 4;
  cout << "Here is a 2x2 matrix m:" << endl << m << endl;
  cout << "Let us now build a 4x4 matrix m2 by assembling together four 2x2 blocks." << endl;
  MatrixX<double> m2(4, 4); // dynamic matrix with initial size 4x4 and uninitialized entries
  // notice how we are mixing fixed-size and dynamic-size types.
  
  cout << "In the top-left block, we put the matrix m shown above." << endl;
  // here we need to use .xpr() to allow write access to the block.
  m2.xpr().block(0,1,0,1) = m;
  cout << "In the bottom-left block, we put the matrix m*m, which is:" << endl << m*m << endl;
  m2.xpr().block(2,3,0,1) = m * m;
  cout << "In the top-right block, we put the matrix m+m, which is:" << endl << m+m << endl;
  m2.xpr().block(0,1,2,3) = m + m;
  cout << "In the bottom-right block, we put the matrix m-m, which is:" << endl << m-m << endl;
  m2.xpr().block(2,3,2,3) = m - m;
  cout << "Now the 4x4 matrix m2 is:" << endl << m2 << endl;
  
  // here we don't need to use .xpr() because we only need read access.
  cout << "The central 2x2 block of m2 is:" << endl << m2.block(1,2,1,2) << endl;
  cout << "Row 0 of m2, written as a column vector, is:" << endl << m2.row(0) << endl;
  cout << "Column 1 of m2 is:" << endl << m2.col(1) << endl;
  cout << "The matrix m2 with row 0 and column 1 removed is:" << endl << m2.minor(0,1) << endl;

  cout << endl << "Now let us study a tricky issue." << endl;
  cout << "Recall that the matrix product m*m is:" << endl << m*m << endl;
  cout << "We want to store that into m, i.e. do \"m = m * m;\"" << endl;
  cout << "Here we must be very careful. For if we do \"m = m * m;\"," << endl
       << "the matrix m becomes" << endl;
  Matrix<double,2,2> m_save = m;
  m = m * m; // the bogus operation
  cout << m << "," << endl;
  cout << "which is not what was wanted!" << endl
       << "Explanation: because of the way expression templates work, the matrix m gets" << endl
       << "overwritten _while_ the matrix product m * m is being computed." << endl
       << "This is the counterpart of eliminating temporary objects!" << endl
       << "Anyway, if you want to store m * m into m, you can do this:" << endl
       << "            m.alias() = m * m;" << endl;
  m = m_save;
  m.alias() = m * m;
  cout << "And m is now:" << endl << m << endl << "as was expected." << endl;
  cout << "To make your life easier, operator*= between matrices automatically" << endl
       << "creates an alias. So a *= b is equivalent to a.alias() *= b," << endl
       << "when a and b are matrices. So, coming back to the original matrix m," << endl
       << "if we do m *= m, the matrix m becomes:" << endl;
  m = m_save;
  m *= m;
  cout << m << endl << "as expected." << endl;
  return 0;
}
