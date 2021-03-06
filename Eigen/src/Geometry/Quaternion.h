// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2009 Mathieu Gautier <mathieu.gautier@cea.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_QUATERNION_H
#define EIGEN_QUATERNION_H

/***************************************************************************
* Definition of QuaternionBase<Derived>
* The implementation is at the end of the file
***************************************************************************/

template<typename Other,
         int OtherRows=Other::RowsAtCompileTime,
         int OtherCols=Other::ColsAtCompileTime>
struct ei_quaternionbase_assign_impl;

template<class Derived>
class QuaternionBase : public RotationBase<Derived, 3>
{
  typedef RotationBase<Derived, 3> Base;
public:
  using Base::operator*;
  using Base::derived;

  typedef typename ei_traits<Derived>::Scalar Scalar;
  typedef typename NumTraits<Scalar>::Real RealScalar;
  typedef typename ei_traits<Derived>::Coefficients Coefficients;

 // typedef typename Matrix<Scalar,4,1> Coefficients;
  /** the type of a 3D vector */
  typedef Matrix<Scalar,3,1> Vector3;
  /** the equivalent rotation matrix type */
  typedef Matrix<Scalar,3,3> Matrix3;
  /** the equivalent angle-axis type */
  typedef AngleAxis<Scalar> AngleAxisType;



  /** \returns the \c x coefficient */
  inline Scalar x() const { return this->derived().coeffs().coeff(0); }
  /** \returns the \c y coefficient */
  inline Scalar y() const { return this->derived().coeffs().coeff(1); }
  /** \returns the \c z coefficient */
  inline Scalar z() const { return this->derived().coeffs().coeff(2); }
  /** \returns the \c w coefficient */
  inline Scalar w() const { return this->derived().coeffs().coeff(3); }

  /** \returns a reference to the \c x coefficient */
  inline Scalar& x() { return this->derived().coeffs().coeffRef(0); }
  /** \returns a reference to the \c y coefficient */
  inline Scalar& y() { return this->derived().coeffs().coeffRef(1); }
  /** \returns a reference to the \c z coefficient */
  inline Scalar& z() { return this->derived().coeffs().coeffRef(2); }
  /** \returns a reference to the \c w coefficient */
  inline Scalar& w() { return this->derived().coeffs().coeffRef(3); }

  /** \returns a read-only vector expression of the imaginary part (x,y,z) */
  inline const VectorBlock<Coefficients,3> vec() const { return coeffs().template head<3>(); }

  /** \returns a vector expression of the imaginary part (x,y,z) */
  inline VectorBlock<Coefficients,3> vec() { return coeffs().template head<3>(); }

  /** \returns a read-only vector expression of the coefficients (x,y,z,w) */
  inline const typename ei_traits<Derived>::Coefficients& coeffs() const { return derived().coeffs(); }

  /** \returns a vector expression of the coefficients (x,y,z,w) */
  inline typename ei_traits<Derived>::Coefficients& coeffs() { return derived().coeffs(); }

  EIGEN_STRONG_INLINE QuaternionBase<Derived>& operator=(const QuaternionBase<Derived>& other);
  template<class OtherDerived> EIGEN_STRONG_INLINE Derived& operator=(const QuaternionBase<OtherDerived>& other);

// disabled this copy operator as it is giving very strange compilation errors when compiling
// test_stdvector with GCC 4.4.2. This looks like a GCC bug though, so feel free to re-enable it if it's
// useful; however notice that we already have the templated operator= above and e.g. in MatrixBase
// we didn't have to add, in addition to templated operator=, such a non-templated copy operator.
//  Derived& operator=(const QuaternionBase& other)
//  { return operator=<Derived>(other); }

  Derived& operator=(const AngleAxisType& aa);
  template<class OtherDerived> Derived& operator=(const MatrixBase<OtherDerived>& m);

  /** \returns a quaternion representing an identity rotation
    * \sa MatrixBase::Identity()
    */
  inline static Quaternion<Scalar> Identity() { return Quaternion<Scalar>(1, 0, 0, 0); }

  /** \sa QuaternionBase::Identity(), MatrixBase::setIdentity()
    */
  inline QuaternionBase& setIdentity() { coeffs() << 0, 0, 0, 1; return *this; }

  /** \returns the squared norm of the quaternion's coefficients
    * \sa QuaternionBase::norm(), MatrixBase::squaredNorm()
    */
  inline Scalar squaredNorm() const { return coeffs().squaredNorm(); }

  /** \returns the norm of the quaternion's coefficients
    * \sa QuaternionBase::squaredNorm(), MatrixBase::norm()
    */
  inline Scalar norm() const { return coeffs().norm(); }

  /** Normalizes the quaternion \c *this
    * \sa normalized(), MatrixBase::normalize() */
  inline void normalize() { coeffs().normalize(); }
  /** \returns a normalized copy of \c *this
    * \sa normalize(), MatrixBase::normalized() */
  inline Quaternion<Scalar> normalized() const { return Quaternion<Scalar>(coeffs().normalized()); }

    /** \returns the dot product of \c *this and \a other
    * Geometrically speaking, the dot product of two unit quaternions
    * corresponds to the cosine of half the angle between the two rotations.
    * \sa angularDistance()
    */
  template<class OtherDerived> inline Scalar dot(const QuaternionBase<OtherDerived>& other) const { return coeffs().dot(other.coeffs()); }

  template<class OtherDerived> Scalar angularDistance(const QuaternionBase<OtherDerived>& other) const;

  /** \returns an equivalent 3x3 rotation matrix */
  Matrix3 toRotationMatrix() const;

  /** \returns the quaternion which transform \a a into \a b through a rotation */
  template<typename Derived1, typename Derived2>
  Derived& setFromTwoVectors(const MatrixBase<Derived1>& a, const MatrixBase<Derived2>& b);

  template<class OtherDerived> EIGEN_STRONG_INLINE Quaternion<Scalar> operator* (const QuaternionBase<OtherDerived>& q) const;
  template<class OtherDerived> EIGEN_STRONG_INLINE Derived& operator*= (const QuaternionBase<OtherDerived>& q);

  /** \returns the quaternion describing the inverse rotation */
  Quaternion<Scalar> inverse() const;

  /** \returns the conjugated quaternion */
  Quaternion<Scalar> conjugate() const;

  /** \returns an interpolation for a constant motion between \a other and \c *this
    * \a t in [0;1]
    * see http://en.wikipedia.org/wiki/Slerp
    */
  template<class OtherDerived> Quaternion<Scalar> slerp(Scalar t, const QuaternionBase<OtherDerived>& other) const;

  /** \returns \c true if \c *this is approximately equal to \a other, within the precision
    * determined by \a prec.
    *
    * \sa MatrixBase::isApprox() */
  template<class OtherDerived>
  bool isApprox(const QuaternionBase<OtherDerived>& other, RealScalar prec = NumTraits<Scalar>::dummy_precision()) const
  { return coeffs().isApprox(other.coeffs(), prec); }

	/** return the result vector of \a v through the rotation*/
  EIGEN_STRONG_INLINE Vector3 _transformVector(Vector3 v) const;

  /** \returns \c *this with scalar type casted to \a NewScalarType
    *
    * Note that if \a NewScalarType is equal to the current scalar type of \c *this
    * then this function smartly returns a const reference to \c *this.
    */
  template<typename NewScalarType>
  inline typename ei_cast_return_type<Derived,Quaternion<NewScalarType> >::type cast() const
  {
    return typename ei_cast_return_type<Derived,Quaternion<NewScalarType> >::type(
      coeffs().template cast<NewScalarType>());
  }
  
#ifdef EIGEN_QUATERNIONBASE_PLUGIN
# include EIGEN_QUATERNIONBASE_PLUGIN
#endif
};

/***************************************************************************
* Definition/implementation of Quaternion<Scalar>
***************************************************************************/

/** \geometry_module \ingroup Geometry_Module
  *
  * \class Quaternion
  *
  * \brief The quaternion class used to represent 3D orientations and rotations
  *
  * \param _Scalar the scalar type, i.e., the type of the coefficients
  *
  * This class represents a quaternion \f$ w+xi+yj+zk \f$ that is a convenient representation of
  * orientations and rotations of objects in three dimensions. Compared to other representations
  * like Euler angles or 3x3 matrices, quatertions offer the following advantages:
  * \li \b compact storage (4 scalars)
  * \li \b efficient to compose (28 flops),
  * \li \b stable spherical interpolation
  *
  * The following two typedefs are provided for convenience:
  * \li \c Quaternionf for \c float
  * \li \c Quaterniond for \c double
  *
  * \sa  class AngleAxis, class Transform
  */

template<typename _Scalar>
struct ei_traits<Quaternion<_Scalar> >
{
  typedef Quaternion<_Scalar> PlainObject;
  typedef _Scalar Scalar;
  typedef Matrix<_Scalar,4,1> Coefficients;
  enum{
    PacketAccess = Aligned
  };
};

template<typename _Scalar>
class Quaternion : public QuaternionBase<Quaternion<_Scalar> >{
  typedef QuaternionBase<Quaternion<_Scalar> > Base;
public:
  typedef _Scalar Scalar;

  EIGEN_INHERIT_ASSIGNMENT_EQUAL_OPERATOR(Quaternion<Scalar>)
  using Base::operator*=;

  typedef typename ei_traits<Quaternion<Scalar> >::Coefficients Coefficients;
  typedef typename Base::AngleAxisType AngleAxisType;

  /** Default constructor leaving the quaternion uninitialized. */
  inline Quaternion() {}

  /** Constructs and initializes the quaternion \f$ w+xi+yj+zk \f$ from
    * its four coefficients \a w, \a x, \a y and \a z.
    *
    * \warning Note the order of the arguments: the real \a w coefficient first,
    * while internally the coefficients are stored in the following order:
    * [\c x, \c y, \c z, \c w]
    */
  inline Quaternion(Scalar w, Scalar x, Scalar y, Scalar z) : m_coeffs(x, y, z, w){}

  /** Constructs and initialize a quaternion from the array data */
  inline Quaternion(const Scalar* data) : m_coeffs(data) {}

  /** Copy constructor */
  template<class Derived> EIGEN_STRONG_INLINE Quaternion(const QuaternionBase<Derived>& other) { this->Base::operator=(other); }

  /** Constructs and initializes a quaternion from the angle-axis \a aa */
  explicit inline Quaternion(const AngleAxisType& aa) { *this = aa; }

  /** Constructs and initializes a quaternion from either:
    *  - a rotation matrix expression,
    *  - a 4D vector expression representing quaternion coefficients.
    */
  template<typename Derived>
  explicit inline Quaternion(const MatrixBase<Derived>& other) { *this = other; }

  inline Coefficients& coeffs() { return m_coeffs;}
  inline const Coefficients& coeffs() const { return m_coeffs;}

protected:
  Coefficients m_coeffs;
};

/** \ingroup Geometry_Module
  * single precision quaternion type */
typedef Quaternion<float> Quaternionf;
/** \ingroup Geometry_Module
  * double precision quaternion type */
typedef Quaternion<double> Quaterniond;

/***************************************************************************
* Specialization of Map<Quaternion<Scalar>>
***************************************************************************/

template<typename _Scalar, int _PacketAccess>
struct ei_traits<Map<Quaternion<_Scalar>, _PacketAccess> >:
ei_traits<Quaternion<_Scalar> >
{
  typedef _Scalar Scalar;
  typedef Map<Matrix<_Scalar,4,1>, _PacketAccess> Coefficients;
  enum {
    PacketAccess = _PacketAccess
  };
};

/** \brief Expression of a quaternion from a memory buffer
  *
  * \param _Scalar the type of the Quaternion coefficients
  * \param PacketAccess see class Map
  *
  * This is a specialization of class Map for Quaternion. This class allows to view
  * a 4 scalar memory buffer as an Eigen's  Quaternion object.
  *
  * \sa class Map, class Quaternion, class QuaternionBase
  */
template<typename _Scalar, int PacketAccess>
class Map<Quaternion<_Scalar>, PacketAccess >
  : public QuaternionBase<Map<Quaternion<_Scalar>, PacketAccess> >
{
    typedef QuaternionBase<Map<Quaternion<_Scalar>, PacketAccess> > Base;

  public:
    typedef _Scalar Scalar;
    typedef typename ei_traits<Map>::Coefficients Coefficients;
    EIGEN_INHERIT_ASSIGNMENT_EQUAL_OPERATOR(Map)
    using Base::operator*=;

    /** Constructs a Mapped Quaternion object from the pointer \a coeffs
      *
      * The pointer \a coeffs must reference the four coeffecients of Quaternion in the following order:
      * \code *coeffs == {x, y, z, w} \endcode
      *
      * If the template parameter PacketAccess is set to Aligned, then the pointer coeffs must be aligned. */
    EIGEN_STRONG_INLINE Map(const Scalar* coeffs) : m_coeffs(coeffs) {}

    inline Coefficients& coeffs() { return m_coeffs;}
    inline const Coefficients& coeffs() const { return m_coeffs;}

  protected:
    Coefficients m_coeffs;
};

/** \ingroup Geometry_Module
  * Map an unaligned array of single precision scalar as a quaternion */
typedef Map<Quaternion<float>, 0>         QuaternionMapf;
/** \ingroup Geometry_Module
  * Map an unaligned array of double precision scalar as a quaternion */
typedef Map<Quaternion<double>, 0>        QuaternionMapd;
/** \ingroup Geometry_Module
  * Map a 16-bits aligned array of double precision scalars as a quaternion */
typedef Map<Quaternion<float>, Aligned>   QuaternionMapAlignedf;
/** \ingroup Geometry_Module
  * Map a 16-bits aligned array of double precision scalars as a quaternion */
typedef Map<Quaternion<double>, Aligned>  QuaternionMapAlignedd;

/***************************************************************************
* Implementation of QuaternionBase methods
***************************************************************************/

// Generic Quaternion * Quaternion product
// This product can be specialized for a given architecture via the Arch template argument.
template<int Arch, class Derived1, class Derived2, typename Scalar, int PacketAccess> struct ei_quat_product
{
  EIGEN_STRONG_INLINE static Quaternion<Scalar> run(const QuaternionBase<Derived1>& a, const QuaternionBase<Derived2>& b){
    return Quaternion<Scalar>
    (
      a.w() * b.w() - a.x() * b.x() - a.y() * b.y() - a.z() * b.z(),
      a.w() * b.x() + a.x() * b.w() + a.y() * b.z() - a.z() * b.y(),
      a.w() * b.y() + a.y() * b.w() + a.z() * b.x() - a.x() * b.z(),
      a.w() * b.z() + a.z() * b.w() + a.x() * b.y() - a.y() * b.x()
    );
  }
};

/** \returns the concatenation of two rotations as a quaternion-quaternion product */
template <class Derived>
template <class OtherDerived>
EIGEN_STRONG_INLINE Quaternion<typename ei_traits<Derived>::Scalar>
QuaternionBase<Derived>::operator* (const QuaternionBase<OtherDerived>& other) const
{
  EIGEN_STATIC_ASSERT((ei_is_same_type<typename Derived::Scalar, typename OtherDerived::Scalar>::ret),
   YOU_MIXED_DIFFERENT_NUMERIC_TYPES__YOU_NEED_TO_USE_THE_CAST_METHOD_OF_MATRIXBASE_TO_CAST_NUMERIC_TYPES_EXPLICITLY)
  return ei_quat_product<Architecture::Target, Derived, OtherDerived,
                         typename ei_traits<Derived>::Scalar,
                         ei_traits<Derived>::PacketAccess && ei_traits<OtherDerived>::PacketAccess>::run(*this, other);
}

/** \sa operator*(Quaternion) */
template <class Derived>
template <class OtherDerived>
EIGEN_STRONG_INLINE Derived& QuaternionBase<Derived>::operator*= (const QuaternionBase<OtherDerived>& other)
{
  derived() = derived() * other.derived();
  return derived();
}

/** Rotation of a vector by a quaternion.
  * \remarks If the quaternion is used to rotate several points (>1)
  * then it is much more efficient to first convert it to a 3x3 Matrix.
  * Comparison of the operation cost for n transformations:
  *   - Quaternion2:    30n
  *   - Via a Matrix3: 24 + 15n
  */
template <class Derived>
EIGEN_STRONG_INLINE typename QuaternionBase<Derived>::Vector3
QuaternionBase<Derived>::_transformVector(Vector3 v) const
{
    // Note that this algorithm comes from the optimization by hand
    // of the conversion to a Matrix followed by a Matrix/Vector product.
    // It appears to be much faster than the common algorithm found
    // in the litterature (30 versus 39 flops). It also requires two
    // Vector3 as temporaries.
    Vector3 uv = Scalar(2) * this->vec().cross(v);
    return v + this->w() * uv + this->vec().cross(uv);
}

template<class Derived>
EIGEN_STRONG_INLINE QuaternionBase<Derived>& QuaternionBase<Derived>::operator=(const QuaternionBase<Derived>& other)
{
  coeffs() = other.coeffs();
  return derived();
}

template<class Derived>
template<class OtherDerived>
EIGEN_STRONG_INLINE Derived& QuaternionBase<Derived>::operator=(const QuaternionBase<OtherDerived>& other)
{
  coeffs() = other.coeffs();
  return derived();
}

/** Set \c *this from an angle-axis \a aa and returns a reference to \c *this
  */
template<class Derived>
EIGEN_STRONG_INLINE Derived& QuaternionBase<Derived>::operator=(const AngleAxisType& aa)
{
  Scalar ha = Scalar(0.5)*aa.angle(); // Scalar(0.5) to suppress precision loss warnings
  this->w() = ei_cos(ha);
  this->vec() = ei_sin(ha) * aa.axis();
  return derived();
}

/** Set \c *this from the expression \a xpr:
  *   - if \a xpr is a 4x1 vector, then \a xpr is assumed to be a quaternion
  *   - if \a xpr is a 3x3 matrix, then \a xpr is assumed to be rotation matrix
  *     and \a xpr is converted to a quaternion
  */

template<class Derived>
template<class MatrixDerived>
inline Derived& QuaternionBase<Derived>::operator=(const MatrixBase<MatrixDerived>& xpr)
{
  EIGEN_STATIC_ASSERT((ei_is_same_type<typename Derived::Scalar, typename MatrixDerived::Scalar>::ret),
   YOU_MIXED_DIFFERENT_NUMERIC_TYPES__YOU_NEED_TO_USE_THE_CAST_METHOD_OF_MATRIXBASE_TO_CAST_NUMERIC_TYPES_EXPLICITLY)
  ei_quaternionbase_assign_impl<MatrixDerived>::run(*this, xpr.derived());
  return derived();
}

/** Convert the quaternion to a 3x3 rotation matrix. The quaternion is required to
  * be normalized, otherwise the result is undefined.
  */
template<class Derived>
inline typename QuaternionBase<Derived>::Matrix3
QuaternionBase<Derived>::toRotationMatrix(void) const
{
  // NOTE if inlined, then gcc 4.2 and 4.4 get rid of the temporary (not gcc 4.3 !!)
  // if not inlined then the cost of the return by value is huge ~ +35%,
  // however, not inlining this function is an order of magnitude slower, so
  // it has to be inlined, and so the return by value is not an issue
  Matrix3 res;

  const Scalar tx  = 2*this->x();
  const Scalar ty  = 2*this->y();
  const Scalar tz  = 2*this->z();
  const Scalar twx = tx*this->w();
  const Scalar twy = ty*this->w();
  const Scalar twz = tz*this->w();
  const Scalar txx = tx*this->x();
  const Scalar txy = ty*this->x();
  const Scalar txz = tz*this->x();
  const Scalar tyy = ty*this->y();
  const Scalar tyz = tz*this->y();
  const Scalar tzz = tz*this->z();

  res.coeffRef(0,0) = 1-(tyy+tzz);
  res.coeffRef(0,1) = txy-twz;
  res.coeffRef(0,2) = txz+twy;
  res.coeffRef(1,0) = txy+twz;
  res.coeffRef(1,1) = 1-(txx+tzz);
  res.coeffRef(1,2) = tyz-twx;
  res.coeffRef(2,0) = txz-twy;
  res.coeffRef(2,1) = tyz+twx;
  res.coeffRef(2,2) = 1-(txx+tyy);

  return res;
}

/** Sets \c *this to be a quaternion representing a rotation between
  * the two arbitrary vectors \a a and \a b. In other words, the built
  * rotation represent a rotation sending the line of direction \a a
  * to the line of direction \a b, both lines passing through the origin.
  *
  * \returns a reference to \c *this.
  *
  * Note that the two input vectors do \b not have to be normalized, and
  * do not need to have the same norm.
  */
template<class Derived>
template<typename Derived1, typename Derived2>
inline Derived& QuaternionBase<Derived>::setFromTwoVectors(const MatrixBase<Derived1>& a, const MatrixBase<Derived2>& b)
{
  Vector3 v0 = a.normalized();
  Vector3 v1 = b.normalized();
  Scalar c = v1.dot(v0);

  // if dot == -1, vectors are nearly opposites
  // => accuraletly compute the rotation axis by computing the
  //    intersection of the two planes. This is done by solving:
  //       x^T v0 = 0
  //       x^T v1 = 0
  //    under the constraint:
  //       ||x|| = 1
  //    which yields a singular value problem
  if (c < Scalar(-1)+NumTraits<Scalar>::dummy_precision())
  {
    c = std::max<Scalar>(c,-1);
    Matrix<Scalar,2,3> m; m << v0.transpose(), v1.transpose();
    JacobiSVD<Matrix<Scalar,2,3> > svd(m);
    Vector3 axis = svd.matrixV().col(2);

    Scalar w2 = (Scalar(1)+c)*Scalar(0.5);
    this->w() = ei_sqrt(w2);
    this->vec() = axis * ei_sqrt(Scalar(1) - w2);
    return derived();
  }
  Vector3 axis = v0.cross(v1);
  Scalar s = ei_sqrt((Scalar(1)+c)*Scalar(2));
  Scalar invs = Scalar(1)/s;
  this->vec() = axis * invs;
  this->w() = s * Scalar(0.5);

  return derived();
}

/** \returns the multiplicative inverse of \c *this
  * Note that in most cases, i.e., if you simply want the opposite rotation,
  * and/or the quaternion is normalized, then it is enough to use the conjugate.
  *
  * \sa QuaternionBase::conjugate()
  */
template <class Derived>
inline Quaternion<typename ei_traits<Derived>::Scalar> QuaternionBase<Derived>::inverse() const
{
  // FIXME should this function be called multiplicativeInverse and conjugate() be called inverse() or opposite()  ??
  Scalar n2 = this->squaredNorm();
  if (n2 > 0)
    return Quaternion<Scalar>(conjugate().coeffs() / n2);
  else
  {
    // return an invalid result to flag the error
    return Quaternion<Scalar>(Coefficients::Zero());
  }
}

/** \returns the conjugate of the \c *this which is equal to the multiplicative inverse
  * if the quaternion is normalized.
  * The conjugate of a quaternion represents the opposite rotation.
  *
  * \sa Quaternion2::inverse()
  */
template <class Derived>
inline Quaternion<typename ei_traits<Derived>::Scalar>
QuaternionBase<Derived>::conjugate() const
{
  return Quaternion<Scalar>(this->w(),-this->x(),-this->y(),-this->z());
}

/** \returns the angle (in radian) between two rotations
  * \sa dot()
  */
template <class Derived>
template <class OtherDerived>
inline typename ei_traits<Derived>::Scalar
QuaternionBase<Derived>::angularDistance(const QuaternionBase<OtherDerived>& other) const
{
  double d = ei_abs(this->dot(other));
  if (d>=1.0)
    return Scalar(0);
  return static_cast<Scalar>(2 * std::acos(d));
}

/** \returns the spherical linear interpolation between the two quaternions
  * \c *this and \a other at the parameter \a t
  */
template <class Derived>
template <class OtherDerived>
Quaternion<typename ei_traits<Derived>::Scalar>
QuaternionBase<Derived>::slerp(Scalar t, const QuaternionBase<OtherDerived>& other) const
{
  static const Scalar one = Scalar(1) - NumTraits<Scalar>::epsilon();
  Scalar d = this->dot(other);
  Scalar absD = ei_abs(d);

  Scalar scale0;
  Scalar scale1;

  if (absD>=one)
  {
    scale0 = Scalar(1) - t;
    scale1 = t;
  }
  else
  {
    // theta is the angle between the 2 quaternions
    Scalar theta = std::acos(absD);
    Scalar sinTheta = ei_sin(theta);

    scale0 = ei_sin( ( Scalar(1) - t ) * theta) / sinTheta;
    scale1 = ei_sin( ( t * theta) ) / sinTheta;
    if (d<0)
      scale1 = -scale1;
  }

  return Quaternion<Scalar>(scale0 * coeffs() + scale1 * other.coeffs());
}

// set from a rotation matrix
template<typename Other>
struct ei_quaternionbase_assign_impl<Other,3,3>
{
  typedef typename Other::Scalar Scalar;
  typedef DenseIndex Index;
  template<class Derived> inline static void run(QuaternionBase<Derived>& q, const Other& mat)
  {
    // This algorithm comes from  "Quaternion Calculus and Fast Animation",
    // Ken Shoemake, 1987 SIGGRAPH course notes
    Scalar t = mat.trace();
    if (t > Scalar(0))
    {
      t = ei_sqrt(t + Scalar(1.0));
      q.w() = Scalar(0.5)*t;
      t = Scalar(0.5)/t;
      q.x() = (mat.coeff(2,1) - mat.coeff(1,2)) * t;
      q.y() = (mat.coeff(0,2) - mat.coeff(2,0)) * t;
      q.z() = (mat.coeff(1,0) - mat.coeff(0,1)) * t;
    }
    else
    {
      DenseIndex i = 0;
      if (mat.coeff(1,1) > mat.coeff(0,0))
        i = 1;
      if (mat.coeff(2,2) > mat.coeff(i,i))
        i = 2;
      DenseIndex j = (i+1)%3;
      DenseIndex k = (j+1)%3;

      t = ei_sqrt(mat.coeff(i,i)-mat.coeff(j,j)-mat.coeff(k,k) + Scalar(1.0));
      q.coeffs().coeffRef(i) = Scalar(0.5) * t;
      t = Scalar(0.5)/t;
      q.w() = (mat.coeff(k,j)-mat.coeff(j,k))*t;
      q.coeffs().coeffRef(j) = (mat.coeff(j,i)+mat.coeff(i,j))*t;
      q.coeffs().coeffRef(k) = (mat.coeff(k,i)+mat.coeff(i,k))*t;
    }
  }
};

// set from a vector of coefficients assumed to be a quaternion
template<typename Other>
struct ei_quaternionbase_assign_impl<Other,4,1>
{
  typedef typename Other::Scalar Scalar;
  template<class Derived> inline static void run(QuaternionBase<Derived>& q, const Other& vec)
  {
    q.coeffs() = vec;
  }
};


#endif // EIGEN_QUATERNION_H
