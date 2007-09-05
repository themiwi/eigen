// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2007 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with Eigen; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. This exception does not invalidate any other reasons why a work
// based on this file might be covered by the GNU General Public License.

#ifndef EIGEN_MINOR_H
#define EIGEN_MINOR_H

namespace Eigen {

template<typename MatrixType> class MatrixMinor
{
  public:
    typedef typename MatrixType::Scalar Scalar;

    MatrixMinor(const MatrixType& matrix, int row, int col = 0)
      : m_matrix(matrix), m_row(row), m_col(col)
    {
      EIGEN_CHECK_RANGES(matrix, row, col);
    }
    
    MatrixMinor(const MatrixMinor& other)
      : m_matrix(other.m_matrix), m_row(other.m_row), m_col(other.m_col) {}
    
    int rows() const { return m_matrix.rows() - 1; }
    int cols() const { return m_matrix.cols() - 1; }
    
    Scalar& operator()(int row, int col=0)
    {
      return m_matrix(row + (row >= m_row), col + (col >= m_col));
    }
    
    Scalar operator()(int row, int col=0) const
    {
      return m_matrix(row + (row >= m_row), col + (col >= m_col));
    }
    
  protected:
    MatrixType m_matrix;
    const int m_row, m_col;
};

template<typename Derived>
MatrixConstXpr<
  MatrixMinor<
    const MatrixConstRef<
      MatrixBase<Derived>
    >
  >
>
MatrixBase<Derived>::minor(int row, int col) const
{
  typedef MatrixMinor<const ConstRef> ProductType;
  typedef MatrixConstXpr<ProductType> XprType;
  return XprType(ProductType(constRef(), row, col));
}

template<typename Content>
MatrixConstXpr<
  MatrixMinor<
    const MatrixConstXpr<Content>
  >
>
MatrixConstXpr<Content>::minor(int row, int col) const
{
  typedef MatrixMinor<
            const MatrixConstXpr<Content>
          > ProductType;
  typedef MatrixConstXpr<ProductType> XprType;
  return XprType(ProductType(*this, row, col));
}

template<typename Content>
MatrixXpr<
  MatrixMinor<
    MatrixXpr<Content>
  >
>
MatrixXpr<Content>::minor(int row, int col)
{
  typedef MatrixMinor<
            MatrixXpr<Content>
          > ProductType;
  typedef MatrixXpr<ProductType> XprType;
  return XprType(ProductType(*this, row, col));
}

}

#endif // EIGEN_MINOR_H
