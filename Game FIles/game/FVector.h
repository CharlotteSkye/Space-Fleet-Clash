// CFVector - a fast inline class

#ifndef __FVECTOR_H__
#define __FVECTOR_H__

class CFVector
{
public:
	double m_x;
	double m_y;

public:
	// Constructor
	CFVector(double x, double y)		{ Set(x, y); }
	CFVector(CFVector& pt)				{ Set(pt); }
	CFVector(CVector& pt)				{ Set(pt); }
	CFVector()							{ Set(0, 0); }

	// Destructor
	~CFVector()							{ }

	// Getters & Setters
	double& X()							{ return m_x; }
	double& Y()							{ return m_y; }
	double GetX() const					{ return m_x; }
	double GetY() const					{ return m_y; }

	void SetX(double x)					{ X() = x; }
	void SetY(double y)					{ Y() = y; }
	CFVector& Set(double x, double y)	{ X() = x; Y() = y; return(*this); }
	CFVector& Set(CFVector& pt)			{ X() = pt.X(); Y() = pt.Y(); return(*this); }
	CFVector& Set(CVector& pt)			{ X() = pt.X(); Y() = pt.Y(); return(*this); }
	CFVector& Copy(CFVector& pt)		{ return Set(pt); }
	CFVector& Copy(CVector& pt)			{ return Set(pt); }

	// Moving & Scaling
	CFVector& Offset(double dx, double dy){ X() += dx; Y() += dy; return (*this); }
	CFVector& Offset(CFVector& pt)		{ X() += pt.X(); Y() += pt.Y(); return (*this); }
	CFVector& Offset(CVector& pt)		{ X() += pt.X(); Y() += pt.Y(); return (*this); }
	CFVector& Add(CFVector& pt)			{ return Offset(pt); }								// the same as Offset
	CFVector& Add(CVector& pt)			{ return Offset(pt); }								// the same as Offset
	CFVector& Subtract(CFVector& pt)	{ X() -= pt.X(); Y() -= pt.Y(); return (*this); }
	CFVector& Subtract(CVector& pt)		{ X() -= pt.X(); Y() -= pt.Y(); return (*this); }
	CFVector& Scale(double scalar)		{ X() *= scalar; Y() *= scalar; return (*this); }
	CFVector& ScaleUp(double scalar)	{ return Scale(scalar); }							// the same as Scale
	CFVector& ScaleDown(double scalar)	{ X() /= scalar; Y() /= scalar; return (*this); }

	// Distance
	double Distance(CFVector& pt)
	{
		double x = X() - pt.X(), y = Y() - pt.Y(); 
		return (double)sqrt((float)(x*x + y*y));
	}

	// Length and normalisation
	double GetLength()						{ return sqrt(X() * X() + Y() * Y()); }
	CFVector& SetLength(double l)			{ double m = GetLength(); return operator *=(l / m); }
	CFVector& Normalise()					{ double l = GetLength(); return operator /=(l); }
	CFVector& Limit(double l)				{ if (GetLength() > l) return SetLength(l); else return *this; }

	// Operators

	// assignment
	CFVector& operator =(CFVector& pt)		{ return Copy(pt); }
	CFVector& operator =(CVector& pt)		{ return Copy(pt); }
	CFVector& operator +=(CFVector& pt)		{ return Add(pt); return (*this); }
	CFVector& operator +=(CVector& pt)		{ return Add(pt); return (*this); }
	CFVector& operator -=(CFVector& pt)		{ return Subtract(pt); }
	CFVector& operator -=(CVector& pt)		{ return Subtract(pt); }
	CFVector& operator *=(double scalar)	{ return ScaleUp(scalar); }
	CFVector& operator /=(double scalar)	{ return ScaleDown(scalar); }

	// unary
	CFVector operator -()					{ return CFVector(-X(), -Y()); } 
	CFVector operator +()					{ return CFVector(X(), Y()); } 

	// binary
	CFVector operator +(CFVector& pt)		{ CFVector res = *this; res += pt; return res; }
	CFVector operator +(CVector& pt)		{ CFVector res = *this; res += pt; return res; }
	CFVector operator -(CFVector& pt)		{ CFVector res = *this; res -= pt; return res; }
	CFVector operator -(CVector& pt)		{ CFVector res = *this; res -= pt; return res; }
	CFVector operator *(double scalar)		{ CFVector res = *this; res *= scalar; return res; }
	CFVector operator /(double scalar)		{ CFVector res = *this; res /= scalar; return res; }

	// comparison
	bool operator ==(CFVector pt)			{ return (X() == pt.X() && Y() == pt.Y()); }
	bool operator ==(CVector pt)			{ return (X() == pt.X() && Y() == pt.Y()); }
	bool operator !=(CFVector pt)			{ return (X() != pt.X() || Y() != pt.Y()); }
	bool operator !=(CVector pt)			{ return (X() != pt.X() || Y() != pt.Y()); }
};

#endif