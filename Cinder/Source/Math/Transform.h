#pragma once

#include "Globals.h"
#include "Matrix.h"

class Transform
{
public:
	Transform() = default;
	Transform(const Vector& position, const Vector& rotation, const Vector& scale);

	const Matrix& GetTransform() { return m_Transform; }
	const Matrix& GetTransformInverse() { return m_TransformInverse; }
	const Matrix& GetRotation() { return m_Rotation; }

private:
	Matrix m_Transform;
	Matrix m_TransformInverse;
	Matrix m_Rotation;
};

namespace YAML
{
template<>
struct convert<Transform>
{
	static bool decode(const Node& node, Transform& transform);
};
}
