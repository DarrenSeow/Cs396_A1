/******************************************************************************
filename: Vector2D.cpp
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the definition of Vector2D
******************************************************************************/

#include "Vector2D.h"

namespace PE_Math
{
	constexpr auto epsilon = std::numeric_limits<float>::epsilon();
	constexpr auto epsilonSquared = std::numeric_limits<float>::epsilon() * std::numeric_limits<float>::epsilon();
	/******************************************************************************
	* \brief	Default Constructor for Vector2D class.
	******************************************************************************/
	Vector2D::Vector2D() :
		x { 0.0f }, y { 0.0f }
	{
	}

	/******************************************************************************
	* \brief	Conversion Constructor for Vector2D class.
	******************************************************************************/
	Vector2D::Vector2D(const float _x, const float _y) :
		x { _x }, y { _y }
	{
	}

	/******************************************************************************
	* \brief	Copy Constructor for Vector2D class.
	******************************************************************************/
	Vector2D::Vector2D(const Vector2D& _rhs) :
		x{ _rhs.x }, y { _rhs.y }
	{
	}

	/******************************************************************************
	* \brief	Copy Assignment for Vector2D class.
	******************************************************************************/
	Vector2D& Vector2D::operator=(const Vector2D& _rhs)
	{
		if (this != &_rhs)
		{
			x = _rhs.x;
			y = _rhs.y;
		}
		return *this;
	}

	/******************************************************************************
	* \brief	Conversion Operator to bool used to check if \a this Vector2D 
	*			is a zero vector or not.
	******************************************************************************/
	Vector2D::operator bool() const
	{
		// Conversion when Vector2D is not a zero vector
		return !(fabs(MagnitudeSquared(*this)) < epsilonSquared);
	}

	/******************************************************************************
	* \brief	Compound addition operator to add \a this Vector2D to another 
	*			Vector2D.
	******************************************************************************/
	Vector2D& Vector2D::operator+=(const Vector2D& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		return *this;
	}

	/******************************************************************************
	* \brief	Compound addition operator to add \a this Vector2D to a scalar.
	******************************************************************************/
	Vector2D& Vector2D::operator+=(const float _rhs)
	{
		x += _rhs;
		y += _rhs;
		return *this;
	}

	/******************************************************************************
	* \brief	Compound subtraction operator to subtract a Vector2D from \a 
	*			this Vector2D.
	******************************************************************************/
	Vector2D& Vector2D::operator-=(const Vector2D& _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		return *this;
	}

	/******************************************************************************
	* \brief	Compound subtraction operator to subtract a scalar from \a this
	*			Vector2D
	******************************************************************************/
	Vector2D& Vector2D::operator-=(const float _rhs)
	{
		x -= _rhs;
		y -= _rhs;
		return *this;
	}

	/******************************************************************************
	* \brief	Compound multiplication operator to multiply \a this Vector2D 
	*			by a scalar.
	******************************************************************************/
	Vector2D& Vector2D::operator*=(const float _rhs)
	{
		x *= _rhs;
		y *= _rhs;
		return *this;
	}

	/******************************************************************************
	* \brief	Compound division operator to divide \a this Vector2D by a 
	*			scalar.
	******************************************************************************/
	Vector2D& Vector2D::operator/=(const float _rhs)
	{
		if (_rhs >= epsilon || _rhs <= -epsilon)
		{
			x /= _rhs;
			y /= _rhs;
		}
		return *this;
	}
	
	/******************************************************************************
	* \brief	Negates \a this Vector2D.
	******************************************************************************/
	Vector2D Vector2D::operator-() const
	{
		return { -x, -y };
	}

	/******************************************************************************
	* \brief	Checks if \a this Vector2D is equal to the other Vector2D.
	******************************************************************************/
	bool Vector2D::operator==(const Vector2D& _rhs) const
	{
		return (fabs(MagnitudeSquared(*this - _rhs)) < epsilonSquared);
	}

	/******************************************************************************
	* \brief	Checks if two Vector2D are not equal.
	******************************************************************************/
	bool Vector2D::operator!=(const Vector2D& _rhs) const
	{
		return !(*this == _rhs);
	}

	/******************************************************************************
	* \brief	Adds two Vector2D.
	******************************************************************************/
	Vector2D operator+(Vector2D _lhs, const Vector2D& _rhs)
	{
		return (_lhs += _rhs);
	}

	/******************************************************************************
	* \brief	Adds a Vector2D with a scalar.
	******************************************************************************/
	Vector2D operator+(Vector2D _lhs, const float _rhs)
	{
		return (_lhs += _rhs);
	}

	/******************************************************************************
	* \brief	Adds a Vector2D with a scalar.
	******************************************************************************/
	Vector2D operator+(const float _rhs, Vector2D _lhs)
	{
		return (_lhs += _rhs);
	}

	/******************************************************************************
	* \brief	Subtracts a Vector2D from another Vector2D.
	******************************************************************************/
	Vector2D operator-(Vector2D _lhs, const Vector2D& _rhs)
	{
		return (_lhs -= _rhs);
	}

	/******************************************************************************
	* \brief	Subtracts a scalar from a Vector2D.
	******************************************************************************/
	Vector2D operator-(Vector2D _lhs, const float _rhs)
	{
		return (_lhs -= _rhs);
	}

	/******************************************************************************
	* \brief	Subtracts a scalar from a Vector2D.
	******************************************************************************/
	Vector2D operator-(const float _rhs, Vector2D _lhs)
	{
		return (_lhs -= _rhs);
	}

	/******************************************************************************
	* \brief	Multiplies a scalar to a Vector2D.
	******************************************************************************/
	Vector2D operator*(Vector2D _lhs, const float _rhs)
	{
		return (_lhs *= _rhs);
	}

	/******************************************************************************
	* \brief	Multiplies a scalar to a Vector2D.
	******************************************************************************/
	Vector2D operator*(const float _rhs, Vector2D _lhs)
	{
		return (_lhs *= _rhs);
	}

	/******************************************************************************
	* \brief	Divides a scalar from a Vector2D.
	******************************************************************************/
	Vector2D operator/(Vector2D _lhs, const float _rhs)
	{
		if (_rhs >= epsilon || _rhs <= -epsilon)
		{
			return (_lhs /= _rhs);
		}
		return _lhs;
	}

	/******************************************************************************
	* \brief	Get the dot product between two Vector2D.
	******************************************************************************/
	float Dot(const Vector2D& _lhs, const Vector2D& _rhs)
	{
		return (_lhs.x * _rhs.x) + (_lhs.y * _rhs.y);
	}

	/******************************************************************************
	* \brief	Get the cross product between two Vector2D.
	******************************************************************************/
	float Cross(const Vector2D& _lhs, const Vector2D& _rhs)
	{
		return (_lhs.x * _rhs.y) - (_lhs.y * _rhs.x);
	}

	/******************************************************************************
	* \brief	Get the clockwise normal of _vec.
	******************************************************************************/
	Vector2D ClockwiseNormal(const Vector2D& _vec)
	{
		return { _vec.y, -_vec.x };
	}

	/******************************************************************************
	* \brief	Get the anti-clockwise normal of _vec.
	******************************************************************************/
	Vector2D AntiClockwiseNormal(const Vector2D& _vec)
	{
		return { -_vec.y, _vec.x };
	}

	/******************************************************************************
	* \brief	Normalize _vec and store the results back to _vec.
	******************************************************************************/
	void NormalizeVector(Vector2D& _vec)
	{
		_vec /= Magnitude(_vec);
	}

	/******************************************************************************
	* \brief	Normalize _vec and return a copy of the results.
	******************************************************************************/
	Vector2D NormalizedVector(const Vector2D& _vec)
	{
		return _vec / Magnitude(_vec);
	}

	/******************************************************************************
	* \brief	Get the magnitude of _vec.
	******************************************************************************/
	float Magnitude(const Vector2D& _vec)
	{
		return sqrtf((_vec.x * _vec.x) + (_vec.y * _vec.y));
	}

	/******************************************************************************
	* \brief	Get the squared magnitude of _vec.
	******************************************************************************/
	float MagnitudeSquared(const Vector2D& _vec)
	{
		return (_vec.x * _vec.x) + (_vec.y * _vec.y);
	}

	/******************************************************************************
	* \brief	Get the distance between two vectors.
	******************************************************************************/
	float Distance(const Vector2D& _lhs, const Vector2D& _rhs)
	{
		return sqrtf(DistanceSquared(_lhs, _rhs));
	}

	/******************************************************************************
	* \brief	Get the squared distance between two vectors.
	******************************************************************************/
	float DistanceSquared(const Vector2D& _lhs, const Vector2D& _rhs)
	{
		const float& dxSquared = (_lhs.x - _rhs.x) * (_lhs.x - _rhs.x);
		const float& dySquared = (_lhs.y - _rhs.y) * (_lhs.y - _rhs.y);
		
		return (dxSquared + dySquared);
	}

	std::ostream& operator<<(std::ostream& _os, const Vector2D& _rhs)
	{
		return _os << "{ " << _rhs.x << ", " << _rhs.y << " }";
	}
}
