//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "Core/Math/Bound.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Ray.h"

// Transform of an object at an instant in time
class Transform
{
public:
	Transform() = default;
	Transform(float matrix[4][4]);
	Transform(const Matrix& matrix);
	Transform(const Matrix& matrix, const Matrix& inverse);

	Transform GetInverse() const;
	Transform GetTranspose() const;

	bool IsIdentity() const;

	const Matrix& GetMatrix() const { return m_Matrix; }
	const Matrix& GetInverseMatrix() const { return m_Inverse; }

	bool HasScale() const;

	Point operator()(const Point& point) const;
	Vector operator()(const Vector& vector) const;
	Normal operator()(const Normal& normal) const;
	Ray operator()(const Ray& ray) const;
	Bound operator()(const Bound& bound) const;
	Transform operator*(const Transform& transform) const;
	Transform operator*=(const Transform& transform);
	
	bool ChangesHandedness();

	Quaternion ToQuaternion();

private:
	friend bool operator==(const Transform& first, const Transform& second);

	Matrix m_Matrix, m_Inverse;
};

bool operator==(const Transform& first, const Transform& second);
bool operator!=(const Transform& first, const Transform& second);

Transform Translate(const Vector& delta);
Transform Scale(float scale);
Transform Scale(const Vector& scale);
Transform Rotate(const Vector& eulerDegrees);
Transform Rotate(const Quaternion& quaternion);
Transform Rotate(const Vector& axis, float degreeAngle);
Transform LookAt(const Point& location, const Point& focus, const Vector& up);

template<>
struct YAML::convert<Transform>
{
	static bool decode(const Node& node, Transform& transform);
};
