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
	//필요한 모든 리소스를 등록해야지
	ReadMapInfoCSV("../Resource/MapInfo/map1");
	ReadDirInfoCSV("../Resource/MapInfo/direction");
	ReadGameInfoCSV("../Resource/MapInfo/game");
}

void ResourceManager::Update(double dt)
{

}

void ResourceManager::Finalize()
{
	//등록한 리소스를 다 지운다

	//엔티티는 여기서 지워? 생각해볼 것
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

	//파일을 제대로 못 읽으면 생성하면
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{

		/// cell 항목 받아오기
		std::getline(file, line);

		//UTF 8용
		//공백 제거
		line = line.substr(offset, line.size());
		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		//,이전의 값 하나 추출
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

		//마지막 값 추출
		CellData[mapsize][mapsize] = std::stoi(line.substr(0, lastfound));


		///데이타 저장
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

			//,이전의 값 하나 추출
			int col = std::stoi(line.substr(0, found));
			//맨앞 저장
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

			//마지막 값 추출
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

	//파일을 제대로 못 읽으면 생성하면
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{

		/// cell 항목 받아오기
		std::getline(file, line);

		//UTF 8용
		//공백 제거
		line = line.substr(offset, line.size());
		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		//,이전의 값 하나 추출
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

		//마지막 값 추출
		DirData[mapsize][mapsize] = std::stoi(line.substr(0, lastfound));


		///데이타 저장
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

			//,이전의 값 하나 추출
			int col = std::stoi(line.substr(0, found));
			//맨앞 저장
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

			//마지막 값 추출
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

	//파일을 제대로 못 읽으면 생성하면
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{

		/// cell 항목 받아오기
		std::getline(file, line);

		//UTF 8용
		//공백 제거
		line = line.substr(offset, line.size());
		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		//,이전의 값 하나 추출
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

		//마지막 값 추출
		GameData[mapsize][mapsize] = std::stoi(line.substr(0, lastfound));


		///데이타 저장
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

			//,이전의 값 하나 추출
			int col = std::stoi(line.substr(0, found));
			//맨앞 저장
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

			//마지막 값 추출
			GameData[mapsize - lineindex][1 + count] = std::stoi(line.substr(0, lastfound));
			lineindex++;
		}

		file.close();
	}
}

