#ifndef _GEOM_H_
#define _GEOM_H_

#include <math.h>
// in this file Mat44, Vec2, Vec3, Vec4

struct Vec2 {
	float x,y;
	Vec2( float _x, float _y) : x(_x), y(_y) {}
	Vec2() {x = 0.0f; y = 0.0f;}
	Vec2 operator+(const Vec2 &other) const { return Vec2(x+other.x, y+other.y); }
	Vec2 operator-(const Vec2 &other) const { return Vec2(x-other.x, y-other.y); }
	Vec2 operator*(const float &other) const { return Vec2(x*other, y*other); }
	Vec2 operator/(const float &other) const { return Vec2(x/other, y/other); }
	Vec2& operator+=(const Vec2 &other) { x+=other.x; y+=other.y; return *this; }
	Vec2& operator-=(const Vec2 &other) { x-=other.x; y-=other.y; return *this; }
	float dot(const Vec2 &other) const { return x*other.x + y*other.y;}
	float abs() const { return sqrtf(dot(*this)); }
	Vec2 normalized() const { return *this * (1.0f / abs()); }
	void normalize() { float mod = 1.0f / abs(); x *= mod; y *= mod; }
	Vec2 operator-() const { return Vec2(-x, -y); }
};

struct Vec3 {
	float x,y,z;
	Vec3( float _x, float _y, float _z ) : x(_x), y(_y), z(_z) {}
	Vec3( const Vec2 v ) : x(v.x), y(v.y), z(0) {}
	Vec3() {}
	Vec3 operator+(const Vec3 &other) const { return Vec3(x+other.x, y+other.y, z+other.z); }
	Vec3 operator-(const Vec3 &other) const { return Vec3(x-other.x, y-other.y, z-other.z); }
	Vec3 operator*(const float &other) const { return Vec3(x*other, y*other, z*other); }
	Vec3 operator/(const float &other) const { return Vec3(x/other, y/other, z/other); }
	Vec3& operator+=(const Vec3 &other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
	Vec3& operator-=(const Vec3 &other) { x-=other.x; y-=other.y; z-=other.z; return *this; }
	float dot(const Vec3 &other) const { return x*other.x + y*other.y + z*other.z; }
	float abs() const { return sqrtf(dot(*this)); }
	Vec3 cross(const Vec3 &other) {
		return Vec3( y*other.z - z*other.y,
				z*other.x - x*other.z,
				x*other.y - y*other.x );
	}
	Vec3 crossY() const { return Vec3( z, 0, -x ); }
	Vec3 normalized() const { return *this * (1.0f / abs()); }
	void normalize() { float mod = 1.0f / abs(); x *= mod; y *= mod; z *= mod; }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }
	Vec2 xy() { return Vec2( x, y ); }
};

struct Vec4 {
	float x,y,z,w;
	Vec4( float _x, float _y, float _z, float _w ) : x(_x), y(_y), z(_z), w(_w) {}
	Vec4( const Vec3 &v ) : x(v.x), y(v.y), z(v.z), w(1.0f) {}
	Vec4() {}
	Vec4 operator+(const Vec4 &other) const { return Vec4(x+other.x, y+other.y, z+other.z, w+other.w); }
	Vec4 operator-(const Vec4 &other) const { return Vec4(x-other.x, y-other.y, z-other.z, w-other.w); }
	Vec4 operator*(const float &other) const { return Vec4(x*other, y*other, z*other, w*other); }
	Vec4& operator*=(const float &other) { *this = Vec4(x*other, y*other, z*other, w*other); return *this; }
	Vec4 operator/(const float &other) const { return Vec4(x/other, y/other, z/other, w/other); }
	operator Vec3() const { return Vec3( x, y, z ); }
};

struct Mat44 {
	Vec4 x,y,z,w;
	Mat44( Vec3 _x, Vec3 _y, Vec3 _z, Vec3 _w ) : x(_x), y(_y), z(_z), w(_w) {}
	Mat44( Vec4 _x, Vec4 _y, Vec4 _z, Vec4 _w ) : x(_x), y(_y), z(_z), w(_w) {}
	Mat44() {}
	Mat44 RotPart() { return Mat44( x,y,z, Vec4(0,0,0,1) ); }
	void SetIdentity() { *this = Mat44( Vec4(1,0,0,0), Vec4(0,1,0,0), Vec4(0,0,1,0), Vec4(0,0,0,1) ); }
	void Translate(const Vec3 v) { w = Vec4(v+w); }
	void Rotation(float rotation, const Vec3 axis) {
		const float c = cos(rotation), s = sin(rotation);
		const float X = axis.x, Y = axis.y, Z = axis.z;
		x = Vec4( X*X+(1-X*X)*c, X*Y*(1-c)+Z*s, X*Z*(1-c)-Y*s, 0 );
		y = Vec4( X*Y*(1-c)-Z*s, Y*Y+(1-Y*Y)*c, Y*Z*(1-c)+X*s, 0 );
		z = Vec4( X*Z*(1-c)+Y*s, Y*Z*(1-c)-X*s, Z*Z+(1-Z*Z)*c, 0 );
	}
	void RotX(float rotation) {
		const float c = cos(rotation), s = sin(rotation);
		x = Vec4( 1, 0, 0, 0 );
		y = Vec4( 0, c, s, 0 );
		z = Vec4( 0, -s, c, 0 );
	}
	void RotY(float rotation) {
		const float c = cos(rotation), s = sin(rotation);
		x = Vec4( c, 0, -s, 0 );
		y = Vec4( 0, 1, 0, 0 );
		z = Vec4( s, 0, c, 0 );
	}
	void RotZ(float rotation) {
		const float c = cos(rotation), s = sin(rotation);
		x = Vec4( c, s, 0, 0 );
		y = Vec4( -s, c, 0, 0 );
		z = Vec4( 0, 0, 1, 0 );
	}
	void Scale(float scale) {
		x *= scale;
		y *= scale;
		z *= scale;
	}
	operator float*() { return &x.x; }
	operator const float*() const { return &x.x; }
};

// Vec2 operations
inline float dot( const Vec2 a, const Vec2 b ) { return a.dot(b); }
inline Vec2 operator*( float a, const Vec2 b ) { return b*a; }

// Vec3 operations
inline float dot( const Vec3 a, const Vec3 b ) { return a.dot(b); }
inline Vec3 operator*( float a, const Vec3 b ) { return b*a; }

// Vec4 operations
inline Vec3 RayHitsPlaneAt( Vec3 start, Vec3 ray, Vec4 plane ) {
	Vec3 normal = plane;
	const float posDist = normal.dot( start ) - plane.w;
	const float rayClosing = normal.dot( ray );
	const float rayAmplification = posDist / rayClosing;
	const Vec3 hittingRay = ray * rayAmplification;
	return start + hittingRay;
}


// Mat44 operations
inline Mat44 Mat44Perspective(float fFovRad, float fAspect, float fZNear, float fZFar) {
	float fYMax = fZNear * tanf(fFovRad*0.5f);
	float fXMax = fYMax * fAspect;
	float fDiv = 1.0f / (fZFar - fZNear);

	const Vec4 x( fZNear / fXMax, 0, 0, 0 );
	const Vec4 y( 0, fZNear / fYMax, 0, 0 );
	const Vec4 z( 0, 0, -(fZFar+fZNear) * fDiv, -1 );
	const Vec4 w( 0, 0, -2 * fZFar * fZNear * fDiv, 0 );
	return Mat44( x, y, z, w );
}

inline Mat44 Mat44Orthographic( float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar ) {
	const Vec4 x( 2.0f / (fRight - fLeft), 0, 0, 0 );
	const Vec4 y( 0, 2.0f / (fTop - fBottom), 0, 0 );
	const Vec4 z( 0, 0, -2.0f / (fFar - fNear), 0 );
	const Vec4 w(
			- ( fRight + fLeft ) / ( fRight - fLeft ),
			- ( fTop + fBottom ) / ( fTop - fBottom ),
			- ( fFar + fNear ) / ( fFar - fNear ),
			1.0f );

	return Mat44( x, y, z, w );
}
inline Mat44 Mat44LookAt( const Vec3& vPos, const Vec3& vFocus, const Vec3& vUpBase) {
	Vec3 vFwd = (vFocus - vPos).normalized();
	Vec3 vSide = vFwd.cross(vUpBase).normalized();
	Vec3 vUp = vSide.cross(vFwd);
	return Mat44(
			Vec4( vSide.x, vUp.x, -vFwd.x, 0 ),
			Vec4( vSide.y, vUp.y, -vFwd.y, 0 ),
			Vec4( vSide.z, vUp.z, -vFwd.z, 0 ),
			Vec4( -vSide.dot(vPos), -vUp.dot(vPos), vFwd.dot(vPos), 1.0f ) );
}

inline Mat44 operator *( const Mat44 &lhs, const Mat44 &rhs )
{
	return Mat44(
			Vec4(
				lhs.x.x * rhs.x.x + lhs.x.y * rhs.y.x + lhs.x.z * rhs.z.x + lhs.x.w * rhs.w.x,
				lhs.x.x * rhs.x.y + lhs.x.y * rhs.y.y + lhs.x.z * rhs.z.y + lhs.x.w * rhs.w.y,
				lhs.x.x * rhs.x.z + lhs.x.y * rhs.y.z + lhs.x.z * rhs.z.z + lhs.x.w * rhs.w.z,
				lhs.x.x * rhs.x.w + lhs.x.y * rhs.y.w + lhs.x.z * rhs.z.w + lhs.x.w * rhs.w.w ),
			Vec4( lhs.y.x * rhs.x.x + lhs.y.y * rhs.y.x + lhs.y.z * rhs.z.x + lhs.y.w * rhs.w.x,
				lhs.y.x * rhs.x.y + lhs.y.y * rhs.y.y + lhs.y.z * rhs.z.y + lhs.y.w * rhs.w.y,
				lhs.y.x * rhs.x.z + lhs.y.y * rhs.y.z + lhs.y.z * rhs.z.z + lhs.y.w * rhs.w.z,
				lhs.y.x * rhs.x.w + lhs.y.y * rhs.y.w + lhs.y.z * rhs.z.w + lhs.y.w * rhs.w.w ),
			Vec4( lhs.z.x * rhs.x.x + lhs.z.y * rhs.y.x + lhs.z.z * rhs.z.x + lhs.z.w * rhs.w.x,
				lhs.z.x * rhs.x.y + lhs.z.y * rhs.y.y + lhs.z.z * rhs.z.y + lhs.z.w * rhs.w.y,
				lhs.z.x * rhs.x.z + lhs.z.y * rhs.y.z + lhs.z.z * rhs.z.z + lhs.z.w * rhs.w.z,
				lhs.z.x * rhs.x.w + lhs.z.y * rhs.y.w + lhs.z.z * rhs.z.w + lhs.z.w * rhs.w.w ),
			Vec4( lhs.w.x * rhs.x.x + lhs.w.y * rhs.y.x + lhs.w.z * rhs.z.x + lhs.w.w * rhs.w.x,
				lhs.w.x * rhs.x.y + lhs.w.y * rhs.y.y + lhs.w.z * rhs.z.y + lhs.w.w * rhs.w.y,
				lhs.w.x * rhs.x.z + lhs.w.y * rhs.y.z + lhs.w.z * rhs.z.z + lhs.w.w * rhs.w.z,
				lhs.w.x * rhs.x.w + lhs.w.y * rhs.y.w + lhs.w.z * rhs.z.w + lhs.w.w * rhs.w.w )
			);
}
inline Vec4 operator *( const Mat44 &lhs, const Vec4 &rhs )
{
	return
		rhs.x * lhs.x +
		rhs.y * lhs.y +
		rhs.z * lhs.z +
		rhs.w * lhs.w;
}

inline Vec3 RayFromCam( const Mat44 &mat, Vec2 screenPos, float xscale, float yscale ) {
	return
		(
		 (screenPos.x*2-1) / xscale * mat.x +
		 (1-screenPos.y*2) / yscale * mat.y +
		 mat.z
		).normalized();
}

inline Mat44 Translation( Vec3 v ) {
	return Mat44( Vec4(1,0,0,0), Vec4(0,1,0,0), Vec4(0,0,1,0), Vec4(v) );
}

extern Vec3 gXVec3;
extern Vec3 gYVec3;
extern Vec3 gZVec3;
extern Vec4 gXVec4;
extern Vec4 gYVec4;
extern Vec4 gZVec4;
extern Vec4 gWVec4;
extern Mat44 gIdentityMat;

#endif
