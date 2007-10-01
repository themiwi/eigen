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

#ifndef EI_BLOCK_H
#define EI_BLOCK_H

template<typename MatrixType> class EiBlock
  : public EiObject<typename MatrixType::Scalar, EiBlock<MatrixType> >
{
  public:
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::Ref MatRef;
    friend class EiObject<Scalar, EiBlock<MatrixType> >;
    
    static const int RowsAtCompileTime = EiDynamic,
                     ColsAtCompileTime = EiDynamic;

    EiBlock(const MatRef& matrix,
                int startRow, int endRow,
                int startCol = 0, int endCol = 0)
      : m_matrix(matrix), m_startRow(startRow), m_endRow(endRow),
                          m_startCol(startCol), m_endCol(endCol)
    {
      assert(startRow >= 0 && startRow <= endRow && endRow < matrix.rows()
          && startCol >= 0 && startCol <= endCol && endCol < matrix.cols());
    }
    
    EiBlock(const EiBlock& other)
      : m_matrix(other.m_matrix), m_startRow(other.m_startRow), m_endRow(other.m_endRow),
                                  m_startCol(other.m_startCol), m_endCol(other.m_endCol) {}
    
    EI_INHERIT_ASSIGNMENT_OPERATORS(EiBlock)
    
  private:
    EiBlock& _ref() { return *this; }
    const EiBlock& _constRef() const { return *this; }
    int _rows() const { return m_endRow - m_startRow + 1; }
    int _cols() const { return m_endCol - m_startCol + 1; }
    
    Scalar& _write(int row, int col=0)
    {
      return m_matrix.write(row + m_startRow, col + m_startCol);
    }
    
    Scalar _read(int row, int col=0) const
    {
      return m_matrix.read(row + m_startRow, col + m_startCol);
    }
    
  protected:
    MatRef m_matrix;
    const int m_startRow, m_endRow, m_startCol, m_endCol;
};

template<typename Scalar, typename Derived>
EiBlock<Derived>
EiObject<Scalar, Derived>::block(int startRow, int endRow, int startCol, int endCol)
{
  return EiBlock<Derived>(static_cast<Derived*>(this)->ref(), startRow, endRow, startCol, endCol);
}

#endif // EI_BLOCK_H
