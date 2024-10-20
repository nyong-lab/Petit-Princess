#pragma once

#include "Animation.h"

class Mesh;

struct Node
{
	std::weak_ptr<Node> parent;
	std::vector<std::shared_ptr<Node>> children;

	std::string name;
	std::vector<std::shared_ptr<Mesh>> meshes;

	Matrix localMatrix = Matrix::Identity;
	Matrix transformation = Matrix::Identity;
	Matrix worldMatrix = Matrix::Identity;

	void Update(float progressTime, const Animation& animation)
	{
		const auto& find = animation.m_animationNodes.find(name);

		if (find != animation.m_animationNodes.end())
		{
			const auto& animationNode = find->second;
			assert(animation.m_duration != 0.0);
			localMatrix = animationNode.Interpolation(fmod(progressTime, animation.m_duration));
		}
		else
		{
			localMatrix = transformation;
		}

		if (parent.lock() == nullptr)
		{
			worldMatrix = localMatrix;
		}
		else
		{
			worldMatrix = localMatrix * parent.lock()->worldMatrix;
		}

		for (auto child : children)
		{
			child->Update(progressTime, animation);
		}
	}
};
