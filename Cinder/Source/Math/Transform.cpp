#include "Transform.h"

Transform::Transform(const Vector& position, const Vector& rotation, const Vector& scale)
{
	m_Rotation = Matrix::Rotate(rotation);

	m_Transform = Matrix::Scale(scale) * // Scale
		m_Rotation * // Then rotate
		Matrix::Translate(position); // And then translate

	m_TransformInverse = Matrix::Inverse(m_Transform);
}

namespace YAML
{
bool convert<Transform>::decode(const Node& node, Transform& transform)
{
	if (!node["Position"])
	{
		Error("Transform does not have a position! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Rotation"])
	{
		Error("Transform does not have a rotation! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}
	if (!node["Scale"])
	{
		Error("Transform does not have a scale! ("
			COLOR, "line ", node.Mark().line, "\x1b[31m).");
	}

	transform = Transform(
		node["Position"].as<Vector>(),
		node["Rotation"].as<Vector>(),
		node["Scale"].as<Vector>()
	);

	return true;
}
}
