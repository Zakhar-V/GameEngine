#pragma once


#include <math.h>
#include <stdint.h>

//----------------------------------------------------------------------------//
// Constants
//----------------------------------------------------------------------------//

//!
static const float EPSILON = 1e-6f;
//!
static const float EPSILON2 = 1e-12f;
//!
static const float PI = 3.1415926535897932384626433832795f;
//!
static const float HALF_PI = PI * 0.5f;
//!
static const float QUAD_PI = PI * 0.25f;
//!
static const float DEGREES = 57.295779513082320876798154814105f;
//!
static const float RADIANS = 0.01745329251994329576923690768489f;
//!
static const float BYTE2FLOAT = 1.0f / 255.0f;
//!
static const float FLOAT2BYTE = 255.0f;

//----------------------------------------------------------------------------//
// MATH
//----------------------------------------------------------------------------//

//!
template <typename T> const T& Min(const T& _a, const T& _b) { return _a < _b ? _a : _b; }
//!
template <typename T> const T& Min(const T& _a, const T& _b, const T& _c) { return _a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c); }
//!
template <typename T> const T& Max(const T& _a, const T& _b) { return _a > _b ? _a : _b; }
//!
template <typename T> const T& Max(const T& _a, const T& _b, const T& _c) { return _a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c); }
//!
template <typename T> const T Clamp(T _x, T _l, T _u) { return _x > _l ? (_x < _u ? _x : _u) : _l; }
//!
template <typename T> T Mix(const T& _a, const T& _b, float _t) { return _a + (_b - _a) * _t; }
//!
template <typename T> T Abs(T _x) { return abs(_x); }
//!
inline float Abs(float _x) { return fabsf(_x); }
//!
inline float Radians(float _val) { return _val * RADIANS; }
//!
inline float Degrees(float _val) { return _val * DEGREES; }
//!
template <typename T> T Sqr(T _x) { return _x * _x; }
//!
inline float Sqrt(float _x) { return sqrtf(_x); }
//!
inline float RSqrt(float _x) { return 1 / Sqrt(_x); }
//!
inline float Sin(float _x) { return sinf(_x); }
//!
inline float Cos(float _x) { return cosf(_x); }
//!
inline void SinCos(float _a, float& _s, float& _c) { _s = sinf(_a), _c = cosf(_a); }
//!
inline float Tan(float _x) { return tanf(_x); }
//!
inline float ATan2(float _y, float _x) { return atan2f(_y, _x); }
//!
inline float ASin(float _x) { return asinf(_x); }
//!
inline float ACos(float _x) { return acosf(_x); }
//!
inline float Log(float _x) { return logf(_x); }
//!
inline float Log2(float _x) { return log2f(_x); }
//!
inline int Log2i(int _x) { return (int)Log2((float)_x);}
//!
inline float Floor(float _x) { return floorf(_x); }
//!
inline float Ceil(float _x) { return ceilf(_x); }
//!
float Wrap(float _x, float _l, float _u);
//!
uint32_t FirstPow2(uint32_t _val);
//!
inline bool IsPow2(uint32_t _val) { return (_val & (_val - 1)) == 0; }
//!
inline uint8_t FloatToByte(float _x) { return (uint8_t)(_x * FLOAT2BYTE); }
//!
inline float ByteToFloat(uint8_t _x) { return _x * BYTE2FLOAT; }
//!
inline uint8_t FloatToSByte(float _x) { return (uint8_t)((_x * 0.5f) * FLOAT2BYTE); }
//!
inline float SByteToFloat(uint8_t _x) { return int8_t(_x) * BYTE2FLOAT; }
//!
uint16_t FloatToHalf(float _value);
//!
float HalfToFloat(uint16_t _value);
//!
inline float Cerp(float _a, float _b, float _t) { return Mix(_a, _b, (1 - Cos(_t * PI)) * 0.5f); }

//----------------------------------------------------------------------------//
// Vector2
//----------------------------------------------------------------------------//

//!
struct Vector2
{
	//!
	Vector2(void) = default;
	//!
	Vector2(float _s) : x(_s), y(_s) { }
	//!
	Vector2(float _x, float _y) : x(_x), y(_y) { }

	//!
	const float operator [] (int _index) const { return v[_index]; }
	//!
	float& operator [] (int _index) { return v[_index]; }
	//!
	const float* operator * (void) const { return v; }
	//!
	float* operator * (void) { return v; }

	//!
	Vector2 operator - (void) const { return Vector2(-x, -y); }
	//!
	Vector2 operator + (const Vector2& _rhs) const { return Vector2(x + _rhs.x, y + _rhs.y); }
	//!
	Vector2 operator - (const Vector2& _rhs) const { return Vector2(x - _rhs.x, y - _rhs.y); }
	//!
	Vector2 operator * (const Vector2& _rhs) const { return Vector2(x * _rhs.x, y * _rhs.y); }
	//!
	Vector2 operator / (const Vector2& _rhs) const { return Vector2(x / _rhs.x, y / _rhs.y); }
	//!
	Vector2 operator * (float _rhs) const { return Vector2(x * _rhs, y * _rhs); }
	//!
	Vector2 operator / (float _rhs) const { return Vector2(x / _rhs, y / _rhs); }
	//!
	Vector2& operator += (const Vector2& _rhs) { x += _rhs.x, y += _rhs.y; return *this; }
	//!
	Vector2& operator -= (const Vector2& _rhs) { x -= _rhs.x, y -= _rhs.y; return *this; }
	//!
	Vector2& operator *= (const Vector2& _rhs) { x *= _rhs.x, y *= _rhs.y; return *this; }
	//!
	Vector2& operator /= (const Vector2& _rhs) { x /= _rhs.x, y /= _rhs.y; return *this; }
	//!
	Vector2& operator *= (float _rhs) { x *= _rhs, y *= _rhs; return *this; }
	//!
	Vector2& operator /= (float _rhs) { x /= _rhs, y /= _rhs; return *this; }
	//!
	friend Vector2 operator / (float _lhs, const Vector2& _rhs) { return Vector2(_lhs / _rhs.x, _lhs / _rhs.y); }
	//!
	friend Vector2 operator * (float _lhs, const Vector2& _rhs) { return Vector2(_lhs * _rhs.x, _lhs * _rhs.y); }

	//!
	bool operator == (const Vector2& _rhs) const { return x == _rhs.x && y == _rhs.y; }
	//!
	bool operator != (const Vector2& _rhs) const { return x != _rhs.x || y != _rhs.y; }

	//!
	Vector2& Set(const Vector2& _v) { return *this = _v; }
	//!
	Vector2& Set(float _s) { x = _s, y = _s; return *this; }
	//!
	Vector2& Set(float _x, float _y) { x = _x, y = _y; return *this; }

	//!
	float Dot(const Vector2& _v) const { return x * _v.x + y * _v.y; }
	//!
	float LengthSq(void) const { return Dot(*this); }

	union
	{
		struct { float x, y; };
		float v[2];
	};

	//!
	static const Vector2 Zero;
	//!
	static const Vector2 One;
};

//----------------------------------------------------------------------------//
// IntVector2
//----------------------------------------------------------------------------//

//!
struct IntVector2
{
	//!
	IntVector2(void) = default;
	//!
	IntVector2(int _s) : x(_s), y(_s) { }
	//!
	IntVector2(int _x, int _y) : x(_x), y(_y) { }
	//!
	IntVector2(const Vector2& _v) : x((int)_v.x), y((int)_v.y) { }
	//!
	operator Vector2 (void) const { return Vector2((float)x, (float)y); }

	//!
	const int operator [] (int _index) const { return v[_index]; }
	//!
	int& operator [] (int _index) { return v[_index]; }
	//!
	const int* operator * (void) const { return v; }
	//!
	int* operator * (void) { return v; }

	//!
	IntVector2 operator - (void) const { return IntVector2(-x, -y); }
	//!
	IntVector2 operator + (const IntVector2& _rhs) const { return IntVector2(x + _rhs.x, y + _rhs.y); }
	//!
	IntVector2 operator - (const IntVector2& _rhs) const { return IntVector2(x - _rhs.x, y - _rhs.y); }
	//!
	IntVector2 operator * (const IntVector2& _rhs) const { return IntVector2(x * _rhs.x, y * _rhs.y); }
	//!
	IntVector2 operator / (const IntVector2& _rhs) const { return IntVector2(x / _rhs.x, y / _rhs.y); }
	//!
	IntVector2 operator * (int _rhs) const { return IntVector2(x * _rhs, y * _rhs); }
	//!
	IntVector2 operator / (int _rhs) const { return IntVector2(x / _rhs, y / _rhs); }
	//!
	IntVector2& operator += (const IntVector2& _rhs) { x += _rhs.x, y += _rhs.y; return *this; }
	//!
	IntVector2& operator -= (const IntVector2& _rhs) { x -= _rhs.x, y -= _rhs.y; return *this; }
	//!
	IntVector2& operator *= (const IntVector2& _rhs) { x *= _rhs.x, y *= _rhs.y; return *this; }
	//!
	IntVector2& operator /= (const IntVector2& _rhs) { x /= _rhs.x, y /= _rhs.y; return *this; }
	//!
	IntVector2& operator *= (int _rhs) { x *= _rhs, y *= _rhs; return *this; }
	//!
	IntVector2& operator /= (int _rhs) { x /= _rhs, y /= _rhs; return *this; }
	//!
	friend IntVector2 operator / (int _lhs, const IntVector2& _rhs) { return IntVector2(_lhs / _rhs.x, _lhs / _rhs.y); }
	//!
	friend IntVector2 operator * (int _lhs, const IntVector2& _rhs) { return IntVector2(_lhs * _rhs.x, _lhs * _rhs.y); }

	//!
	bool operator == (const IntVector2& _rhs) const { return x == _rhs.x && y == _rhs.y; }
	//!
	bool operator != (const IntVector2& _rhs) const { return x != _rhs.x || y != _rhs.y; }

	//!
	IntVector2& Set(const IntVector2& _v) { return *this = _v; }
	//!
	IntVector2& Set(int _s) { x = _s, y = _s; return *this; }
	//!
	IntVector2& Set(int _x, int _y) { x = _x, y = _y; return *this; }

	union
	{
		struct { int x, y; };
		int v[2];
	};

	//!
	static const IntVector2 Zero;
	//!
	static const IntVector2 One;
};

//----------------------------------------------------------------------------//
// Vector3
//----------------------------------------------------------------------------//

//!
struct Vector3
{
	//!
	Vector3(void) = default;
	//!
	Vector3(float _s) : x(_s), y(_s), z(_s) { }
	//!
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
	//!
	Vector3 Copy(void) const { return *this; }

	//!
	Vector3(const Vector2& _v, float _z = 0) : x(_v.x), y(_v.y), z(_z) { }
	//!
	operator const Vector2& (void) const { return *reinterpret_cast<const Vector2*>(v); }

	//!
	const float operator [] (int _index) const { return v[_index]; }
	//!
	float& operator [] (int _index) { return v[_index]; }
	//!
	const float* operator * (void) const { return v; }
	//!
	float* operator * (void) { return v; }

	//!
	Vector3 operator - (void) const { return Vector3(-x, -y, -z); }
	//!
	Vector3 operator + (const Vector3& _rhs) const { return Vector3(x + _rhs.x, y + _rhs.y, z + _rhs.z); }
	//!
	Vector3 operator - (const Vector3& _rhs) const { return Vector3(x - _rhs.x, y - _rhs.y, z - _rhs.z); }
	//!
	Vector3 operator * (const Vector3& _rhs) const { return Vector3(x * _rhs.x, y * _rhs.y, z * _rhs.z); }
	//!
	Vector3 operator / (const Vector3& _rhs) const { return Vector3(x / _rhs.x, y / _rhs.y, z / _rhs.z); }
	//!
	Vector3 operator * (float _rhs) const { return Vector3(x * _rhs, y * _rhs, z * _rhs); }
	//!
	Vector3 operator / (float _rhs) const { return Vector3(x / _rhs, y / _rhs, z / _rhs); }
	//!
	Vector3& operator += (const Vector3& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z; return *this; }
	//!
	Vector3& operator -= (const Vector3& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z; return *this; }
	//!
	Vector3& operator *= (const Vector3& _rhs) { x *= _rhs.x, y *= _rhs.y, z *= _rhs.z; return *this; }
	//!
	Vector3& operator /= (const Vector3& _rhs) { x /= _rhs.x, y /= _rhs.y, z /= _rhs.z; return *this; }
	//!
	Vector3& operator *= (float _rhs) { x *= _rhs, y *= _rhs, z *= _rhs; return *this; }
	//!
	Vector3& operator /= (float _rhs) { x /= _rhs, y /= _rhs, z /= _rhs; return *this; }
	//!
	friend Vector3 operator / (float _lhs, const Vector3& _rhs) { return Vector3(_lhs / _rhs.x, _lhs / _rhs.y, _lhs / _rhs.z); }
	//!
	friend Vector3 operator * (float _lhs, const Vector3& _rhs) { return Vector3(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z); }

	//!
	bool operator == (const Vector3& _rhs) const { return x == _rhs.x && y == _rhs.y && z == _rhs.z; }
	//!
	bool operator != (const Vector3& _rhs) const { return !(*this == _rhs); }

	//!
	Vector3& Set(float _s) { x = _s, y = _s, z = _s; return *this; }
	//!
	Vector3& Set(float _x, float _y, float _z) { x = _x, y = _y, z = _z; return *this; }
	//!
	Vector3& Set(const Vector3& _other) { return *this = _other; }

	//!
	Vector3& SetMin(const Vector3& _a, const Vector3& _b) { x = Min(_a.x, _b.x), y = Min(_a.y, _b.y), z = Min(_a.z, _b.z); return *this; }
	//!
	Vector3& SetMin(const Vector3& _other) { return SetMin(*this, _other); }
	//!
	Vector3& SetMax(const Vector3& _a, const Vector3& _b) { x = Max(_a.x, _b.x), y = Max(_a.y, _b.y), z = Max(_a.z, _b.z); return *this; }
	//!
	Vector3& SetMax(const Vector3& _other) { return SetMax(*this, _other); }
	//!
	Vector3& Normalize(void)
	{
		float _l = LengthSq();
		if (_l > EPSILON2)
			*this *= RSqrt(_l);
		return *this;
	}
	//!
	Vector3& NormalizeFast(void) { return *this *= LengthInv(); }
	//!
	float LengthSq(void) const { return x * x + y * y + z * z; }
	//!
	float Length(void) const { return Sqrt(x * x + y * y + z * z); }
	//!
	float LengthInv(void) const { return Sqrt(x * x + y * y + z * z); }
	//!
	float Dot(const Vector3& _rhs) const { return x * _rhs.x + y * _rhs.y + z * _rhs.z; }
	//!
	float AbsDot(const Vector3& _rhs) const { return Abs(x * _rhs.x) + Abs(y * _rhs.y) + Abs(z * _rhs.z); }
	//!
	Vector3 Cross(const Vector3& _rhs) const { return Vector3(y * _rhs.z - z * _rhs.y, z * _rhs.x - x * _rhs.z, x * _rhs.y - y * _rhs.x); }
	//!
	float Distance(const Vector3& _v) const { return (*this - _v).Length(); }
	//!
	float DistanceSq(const Vector3& _v) const { return (*this - _v).LengthSq(); }
	//!
	Vector3 MidPoint(const Vector3& _v) const { return (*this + _v) * 0.5f; }
	//!
	Vector3 Reflect(const Vector3& _n) const { return Vector3(*this - (2 * Dot(_n) * _n)); }
	//!
	Vector3 Perpendicular(void) const
	{
		Vector3 _perp = Cross({ 1, 0, 0 });
		if (_perp.LengthSq() <= EPSILON2)
			_perp = Cross({ 0, 1, 0 });
		return _perp.Normalize();
	}
	//!
	float Angle(const Vector3& _v) const
	{
		float _lp = LengthSq() * _v.LengthSq();
		if (_lp > EPSILON2)
			_lp = RSqrt(_lp);
		return ACos(Clamp<float>(Dot(_v) * _lp, -1, 1));
	}
	//!
	float AbsSum(void) const { return Abs(x) + Abs(y) + Abs(z); }

	//!
	bool operator < (const Vector3& _rhs) const { return x < _rhs.x&& y < _rhs.y&& z < _rhs.z; }
	//!
	bool operator <= (const Vector3& _rhs) const { return x <= _rhs.x && y <= _rhs.y && z <= _rhs.z; }
	//!
	bool operator > (const Vector3& _rhs) const { return x > _rhs.x && y > _rhs.y && z > _rhs.z; }
	//!
	bool operator >= (const Vector3& _rhs) const { return x >= _rhs.x && y >= _rhs.y && z >= _rhs.z; }

	union
	{
		struct { float x, y, z; };
		float v[3];
	};

	//!
	static const Vector3 Zero;
	//!
	static const Vector3 One;
	//!
	static const Vector3 UnitX;
	//!
	static const Vector3 UnitY;
	//!
	static const Vector3 UnitZ;
};

//----------------------------------------------------------------------------//
// Vector4
//----------------------------------------------------------------------------//

//!
struct Vector4
{
	//!
	Vector4(void) = default;
	//!
	Vector4(float _s) : x(_s), y(_s), z(_s), w(_s) { }
	//!
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
	//!
	Vector4(const Vector3& _v, float _w = 1) : x(_v.x), y(_v.y), z(_v.z), w(_w) { }

	//!
	Vector4 Copy(void) const { return *this; }

	//!
	const float operator [] (int _index) const { return v[_index]; }
	//!
	float& operator [] (int _index) { return v[_index]; }
	//!
	const float* operator * (void) const { return v; }
	//!
	float* operator * (void) { return v; }

	//!
	operator Vector3& (void) { return *(Vector3*)v; }
	//!
	operator const Vector3& (void) const { return *(const Vector3*)v; }

	//!
	Vector4 operator - (void) const { return Vector4(-x, -y, -z, -w); }
	//!
	Vector4 operator + (const Vector4& _rhs) const { return Vector4(x + _rhs.x, y + _rhs.y, z + _rhs.z, w + _rhs.w); }
	//!
	Vector4 operator - (const Vector4& _rhs) const { return Vector4(x - _rhs.x, y - _rhs.y, z - _rhs.z, w - _rhs.w); }
	//!
	Vector4 operator * (const Vector4& _rhs) const { return Vector4(x * _rhs.x, y * _rhs.y, z * _rhs.z, w * _rhs.w); }
	//!
	Vector4 operator / (const Vector4& _rhs) const { return Vector4(x / _rhs.x, y / _rhs.y, z / _rhs.z, w / _rhs.w); }
	//!
	Vector4 operator * (float _rhs) const { return Vector4(x * _rhs, y * _rhs, z * _rhs, w * _rhs); }
	//!
	Vector4 operator / (float _rhs) const { return Vector4(x / _rhs, y / _rhs, z / _rhs, w / _rhs); }
	//!
	Vector4& operator += (const Vector4& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z, w += _rhs.w; return *this; }
	//!
	Vector4& operator -= (const Vector4& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z, w -= _rhs.w; return *this; }
	//!
	Vector4& operator *= (const Vector4& _rhs) { x *= _rhs.x, y *= _rhs.y, z *= _rhs.z, w *= _rhs.w; return *this; }
	//!
	Vector4& operator /= (const Vector4& _rhs) { x /= _rhs.x, y /= _rhs.y, z /= _rhs.z, w /= _rhs.w; return *this; }
	//!
	Vector4& operator *= (float _rhs) { x *= _rhs, y *= _rhs, z *= _rhs, w *= _rhs; return *this; }
	//!
	Vector4& operator /= (float _rhs) { x /= _rhs, y /= _rhs, z /= _rhs, w /= _rhs; return *this; }
	//!
	friend Vector4 operator / (float _lhs, const Vector4& _rhs) { return Vector4(_lhs / _rhs.x, _lhs / _rhs.y, _lhs / _rhs.z, _lhs / _rhs.w); }
	//!
	friend Vector4 operator * (float _lhs, const Vector4& _rhs) { return Vector4(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z, _lhs * _rhs.w); }

	//!
	Vector4& Set(float _x, float _y, float _z, float _w = 1) { x = _x, y = _y, z = _z, w = _w; return *this; }
	//!
	Vector4& Set(float _s) { x = _s, y = _s, z = _s, w = _s; return *this; }
	//!
	Vector4& Set(const Vector3& _xyz, float _w = 1) { return Set(_xyz.x, _xyz.y, _xyz.z, _w); }
	//!
	Vector4& Set(const Vector4& _other) { return *this = _other; }

	union
	{
		struct { float r, g, b, a; };
		struct { float x, y, z, w; };
		float v[4];
	};

	//!
	static const Vector4 Zero;
	//!
	static const Vector4 One;
	//!
	static const Vector4 Identity;
};

//----------------------------------------------------------------------------//
// Vector4ub
//----------------------------------------------------------------------------//

struct Vector4ub
{
	//!
	Vector4ub(void) = default;
	//!
	Vector4ub(uint8_t _s) : x(_s), y(_s), z(_s), w(_s) { }
	//!
	Vector4ub(uint8_t _x, uint8_t _y, uint8_t _z, uint8_t _w) : x(_x), y(_y), z(_z), w(_w) { }

	//!
	Vector4ub Copy(void) const { return *this; }

	//!
	const uint8_t operator [] (int _index) const { return v[_index]; }
	//!
	uint8_t& operator [] (int _index) { return v[_index]; }
	//!
	const uint8_t* operator * (void) const { return v; }
	//!
	uint8_t* operator * (void) { return v; }

	union
	{
		struct { uint8_t r, g, b, a; };
		struct { uint8_t x, y, z, w; };
		uint8_t v[4];
	};
};

//----------------------------------------------------------------------------//
// Color
//----------------------------------------------------------------------------//

//!
struct Color
{
	//!
	Color(void) = default;
	//!
	constexpr Color(uint32_t _rgba) : r((_rgba >> 24) & 0xff), g((_rgba >> 16) & 0xff), b((_rgba >> 8) & 0xff), a((_rgba >> 0) & 0xff) { }
	//!
	Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 0xff) : r(_r), g(_g), b(_b), a(_a) { }
	//!
	Color(const Vector4& _v) : r(FloatToByte(_v.r)), g(FloatToByte(_v.g)), b(FloatToByte(_v.b)), a(FloatToByte(_v.a)) { }
	//!
	operator Vector4 (void) const { return Vector4(ByteToFloat(r), ByteToFloat(g), ByteToFloat(b), ByteToFloat(a)); }

	//!
	const uint8_t operator [] (int _index) const { return v[_index]; }
	//!
	uint8_t& operator [] (int _index) { return v[_index]; }
	//!
	const uint8_t* operator * (void) const { return v; }
	//!
	uint8_t* operator * (void) { return v; }

	//!
	uint32_t Argb(void) const { return (rgba >> 8) | (a << 24); }
	//!
	uint32_t Abgr(void) const { return (a << 24) | (b << 16) | (g << 8) | r; }

	//!
	Color& Blend(const Color& _c1, const Color& _c2, const Color& _c3, const Color& _c4)
	{
		r = (uint8_t)(uint32_t(_c1.r + _c2.r + _c3.r + _c4.r) >> 2);
		g = (uint8_t)(uint32_t(_c1.g + _c2.g + _c3.g + _c4.g) >> 2);
		b = (uint8_t)(uint32_t(_c1.b + _c2.b + _c3.b + _c4.b) >> 2);
		a = (uint8_t)(uint32_t(_c1.a + _c2.a + _c3.a + _c4.a) >> 2);
		return *this;
	}
	//!
	Color& Blend(const Color& _c1, const Color& _c2)
	{
		r = (uint8_t)(uint32_t(_c1.r + _c2.r) >> 1);
		g = (uint8_t)(uint32_t(_c1.g + _c2.g) >> 1);
		b = (uint8_t)(uint32_t(_c1.b + _c2.b) >> 1);
		a = (uint8_t)(uint32_t(_c1.a + _c2.a) >> 1);
		return *this;
	}

	union
	{
		struct { uint8_t r, g, b, a; };
		struct { uint8_t x, y, z, w; };
		uint8_t v[4];
		uint32_t rgba;
	};
};

//----------------------------------------------------------------------------//
// Quaternion
//----------------------------------------------------------------------------//

//!
struct Quaternion
{
	//!
	Quaternion(void) = default;

	//!
	Quaternion(float _x, float _z, float _y, float _w) : x(_x), y(_y), z(_z), w(_w) { }
	//!
	Quaternion(float _w) : x(0), y(0), z(0), w(_w) { }
	//!
	Quaternion Copy(void) const { return *this; }

	//!
	Quaternion& Set(const Quaternion& _other) { return *this = _other; }
	//!
	Quaternion& Set(float _x, float _y, float _z, float _w) { x = _x, y = _y, z = _z, w = _w; return *this; }
	//!
	Quaternion& Set(float _w) { x = 0, y = 0, z = 0, w = _w; return *this; }

	//!
	const float operator [] (int _index) const { return v[_index]; }
	//!
	float& operator [] (int _index) { return v[_index]; }
	//!
	const float* operator * (void) const { return v; }
	//!
	float* operator * (void) { return v; }

	//!
	Quaternion operator - (void) const { return Quaternion(-x, -y, -z, -w); }
	//!
	Quaternion operator + (const Quaternion& _rhs) const { return Quaternion(x + _rhs.x, y + _rhs.y, z + _rhs.z, w + _rhs.w); }
	//!
	Quaternion operator - (const Quaternion& _rhs) const { return Quaternion(x - _rhs.x, y - _rhs.y, z - _rhs.z, w - _rhs.w); }
	//!
	Quaternion operator * (const Quaternion& _rhs) const { return Copy().Mul(_rhs); }
	//!
	Quaternion operator * (float _rhs) const { return Quaternion(x * _rhs, y * _rhs, z * _rhs, w * _rhs); }
	//!
	Quaternion operator / (float _rhs) const { return Quaternion(x / _rhs, y / _rhs, z / _rhs, w / _rhs); }
	//!
	Quaternion& operator += (const Quaternion& _rhs) { x += _rhs.x, y += _rhs.y, z += _rhs.z, w += _rhs.w; return *this; }
	//!
	Quaternion& operator -= (const Quaternion& _rhs) { x -= _rhs.x, y -= _rhs.y, z -= _rhs.z, w -= _rhs.w; return *this; }
	//!
	Quaternion& operator *= (const Quaternion& _rhs) { return Mul(_rhs); }
	//!
	Quaternion& operator *= (float _rhs) { x *= _rhs, y *= _rhs, z *= _rhs, w *= _rhs; return *this; }
	//!
	Quaternion& operator /= (float _rhs) { x += _rhs, y += _rhs, z += _rhs, w += _rhs; return *this; }
	//!
	friend Quaternion operator * (float _lhs, const Quaternion& _rhs) { return Quaternion(_lhs * _rhs.x, _lhs * _rhs.y, _lhs * _rhs.z, _lhs * _rhs.w); }
	//!
	friend Vector3 operator * (const Vector3& _lhs, const Quaternion& _rhs) { return _rhs.Transform(_lhs); }

	//!
	Quaternion& Mul(const Quaternion& _rhs);
	//!
	Vector3 Transform(const Vector3& _v) const;
	//!
	float Dot(const Quaternion& _q) const { return x * _q.x + y * _q.y + z * _q.z + w * _q.w; }
	//!
	Quaternion& Normalize(void);
	//!
	Quaternion& Inverse(void);
	//!
	Quaternion& UnitInverse(void) { x = -x, y = -y, z = -z; return *this; }
	//!
	Quaternion Nlerp(const Quaternion& _q, float _t, bool _shortestPath = false) const;
	//!
	Quaternion Slerp(const Quaternion& _q, float _t, bool _shortestPath = false) const;
	//!
	void ToRotationMatrix(float* _r0, float* _r1, float* _r2) const;
	//!
	Quaternion& FromRotationMatrix(const float* _r0, const float* _r1, const float* _r2);
	//!
	Quaternion& FromAxisAngle(const Vector3& _axis, float _angle);
	//!
	Quaternion& FromLookRotation(const Vector3& _dir, const Vector3& _up = Vector3::UnitY);
	//!
	Quaternion& FromRotationTo(const Vector3& _start, const Vector3& _end);

	//!
	float Yaw(void) const;
	//!
	float Pitch(void) const;
	//!
	float Roll(void) const;

	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};

	//!
	static const Quaternion Zero;
	//!
	static const Quaternion Identity;
};

//!
inline Quaternion Mix(const Quaternion& _a, const Quaternion& _b, float _t) { return _a.Slerp(_b, _t); }

//----------------------------------------------------------------------------//
// Matrix24
//----------------------------------------------------------------------------//

//! Row-major matrix 2x4 (for OpenGL 2x2, 2x3 and 2x4 matrices in uniform blocks)
struct Matrix24
{
	Vector4 r[2];
};

//----------------------------------------------------------------------------//
// Matrix34
//----------------------------------------------------------------------------//

//! Row-major matrix 3x4 (for OpenGL 3x3 and 3x4 matrices in uniform blocks)
struct Matrix34
{
	Vector4 r[3];
};

//----------------------------------------------------------------------------//
// Matrix44
//----------------------------------------------------------------------------//

//! Row-major matrix 4x4
struct Matrix44
{
	//!
	Matrix44(void) = default;
	//!
	explicit Matrix44(float _val) { SetIdentity(_val); }
	//!
	explicit Matrix44(const float* _m44) { FromPtr(_m44); }
	//!
	Matrix44(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33);

	//!
	operator Matrix34& (void) { return *(Matrix34*)v; }
	//!
	operator const Matrix34& (void) const { return *(Matrix34*)v; }

	//!
	Matrix44 Copy(void) const { return *this; }
	//!
	Matrix44& FromPtr(const float* _m44);
	//!
	Matrix44& Set(const Matrix44& _other) { return *this = _other; }
	//!
	Matrix44& Set(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33);
	//!
	Matrix44& SetDiagonal(float _val) { m00 = _val, m11 = _val, m22 = _val, m33 = _val; return *this; }
	//!
	Matrix44& SetZero(void) { for (int i = 0; i < 16; ++i) v[i] = 0; return *this; }
	//!
	Matrix44& SetIdentity(float _val = 1) { return SetZero().SetDiagonal(_val); }

	//!
	Vector3& Row(int _r) { return *(Vector3*)(m[_r]); }
	//!
	const Vector3& Row(int _r) const { return *(Vector3*)(m[_r]); }

	//!
	float* operator [] (int _r) { return m[_r]; }
	//!
	const float* operator [] (int _r) const { return m[_r]; }
	//!
	const float* operator * (void) const { return v; }
	//!
	float* operator * (void) { return v; }

	//!
	Vector3 Translate(const Vector3& _v) const { return Vector3(_v.x + m03, _v.y + m13, _v.z + m23); }
	//!
	Vector3 Transform(const Vector3& _v) const;
	//!
	Vector3 TransformVectorAbs(const Vector3& _v) const;
	//!
	Vector3 TransformVector(const Vector3& _v) const;
	//!
	Vector3 TransformProj(const Vector3& _v) const;
	//!
	Vector4 Transform(const Vector4& _v) const;
	//!
	friend Vector3 operator * (const Vector3& _lhs, const Matrix44& _rhs) { return _rhs.TransformProj(_lhs); }
	//!
	friend Vector4 operator * (const Vector4& _lhs, const Matrix44& _rhs) { return _rhs.Transform(_lhs); }

	//!
	Matrix44& Add(const Matrix44& _rhs) { for (int i = 0; i < 16; ++i) v[i] += _rhs.v[i]; return *this; }
	//!
	Matrix44 operator + (const Matrix44& _rhs) const { return Copy().Add(_rhs); }
	//!
	Matrix44& operator += (const Matrix44& _rhs) { return Add(_rhs); }

	//!
	Matrix44& Mul(float _rhs) { for (int i = 0; i < 16; ++i) v[i] *= _rhs; return *this; }
	//!
	Matrix44 operator * (float _rhs) const { return Copy().Mul(_rhs); }
	//!
	Matrix44& operator *= (float _rhs) { return Mul(_rhs); }

	//!
	Matrix44& Mul(const Matrix44& _rhs);
	//!
	Matrix44 operator * (const Matrix44& _rhs) const { return Copy().Mul(_rhs); }
	//!
	Matrix44& operator *= (const Matrix44& _rhs) { return Mul(_rhs); }

	//!
	bool IsAffine(void) const { return m30 == 0 && m31 == 0 && m32 == 0 && m33 == 1; }

	//!
	Vector3 GetTranslation(void) const { return Vector3(m03, m13, m23); }
	//!
	Matrix44& SetTranslation(const Vector3& _t) { m03 = _t.x, m13 = _t.y, m23 = _t.z; return *this; }
	//!
	Matrix44& CreateTranslation(const Vector3& _t) { return SetIdentity().SetTranslation(_t); }

	//!
	Vector3 GetScale(void) const;
	//!
	Matrix44& SetScale(const Vector3& _s) { return SetRotationScale(GetRotation(), _s); }
	//!
	Matrix44& CreateScale(const Vector3& _s);

	//!
	Quaternion GetRotation(void) const;
	//!
	Matrix44& SetRotation(const Quaternion& _r) { return SetRotationScale(_r, GetScale()); }
	//!
	Matrix44& CreateRotation(const Quaternion& _r) { return SetIdentity().SetRotationScale(_r, 1); }

	//!
	Matrix44& SetRotationScale(const Quaternion& _r, const Vector3& _s);
	//!
	Matrix44& CreateRotationScale(const Quaternion& _r, const Vector3& _s) { return SetIdentity().SetRotationScale(_r, _s); }
	//!
	Matrix44& CreateTransform(const Vector3& _t, const Quaternion& _r, const Vector3& _s = 1);
	//!
	Matrix44& CreateInverseTransform(const Vector3& _t, const Quaternion& _r, const Vector3& _s = 1);

	//!
	Matrix44& Inverse(void);
	//!
	Matrix44& Transpose(void);

	//!
	Vector3 Right(void) const { return Vector3(m00, m01, m02); }
	//!
	Vector3 Up(void) const { return Vector3(m10, m11, m12); }
	//!
	Vector3 Forward(void) const { return Vector3(m20, m21, m22); }


	//!
	Matrix44& CreateOrtho(float _left, float _right, float _bottom, float _top, float _near, float _far);
	//!
	Matrix44& CreateProjection(float _fov, float _aspect, float _near, float _far);

	union
	{
		float m[4][4]; // [row][column]
		float v[16];
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};

	//!
	static const Matrix44 Zero;
	//!
	static const Matrix44 Identity;
	//!
	static const Matrix44 Flip;
};

//----------------------------------------------------------------------------//
// Ray 
//----------------------------------------------------------------------------//

//!
struct Ray
{
	//!
	Ray(void) : origin(Vector3::Zero), dir(Vector3::UnitZ) { }
	//!
	Ray(const Vector3& _origin, const Vector3& _dir) : origin(_origin), dir(_dir) { }
	//!
	Vector3 Point(float _t) const { return origin + dir * _t; }
	//!
	Vector3 operator * (float _t) const { return Point(_t); }
	//!
	friend Vector3 operator * (float _t, const Ray& _ray) { return _ray.Point(_t); }

	//!
	Vector3 origin;
	//!
	Vector3 dir;
};

//----------------------------------------------------------------------------//
// Plane
//----------------------------------------------------------------------------//

//!
struct Plane
{
	//!
	Plane(void) : normal(Vector3::Zero), dist(0) { }
	//!
	Plane(float _nx, float _ny, float _nz, float _d) : normal(_nx, _ny, _nz), dist(_d) { }
	//!
	Plane(const Vector3& _n, float _d) : normal(_n), dist(_d) { }

	//!
	Plane Copy(void) const { return *this; }

	//!
	Plane& Set(const Plane& _other) { return *this = _other; }
	//!
	Plane& Set(const Vector4& _v) { return Set(_v.x, _v.y, _v.z, Abs(_v.w)); }
	//!
	Plane& Set(float _nx, float _ny, float _nz, float _d) { normal.Set(_nx, _ny, _nz); dist = _d; return *this; }
	//!
	Plane& Set(const Vector3& _normal, float _distance) { normal = _normal, dist = _distance; return *this; }

	//!
	Plane& FromNormalPoint(const Vector3& _normal, const Vector3& _point) { return Set(_normal, -_normal.AbsDot(_point)); }
	//!
	Plane& FromTriangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2) { return FromNormalPoint((_v1 - _v0).Cross(_v2 - _v0), _v0); }

	//!
	Vector4& AsVec4(void) { return *(Vector4*)&normal; }
	//!
	const Vector4& AsVec4(void) const { return *(const Vector4*)&normal; }

	//!
	Plane& Normalize(void);
	//!
	Plane& Transform(const Matrix44& _m) { return Set(AsVec4() * _m.Copy().Inverse().Transpose()); }

	//!
	Plane operator * (const Matrix44& _m) const { return Copy().Transform(_m); }
	//!
	Plane& operator *= (const Matrix44& _m) { return Transform(_m); }

	//!
	float Distance(const Vector3& _point) const { return normal.Dot(_point) + dist; }
	//!
	float Distance(const Vector3& _center, float _radius) const;
	//!
	float Distance(const Ray& _ray) const;
	//!
	Vector3 Point(const Ray& _ray)	const { return _ray * Distance(_ray); }

	//!
	Vector3 Reflect(const Vector3& _dir) const { return _dir - (2.f * normal.Dot(_dir) * normal); }
	//!
	Matrix44 GetReflectionMatrix(void) const;

	//!
	Vector3 normal;
	//!
	float dist;
};

//----------------------------------------------------------------------------//
// Rect
//----------------------------------------------------------------------------//

//!
struct Rect
{
	//!
	Rect(void) = default;
	//!
	Rect(float _left, float _top, float _right, float _bottom) : left(_left), top(_top), right(_right), bottom(_bottom) { }

	//!
	float Width(void) const { return right - left; }
	//!
	float Height(void) const { return bottom - top; }
	//!
	const Vector2 Size(void) const { return Vector2(Width(), Height()); }
	//!
	Rect& SetSize(const Vector2& _size) { return SetSize(_size.x, _size.y); }
	//!
	Rect& SetSize(float _width, float _height) { right = left + _width, bottom = top + _height; return *this; }

	//!
	float AbsWidth(void) const { return Abs(right - left); }
	//!
	float AbsHeight(void) const { return Abs(bottom - top); }
	//!
	Vector2 AbsSize(void) const { return Vector2(AbsWidth(), AbsHeight()); }

	//!
	const Vector2& Origin(void) const { return *reinterpret_cast<const Vector2*>(&left); }
	//!
	Rect& SetOrigin(const Vector2& _pos) { return SetOrigin(_pos.x, _pos.y); }
	//!
	Rect& SetOrigin(float _left, float _top);

	//!
	const Vector2& Position(void) const { return *reinterpret_cast<const Vector2*>(&left); }
	//!
	Vector2& Position(void) { return *reinterpret_cast<Vector2*>(&left); }

	//!
	const Vector2& Position2(void) const { return *reinterpret_cast<const Vector2*>(&right); }
	//!
	Vector2& Position2(void) { return *reinterpret_cast<Vector2*>(&right); }

	//!
	Vector2 HalfSize(void) const { return Size() * .5f; }
	//!
	Vector2 Center(void) const { return Position() + HalfSize(); }

	//!
	bool IsValid(void) const { return left <= right && top <= bottom; }

	//!
	Rect operator + (const Vector2& _rhs) const { return Rect(left + _rhs.x, top + _rhs.y, right + _rhs.x, bottom + _rhs.y); }
	//!
	friend Rect operator + (const Vector2& _lhs, const Rect& _rhs) { return _rhs + _lhs; }
	//!
	Rect operator - (const Vector2& _rhs) const { return Rect(left - _rhs.x, top - _rhs.y, right - _rhs.x, bottom - _rhs.y); }
	//!
	friend Rect operator - (const Vector2& _lhs, const Rect& _rhs) { return _rhs - _lhs; }
	//!
	Rect operator * (const Vector2& _scale) const { return Rect(left * _scale.x, top * _scale.y, right * _scale.x, bottom * _scale.y); }
	//!
	friend Rect operator * (const Vector2& _lhs, const Rect& _rhs) { return _rhs * _lhs; }

	//!
	Rect operator * (const Rect& _rhs) { return Rect(left * _rhs.left, top * _rhs.top, right * _rhs.right, bottom * _rhs.bottom); }

	//!
	bool operator == (const Rect& _rhs) const { return left == _rhs.left && top == _rhs.top && right == _rhs.right && bottom == _rhs.bottom; }
	//!
	bool operator != (const Rect& _rhs) const { return !(*this == _rhs); }

	//!
	float left = 0;
	//!
	float top = 0;
	//!
	float right = 0;
	//!
	float bottom = 0;

	//!
	static const Rect Zero;
	//!
	static const Rect Identity;
};

//----------------------------------------------------------------------------//
// IntRect
//----------------------------------------------------------------------------//

//!
struct IntRect
{
	//!
	IntRect(void) = default;
	//!
	IntRect(int _left, int _top, int _right, int _bottom) : left(_left), top(_top), right(_right), bottom(_bottom) { }
	//!
	IntRect(const Rect& _rect) : left((int)_rect.left), top((int)_rect.top), right((int)_rect.right), bottom((int)_rect.bottom) { }
	//!
	IntRect(const IntVector2& _origin, const IntVector2& _size) : left(_origin.x), top(_origin.y), right(_origin.x + _size.x), bottom(_origin.y + _size.y) { }

	//!
	operator Rect (void) const { return Rect((float)left, (float)top, (float)right, (float)bottom); }
	//!
	IntRect& operator = (const Rect& _rect) { left = (int)_rect.left, top = (int)_rect.top, right = (int)_rect.right, bottom = (int)_rect.bottom; return *this; }

	//!
	int Width(void) const { return right - left; }
	//!
	int Height(void) const { return bottom - top; }
	//!
	IntVector2 Size(void) const { return IntVector2(Width(), Height()); }
	//!
	IntRect& SetSize(const IntVector2& _size) { return SetSize(_size.x, _size.y); }
	//!
	IntRect& SetSize(int _width, int _height) { right = left + _width, bottom = top + _height; return *this; }

	//!
	int AbsWidth(void) const { return Abs(right - left); }
	//!
	int AbsHeight(void) const { return Abs(bottom - top); }
	//!
	IntVector2 AbsSize(void) const { return IntVector2(AbsWidth(), AbsHeight()); }
	//!
	bool IsFinite(void) const { return left <= right && top <= bottom; }
	//!
	const IntVector2& Origin(void) const { return *reinterpret_cast<const IntVector2*>(&left); }
	//!
	IntRect& SetOrigin(const IntVector2& _pos) { return SetOrigin(_pos.x, _pos.y); }
	//!
	IntRect& SetOrigin(int _left, int _top);

	//!
	const IntVector2& Position(void) const { return *reinterpret_cast<const IntVector2*>(&left); }
	//!
	IntVector2& Position(void) { return *reinterpret_cast<IntVector2*>(&left); }

	//!
	const IntVector2& Position2(void) const { return *reinterpret_cast<const IntVector2*>(&right); }
	//!
	IntVector2& Position2(void) { return *reinterpret_cast<IntVector2*>(&right); }

	//!
	IntRect Overlay(const IntRect& _r2);

	//!
	bool operator == (const IntRect& _rhs) const { return left == _rhs.left && top == _rhs.top && right == _rhs.right && bottom == _rhs.bottom; }
	//!
	bool operator != (const IntRect& _rhs) const { return !(*this == _rhs); }

	//!
	int left = 0;
	//!
	int top = 0;
	//!
	int right = 0;
	//!
	int bottom = 0;

	//!
	static const IntRect Zero;
};

//----------------------------------------------------------------------------//
// Sphere
//----------------------------------------------------------------------------//

//!
struct Sphere
{
	//!
	Sphere(void) = default;
	//!
	Sphere(const Vector3& _center, float _radius) : center(_center), radius(_radius) {}

	//!
	Vector3 center = Vector3::Zero;
	//!
	float radius = 0;
};

//----------------------------------------------------------------------------//
// Box
//----------------------------------------------------------------------------//

//!
struct Box
{
	enum Corner
	{
		LeftBottomFar = 0,// min
		RightBottomFar,
		RightBottomNear,
		LeftBottomNear,
		RightTopNear, // max
		LeftTopNear,
		LeftTopFar,
		RightTopFar,
	};

	//!
	static const unsigned LineIndices[24];
	//!
	static const unsigned QuadIndices[24];
	//!
	static const unsigned TriangleIndices[36];
};

//----------------------------------------------------------------------------//
// AlignedBox
//----------------------------------------------------------------------------//

//!
struct AlignedBox
{
	//!
	AlignedBox(void) : mn(999999.9f), mx(-999999.9f) { }
	//!
	AlignedBox(const Vector3& _min, const Vector3& _max) : mn(_min), mx(_max) { }

	//!
	AlignedBox Copy(void) const { return *this; }

	//!
	AlignedBox& Set(const AlignedBox& _b) { return *this = _b; }
	//!
	AlignedBox& Set(const Vector3& _min, const Vector3& _max) { mn = _min, mx = _max; return *this; }
	//!
	AlignedBox& SetMinMax(const Vector3& _a, const Vector3& _b) { mn.SetMin(_a, _b), mx.SetMax(_a, _b); return *this; }
	//!
	AlignedBox& SetZero(void) { mn = Vector3::Zero, mx = Vector3::Zero; return *this; }
	//!
	AlignedBox& FromCenterExtends(const Vector3& _center, const Vector3& _extends) { mn = _center - _extends, mx = _center + _extends; return *this; }
	//!
	AlignedBox& FromViewProjMatrix(const Matrix44& _m) { return *this = AlignedBox(-1, 1).TransformProj(_m); }

	//!
	AlignedBox& Reset(const Vector3& _pt) { return Set(_pt, _pt); }
	//!
	AlignedBox& Reset(void) { mn = 999999.9f, mx = -999999.9f; return *this; }
	//!
	AlignedBox& AddPoint(const Vector3& _pt) { mn.SetMin(_pt), mx.SetMax(_pt); return *this; }
	//!
	AlignedBox& AddVertices(const void* _src, int _count, size_t _stride = 0, size_t _offset = 0);

	//!
	bool IsZero(void) const { return mn == mx && mn == Vector3::Zero; }
	//!
	bool IsFinite(void) const { return mn.x <= mx.x && mn.y <= mx.y && mn.z <= mx.z; }

	//!
	Vector3 Size(void) const { return mx - mn; }
	//!
	Vector3 Extends(void) const { return (mx - mn) * 0.5f; }
	//!
	Vector3 Center(void) const { return (mx + mn) * 0.5f; }
	//!
	operator Sphere (void) const { return Sphere(Center(), Radius()); }
	//!
	float Diagonal(void) const { return (mx - mn).Length(); }
	//!
	float DiagonalSq(void) const { return (mx - mn).LengthSq(); }
	//!
	float Radius(void) const { return Diagonal() * 0.5f; }
	//!
	float RadiusSq(void) const { return DiagonalSq() * 0.5f; }
	//!
	float Width(void) const { return mx.x - mn.x; }
	//!
	float Height(void) const { return mx.y - mn.y; }
	//!
	float Depth(void) const { return mx.z - mn.z; }
	//!
	float Volume(void) const { return (mx - mn).LengthSq(); }
	//!
	void GetAllCorners(const void* _dst, size_t _stride = 0, size_t _offset = 0) const;

	// [for dbvt]
	//!
	float Proximity(const AlignedBox& _other) const { return ((mn + mx) - (_other.mn + _other.mx)).AbsSum(); }
	//!
	int Select(const AlignedBox& _a, const AlignedBox& _b) const { return Proximity(_a) < Proximity(_b) ? 0 : 1; }

	//!
	AlignedBox& TransformAffine(const Matrix44& _rhs) { return FromCenterExtends(Center() * _rhs, _rhs.TransformVectorAbs(Extends())); }
	//!
	AlignedBox& TransformProj(const Matrix44& _rhs);

	//!
	AlignedBox operator + (const Vector3& _point) const { return Copy().AddPoint(_point); }
	//!
	AlignedBox operator + (const AlignedBox& _box) const { return Copy().AddPoint(_box.mn).AddPoint(_box.mx); }
	//!
	AlignedBox& operator += (const Vector3& _point) { return AddPoint(_point); }
	//!
	AlignedBox& operator += (const AlignedBox& _box) { return AddPoint(_box.mn).AddPoint(_box.mx); }
	//!
	AlignedBox& operator *= (const Matrix44& _rhs) { return TransformAffine(_rhs); }
	//!
	AlignedBox operator * (const Matrix44& _rhs) const { return Copy().TransformAffine(_rhs); }

	//!
	bool operator == (const AlignedBox& _rhs) const { return mn == _rhs.mn && mx == _rhs.mx; }
	//!
	bool operator != (const AlignedBox& _rhs) const { return !(*this == _rhs); }

	//!
	bool Contains(const Vector3& _point) const { return _point >= mn && _point <= mx; }
	//!
	bool Contains(const AlignedBox& _box) const { return _box.mn >= mn && _box.mx <= mx; }
	//!
	bool Intersects(const AlignedBox& _box) const { return !(_box.mx < mn || _box.mn > mx); }

	//!
	Vector3 mn;
	//!
	Vector3 mx;
};

//----------------------------------------------------------------------------//
// OrientedBox
//----------------------------------------------------------------------------//

//!
struct OrientedBox
{

};

//----------------------------------------------------------------------------//
// Frustum
//----------------------------------------------------------------------------//

//!
struct Frustum
{
	//!
	enum PlaneID
	{
	   Left = 0,
	   Right,
	   Bottom,
	   Top,
	   Near,
	   Far
	};

	//!
	Frustum(void) = default;

	//!
	Frustum& FromCameraMatrices(const Matrix44& _view, const Matrix44& _proj);

	//!
	bool Intersects(const Vector3& _point) const;
	//!
	bool Intersects(const Vector3& _center, float _radius) const;
	//!
	bool Intersects(const AlignedBox& _box, bool* _contains = nullptr) const;
	//!
	bool Intersects(const Frustum& _frustum, bool* _contains = nullptr) const;

	//!
	float Distance(const Vector3& _point) const { return origin.Distance(_point); }
	//!
	float Distance(const Vector3& _center, float _radius) const { float _d = origin.Distance(_center); return _d < _radius ? 0 : _d - _radius; }
	//!
	float Distance(const AlignedBox& _box) const { return Distance(_box.Center(), _box.Radius()); }

	//!
	static void GetPlanes(Plane* _planes, const Matrix44& _m);

	//!
	Plane planes[6];
	//!
	Vector3 corners[8];
	//!
	Vector3 origin;
	//!
	AlignedBox box;
};

//----------------------------------------------------------------------------//
// Triangle
//----------------------------------------------------------------------------//

//!
struct Triangle
{
	//!
	//Triangle& Create(const Vector3& _a, const Vector3& _b, const Vector3& _c);

	//!
	//bool Intersects(Triangle* _other, const Matrix44& _matrix);

	//!
	static Vector3 Normal(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2) { return (_v1 - _v0).Cross(_v2 - _v0); }

	//!
	static bool Intersects(const Vector3& _t0, const Vector3& _t1, const Vector3& _t2, const Ray& _ray, float* _dist = nullptr);
	
	//!
	//Vector3 v[3]; // A, B, C
	//!
	//Vector3 ray[3]; // AB, BC, CA
	//!
	//Plane plane;
};

//----------------------------------------------------------------------------//
// DbvtNode
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// DbvtCallback
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// Dbvt
//----------------------------------------------------------------------------//

//! Dynamic Bounding-Volumes Tree (BVH)
class Dbvt
{
public:

	//!
	struct Node
	{
		//!
		Node(void) : child0(nullptr), child1(nullptr) { }

		//!
		bool IsLeaf(void) const { return child[1] == 0; }
		//!
		bool IsNode(void) const { return child[1] != 0; }
		//!
		bool IsValidLeaf(void) const { return IsLeaf() && object && box.IsFinite(); }
		//!
		AlignedBox GetChildBounds(void) { return IsLeaf() ? box : child0->GetChildBounds() + child1->GetChildBounds(); }

		//!
		AlignedBox box;
		//!
		Node* parent = nullptr;
		//!
		union
		{
			Node* child[2];
			struct
			{
				Node* child0;
				Node* child1;
			};
			void* object;
			int value;
		};
	};

	struct Callback
	{
		enum TestResult
		{
			Stop = 0, // stop callback
			Skip, // skip node
			WithTest, // add all leafs in node with test
			WithoutTest, // add all leafs in node without test
		};

		//!
		virtual TestResult TestNode(Node* _node) { return WithoutTest; }
		//!
		virtual void AddLeaf(Node* _leaf, TestResult _testResult) { }
	};

	//!
	Dbvt(void) = default;
	//!
	~Dbvt(void) { _Clear(); }

	//!
	void Insert(Node* _leaf);
	//!
	void Remove(Node* _leaf);
	//!
	void Update(Node* _leaf);

	//!
	Node* Root(void) { return m_root; }
	//!
	void Enum(Callback* _cb, bool _withTest = true);

	// TODO: enum directional

		//!
	int GetMaxDepth(void);

	//!
	void _Clear(void);

protected:
	//!
	void _Insert(Node* _leaf, Node* _root);
	//!
	Node* _Remove(Node* _leaf);
	//!
	void _Delete(Node* _node);

	//!
	Node* m_root = nullptr;
	//! last deleted node
	Node* m_free = nullptr; 

private:
	//!
	Dbvt(const Dbvt&) = delete;
	//!
	Dbvt& operator = (const Dbvt&) = delete;
};

//----------------------------------------------------------------------------//
// Dbvt utils
//----------------------------------------------------------------------------//

struct DbvtCallbackContainer : Dbvt::Callback
{

	typedef bool(*CallbackFunc)(void* _object, const AlignedBox& _bbox, bool _withTest, void* _userData);

	void AddLeaf(Dbvt::Node* _leaf, TestResult _testResult) override
	{
		//ASSERT(_leaf != nullptr);
		stop = !callback(_leaf->object, _leaf->box, _testResult == WithTest, userData);
	}

	//Function<bool(void* _object, const AlignedBox& _bbox, bool _withTest, void* _userData)> callback;

	CallbackFunc callback;
	bool stop = false; //!< Stop callback. Do reset before query.

	void* userData = nullptr;
};

//!
struct DbvtFrustumCallback : DbvtCallbackContainer
{
	//!
	TestResult TestNode(Dbvt::Node* _node) override
	{
		if (stop)
			return Stop;

		bool _contains;
		if (volume.Intersects(_node->box, &_contains))
			return _contains ? WithoutTest : WithTest;
		return Skip;
	}

	Frustum volume;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
