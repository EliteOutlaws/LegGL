#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <Windows.h>
#include <math.h>
class Vector;
class Matrix;

#define sqr( X )	( (X) * (X) )
#define PI				3.14159265358979323846f
#define DEG2RAD(X)		(PI*(X)/180.0)	
#define RAD2DEG(X)		((X)*180.0/PI)


enum {_XX = 0, _XY, _XZ, _YX, _YY, _YZ, _ZX, _ZY, _ZZ};

class Matrix
{
public:
	float	p[9];

	// constructors
	Matrix()								{ memset(p,0,sizeof(float)*9); }					// nullify matrix
	Matrix(	float xx, float xy, float xz,
			float yx, float yy, float yz,
			float zx, float zy, float zz)	{ Set(xx, xy, xz,  yx, yy, yz,  zx, zy, zz); }		// construct matrix from 9 floats
	Matrix(float ax, float ay, float az)	{ SetAng(ax, ay, az); };							// construct matrix form 3 angles
	Matrix(Matrix &m)						{ memcpy(p, m.p, sizeof(float)*9); }				// construct matrix from another matrix


	// operators
	Matrix operator *(Matrix &m);	// return MatrixMulMatrix(this, m)
	Vector operator *(Vector &v);	// return MatrixMulVector(this, v)
	Matrix operator *(float f);		// return MatrixScale(this, f)
	void operator *=(Matrix &m);	// this = MatrixMulMatrix(this, m)
	void operator *=(float f);		// this = MatrixScale(this, f)

	Matrix operator !();			// return MatrixInverse(this)

	// functions
	void Set(	float xx, float xy, float xz,
				float yx, float yy, float yz,
				float zx, float zy, float zz);	// set teh matrix's values directly
	void SetAng(float ax, float ay, float az);	// specify a xyz rotation matrix
	void Identity();							// set this matrix to identity
	void Invert();								// invert this matrix
};




class Vector
{
public:
	float	x, y, z;

	// constructors
	Vector()								{ Set(0, 0, 0); }
	Vector(float _x, float _y, float _z)	{ Set(_x, _y, _z); }
	Vector(Vector &v)						{ Set(v.x, v.y, v.z); }

	// operators
	Vector operator +(Vector &v);	// return VectorAdd(this, v)
	void operator +=(Vector &v);	// this = VectorAdd(this, v)

	Vector operator -(Vector &v);	// return VectorSub(this, v)
	void operator -=(Vector &v);	// this = VectorSub(this, v)

	Vector operator *(Matrix &m);	// return MatrixMulVector(this, m)
	Vector operator *(Vector &v);	// return VectorMul(this, v)
	Vector operator *(float f);		// return VectorScale(this, f)
	void operator *=(Matrix &m);	// this = MatrixMulVector(this, m)
	void operator *=(Vector &v);	// this = VectorMul(this, v)
	void operator *=(float f);		// this = VectorScale(this, f)

	Vector operator ^(Vector &v);	// return VectorCross(this, v)
	void operator ^=(Vector &v);	// this = VectorCross(this, v)

	Vector operator !();			// return VectorNegate(this)

	// functions
	void	Set(float _x, float _y, float _z);
	float	Dot(Vector &v);
	float	SelfDot();
	Vector	Normalize();
	void	SelfNormalize();
	float	Length();
	float	Distance(Vector &v);
	Vector	Lerp(Vector &v, float f);
	void	SelfLerp(Vector &v, float f);

	float	*GetPtr() { return (float*)this; };
};

#endif 