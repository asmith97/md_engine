#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <string>
#include <sstream>
#include "globalDefs.h"
#include "cutils_math.h"

#define EPSILON .000001f

void export_Vector();
void export_VectorInt();

/*! \class VectorGeneric
 * \brief A three-element vector
 *
 * \tparam T Type of data stored in the vector.
 * \tparam K I have no idea.
 *
 * This class defines a simple three-element vector and the corresponding
 * vector operations.
 *
 * \todo Remove usage of typename K. Is it even used? And if yes, what does
 *       it represent? This is very confusing.
 */
//theme for these operations is that if we're operating on unlike types, switch
//to num representation.  would like better way to do this
template <typename T>
class VectorGeneric {
	T vals[3]; //!< Array storing the values
public:

    /*! \brief Default constructor */
    VectorGeneric<T> () {
        vals[0] = vals[1] = vals[2] = 0;
    }

    // if using literals, have to cast each as int or num so compiler can
    // distinguish between these two constructors
    /*! \brief Constructor
     *
     * \param x First element as double
     * \param y Second element as double
     * \param z Third element as double
     *
     * \todo It would be much more sensible to have
     *       VectorGeneric<T, K> (T x, T y, T z)
     */
    VectorGeneric<T> (T x, T y, T z) {
        vals[0] = x;
        vals[1] = y;
        vals[2] = z;
    }

    /*! \brief Constructor from pointer
     *
     * \param vals_ Pointer to three element int array
     *
     * \todo The [3] specifier does not do anything. It is still just a
     *       pointer being passed to the function.
     */
    VectorGeneric<T> (T *vals_) {
        for (int i=0; i<3; i++) {
            vals[i] = (T) vals_[i];
        }
    }

    /*! \brief Constructor from float3
     *
     * \param other Float3 to use as values for the vector
     */
    VectorGeneric<T> (float3 other) {
        vals[0] = other.x;
        vals[1] = other.y;
        vals[2] = other.z;
    }

    /*! \brief Constructor from float4
     *
     * \param other Float4 to use as values for the vector
     *
     * The forth value in the float4 will be discarded.
     */
    VectorGeneric<T> (float4 other) {
        vals[0] = other.x;
        vals[1] = other.y;
        vals[2] = other.z;
    }

    /*! \brief Convert vector to float4
     *
     * The first three entries correspond to the vector elements, the forth
     * entry will be set to zero.
     */
    float4 asFloat4() {
        return make_float4(vals[0], vals[1], vals[2], 0);
    }

    /*! \brief Convert vector to int4
     *
     * The first three entries correspond to the vector elements, the forth
     * entry will be set to zero.
     */
    int4 asInt4() {
        return make_int4(vals[0], vals[1], vals[2], 0);
    }

    /*! \brief Convert vector to float3 */
    float3 asFloat3() {
        return make_float3(vals[0], vals[1], vals[2]);
    }

    /*! \brief Convert vector to int3 */
    int3 asInt3() {
        return make_int3(vals[0], vals[1], vals[2]);
    }

    /*! \brief Set all vector elements to zero */
    void zero() {
        vals[0] = vals[1] = vals[2] = 0;
    }

    //void skew(num cao, num sao, num cbo, num sbo, num caf, num saf, num cbf, num sbf) { // c->cos, s->sin, a->alphe, b->beta, o->orig, f->final
    //    num x = vals[0]; //See mathematica notebook for this math.  Basically untransforming to no rotation, then transforming to new coords
    //    num y = vals[1];
    //    num denom = cao*cbo - sao*sbo;
    //    vals[0] = ((cao*y-x*sao)*sbf + caf*(cbo*x-y*sbo)) / denom;
    //    vals[1] = (cbo*x*saf + cbf * (cao*y - x*sao) - y*saf*sbo) / denom;
    //}
    //void skew(num ao, num bo, num af, num bf) {
    //    skew(cos(ao), sin(ao), cos(bo), sin(bo), cos(af), sin(af), cos(bf), sin(bf));
    //}
    //void skewPy(num ao, num bo, num af, num bf) {
    //    skew(ao, bo, af, bf);
    //}
    //void skewFromZero(num ca, num sa, num cb, num sb) {
    //    num xo = vals[0];
    //    num yo = vals[1];
    //    vals[0] = xo*ca + yo*sb;
    //    vals[1] = xo*sa + yo*cb;
    //}
    //void unskew(num ca, num sa, num cb, num sb) {
    //    num denom = ca*cb - sa*sb;
    //    num xo = vals[0];
    //    num yo = vals[1];
    //    vals[0] = (cb*xo - sb*yo) / denom;
    //    vals[1] = (ca*yo - sa*xo) / denom;
    //}

    /*! \brief Vector connecting two points
     *
     * \param v Other point
     * \returns A new vector connecting the points defined by this vector and
     *          vector v
     *
     * This function gives the vector from the point specified by vector v to
     * the point this vector is pointing to.
     *
     * \todo This is essentially (v - *this). Do we really need this function?
     */
    template<typename U>
    auto VTo(const VectorGeneric<U> &v) -> VectorGeneric< decltype(v[0]-vals[0]) > {
        return VectorGeneric< decltype(v[0] - vals[0]) >(
                            v[0] - vals[0], v[1] - vals[1], v[2] - vals[2]);
    }

    /*! \brief Sum of all entries */
    T sum() {
        return vals[0] + vals[1] + vals[2];
    }

    /*! \brief Product of all entries */
    T prod() {
        return vals[0] * vals[1] * vals[2];
    }

    /*! \brief Operator accessing vector elements */
    T &operator[]( int n ){
        return vals[n];
    }

    /*! \brief Const operator accessing vector elements
     *
     * \todo The const int is not necessary. Returning const T & would be more
     *       sensible (It isn't much more efficient, but more the typical
     *       structure of access operators).
     */
    T operator[]( const int n )const{
        return vals[n];
    }

    /*! \brief Convert all elements to their absolute value
     *
     * \returns A new vector with the transformed elements.
     */
    VectorGeneric<T> abs() {
        return VectorGeneric<T>(std::abs(vals[0]), std::abs(vals[1]), std::abs(vals[2]));
    }

    /*! \brief Unary minus operator */
    VectorGeneric<T> operator-()const{
        return VectorGeneric<T>(-vals[0], -vals[1], -vals[2]);
    }

    /*! \brief Multiplication with generic type */
    template<typename U>
    auto operator*( U scale ) const -> VectorGeneric< decltype(vals[0]*scale) > {
        return VectorGeneric< decltype(vals[0]*scale) >( vals[0]*scale,vals[1]*scale,vals[2]*scale );
    }

    /*! \brief Multiplication with other vector operator */
    template<typename U>
    auto operator*( const VectorGeneric<U> &q ) const -> VectorGeneric< decltype(vals[0]*q[0]) > {
        return VectorGeneric< decltype(vals[0]*q[0]) >( vals[0]*q[0],vals[1]*q[1],vals[2]*q[2]);
    }

    /*! \brief Division with int operator */
    template<typename U>
    auto operator/( U scale ) const -> VectorGeneric< decltype(vals[0]/scale) > {
        return VectorGeneric< decltype(vals[0]/scale) >( vals[0]/scale,vals[1]/scale,vals[2]/scale );
    }

    /*! \brief rotation in x-y plane
     *
     * \param rotation Rotation angle
     *
     * The z-component of the vector remains unchanged.
     */
    VectorGeneric<num> rotate2d( num rotation) const {
        num c = cos(rotation);
        num s = sin(rotation);
        return VectorGeneric<num> (c*vals[0] - s*vals[1], s*vals[0] + c*vals[1], vals[2]);
    }

    /*! \brief Element-wise division */
    template<typename U>
    auto operator/( const VectorGeneric<U> &q ) const -> VectorGeneric< decltype(vals[0]/q[0]) > {
        return VectorGeneric< decltype(vals[0]/q[0]) >( vals[0]/q[0],vals[1]/q[1],vals[2]/q[2] );
    }

    /*! \brief Addition of two vectors */
    template<typename U>
    auto operator+( const VectorGeneric<U> &q ) const -> VectorGeneric< decltype(vals[0]+q[0]) > {
        return VectorGeneric< decltype(vals[0]+q[0]) >( vals[0]+q[0],vals[1]+q[1],vals[2]+q[2] );
    }

    /*! \brief Subtraction of two vectors */
    template<typename U>
    auto operator-( const VectorGeneric<U> &q ) const -> VectorGeneric< decltype(vals[0]-q[0]) > {
        return VectorGeneric< decltype(vals[0]-q[0]) >( vals[0]-q[0],vals[1]-q[1],vals[2]-q[2] );
    }

    /*! \brief Multiplication-assignment operator with int */
    template<typename U>
    VectorGeneric<T> &operator*=( U scale ){
        vals[0]*=scale;vals[1]*=scale;vals[2]*=scale;return *this; // *=, /=, etc won't promote types like binary operations
    }

    /*! \brief Multiplication-assignment operator with other vector
     *
     * Performs element-wise multiplication.
     */
    template<typename U>
    VectorGeneric<T> &operator*=( const VectorGeneric<U> &q ){
        vals[0]*=q[0];vals[1]*=q[1];vals[2]*=q[2];return *this;
    }

    /*! \brief Division-assignment operator with int */
    template<typename U>
    VectorGeneric<T> &operator/=( U scale ){
        vals[0]/=scale;vals[1]/=scale;vals[2]/=scale;return *this;
    }

    /*! \brief Division-assignment operator with other vector
     *
     * Performs element-wise division.
     */
    template<typename U>
    VectorGeneric<T> &operator/=( const VectorGeneric<U> &q ){
        vals[0]/=q[0];vals[1]/=q[1];vals[2]/=q[2];return *this;
    }

    /*! \brief Addition-assignment operator */
    template<typename U>
    VectorGeneric<T> &operator+=( const VectorGeneric<U> &q ){
        vals[0]+=q[0];vals[1]+=q[1];vals[2]+=q[2];return *this;
    }

    /*! \brief Subtraction-assigment operator */
    template<typename U>
    VectorGeneric<T> &operator-=( const VectorGeneric<U> &q ){
        vals[0]-=q[0];vals[1]-=q[1];vals[2]-=q[2];return *this;
    }

    /*! \brief Smaller than comparison operator */
    template<typename U>
    bool operator<( const VectorGeneric<U> &q )const{
        if( std::abs(vals[0]-q[0])>EPSILON ) return vals[0]<q[0] ? true : false;
        if( std::abs(vals[1]-q[1])>EPSILON ) return vals[1]<q[1] ? true : false;
        return std::abs(vals[2]-q[2])>EPSILON && vals[2]<q[2];
    }

    /*! \brief Larger than comparison operator */
    template<typename U>
    bool operator>( const VectorGeneric<U> &q )const{
        if( std::abs(vals[0]-q[0])>EPSILON ) return vals[0]>q[0] ? true : false;
        if( std::abs(vals[1]-q[1])>EPSILON ) return vals[1]>q[1] ? true : false;
        return std::abs(vals[2]-q[2])>EPSILON && vals[2]>q[2];
    }

    /*! \brief Equality comparison operator */
    template<typename U>
    bool operator==( const VectorGeneric<U> &q )const{
        return std::abs(vals[0]-q[0])<=EPSILON &&
               std::abs(vals[1]-q[1])<=EPSILON &&
               std::abs(vals[2]-q[2])<=EPSILON;
    }

    /*! \brief Non-equal comparison operator */
    template<typename U>
    bool operator!=( const VectorGeneric<U> &q )const{
        return fabs(vals[0]-q[0])>EPSILON || fabs(vals[1]-q[1])>EPSILON || fabs(vals[2]-q[2])>EPSILON;
    }

    /*! \brief Dot product with another vector */
    template<typename U>
    auto dot( const VectorGeneric<U> &q ) const -> decltype(vals[0]*q[0]+vals[1]*q[1]) {
        return vals[0]*q[0]+vals[1]*q[1]+vals[2]*q[2];
    }

    /*! \brief Cross product with another vector */
    template<typename U>
    auto cross( const VectorGeneric<U> &q ) const -> VectorGeneric< decltype(vals[1]*q[2] - vals[2]*q[1]) > {
        return VectorGeneric< decltype(vals[1]*q[2] - vals[2]*q[1]) >( vals[1]*q[2]-vals[2]*q[1],vals[2]*q[0]-vals[0]*q[2],vals[0]*q[1]-vals[1]*q[0] );
    }

    /*! \brief Length of vector */
    auto len() const -> decltype(std::sqrt(vals[0]*vals[0]+vals[1]*vals[1])) {
        return std::sqrt(vals[0]*vals[0]+vals[1]*vals[1]+vals[2]*vals[2]);
    }

    /*! \brief Squared length of vector */
    auto lenSqr() const -> decltype(vals[0]*vals[0]+vals[1]*vals[1]) {
        return vals[0]*vals[0]+vals[1]*vals[1]+vals[2]*vals[2];
    }

    /*! \brief Distance between two points
     *
     * The points are specified by this and by the q vector.
     */
    template<typename U>
    auto dist( const VectorGeneric<U> &q ) const -> decltype(std::sqrt((vals[0]-q[0])*(vals[0]-q[0]))) {
        auto dx=vals[0]-q[0];
        auto dy=vals[1]-q[1];
        auto dz=vals[2]-q[2];
        return std::sqrt(dx*dx+dy*dy+dz*dz);
    }

    /*! \brief Squared distance between two points */
    template<typename U>
    auto distSqr( const VectorGeneric<U> &q) -> decltype((vals[0]-q[0])*(vals[0]-q[0])) {
        auto dx=vals[0]-q[0];
        auto dy=vals[1]-q[1];
        auto dz=vals[2]-q[2];
        return dx*dx+dy*dy+dz*dz;
    }

    /*! \brief Return normalized form of this vector */
    auto normalized() const -> VectorGeneric< decltype(vals[0]/std::sqrt(vals[0])) > {
        auto l=len();
        return VectorGeneric< decltype(vals[0]/l) >( vals[0]/l,vals[1]/l,vals[2]/l );
    }

    /*! \brief Normalize this vector */
    void normalize(){
        auto l=len();vals[0]/=l;vals[1]/=l;vals[2]/=l; //will not necessarily normalize int vectors
    }

    /*! \brief Mirror vector along y direction */
    VectorGeneric<T> perp2d() {
        return VectorGeneric<T>(vals[1], -vals[0], vals[2]);
    }

    /*! \brief Convert vector to string for output */
    std::string asStr() const {
        std::ostringstream oss;
        oss << "x: " << vals[0] << ", y: " << vals[1] << ", z: " << vals[2];
        return oss.str();
    }

    /*! \brief Assignment operator */
    template<typename U>
    VectorGeneric<T> &operator=(const VectorGeneric<U> &other) {
        for (int i=0; i<3; i++) {
            vals[i] = other[i];
        }
        return *this;
    }

    /*! \brief Assignment operator
     *
     * \todo This function is already implicitly defined via the float3
     *       constructor
     */
    VectorGeneric<T> &operator=(const float3 &other) {
        vals[0] = other.x;
        vals[1] = other.y;
        vals[2] = other.z;
        return *this;
    }

    /*! \brief Assignment operator
     *
     * \todo This function is already implicitly defined via the float4
     *       constructor
     */
    VectorGeneric<T> &operator=(const float4 &other) {
        vals[0] = other.x;
        vals[1] = other.y;
        vals[2] = other.z;
        return *this;
    }

    /*! \brief Copy constructor */
    template<typename U>
    VectorGeneric<T> (const VectorGeneric<U> &other) {
        for (int i=0; i<3; i++) {
            vals[i] = other[i];
        }
    }

    /*! \brief Get a specific element */
    T get(int i) {
        return vals[i];
    }

    /*! \brief Set a specific element */
    void set(int i, T val) {
        vals[i] = val;
    }

    /*! \brief I have no idea
     *
     * \todo Someone explain to me what this function does? And write this
     *       documentation please.
     */
    VectorGeneric<T> loopedVTo(const VectorGeneric<T> &other, const VectorGeneric<T> &trace) {
        VectorGeneric<T> dist = other - *this;
        VectorGeneric<T> halfTrace = trace/ (T) 2.0;
        for (int i=0; i<3; i++) {
            if (dist[i] > halfTrace[i]) {
                dist[i] -= trace[i];
            } else if (dist[i] < -halfTrace[i]) {
                dist[i] += trace[i];
            }
        }
        return dist;

    }
};

typedef VectorGeneric<num> Vector;
typedef VectorGeneric<int> VectorInt;

std::ostream &operator<<(std::ostream &os, const Vector &v);
std::ostream &operator<<(std::ostream &os, const float4 &v);

#endif
