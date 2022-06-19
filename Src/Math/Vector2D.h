/******************************************************************************
filename: Vector2D.h
author: Seow Jun Hao Darren seow.j@digipen.edu
Project: Cs396_A1
Description:
Contains the declaration of Vector2D class
******************************************************************************/
#pragma once
#include <iostream>

/**********************************************************************************
* \brief	namespace where Vector2D class will be defined in.
**********************************************************************************/
namespace PE_Math
{
	/******************************************************************************
	* \brief	Vector2D class declaration.
	******************************************************************************/
	struct Vector2D
	{
		// Data members of Vector2D
		float x;
		float y;

		/**************************************************************************
		* \func		Vector2D()
		*
		* \brief	Default Constructor for Vector2D class.
		**************************************************************************/
		Vector2D();

		/**************************************************************************
		* \func		Vector2D(const float& _x, const float& _y)
		*
		* \brief	Conversion Constructor for Vector2D class.
		**************************************************************************/
		Vector2D(const float _x, const float _y);

		/**************************************************************************
		* \func		Vector2D(const Vector2D& _rhs)
		*
		* \brief	Copy Constructor for Vector2D class.
		**************************************************************************/
		Vector2D(const Vector2D& _rhs);

		/**************************************************************************
		* \func		Vector2D& operator=(const Vector2D& _rhs)
		*
		* \brief	Copy Assignment for Vector2D class.
		**************************************************************************/
		Vector2D& operator=(const Vector2D& _rhs);

		/**************************************************************************
		* \func		operator bool() const
		*
		* \brief	Conversion Operator to bool used to check if \a this Vector2D 
		*			is a zero vector or not.
		**************************************************************************/
		operator bool() const;

		/**************************************************************************
		* \func		Vector2D& operator+=(const Vector2D& _rhs)
		*
		* \brief	Compound addition operator to add \a this Vector2D to another 
		*			Vector2D.
		**************************************************************************/
		Vector2D& operator+=(const Vector2D& _rhs);

		/**************************************************************************
		* \func		Vector2D& operator+=(const float& _rhs)
		*
		* \brief	Compound addition operator to add \a this Vector2D to a scalar.
		**************************************************************************/
		Vector2D& operator+=(const float _rhs);

		/**************************************************************************
		* \func		Vector2D& operator-=(const Vector2D& _rhs)
		*
		* \brief	Compound subtraction operator to subtract a Vector2D from \a 
		*			this Vector2D.
		**************************************************************************/
		Vector2D& operator-=(const Vector2D& _rhs);

		/**************************************************************************
		* \func		Vector2D& operator-=(const float& _rhs)
		*
		* \brief	Compound subtraction operator to subtract a scalar from \a this
		*			Vector2D.
		**************************************************************************/
		Vector2D& operator-=(const float _rhs);

		/**************************************************************************
		* \func		Vector2D& operator*=(const float& _rhs)
		*
		* \brief	Compound multiplication operator to multiply \a this Vector2D 
		*			by a scalar.
		**************************************************************************/
		Vector2D& operator*=(const float _rhs);

		/**************************************************************************
		* \func		Vector2D& operator/=(const float& _rhs)
		*
		* \brief	Compound division operator to divide \a this Vector2D by a 
		*			scalar.
		**************************************************************************/
		Vector2D& operator/=(const float _rhs);

		/**************************************************************************
		* \func		Vector2D operator-()
		*
		* \brief	Negates \a this Vector2D.
		**************************************************************************/
		Vector2D operator-() const;

		/**************************************************************************
		* \func		bool operator==(const Vector2D& _rhs) const
		*
		* \brief	Checks if \a this Vector2D is equal to the other Vector2D.
		**************************************************************************/
		bool operator==(const Vector2D& _rhs) const;

		/******************************************************************************
		* \func		bool operator!=(const Vector2D& _rhs) const
		*
		* \brief	Checks if two Vector2D are not equal.
		******************************************************************************/
		bool operator!=(const Vector2D& _rhs) const;

		/******************************************************************************
		* \func		friend std::ostream& operator<<(std::ostream& _os, const Vector2D& _rhs)
		*
		* \brief	Prints Vector2D object in a specified format.
		******************************************************************************/
		friend std::ostream& operator<<(std::ostream& _os, const Vector2D& _rhs);
	};

	/******************************************************************************
	* \func		Vector2D operator+(Vector2D _lhs, const Vector2D& _rhs)
	*
	* \brief	Adds two Vector2D.
	******************************************************************************/
	Vector2D operator+(Vector2D _lhs, const Vector2D& _rhs);

	/******************************************************************************
	* \func		Vector2D operator+(Vector2D _lhs, const float& _rhs)
	*
	* \brief	Adds a Vector2D with a scalar.
	******************************************************************************/
	Vector2D operator+(Vector2D _lhs, const float _rhs);

	/******************************************************************************
	* \func		Vector2D operator+(const float& _rhs, Vector2D _lhs)
	*
	* \brief	Adds a Vector2D with a scalar.
	******************************************************************************/
	Vector2D operator+(const float _rhs, Vector2D _lhs);

	/******************************************************************************
	* \func		Vector2D operator-(Vector2D _lhs, const Vector2D& _rhs)
	*
	* \brief	Subtracts a Vector2D from another Vector2D.
	******************************************************************************/
	Vector2D operator-(Vector2D _lhs, const Vector2D& _rhs);

	/******************************************************************************
	* \func		Vector2D operator-(Vector2D _lhs, const float& _rhs)
	*
	* \brief	Subtracts a scalar from a Vector2D.
	******************************************************************************/
	Vector2D operator-(Vector2D _lhs, const float _rhs);

	/******************************************************************************
	* \func		Vector2D operator-(const float& _rhs, Vector2D _lhs)
	*
	* \brief	Subtracts a scalar from a Vector2D.
	******************************************************************************/
	Vector2D operator-(const float _rhs, Vector2D _lhs);

	/******************************************************************************
	* \func		Vector2D operator*(Vector2D _lhs, const float& _rhs)
	*
	* \brief	Multiplies a scalar to a Vector2D.
	******************************************************************************/
	Vector2D operator*(Vector2D _lhs, const float _rhs);

	/******************************************************************************
	* \func		Vector2D operator*(const float& _rhs, Vector2D _lhs)
	*
	* \brief	Multiplies a scalar to a Vector2D.
	******************************************************************************/
	Vector2D operator*(const float _rhs, Vector2D _lhs);

	/******************************************************************************
	* \func		Vector2D operator/(Vector2D _lhs, const float& _rhs)
	*
	* \brief	Divides a scalar from a Vector2D.
	******************************************************************************/
	Vector2D operator/(Vector2D _lhs, const float _rhs);

	/******************************************************************************
	* \func		float Dot(const Vector2D& _lhs, const Vector2D& _rhs)
	*
	* \brief	Get the dot product between two Vector2D.
	******************************************************************************/
	float Dot(const Vector2D& _lhs, const Vector2D& _rhs);

	/******************************************************************************
	* \func		float Cross(const Vector2D _lhs, const Vector2D& _rhs)
	*
	* \brief	Get the cross product between two Vector2D.
	*			The result gives the sine of the angle between the two Vector2D.
	******************************************************************************/
	float Cross(const Vector2D& _lhs, const Vector2D& _rhs);

	/******************************************************************************
	* \func		Vector2D ClockwiseNormal(const Vector2D& _vec)
	*
	* \brief	Get the clockwise normal of _vec.
	******************************************************************************/
	Vector2D ClockwiseNormal(const Vector2D& _vec);

	/******************************************************************************
	* \func		Vector2D AntiClockwiseNormal(const Vector2D& _vec)
	*
	* \brief	Get the anti-clockwise normal of _vec.
	******************************************************************************/
	Vector2D AntiClockwiseNormal(const Vector2D& _vec);

	/******************************************************************************
	* \func		void NormalizeVector(Vector2D& _vec)
	*
	* \brief	Normalize _vec and store the results back to _vec.
	******************************************************************************/
	void NormalizeVector(Vector2D& _vec);

	/******************************************************************************
	* \func		Vector2D NormalizeVector(const Vector2D& _vec)
	*
	* \brief	Normalize _vec and return a copy of the results.
	******************************************************************************/
	Vector2D NormalizedVector(const Vector2D& _vec);

	/******************************************************************************
	* \func		float Magnitude(const Vector2D& _vec)
	*
	* \brief	Get the magnitude of _vec.
	******************************************************************************/
	float Magnitude(const Vector2D& _vec);

	/******************************************************************************
	* \func		float MagnitudeSquared(const Vector2D& _vec)
	*
	* \brief	Get the squared magnitude of _vec.
	******************************************************************************/
	float MagnitudeSquared(const Vector2D& _vec);

	/******************************************************************************
	* \func		float Distance(const Vector2D& _lhs, const Vector2D& _rhs)
	*
	* \brief	Get the distance between two vectors.
	******************************************************************************/
	float Distance(const Vector2D& _lhs, const Vector2D& _rhs);

	/******************************************************************************
	* \func		float DistanceSquared(const Vector2D& _lhs, const Vector2D& _rhs)
	*
	* \brief	Get the squared distance between two vectors.
	******************************************************************************/
	float DistanceSquared(const Vector2D& _lhs, const Vector2D& _rhs);

	/******************************************************************************
	* \func		std::ostream& operator<<(std::ostream& _os, const Vector2D& _rhs)
	*
	* \brief	Prints Vector2D object in a specified format.
	******************************************************************************/
	std::ostream& operator<<(std::ostream& _os, const Vector2D& _rhs);
}

using Vector2D = PE_Math::Vector2D;
