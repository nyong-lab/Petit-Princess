#pragma once


class TypeInfoManager
{
	struct TypeInfomation
	{
		std::type_index typeIndex;
		TypeInfo* typeInfo;
	};

private:
	TypeInfoManager() {}
	~TypeInfoManager() {}

	friend class TypeInfo;

public:
	static TypeInfoManager* GetInstance() { static TypeInfoManager manager; return &manager; }

	// Ÿ�� �̸����� type_index�� ã�´�.
	std::type_index GetTypeIndex(const std::string& name)const;

	// Ÿ�� �̸����� Ÿ�� ��ü�� ã�´�.
	TypeInfo* GetTypeInfo(std::string name) const;
	
	// Ÿ�� �ε����� Ÿ�� ��ü�� ã�´�.
	TypeInfo* GetTypeInfo(std::type_index index) const;

	const std::unordered_map<std::string, TypeInfomation>& GetTypeInfomations() const { return m_infomations; }

	const std::vector<TypeInfo*> GetComponentTypeInfo() const;

private:
	void AddTypeInformation(const std::string& name, const TypeInfomation& info);

private:
	std::unordered_map<std::string, TypeInfomation> m_infomations;
};

