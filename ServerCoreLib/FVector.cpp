#include "FVector.h"

const FVector FVector::ZeroVector(0.0f, 0.0f, 0.0f);
const FVector FVector::OneVector(1.0f, 1.0f, 1.0f);
const FVector FVector::UpVector(0.0f, 0.0f, 1.0f);
const FVector FVector::ForwardVector(1.0f, 0.0f, 0.0f);
const FVector FVector::RightVector(0.0f, 1.0f, 0.0f);
const FRotator FRotator::ZeroRotator(0.0f, 0.0f, 0.0f);

FVector FVector::operator^(const FVector& V) const
{
	return FVector(	m_fY * V.m_fZ - m_fZ * V.m_fY,
					m_fZ * V.m_fX - m_fX * V.m_fZ,
					m_fX * V.m_fY - m_fY * V.m_fX);
}
FVector FVector::CrossProduct(const FVector& A, const FVector& B)
{
	return A ^ B;
}
float	FVector::operator|(const FVector& V) const
{
	return (m_fX * V.m_fX) + (m_fY * V.m_fY) + (m_fZ * V.m_fZ);
}
float	FVector::DotProduct(const FVector& A, const FVector& B)
{
	return A | B;
}
FVector FVector::operator+(const FVector& V) const
{
	return FVector((m_fX + V.m_fX), (m_fY + V.m_fY), (m_fZ + V.m_fZ));
}
FVector FVector::operator-(const FVector& V) const
{
	return FVector((m_fX - V.m_fX), (m_fY - V.m_fY), (m_fZ - V.m_fZ));
}
FVector FVector::operator*(const FVector& V) const
{
	return FVector((m_fX * V.m_fX), (m_fY * V.m_fY), (m_fZ * V.m_fZ));
}
FVector FVector::operator/(const FVector& V) const
{
	return FVector((m_fX / V.m_fX), (m_fY / V.m_fY), (m_fZ / V.m_fZ));
}

FVector FVector::operator+(float Bias) const
{
	return FVector((m_fX + Bias), (m_fY + Bias), (m_fZ + Bias));
}
FVector FVector::operator-(float Bias) const
{
	return FVector((m_fX - Bias), (m_fY - Bias), (m_fZ - Bias));
}
FVector FVector::operator*(float Scale) const
{
	return FVector((m_fX * Scale), (m_fY * Scale), (m_fZ * Scale));
}
FVector FVector::operator/(float Scale) const
{
	const float RSclae = 1.0f / Scale;
	return FVector((m_fX * RSclae), (m_fY * RSclae), (m_fZ * RSclae));
}

bool FVector::operator==(const FVector& V) const
{
	return (m_fX == V.m_fX) && (m_fY == V.m_fY) && (m_fZ == V.m_fZ);
}
bool FVector::operator!=(const FVector& V) const
{
	return (m_fX != V.m_fX) || (m_fY != V.m_fY) || (m_fZ != V.m_fZ);
}
bool FVector::Equals(const FVector& V, float Tolerance) const
{
	return (fabs(m_fX - V.m_fX) <= Tolerance) && (fabs(m_fY - V.m_fY) <= Tolerance) && (fabs(m_fZ - V.m_fZ) <= Tolerance);
}
bool FVector::AllComponentsEqual(float Tolerance) const
{
	return ((fabs(m_fX - m_fY) <= Tolerance) && (fabs(m_fX - m_fZ) <= Tolerance) && (fabs(m_fY - m_fZ) <= Tolerance));
}
FVector FVector::operator-() const
{
	return FVector(-m_fX, -m_fY, -m_fZ);
}
FVector FVector::operator+=(const FVector& V)
{
	m_fX += V.m_fX;
	m_fY += V.m_fY;
	m_fZ += V.m_fZ;
	return *this;
}
FVector FVector::operator-=(const FVector& V)
{
	m_fX -= V.m_fX;
	m_fY -= V.m_fY;
	m_fZ -= V.m_fZ;
	return *this;
}
FVector FVector::operator*=(const FVector& V)
{
	m_fX *= V.m_fX;
	m_fY *= V.m_fY;
	m_fZ *= V.m_fZ;
	return *this;
}
FVector FVector::operator/=(const FVector& V)
{
	m_fX /= V.m_fX;
	m_fY /= V.m_fY;
	m_fZ /= V.m_fZ;
	return *this;
}

FVector FVector::operator*=(float Scale)
{
	m_fX *= Scale;
	m_fY *= Scale;
	m_fZ *= Scale;
	return *this;
}
FVector FVector::operator/=(float V)
{
	const float RV = 1.0f / V;
	m_fX *= RV;
	m_fY *= RV;
	m_fZ *= RV;
	return *this;
}

float& FVector::operator[](int Index)
{
	if (Index == 0)
	{
		return m_fX;
	}
	else if (Index == 1)
	{
		return m_fY;
	}
	else
	{
		return m_fZ;
	}
}
float FVector::operator[](int Index) const
{
	if (Index < 0 && Index >= 3)
		return 0.0f;

	if (Index == 0)
	{
		return m_fX;
	}
	else if (Index == 1)
	{
		return m_fY;
	}
	else
	{
		return m_fZ;
	}
}

void FVector::Set(float fX, float fY, float fZ)
{
	m_fX = fX;
	m_fY = fY;
	m_fZ = fZ;
}
float FVector::Size() const
{
	return sqrt((m_fX * m_fX) + (m_fY * m_fY) + (m_fZ * m_fZ));
}
float FVector::SizeSquared() const
{
	return ((m_fX * m_fX) + (m_fY * m_fY) + (m_fZ * m_fZ));
}
bool FVector::IsZero() const
{
	return (m_fX == 0.0f && m_fY == 0.0f && m_fZ == 0.0f);
}
bool FVector::Normalize(float Tolerance)
{
	const float SqureSum = (m_fX * m_fX) + (m_fY * m_fY) + (m_fZ * m_fZ);
	if (SqureSum > Tolerance)
	{
		const float Scale = 1.0f / SqureSum;
		m_fX *= Scale;
		m_fY *= Scale;
		m_fZ *= Scale;
		return true;
	}
	return false;
}
float FVector::Dist(const FVector& V1, const FVector& V2)
{
	return sqrt(FVector::DistSquared(V1, V2));
}
float FVector::DistanceTo(FVector vDest)
{
	return sqrt(FVector::DistSquared(*this, vDest));
}

float FVector::DistSquared(const FVector& V1, const FVector& V2)
{
	return Square(V2.m_fX - V1.m_fX) + Square(V2.m_fY - V2.m_fY) + Square(V2.m_fZ - V1.m_fZ);
}


FVector::~FVector()
{
}
