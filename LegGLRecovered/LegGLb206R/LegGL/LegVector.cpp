#include "LegVector.h"


Vector Vector::operator +(Vector &v)	// return VectorAdd(this, v)
{
	Vector temp(x+v.x, y+v.y, z+v.z);
	return temp;
}
void Vector::operator +=(Vector &v)		// this = VectorAdd(this, v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector Vector::operator -(Vector &v)	// return VectorSub(this, v)
{
	Vector temp(x-v.x, y-v.y, z-v.z);

	return temp;
}
void Vector::operator -=(Vector &v)		// this = VectorSub(this, v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector Vector::operator *(Matrix &m)	// return MatrixMulVector(this, m)
{
	return (m * (*this));
}
Vector Vector::operator *(Vector &v)	// return VectorScale(this, v)
{
	Vector temp(x*v.x, y*v.y, z*v.z);
	return temp;
}
Vector Vector::operator *(float f)		// return VectorScale(this, f)
{
	Vector temp(x*f, y*f, z*f);
	return temp;
}
void Vector::operator *=(Matrix &m)		// this = MatrixMulVector(this, m)
{
	*this = *this * m;
}
void Vector::operator *=(Vector &v)		// this = VectorScale(this, v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}
void Vector::operator *=(float f)		// this = VectorScale(this, v)
{
	x *= f;
	y *= f;
	z *= f;
}

Vector Vector::operator ^(Vector &v)	// return VectorCross(this, v)
{
	Vector temp;

	temp.x = y*v.z - z*v.y;
	temp.y = z*v.x - x*v.z;
	temp.z = x*v.y - y*v.x;

	return temp;
}
void Vector::operator ^=(Vector &v)		// this = VectorCross(this, v)
{
	Set(y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x);
}

Vector Vector::operator !()
{
	Vector temp(-x,-y,-z);
	return temp;
}

void Vector::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float Vector::Dot(Vector &v)
{
	return x*v.x + y*v.y + z*v.z;
}

float Vector::SelfDot()
{
	return x*x + y*y + z*z;
}

Vector Vector::Normalize()
{
	Vector	temp;

	temp = *this * (1.f / sqrtf(SelfDot()));

	return temp;
}


void Vector::SelfNormalize()
{
	*this *= 1.f / sqrtf(SelfDot());
}

float Vector::Length()
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector::Distance(Vector &v)
{
	return (*this - v).Length();
}

Vector Vector::Lerp(Vector &v, float f)
{
	Vector temp;

	temp = (*this * f) + (v * (1.f-f));

	return temp;
}

void Vector::SelfLerp(Vector &v, float f)
{
	*this = (*this * f) + (v * (1.f-f));
}







///////////////

void vmake(Vector *a, float x, float y, float z)
{
	a->x = x;
	a->y = y;
	a->z = z;
}

//---

void vadd(Vector *a, Vector *b, Vector *c)
{
	c->x = a->x+b->x;
	c->y = a->y+b->y;
	c->z = a->z+b->z;
}

void vselfadd(Vector *a, Vector *b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

//---

void vsub(Vector *a, Vector *b, Vector *c)
{
	c->x = a->x-b->x;
	c->y = a->y-b->y;
	c->z = a->z-b->z;
}

void vselfsub(Vector *a, Vector *b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
}

//---

void vmul(Vector *a, Vector *b, Vector *c)
{
	c->x = a->x*b->x;
	c->y = a->y*b->y;
	c->z = a->z*b->z;
}

void vselfmul(Vector *a, Vector *b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
}

//---

void vscale(Vector *a, float d, Vector *c)
{
	c->x = a->x * d;
	c->y = a->y * d;
	c->z = a->z * d;
}

void vselfscale(Vector *a, float d)
{
	a->x *= d;
	a->y *= d;
	a->z *= d;
}

//---

void vneg(Vector *a, Vector *c)
{
	c->x = -a->x;
	c->y = -a->y;
	c->z = -a->z;
}

void vselfneg(Vector *a)
{
	a->x *= -1.f;
	a->y *= -1.f;
	a->z *= -1.f;
}

//---

float vdot(Vector *a, Vector *b)
{
	return( a->x*b->x + a->y*b->y + a->z*b->z );
}

float vselfdot(Vector *a)
{
	return ( sqr(a->x) + sqr(a->y) + sqr(a->z) );
}

//---

void vcross(Vector *a, Vector *b, Vector *c)
{
	c->x = a->y*b->z - a->z*b->y;
	c->y = a->z*b->x - a->x*b->z;
	c->z = a->x*b->y - a->y*b->x;
}

//---

void vnorm(Vector *a)
{
	float Length, C;

	Length = sqr(a->x) + sqr(a->y) + sqr(a->z);

	if (Length != 0.f)
	{
		C = 1.f / (float)sqrt(Length);
		a->x *= C;
		a->y *= C;
		a->z *= C;
	}
}

//---

float vlen(Vector *a)
{
	return (float)sqrt( sqr(a->x) + sqr(a->y) + sqr(a->z) );
}

//---

float vdist(Vector *a, Vector *b)
{
	float x;
	
	x = sqr(b->x-a->x) + sqr(b->y-a->y) + sqr(b->z-a->z);

	if (x != 0.f)
		return (float)sqrt( x );
	else return 0.f;
}

//---

void vselflerp(Vector *a, Vector *b, float f)
{
	float	t;
	
	t = 1.f-f;

	a->x = a->x*f + b->x*t;
	a->y = a->y*f + b->y*t;
	a->z = a->z*f + b->z*t;
}

void vlerp(Vector *a, Vector *b, float f, Vector *c)
{
	float	t;
	
	t = 1.f-f;

	c->x = a->x*f + b->x*t;
	c->y = a->y*f + b->y*t;
	c->z = a->z*f + b->z*t;
}



Matrix Matrix::operator *(Matrix &m)	// return MatrixMulMatrix(this, m)
{
	Matrix temp;

	temp.p[_XX] = p[_XX]*m.p[_XX]+p[_XY]*m.p[_YX]+p[_XZ]*m.p[_ZX];
	temp.p[_YX] = p[_YX]*m.p[_XX]+p[_YY]*m.p[_YX]+p[_YZ]*m.p[_ZX];
	temp.p[_ZX] = p[_ZX]*m.p[_XX]+p[_ZY]*m.p[_YX]+p[_ZZ]*m.p[_ZX];

	temp.p[_XY] = p[_XX]*m.p[_XY]+p[_XY]*m.p[_YY]+p[_XZ]*m.p[_ZY];
	temp.p[_YY] = p[_YX]*m.p[_XY]+p[_YY]*m.p[_YY]+p[_YZ]*m.p[_ZY];
	temp.p[_ZY] = p[_ZX]*m.p[_XY]+p[_ZY]*m.p[_YY]+p[_ZZ]*m.p[_ZY];

	temp.p[_XZ] = p[_XX]*m.p[_XZ]+p[_XY]*m.p[_YZ]+p[_XZ]*m.p[_ZZ];
	temp.p[_YZ] = p[_YX]*m.p[_XZ]+p[_YY]*m.p[_YZ]+p[_YZ]*m.p[_ZZ];
	temp.p[_ZZ] = p[_ZX]*m.p[_XZ]+p[_ZY]*m.p[_YZ]+p[_ZZ]*m.p[_ZZ];

	return temp;
}

Vector Matrix::operator *(Vector &v)	// return MatrixMulVector(this, v)
{
	Vector	temp;

	temp.x = p[_XX] * v.x + p[_XY] * v.y + p[_XZ] * v.z;
	temp.y = p[_YX] * v.x + p[_YY] * v.y + p[_YZ] * v.z;
	temp.z = p[_ZX] * v.x + p[_ZY] * v.y + p[_ZZ] * v.z;

	return temp;
}

Matrix Matrix::operator *(float f)		// return MatrixScale(this, f)
{
	Matrix temp;

	for (int i=0; i<9; i++)
		temp.p[i] = p[i] * f;

	return temp;
}


void Matrix::operator *=(Matrix &m)		// this = MatrixMulMatrix(this, m)
{
	*this = *this * m;
}


void Matrix::operator *=(float f)		// this = MatrixScale(this, f)
{
	*this = *this * f;
}

Matrix Matrix::operator !()				// return MatrixInverse(this)
{
	Matrix temp;

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			temp.p[i+j*3] = p[j+i*3];
		
	return temp;
}




void Matrix::Set(	float xx, float xy, float xz,
					float yx, float yy, float yz,
					float zx, float zy, float zz)
{
	p[_XX] = xx; p[_XY] = xy; p[_XZ] = xz;
	p[_YX] = yx; p[_YY] = yy; p[_YZ] = yz;
	p[_ZX] = zx; p[_ZY] = zy; p[_ZZ] = zz;
}

void Matrix::SetAng(float ax, float ay, float az)
{
	float sinx, siny, sinz, cosx, cosy, cosz, syz, cxz, sxcz;

	sinx = sinf(ax); cosx = cosf(ax);
	siny = sinf(ay); cosy = cosf(ay);
	sinz = sinf(az); cosz = cosf(az);
	
	syz = siny*sinz;
	cxz = cosx*cosz;
	sxcz = sinx*cosz;

	p[_XX] = sinx * syz + cxz;
	p[_XY] = cosy * sinz;
	p[_XZ] = sxcz - cosx * syz;
	p[_YX] = sxcz * siny - cosx * sinz;
	p[_YY] = cosy * cosz;
	p[_YZ] = -cxz * siny - sinx * sinz;
	p[_ZX] = -sinx * cosy;
	p[_ZY] = siny;
	p[_ZZ] = cosx * cosy;
}

void Matrix::Identity()
{
	p[_XX] = 1.f; p[_XY] = 0;   p[_XZ] = 0;
	p[_YX] = 0;   p[_YY] = 1.f; p[_YZ] = 0;
	p[_ZX] = 0;   p[_ZY] = 0;   p[_ZZ] = 1.f;
}

void Matrix::Invert()
{
	*this = !(*this);
}