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

#ifndef EIGEN_MATRIXREF_H
#define EIGEN_MATRIXREF_H

namespace Eigen
{

template<typename MatrixType> class MatrixRef
{
  public:
    typedef typename ForwardDecl<MatrixType>::Scalar Scalar;
    typedef MatrixXpr<MatrixRef<MatrixType> > Xpr;
    
    MatrixRef(MatrixType& matrix) : m_matrix(matrix) {}
    MatrixRef(const MatrixRef& other) : m_matrix(other.m_matrix) {}
    ~MatrixRef() {}

    static bool hasDynamicNumRows()
    {
      return MatrixType::hasDynamicNumRows();
    }

    static bool hasDynamicNumCols()
    {
      return MatrixType::hasDynamicNumCols();
    }
    
    int rows() const { return m_matrix.rows(); }
    int cols() const { return m_matrix.cols(); }

    const Scalar& read(int row, int col) const
    {
      return m_matrix.read(row, col);
    }
    
    Scalar& write(int row, int col)
    {
      return m_matrix.write(row, col);
    }

    Xpr xpr()
    {
      return Xpr(*this);
    }

  protected:
    MatrixType& m_matrix;
};

} // namespace Eigen

#endif // EIGEN_MATRIXREF_H
