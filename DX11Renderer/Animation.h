#pragma once
#include "IResource.h"

enum class AnimationState
{
	Loop,		// 반복 재생
	Once,		// 한번만 재생
};

struct AnimationKey
{
	double time;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};

struct AnimationNode
{	
	std::vector<AnimationKey> keyAnimations;

	Matrix Interpolation(double progressTime) const
	{
		Vector3 position{};
		Quaternion rotation{};
		Vector3 scale{};

		if (keyAnimations.empty())
			return Matrix::Identity;

		if (progressTime <= keyAnimations.front().time)
		{
			position = keyAnimations.front().position;
			rotation = keyAnimations.front().rotation;
			scale = keyAnimations.front().scale;
			return Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);
		}

		auto iter = keyAnimations.begin();
		for (; iter != keyAnimations.end(); ++iter)
		{
			if (iter->time >= progressTime)
			{
				auto begin = iter - 1;
				float delta = progressTime - begin->time;
				float deltaRatio = delta / (iter->time - begin->time);

				position = Vector3::Lerp(begin->position, iter->position, deltaRatio);
				rotation = Quaternion::Slerp(begin->rotation, iter->rotation, deltaRatio);
				scale = Vector3::Lerp(begin->scale, iter->scale, deltaRatio);

				break;
			}
		}

		if (iter == keyAnimations.end())
		{
			position = keyAnimations.back().position;
			rotation = keyAnimations.back().rotation;
			scale = keyAnimations.back().scale;
		}

		return Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);
	}
};

class Animation : public IResource
{
public:
	bool Load() { return true; }
	void UnLoad() {}

	double m_duration = 0.f;
	std::map<std::string, AnimationNode> m_animationNodes;
	AnimationState m_state;
	bool m_isAnimationEnd = false;
};

