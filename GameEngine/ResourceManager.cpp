#include "GameEngine_pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Initialize()
{
	//�ʿ��� ��� ���ҽ��� ����ؾ���
	ReadMapInfoCSV("../Resource/MapInfo/map1");
	ReadDirInfoCSV("../Resource/MapInfo/direction");
	ReadGameInfoCSV("../Resource/MapInfo/game");
}

void ResourceManager::Update(double dt)
{

}

void ResourceManager::Finalize()
{
	//����� ���ҽ��� �� �����

	//��ƼƼ�� ���⼭ ����? �����غ� ��
}

void ResourceManager::ReadMapInfoCSV(std::string path)
{
	int mapsize = 151 - 1;


	std::ifstream file(path + ".csv");

	std::string line;
	std::string cell;

	std::vector<int> columnName;

	//UTF 8 offset
	int offset = 3;

	//int offset = 0;

	std::size_t prefound = 0;
	std::size_t found = 0;
	std::size_t lastfound = 0;

	//������ ����� �� ������ �����ϸ�
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{

		/// cell �׸� �޾ƿ���
		std::getline(file, line);

		//UTF 8��
		//���� ����
		line = line.substr(offset, line.size());
		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		//,������ �� �ϳ� ����
		int data = std::stoi(line.substr(0, found));

		int count = 0;
		while (lastfound != prefound)
		{
			prefound = found;
			found = line.find_first_of(',', found + 1);

			data = std::stoi(line.substr(prefound + 1, found - prefound - 1));


			CellData[mapsize][0 + count] = data;

			count++;
		}

		//������ �� ����
		CellData[mapsize][mapsize] = std::stoi(line.substr(0, lastfound));


		///����Ÿ ����
		int lineindex = 1;

		while (std::getline(file, line))
		{
			if (line._Equal(""))
			{
				break;
			}

			prefound = 0;
			found = line.find_first_of(',');
			lastfound = line.find_last_of(',');

			int count = 0;

			//,������ �� �ϳ� ����
			int col = std::stoi(line.substr(0, found));
			//�Ǿ� ����
			CellData[mapsize - lineindex][0 + count] = col;

			while (lastfound != prefound)
			{
				prefound = found;
				if (prefound == lastfound)
				{
					break;
				}
				found = line.find_first_of(',', found + 1);
				std::string test = line.substr(prefound + 1, found - prefound - 1);
				col = std::stoi(line.substr(prefound + 1, found - prefound - 1));

				CellData[mapsize - lineindex][1 + count] = col;

				count++;
			}

			//������ �� ����
			CellData[mapsize - lineindex][1 + count] = std::stoi(line.substr(0, lastfound));
			lineindex++;
		}

		file.close();
	}
}

void ResourceManager::ReadDirInfoCSV(std::string path)
{
	int mapsize = 151 - 1;


	std::ifstream file(path + ".csv");

	std::string line;
	std::string cell;

	std::vector<int> columnName;

	//UTF 8 offset
	int offset = 3;

	//int offset = 0;

	std::size_t prefound = 0;
	std::size_t found = 0;
	std::size_t lastfound = 0;

	//������ ����� �� ������ �����ϸ�
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{

		/// cell �׸� �޾ƿ���
		std::getline(file, line);

		//UTF 8��
		//���� ����
		line = line.substr(offset, line.size());
		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		//,������ �� �ϳ� ����
		int data = std::stoi(line.substr(0, found));

		int count = 0;
		while (lastfound != prefound)
		{
			prefound = found;
			found = line.find_first_of(',', found + 1);

			data = std::stoi(line.substr(prefound + 1, found - prefound - 1));


			DirData[mapsize][0 + count] = data;

			count++;
		}

		//������ �� ����
		DirData[mapsize][mapsize] = std::stoi(line.substr(0, lastfound));


		///����Ÿ ����
		int lineindex = 1;

		while (std::getline(file, line))
		{
			if (line._Equal(""))
			{
				break;
			}

			prefound = 0;
			found = line.find_first_of(',');
			lastfound = line.find_last_of(',');

			int count = 0;

			//,������ �� �ϳ� ����
			int col = std::stoi(line.substr(0, found));
			//�Ǿ� ����
			DirData[mapsize - lineindex][0 + count] = col;

			while (lastfound != prefound)
			{
				prefound = found;
				if (prefound == lastfound)
				{
					break;
				}
				found = line.find_first_of(',', found + 1);
				std::string test = line.substr(prefound + 1, found - prefound - 1);
				col = std::stoi(line.substr(prefound + 1, found - prefound - 1));

				DirData[mapsize - lineindex][1 + count] = col;

				count++;
			}

			//������ �� ����
			DirData[mapsize - lineindex][1 + count] = std::stoi(line.substr(0, lastfound));
			lineindex++;
		}

		file.close();
	}
}

void ResourceManager::ReadGameInfoCSV(std::string path)
{
	int mapsize = 151 - 1;


	std::ifstream file(path + ".csv");

	std::string line;
	std::string cell;

	std::vector<int> columnName;

	//UTF 8 offset
	int offset = 3;

	//int offset = 0;

	std::size_t prefound = 0;
	std::size_t found = 0;
	std::size_t lastfound = 0;

	//������ ����� �� ������ �����ϸ�
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{

		/// cell �׸� �޾ƿ���
		std::getline(file, line);

		//UTF 8��
		//���� ����
		line = line.substr(offset, line.size());
		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		//,������ �� �ϳ� ����
		int data = std::stoi(line.substr(0, found));

		int count = 0;
		while (lastfound != prefound)
		{
			prefound = found;
			found = line.find_first_of(',', found + 1);

			data = std::stoi(line.substr(prefound + 1, found - prefound - 1));


			GameData[mapsize][0 + count] = data;

			count++;
		}

		//������ �� ����
		GameData[mapsize][mapsize] = std::stoi(line.substr(0, lastfound));


		///����Ÿ ����
		int lineindex = 1;

		while (std::getline(file, line))
		{
			if (line._Equal(""))
			{
				break;
			}

			prefound = 0;
			found = line.find_first_of(',');
			lastfound = line.find_last_of(',');

			int count = 0;

			//,������ �� �ϳ� ����
			int col = std::stoi(line.substr(0, found));
			//�Ǿ� ����
			GameData[mapsize - lineindex][0 + count] = col;

			while (lastfound != prefound)
			{
				prefound = found;
				if (prefound == lastfound)
				{
					break;
				}
				found = line.find_first_of(',', found + 1);
				std::string test = line.substr(prefound + 1, found - prefound - 1);
				col = std::stoi(line.substr(prefound + 1, found - prefound - 1));

				GameData[mapsize - lineindex][1 + count] = col;

				count++;
			}

			//������ �� ����
			GameData[mapsize - lineindex][1 + count] = std::stoi(line.substr(0, lastfound));
			lineindex++;
		}

		file.close();
	}
}

