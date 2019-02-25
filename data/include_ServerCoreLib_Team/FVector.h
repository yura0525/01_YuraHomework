#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>//std::setprecision
using namespace std;

/** Returns highest of 3 values */
template<class T>
static T Max3(const T A, const T B, const T C)
{
	return Max(Max(A, B), C);
}

/** Returns lowest of 3 values */
template<class T>
static T Min3(const T A, const T B, const T C)
{
	return Min(Min(A, B), C);
}

/** Multiples value by itself */
template<class T>
static T Square(const T A)
{
	return A * A;
}

/** Clamps X to be between Min and Max, inclusive */
template<class T>
static T Clamp(const T X, const T Min, const T Max)
{
	return (X < Min) ? Min : ((X < Max) ? X : Max);
}

class FVector
{
public:
	float m_fX, m_fY, m_fZ;

public:
	static const FVector ZeroVector;		/** A zero vector (0,0,0) */
	static const FVector OneVector;			/** One vector (1,1,1) */
	static const FVector UpVector;			/** World up vector (0,0,1) */
	static const FVector ForwardVector;		/** Unreal forward vector (1,0,0) */
	static const FVector RightVector;		/** Unreal right vector (0,1,0) */

public:
	FVector operator^(const FVector& V) const;
	FVector CrossProduct(const FVector& A, const FVector& B);
	float operator|(const FVector& V) const;
	float DotProduct(const FVector& A, const FVector& B);

	FVector operator+(const FVector& V) const;
	FVector operator-(const FVector& V) const;
	FVector operator*(const FVector& V) const;
	FVector operator/(const FVector& V) const;

	FVector operator+(float Bias) const;
	FVector operator-(float Bias) const;
	FVector operator*(float Scale) const;
	FVector operator/(float Scale) const;

	bool operator==(const FVector& V) const;
	bool operator!=(const FVector& V) const;
	bool Equals(const FVector& V, float Tolerance) const;
	bool AllComponentsEqual(float Tolerance) const;

	FVector operator-() const;
	FVector operator+=(const FVector& V);
	FVector operator-=(const FVector& V);
	FVector operator*=(const FVector& V);
	FVector operator/=(const FVector& V);

	FVector operator*=(float Scale);
	FVector operator/=(float V);

	float& operator[](int Index);
	float operator[](int Index) const;

	void Set(float fX, float fY, float fZ);
	float Size() const;
	float SizeSquared() const;
	bool IsZero() const;
	bool Normalize(float Tolerance);

	float Dist(const FVector& V1, const FVector& V2);
	float DistanceTo(FVector vDest);
	float DistSquared(const FVector& V1, const FVector& V2);

public:
	FVector()
	{
		m_fX = m_fY = m_fZ = 0.0f;
	}
	FVector(float fX, float fY, float fZ)
	{
		m_fX = fX;
		m_fY = fY;
		m_fZ = fZ;
	}
	~FVector();
};

class FRotator
{
public:
	float m_fX, m_fY, m_fZ;
	static const FRotator ZeroRotator;
public:
	FRotator()
	{
		m_fX = m_fY = m_fZ = 0.0f;
	}
	FRotator(float fx, float fy, float fz)
	{
		m_fX = fx;
		m_fY = fy;
		m_fZ = fz;
	}
};

static ostream& operator<<(ostream& stream, const FVector& vector)
{
	stream.write(reinterpret_cast<const char*>(&vector), sizeof(FVector));
	return stream;
}

static istream& operator>>(istream& stream, FVector& vector)
{
	stream.read(reinterpret_cast<char*>(&vector), sizeof(FVector));
	return stream;
}

static ostream& operator<<(ostream& stream, const FRotator& vector)
{
	stream.write(reinterpret_cast<const char*>(&vector), sizeof(FRotator));
	return stream;
}

static istream& operator>>(istream& stream, FRotator& vector)
{
	stream.read(reinterpret_cast<char*>(&vector), sizeof(FRotator));
	return stream;
}
