#pragma once

struct Node;

struct Bone
{
	std::string name;
	std::shared_ptr<Node> node;
	Matrix BoneOffset;
	Matrix FinalTransformation;
};

struct Skeleton
{
	std::vector<Bone> m_bones;
};
