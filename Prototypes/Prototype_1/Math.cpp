#include "Math.hpp"
#include <memory.h>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------//
// MATH
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
float Wrap(float _x, float _l, float _u)
{
	float _y = _u - _l;
	_x -= _u * floorf(_x / _u);
	if (_x < _l)
		_x = _l - _x;
	return _x;
}
//----------------------------------------------------------------------------//
uint32_t FirstPow2(uint32_t _val)
{
	--_val |= _val >> 16;
	_val |= _val >> 8;
	_val |= _val >> 4;
	_val |= _val >> 2;
	_val |= _val >> 1;
	return ++_val;
}
//----------------------------------------------------------------------------//
uint16_t FloatToHalf(float _value)
{
	union { float f; uint32_t i; }_fb = { _value };
#if 0
	return (uint16_t)((_fb.i >> 16) & 0x8000) | ((((_fb.i & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | ((_fb.i >> 13) & 0x03ff);
#else
	uint32_t _s = (_fb.i >> 16) & 0x00008000; // sign
	int32_t _e = ((_fb.i >> 23) & 0x000000ff) - 0x00000070; // exponent
	uint32_t _r = _fb.i & 0x007fffff; // mantissa
	if (_e < 1)
	{
		if (_e < -10)
			return 0;
		_r = (_r | 0x00800000) >> (14 - _e);
		return (uint16_t)(_s | _r);
	}
	else if (_e == 0x00000071)
	{
		if (_r == 0)
			return (uint16_t)(_s | 0x7c00); // Inf
		else
			return (uint16_t)(((_s | 0x7c00) | (_r >>= 13)) | (_r == 0)); // NAN
	}
	if (_e > 30)
		return (uint16_t)(_s | 0x7c00); // Overflow
	return (uint16_t)((_s | (_e << 10)) | (_r >> 13));
#endif
}
//----------------------------------------------------------------------------//
float HalfToFloat(uint16_t _value)
{
	union { uint32_t i; float f; }_fb;
#if 0
	_fb.i = ((_value & 0x8000) << 16) | (((_value & 0x7c00) + 0x1C000) << 13) | ((_value & 0x03FF) << 13);
#else
	int32_t _s = (_value >> 15) & 0x00000001; // sign
	int32_t _e = (_value >> 10) & 0x0000001f; // exponent
	int32_t _r = _value & 0x000003ff; // mantissa
	if (_e == 0)
	{
		if (_r == 0) // Plus or minus zero
		{
			_fb.i = _s << 31;
			return _fb.f;
		}
		else // Denormalized number -- renormalize it
		{
			for (; !(_r & 0x00000400); _r <<= 1, _e -= 1);
			_e += 1;
			_r &= ~0x00000400;
		}
	}
	else if (_e == 31)
	{
		if (_r == 0) // Inf
		{
			_fb.i = (_s << 31) | 0x7f800000;
			return _fb.f;
		}
		else // NaN
		{
			_fb.i = ((_s << 31) | 0x7f800000) | (_r << 13);
			return _fb.f;
		}
	}
	_e = (_e + 112) << 23;
	_r = _r << 13;
	_fb.i = ((_s << 31) | _e) | _r;
#endif
	return _fb.f;
}
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// Vector2
//----------------------------------------------------------------------------//

const Vector2 Vector2::Zero(0);
const Vector2 Vector2::One(1);

//----------------------------------------------------------------------------//
// IntVector2
//----------------------------------------------------------------------------//

const IntVector2 IntVector2::Zero(0);
const IntVector2 IntVector2::One(1);

//----------------------------------------------------------------------------//
// Vector3
//----------------------------------------------------------------------------//

const Vector3 Vector3::Zero(0);
const Vector3 Vector3::One(1);
const Vector3 Vector3::UnitX(1, 0, 0);
const Vector3 Vector3::UnitY(0, 1, 0);
const Vector3 Vector3::UnitZ(0, 0, 1);

//----------------------------------------------------------------------------//
// Vector4
//----------------------------------------------------------------------------//

const Vector4 Vector4::Zero(0);
const Vector4 Vector4::One(1);
const Vector4 Vector4::Identity(0, 0, 0, 1);

//----------------------------------------------------------------------------//
// Quaternion
//----------------------------------------------------------------------------//

const Quaternion Quaternion::Zero(0, 0, 0, 0);
const Quaternion Quaternion::Identity(0, 0, 0, 1);

//----------------------------------------------------------------------------//
Quaternion& Quaternion::Mul(const Quaternion& _rhs)
{
	return Set(
		w * _rhs.x + x * _rhs.w + y * _rhs.z - z * _rhs.y,
		w * _rhs.y + y * _rhs.w + z * _rhs.x - x * _rhs.z,
		w * _rhs.z + z * _rhs.w + x * _rhs.y - y * _rhs.x,
		w * _rhs.w - x * _rhs.x - y * _rhs.y - z * _rhs.z
	);
}
//----------------------------------------------------------------------------//
Vector3 Quaternion::Transform(const Vector3& _v) const
{
	const Vector3& _q = *(const Vector3*)(&x);
	Vector3 _uv(_q.Cross(_v));
	return _v + _uv * 2 * w + _q.Cross(_uv) * 2;
}
//----------------------------------------------------------------------------//
Quaternion& Quaternion::Normalize(void)
{
	float _l = x * x + y * y + z * z + w * w;
	if (_l > EPSILON2)
		*this *= RSqrt(_l);
	return *this;
}
//----------------------------------------------------------------------------//
Quaternion& Quaternion::Inverse(void)
{
	float _d = Dot(*this);
	if (_d > 0)
		x *= -_d, y *= -_d, z *= -_d, w *= _d;
	else
		x = 0, y = 0, z = 0, w = 1;
	return *this;
}
//----------------------------------------------------------------------------//
Quaternion Quaternion::Nlerp(const Quaternion& _q, float _t, bool _shortestPath) const
{
	const Quaternion& _p = *this;
	float _c = _p.Dot(_q);
	Quaternion _result;
	if (_c < 0 && _shortestPath)
		_result = _p + _t * ((-_q) - _p);
	else
		_result = _p + _t * (_q - _p);
	return _result.Normalize();
}
//----------------------------------------------------------------------------//
Quaternion Quaternion::Slerp(const Quaternion& _q, float _t, bool _shortestPath) const
{
	const Quaternion& _p = *this;
	float _c = _p.Dot(_q);
	Quaternion _tmp;
	if (_c < 0 && _shortestPath)
		_c = -_c, _tmp = -_q;
	else
		_tmp = _q;
	if (Abs(_c) < 1 - EPSILON)
	{
		float _s = Sqrt(1 - _c * _c);
		float _angle = ATan2(_s, _c);
		float _invs = 1 / _s;
		float _coeff0 = Sin((1 - _t) * _angle) * _invs;
		float _coeff1 = Sin(_t * _angle) * _invs;
		return _coeff0 * _p + _coeff1 * _tmp;
	}
	return Quaternion((1 - _t) * _p + _t * _tmp).Normalize();
}
//----------------------------------------------------------------------------//
void Quaternion::ToRotationMatrix(float* _r0, float* _r1, float* _r2) const
{
	float _x2 = x + x;
	float _y2 = y + y;
	float _z2 = z + z;
	float _wx = _x2 * w;
	float _wy = _y2 * w;
	float _wz = _z2 * w;
	float _xx = _x2 * x;
	float _xy = _y2 * x;
	float _xz = _z2 * x;
	float _yy = _y2 * y;
	float _yz = _z2 * y;
	float _zz = _z2 * z;
	_r0[0] = 1 - (_yy + _zz);
	_r0[1] = _xy + _wz;
	_r0[2] = _xz - _wy;
	_r1[0] = _xy - _wz;
	_r1[1] = 1 - (_xx + _zz);
	_r1[2] = _yz + _wx;
	_r2[0] = _xz + _wy;
	_r2[1] = _yz - _wx;
	_r2[2] = 1 - (_xx + _yy);
}
//----------------------------------------------------------------------------//
Quaternion& Quaternion::FromRotationMatrix(const float* _r0, const float* _r1, const float* _r2)
{
	int _r;
	float _root = _r0[0] + _r1[1] + _r2[2];
	if (_root > 0)
	{
		_r = 3;
		_root = Sqrt(_root + 1);
		//x = _r1[2] - _r2[1];
		//y = _r2[0] - _r0[2];
		//z = _r0[1] - _r1[0];
		x = _r2[1] - _r1[2];
		y = _r0[2] - _r2[0];
		z = _r1[0] - _r0[1];
	}
	else if (_r0[0] > _r1[1] && _r0[0] > _r2[2])
	{
		_r = 0;
		//_root = Sqrt(_r0[0] - _r1[1] - _r2[2] + 1);
		_root = Sqrt(_r0[0] - _r1[1] - _r2[2] + 1);
		y = _r0[1] + _r1[0];
		z = _r0[2] + _r2[0];
		//w = _r1[2] - _r2[1];
		w = _r2[1] - _r1[2];
	}
	else if (_r1[1] > _r0[0] && _r1[1] > _r2[2])
	{
		_r = 1;
		//_root = Sqrt(_r1[1] - _r2[2] - _r0[0] + 1);
		_root = Sqrt(_r1[1] - _r0[0] - _r2[2] + 1);
		x = _r1[0] + _r0[1];
		z = _r1[2] + _r2[1];
		//w = _r2[0] - _r0[2];
		w = _r0[2] - _r2[0];
	}
	else
	{
		_r = 2;
		_root = Sqrt(_r2[2] - _r0[0] - _r1[1] + 1);
		x = _r2[0] + _r0[2];
		y = _r2[1] + _r1[2];
		//w = _r0[1] - _r1[0];
		w = _r1[0] - _r0[1];
	}

	float _invr = 0.5f / _root;
	for (int i = 0; i < 4; ++i)
		(&x)[i] *= _invr;
	(&x)[_r] = _root * 0.5f;

	return *this;
}
//----------------------------------------------------------------------------//
Quaternion& Quaternion::FromAxisAngle(const Vector3& _axis, float _angle)
{
	float _s, _c;
	SinCos(_angle * .5f, _s, _c);
	return Set(_axis.x * _s, _axis.y * _s, _axis.z * _s, _c);
}
//----------------------------------------------------------------------------//
Quaternion& Quaternion::FromLookRotation(const Vector3& _dir, const Vector3& _up)
{
	Vector3 _z = _dir.Copy().Normalize();
	Vector3 _y = _z.Cross(_up).Copy().Normalize().Cross(_z);
	Vector3 _x = _y.Cross(_z);
	return FromRotationMatrix(*_x, *_y, *_z);
}
//----------------------------------------------------------------------------//
Quaternion& Quaternion::FromRotationTo(const Vector3& _start, const Vector3& _end)
{
	Vector3 _ns = _start.Copy().Normalize();
	Vector3 _ne = _end.Copy().Normalize();
	float d = _ns.Dot(_ne);

	if (d > -1 + EPSILON)
	{
		Vector3 _c = _ns.Cross(_ne);
		float _s = Sqrt((1 + d) * 2);
		float _invS = 1 / _s;
		x = _c.x * _invS;
		y = _c.y * _invS;
		z = _c.z * _invS;
		w = 0.5f * _s;
	}
	else
	{
		Vector3 _axis = Vector3::UnitX.Cross(_ns);
		if (_axis.LengthSq() < EPSILON2)
			_axis = Vector3::UnitY.Cross(_ns);

		FromAxisAngle(_axis, HALF_PI);
	}
	return *this;
}
//----------------------------------------------------------------------------//
float Quaternion::Yaw(void) const
{
	return ASin(-2 * (x * z - w * y));
	//return ATan2(2 * (y*z + w*x), w*w - x*x - y*y + z*z);
}
//----------------------------------------------------------------------------//
float Quaternion::Pitch(void) const
{
	return ATan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z);
	//return ASin(-2 * (x*z - w*y));
}
//----------------------------------------------------------------------------//
float Quaternion::Roll(void) const
{
	return ATan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z);
}
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
// Matrix44
//----------------------------------------------------------------------------//

const Matrix44 Matrix44::Zero(0.f);
const Matrix44 Matrix44::Identity(1);
const Matrix44 Matrix44::Flip =
{
	1, 0, 0, 0,
	0, -1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

//----------------------------------------------------------------------------//
Matrix44::Matrix44(float _00, float _01, float _02, float _03,
	float _10, float _11, float _12, float _13,
	float _20, float _21, float _22, float _23,
	float _30, float _31, float _32, float _33) :
	m00(_00), m01(_01), m02(_02), m03(_03),
	m10(_10), m11(_11), m12(_12), m13(_13),
	m20(_20), m21(_21), m22(_22), m23(_23),
	m30(_30), m31(_31), m32(_32), m33(_33)
{
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::FromPtr(const float* _m44)
{
	memcpy(v, _m44, sizeof(float) * 16);
	return *this;
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::Set(float _00, float _01, float _02, float _03,
	float _10, float _11, float _12, float _13,
	float _20, float _21, float _22, float _23,
	float _30, float _31, float _32, float _33)
{
	m00 = _00, m01 = _01, m02 = _02, m03 = _03;
	m10 = _10, m11 = _11, m12 = _12, m13 = _13;
	m20 = _20, m21 = _21, m22 = _22, m23 = _23;
	m30 = _30, m31 = _31, m32 = _32, m33 = _33;
	return *this;
}
//----------------------------------------------------------------------------//
Vector3 Matrix44::Transform(const Vector3& _v) const
{
	return Vector3(
		m00 * _v.x + m01 * _v.y + m02 * _v.z + m03,
		m10 * _v.x + m11 * _v.y + m12 * _v.z + m13,
		m20 * _v.x + m21 * _v.y + m22 * _v.z + m23);
}
//----------------------------------------------------------------------------//
Vector3 Matrix44::TransformVectorAbs(const Vector3& _v) const
{
	return Vector3(
		Abs(m00) * _v.x + Abs(m01) * _v.y + Abs(m02) * _v.z,
		Abs(m10) * _v.x + Abs(m11) * _v.y + Abs(m12) * _v.z,
		Abs(m20) * _v.x + Abs(m21) * _v.y + Abs(m22) * _v.z);
}
//----------------------------------------------------------------------------//
Vector3 Matrix44::TransformVector(const Vector3& _v) const
{
	return Vector3(
		m00 * _v.x + m01 * _v.y + m02 * _v.z,
		m10 * _v.x + m11 * _v.y + m12 * _v.z,
		m20 * _v.x + m21 * _v.y + m22 * _v.z);
}
//----------------------------------------------------------------------------//
Vector3 Matrix44::TransformProj(const Vector3& _v) const
{
	float _iw = 1 / (m30 * _v.x + m31 * _v.y + m32 * _v.z + m33);
	return Vector3(
		(m00 * _v.x + m01 * _v.y + m02 * _v.z + m03) * _iw,
		(m10 * _v.x + m11 * _v.y + m12 * _v.z + m13) * _iw,
		(m20 * _v.x + m21 * _v.y + m22 * _v.z + m23) * _iw);
}
//----------------------------------------------------------------------------//
Vector4 Matrix44::Transform(const Vector4& _v) const
{
	return Vector4(
		m00 * _v.x + m01 * _v.y + m02 * _v.z + m03 * _v.w,
		m10 * _v.x + m11 * _v.y + m12 * _v.z + m13 * _v.w,
		m20 * _v.x + m21 * _v.y + m22 * _v.z + m23 * _v.w,
		m30 * _v.x + m31 * _v.y + m32 * _v.z + m33 * _v.w);
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::Mul(const Matrix44& _rhs)
{
	Matrix44 _r;
	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 4; ++c)
			_r[r][c] = m[r][0] * _rhs[0][c] + m[r][1] * _rhs[1][c] + m[r][2] * _rhs[2][c] + m[r][3] * _rhs[3][c];
	}
	return *this = _r;
}
//----------------------------------------------------------------------------//
Vector3 Matrix44::GetScale(void) const
{
	return Vector3(
		Sqrt(m00 * m00 + m10 * m10 + m20 * m20),
		Sqrt(m01 * m01 + m11 * m11 + m21 * m21),
		Sqrt(m02 * m02 + m12 * m12 + m22 * m22));
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::CreateScale(const Vector3& _s)
{
	SetZero();
	m00 = _s.x;
	m11 = _s.y;
	m22 = _s.z;
	m33 = 1;
	return *this;
}
//----------------------------------------------------------------------------//
Quaternion Matrix44::GetRotation(void) const
{
	Vector3 _m0(m00, m10, m20);
	Vector3 _m1(m01, m11, m21);
	Vector3 _m2(m02, m12, m22);
	Vector3 _q0 = _m0.Copy().Normalize();
	Vector3 _q1 = (_m1 - _q0 * _q0.Dot(_m1)).Normalize();
	Vector3 _q2 = ((_m2 - _q0 * _q0.Dot(_m2)) - _q1 * _q1.Dot(_m2)).Normalize();
	float _det = _q0[0] * _q1[1] * _q2[2] + _q0[1] * _q1[2] * _q2[0] + _q0[2] * _q1[0] * _q2[1] - _q0[2] * _q1[1] * _q2[0] - _q0[1] * _q1[0] * _q2[2] - _q0[0] * _q1[2] * _q2[1];
	if (_det < 0)
		_q0 = -_q0, _q1 = -_q1, _q2 = -_q2;
	return Quaternion().FromRotationMatrix(*_q0, *_q1, *_q2);
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::SetRotationScale(const Quaternion& _r, const Vector3& _s)
{
	_r.ToRotationMatrix(m[0], m[1], m[2]);
	Row(0) *= _s, Row(1) *= _s, Row(2) *= _s;
	return *this;
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::CreateTransform(const Vector3& _t, const Quaternion& _r, const Vector3& _s)
{
	// Ordering: Scale, Rotate, Translate
	float _r0[3], _r1[3], _r2[3];
	_r.ToRotationMatrix(_r0, _r1, _r2);
	m00 = _s.x * _r0[0], m01 = _s.y * _r0[1], m02 = _s.z * _r0[2], m03 = _t.x;
	m10 = _s.x * _r1[0], m11 = _s.y * _r1[1], m12 = _s.z * _r1[2], m13 = _t.y;
	m20 = _s.x * _r2[0], m21 = _s.y * _r2[1], m22 = _s.z * _r2[2], m23 = _t.z;
	m30 = 0, m31 = 0, m32 = 0, m33 = 1;
	return *this;
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::CreateInverseTransform(const Vector3& _t, const Quaternion& _r, const Vector3& _s)
{
	Vector3 _invs(1 / _s);
	Quaternion _invr = _r.Copy().Inverse();
	Vector3 _invt((-_t * _invr) * _invs);
	return CreateTransform(_invt, _invr, _invs);
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::Inverse(void)
{
	static const int dr0[4] = { 2, 2, 1, 1, };
	static const int dr1[4] = { 3, 3, 3, 2, };
	static const int dr2[4] = { 1, 0, 0, 0, };
	static const float ds[4] = { 1, -1, 1, -1, };
	static const int di[4][3] = { { 5, 4, 3, },{ 5, 2, 1, },{ 4, 2, 0, },{ 3, 1, 0, }, };

	Matrix44 _m = *this;

	for (int r = 0; r < 4; ++r)
	{
		int r0 = dr0[r]; 
		int r1 = dr1[r]; 
		int r2 = dr2[r]; 
		float s = ds[r];
		float s2 = -s;
		float t[6];
		
		t[0] = _m[r0][0] * _m[r1][1] - _m[r0][1] * _m[r1][0];
		t[1] = _m[r0][0] * _m[r1][2] - _m[r0][2] * _m[r1][0];
		t[2] = _m[r0][0] * _m[r1][3] - _m[r0][3] * _m[r1][0];
		t[3] = _m[r0][1] * _m[r1][2] - _m[r0][2] * _m[r1][1];
		t[4] = _m[r0][1] * _m[r1][3] - _m[r0][3] * _m[r1][1];
		t[5] = _m[r0][2] * _m[r1][3] - _m[r0][3] * _m[r1][2];
		
		m[0][r] = (t[5] * _m[r2][1] - t[4] * _m[r2][2] + t[3] * _m[r2][3]) * s;
		m[1][r] = (t[5] * _m[r2][0] - t[2] * _m[r2][2] + t[1] * _m[r2][3]) * s2;
		m[2][r] = (t[4] * _m[r2][0] - t[2] * _m[r2][1] + t[0] * _m[r2][3]) * s;
		m[3][r] = (t[3] * _m[r2][0] - t[1] * _m[r2][1] + t[0] * _m[r2][2]) * s2;
	}

	return Mul(1 / (m00 * _m[0][0] + m10 * _m[0][1] + m20 * _m[0][2] + m30 * _m[0][3]));
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::Transpose(void)
{
	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[0][3], m[3][0]);
	std::swap(m[1][2], m[2][1]);
	std::swap(m[1][3], m[3][1]);
	std::swap(m[2][3], m[3][2]);

	return *this;
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::CreateOrtho(float _left, float _right, float _bottom, float _top, float _near, float _far)
{
	SetIdentity();

	m00 = 2 / (_right - _left);
	m11 = 2 / (_top - _bottom);
	m22 = -2 / (_far - _near);
	m03 = -(_right + _left) / (_right - _left);
	m13 = -(_top + _bottom) / (_top - _bottom);
	m23 = -(_far + _near) / (_far - _near);

	return *this;
}
//----------------------------------------------------------------------------//
Matrix44& Matrix44::CreateProjection(float _fov, float _aspect, float _near, float _far)
{
	SetZero();

	if (_aspect != _aspect)
		_aspect = 1; // NaN
	if (_far == _near)
		_far = _near + EPSILON;

	float _h = 1 / Tan(_fov * 0.5f); // *_zoom;
	float _w = _h / _aspect;
	float _d = (_far - _near);

#if 0
	float _q = (_far + _near) / _d;
	float _r = -2 * (_far * _near) / _d;
#else
	float _q = _far / _d;
	float _r = -_q * _near;
#endif

	m00 = _w;
	m11 = _h;
	m22 = _q;
	m23 = _r;
	m32 = 1;

	return *this;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Ray
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Plane
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Plane& Plane::Normalize(void)
{
	float _length = normal.LengthSq();
	if (_length > EPSILON2)
	{
		_length = 1 / _length;
		normal *= _length;
		dist *= _length;
	}
	return *this;
}
//----------------------------------------------------------------------------//
float Plane::Distance(const Vector3& _center, float _radius) const
{
	float _d = normal.Dot(_center) + dist;
	float _md = normal.AbsDot(_radius);
	return (Abs(_md) < Abs(_d)) ? _d : 0;
	//return (_d < -_md ? _d + _md : (_d > _md ? _d - _md : 0));
}
//----------------------------------------------------------------------------//
float Plane::Distance(const Ray& _ray) const
{
	float _denom = -normal.Dot(_ray.dir);
	if (Abs(_denom) < EPSILON2)
		return 0;
	return normal.Dot(_ray.origin) / _denom;
}
//----------------------------------------------------------------------------//
Matrix44 Plane::GetReflectionMatrix(void) const
{
	Vector3 _n = -2 * normal;
	Matrix44 _m;
	_m.SetIdentity();
	for (int r = 0; r < 3; ++r)
	{
		for (int c = 0; c < 4; ++r)
		{
			_m[r][c] += _n[r] * AsVec4()[c];
		}
	}
	return _m;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Rect
//----------------------------------------------------------------------------//

const Rect Rect::Zero(0, 0, 0, 0);
const Rect Rect::Identity(0, 0, 1, 1);

//----------------------------------------------------------------------------//
Rect& Rect::SetOrigin(float _left, float _top)
{
	Vector2 _size = Size();
	left = _left, top = _top;
	SetSize(_size);
	return *this;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// IntRect
//----------------------------------------------------------------------------//

const IntRect IntRect::Zero(0, 0, 0, 0);

//----------------------------------------------------------------------------//
IntRect& IntRect::SetOrigin(int _left, int _top)
{
	IntVector2 _size = Size();
	left = _left, top = _top;
	SetSize(_size);
	return *this;
}
//----------------------------------------------------------------------------//
IntRect IntRect::Overlay(const IntRect& _r2)
{
	IntRect _r;
	_r.left = Max(left, _r2.left);
	_r.right = Min(right, _r2.right);
	_r.top = Max(top, _r2.top);
	_r.bottom = Min(bottom, _r2.bottom);
	if (_r.left > _r.right)
		_r.right = _r.left;
	if (_r.top > _r.bottom)
		_r.bottom = _r.top;
	return _r;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Box
//----------------------------------------------------------------------------//

const unsigned Box::LineIndices[24] = { 0, 1, 1, 2, 2, 3, 3, 0, 3, 5, 2, 4, 1, 7, 0, 6, 4, 5, 5, 6, 6, 7, 7, 4 };
const unsigned Box::QuadIndices[24] = { 5, 3, 2, 4, 4, 2, 1, 7, 7, 1, 0, 6, 6, 0, 3, 5, 6, 5, 4, 7, 2, 3, 0, 1 };
const unsigned Box::TriangleIndices[36] = { 5, 3, 2, 5, 2, 4, 4, 2, 1, 4, 1, 7, 7, 1, 0, 7, 0, 6, 6, 0, 3, 6, 3, 5, 6, 5, 4, 6, 4, 7, 3, 0, 1, 3, 1, 2 };

//----------------------------------------------------------------------------//
// AlignedBox
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
AlignedBox& AlignedBox::AddVertices(const void* _src, int _count, size_t _stride, size_t _offset)
{
	//ASSERT(_src || !_count);

	union { const uint8_t* p; const Vector3* v; } _vertices = { ((const uint8_t*)_src) + _offset };

	if (!_stride)
		_stride = sizeof(Vector3);

	for (int i = 0; i < _count; ++i)
	{
		mn.SetMin(*_vertices.v);
		mx.SetMax(*_vertices.v);
		_vertices.p += _stride;
	}

	return *this;
}
//----------------------------------------------------------------------------//
void AlignedBox::GetAllCorners(const void* _dst, size_t _stride, size_t _offset) const
{
	static const uint8_t _idx[8][3] =
	{
		{ 0, 1, 2 }, // mn.x, mn.y, mn.z  Left Bottom Far
		{ 3, 1, 2 }, // mx.x, mn.y, mn.z  Right Bottom Far
		{ 3, 1, 5 }, // mx.x, mn.y, mx.z  Right Bottom Near
		{ 0, 1, 5 }, // mn.x, mn.y, mx.z  Left Bottom Near
		{ 3, 4, 5 }, // mx.x, mx.y, mx.z  Right Top Near
		{ 0, 4, 5 }, // mn.x, mx.y, mx.z  Left Top Near
		{ 0, 4, 2 }, // mn.x, mx.y, mn.z  Left Top Far
		{ 3, 4, 2 }, // mx.x, mx.y, mn.z  Right Top Far
	};

	//ASSERT(_dst != nullptr);

	union { uint8_t* p; Vector3* v; } _vertices = { ((uint8_t*)_dst) + _offset };

	if (!_stride)
		_stride = sizeof(Vector3);

	const float* _v = &mn.x;
	for (int i = 0; i < 8; ++i)
	{
		_vertices.v->Set(_v[_idx[i][0]], _v[_idx[i][1]], _v[_idx[i][2]]);
		_vertices.p += _stride;
	}
}
//----------------------------------------------------------------------------//
AlignedBox& AlignedBox::TransformProj(const Matrix44& _rhs)
{
	Vector3 _v[8];
	GetAllCorners(_v);
	Reset();
	for (int i = 0; i < 8; ++i)
		AddPoint(_v[i] * _rhs);
	return *this;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Frustum
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Frustum& Frustum::FromCameraMatrices(const Matrix44& _view, const Matrix44& _proj)
{
	Matrix44 _m = _proj * _view; // _view_proj
	GetPlanes(planes, _m);
	origin = Vector3::Zero * _view.Copy().Inverse();
	box.FromViewProjMatrix(_m.Inverse());
	box.GetAllCorners(corners);
	return *this;
}
//----------------------------------------------------------------------------//
bool Frustum::Intersects(const Vector3& _point) const
{
	for (int i = 0; i < 6; ++i)
	{
		if (planes[i].Distance(_point) < 0)
			return false;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool Frustum::Intersects(const Vector3& _center, float _radius) const
{
	for (int i = 0; i < 6; ++i)
	{
		if (planes[i].Distance(_center, _radius) < 0)
			return false;
	}
	return true;
}
//----------------------------------------------------------------------------//
bool Frustum::Intersects(const AlignedBox& _box, bool* _contains) const
{
	if (box.Intersects(_box) && Intersects(_box.Center(), _box.Radius()))
	{
		if (_contains)
		{
			*_contains = true;
			Vector3 _corners[8];
			_box.GetAllCorners(_corners);
			for (unsigned int i = 0; i < 8; ++i)
			{
				if (!Intersects(_corners[i]))
				{
					*_contains = false;
					break;
				}
			}
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------//
bool Frustum::Intersects(const Frustum& _frustum, bool* _contains) const
{
	if (box.Intersects(_frustum.box))
	{
		for (unsigned int i = 0; i < 6; ++i)
		{
			int _out = 0;
			for (unsigned int j = 0; j < 8; ++j)
				if (planes[i].Distance(_frustum.corners[j]) < 0)
					++_out;
			if (_out == 8)
				return false;
		}
		if (_contains)
		{
			*_contains = true;
			for (int i = 0; i < 8; ++i)
			{
				if (!Intersects(_frustum.corners[i]))
				{
					*_contains = false;
					break;
				}
			}
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------//
void Frustum::GetPlanes(Plane* _planes, const Matrix44& _m)
{
	/*
	_planes[Left].  Set(_m[3][0] + _m[0][0], _m[3][1] + _m[0][1], _m[3][2] + _m[0][2], _m[3][3] + _m[0][3]).Normalize();
	_planes[Right]. Set(_m[3][0] - _m[0][0], _m[3][1] - _m[0][1], _m[3][2] - _m[0][2], _m[3][3] - _m[0][3]).Normalize();
	_planes[Bottom].Set(_m[3][0] + _m[1][0], _m[3][1] + _m[1][1], _m[3][2] + _m[1][2], _m[3][3] + _m[1][3]).Normalize();
	_planes[Top].   Set(_m[3][0] - _m[1][0], _m[3][1] - _m[1][1], _m[3][2] - _m[1][2], _m[3][3] - _m[1][3]).Normalize();
	_planes[Near].  Set(_m[3][0] + _m[2][0], _m[3][1] + _m[2][1], _m[3][2] + _m[2][2], _m[3][3] + _m[2][3]).Normalize();
	_planes[Far].   Set(_m[3][0] - _m[2][0], _m[3][1] - _m[2][1], _m[3][2] - _m[2][2], _m[3][3] - _m[2][3]).Normalize();
	*/

	for (int i = 0, j = 0; i < 3; ++i, j += 2)
	{
		_planes[j].Set(_m[3][0] + _m[i][0], _m[3][1] + _m[i][1], _m[3][2] + _m[i][2], _m[3][3] + _m[i][3]).Normalize();
		_planes[j + 1].Set(_m[3][0] - _m[i][0], _m[3][1] - _m[i][1], _m[3][2] - _m[i][2], _m[3][3] - _m[i][3]).Normalize();
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Triangle
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
/*Triangle& Triangle::Create(const Vector3& _a, const Vector3& _b, const Vector3& _c)
{
	v[0] = _a;
	v[1] = _b;
	v[2] = _c;
	plane.FromTriangle(_a, _b, _c);

	return *this;
}
//----------------------------------------------------------------------------//
bool Triangle::Intersects(Triangle* _other, const Matrix44& _matrix)
{
	Vector3 _tv[3];
	for (int i = 0; i < 3; ++i)
		_tv[i] = _matrix.Transform(_other->v[i]);
	Plane _tp;
	_tp.FromTriangle(_tv[0], _tv[1], _tv[2]);

	return false;
}*/
//----------------------------------------------------------------------------//
bool Triangle::Intersects(const Vector3& _t0, const Vector3& _t1, const Vector3& _t2, const Ray& _ray, float* _dist)
{
	Plane _plane;
	_plane.FromTriangle(_t0, _t1, _t2).Normalize();
	float _d = _plane.Distance(_ray);
	Vector3 _p = _ray * _d;
	if (Normal(_t1, _t0, _p).Dot(_plane.normal) >= 0
		&& Normal(_t2, _t1, _p).Dot(_plane.normal) >= 0
		&& Normal(_t0, _t2, _p).Dot(_plane.normal) >= 0)
	{
		if (_dist)
			*_dist = _d;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
// Dbvt
//----------------------------------------------------------------------------//

#define _DBVT_STACK_SIZE 128

//----------------------------------------------------------------------------//
void Dbvt::Insert(Node* _leaf)
{
	//ASSERT(_leaf != nullptr && _leaf->IsValidLeaf());
	//ASSERT(_leaf->parent == nullptr && _leaf != m_root);

	_Insert(_leaf, m_root);
}
//----------------------------------------------------------------------------//
void Dbvt::Remove(Node* _leaf)
{
	//ASSERT(_leaf != nullptr && _leaf->IsValidLeaf());

	_Remove(_leaf);
	_leaf->parent = nullptr;
}
//----------------------------------------------------------------------------//
void Dbvt::Update(Node* _leaf)
{
	//ASSERT(_leaf != nullptr);
	//ASSERT(_leaf->object != nullptr && _leaf->box.IsFinite());

	//_Remove(_leaf);
	//_leaf->parent = nullptr;
	//_Insert(_leaf, m_root);

	Node* _root = _Remove(_leaf);
	while (_root && !_root->box.Contains(_leaf->box))
		_root = _root->parent;
	_Insert(_leaf, _root ? _root : m_root);
}
//----------------------------------------------------------------------------//
int Dbvt::GetMaxDepth(void)
{
	std::vector<Node*> _stack;
	_stack.push_back(m_root);
	size_t _depth = 1;
	if (m_root) do
	{
		_depth = Max(_depth, _stack.size());
		Node* _node = _stack.back();
		_stack.pop_back();

		if (_node->IsNode())
		{
			_stack.push_back(_node->child0);
			_stack.push_back(_node->child1);
		}

	} while (_stack.size() > 0);

	return _depth;
}
//----------------------------------------------------------------------------//
void Dbvt::Enum(Callback* _cb, bool _withTest)
{
	//ASSERT(GetMaxDepth() < _DBVT_STACK_SIZE);
	Node* _stackBase[_DBVT_STACK_SIZE];
	Node** _stack = _stackBase;
	*_stack++ = m_root;

	//DEBUG_CODE(int _maxDepth = 0, _maxDepthWithoutTest = 0, _depth = 1, _numTests = 0, _numObjects = 0);

	if (m_root) do
	{
		Node* _node = *--_stack;
		//DEBUG_CODE(_maxDepth = Max(_maxDepth, --_depth), ++_numTests);
		//ASSERT(_node->box.Contains(_node->_GetChildBounds()));

		Callback::TestResult _result = _withTest ? _cb->TestNode(_node) : Callback::WithoutTest;
		if (_result == Callback::Stop)
			break;
		else if (_result == Callback::Skip)
			continue;
		else if (_result == Callback::WithTest)
		{
			if (_node->child[1])
			{
				//ASSERT(_depth + 2 < _DBVT_STACK_SIZE);
				*_stack++ = _node->child0;
				*_stack++ = _node->child1;
				//DEBUG_CODE(_depth += 2);
			}
			else
			{
				_cb->AddLeaf(_node, _result);
				//DEBUG_CODE(++_numObjects);
			}
		}
		else
		{
			Node** _stack2 = _stack;
			*_stack2++ = _node;
			//DEBUG_CODE(++_depth);
			do
			{
				_node = *--_stack2;
				//DEBUG_CODE(_maxDepthWithoutTest = Max(_maxDepth, --_depth));
				//ASSERT(_node->box.Contains(_node->_GetChildBounds()));

				if (_node->child[1])
				{
					//ASSERT(_depth + 2 < _DBVT_STACK_SIZE);
					*_stack2++ = _node->child0;
					*_stack2++ = _node->child1;
					//DEBUG_CODE(_depth += 2);
				}
				else
				{
					_cb->AddLeaf(_node, _result);
					//DEBUG_CODE(++_numObjects);
				}

			} while (_stack2 > _stack);
		}

	} while (_stack > _stackBase);

#if _DEBUG
	//LOG("max depth = %d/%d, num tests = %d, num objects = %d\n", _maxDepth, _maxDepthWithoutTest, _numTests, _numObjects);
#endif

}
//----------------------------------------------------------------------------//
void Dbvt::_Insert(Node* _leaf, Node* _root)
{
	if (!m_root) // it's new root
	{
		m_root = _leaf;
		return;
	}

	// the target node for insertion instead of him
	//ASSERT(_root != nullptr);
	Node* _target = _root;
	while (_target->IsNode())
		_target = _target->child[_leaf->box.Select(_target->child0->box, _target->child1->box)];

	// parent node
	Node* _parent = _target->parent;

	// new node
	Node* _node = m_free ? m_free : new Node;
	m_free = nullptr;
	_node->box = _target->box + _leaf->box;
	_node->parent = _parent;

	// do insert target and leaf
	_node->child0 = _target;
	_target->parent = _node;
	_node->child1 = _leaf;
	_leaf->parent = _node;

	if (_parent) // it's not root
	{
		_parent->child[_parent->child1 == _target ? 1 : 0] = _node; // do insert new node instead of target
		while (_parent && !_parent->box.Contains(_node->box)) // update bbox for parents
		{
			_parent->box = _parent->child0->box + _parent->child1->box;
			_node = _parent;
			_parent = _node->parent;
		}
	}
	else
		m_root = _node;
}
//----------------------------------------------------------------------------//
Dbvt::Node* Dbvt::_Remove(Node* _leaf)
{
	Node* _parent = _leaf->parent;
	_leaf->parent = nullptr;

	if (_leaf == m_root) // it's root
	{
		m_root = nullptr;
		return nullptr;
	}

	Node* _prev = _parent->parent;
	Node* _sibling = _parent->child[_parent->child0 == _leaf ? 1 : 0];

	if (_prev) // in depth of tree
	{
		// remove parent
		_prev->child[_prev->child1 == _parent ? 1 : 0] = _sibling;
		_sibling->parent = _prev;
		_Delete(_parent);

		// update bbox for parents
		AlignedBox _box;
		while (_prev)
		{
			_box = _prev->box;
			_prev->box = _prev->child0->box + _prev->child1->box;
			if (_box == _prev->box) // no changes 
				break;
			_prev = _prev->parent;
		}
		return _prev;
	}

	// it's child of root
	m_root = _sibling;
	_sibling->parent = nullptr;
	_Delete(_parent);
	return m_root;
}
//----------------------------------------------------------------------------//
void Dbvt::_Delete(Node* _node)
{
	if (!m_free)
		m_free = _node;
	else
		delete _node;
}
//----------------------------------------------------------------------------//
void Dbvt::_Clear(void)
{
	//ASSERT(GetMaxDepth() < _DBVT_STACK_SIZE);
	Node* _stackBase[_DBVT_STACK_SIZE];
	Node** _stack = _stackBase;
	*_stack++ = m_root;
	if (m_root) do
	{
		Node* _top = *_stack--;
		if (_top->IsNode())
		{
			*_stack++ = _top->child0;
			*_stack++ = _top->child1;
			delete _top;
		}
		else
			_top->parent = nullptr;

	} while (_stack > _stackBase);

	delete m_free;
	m_free = nullptr;
	m_root = nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

