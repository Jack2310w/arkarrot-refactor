#pragma once
#include "CommonDefines.h"
#include <unordered_set>
#include <unordered_map>
#include <fstream>

class CSVDataManager {
private:
	//������ָ��
	static CSVDataManager* _single_instance;
	
	struct MonsterInfo {
		string name;
		string plist_path;
		int frame_cnt;
		float speed;
		int lifepoint;
		int damage;
	};

	struct TowerInfo {
		string name;
		string plist_path;
		int frame_cnt;
		int cost;
		float cooldown;
		int attack;

		int bullet_frame_cnt;
		float bullet_speed;
	};

	std::unordered_set<string> visited_path;
	std::unordered_map<string, TowerInfo> towerInfos;
	std::unordered_map<string, MonsterInfo> monsterInfos;

	//˽�й��캯�����������ⲿ��������
	CSVDataManager();
public:
	/*
	* @brief ��ȡcsv���ݼ������ĵ�������
	*/
	static CSVDataManager* getInstance();
	/*
	* @brief ���ط�����������
	* @param towers.csv��·��
	*/
	void loadTowerData(const string& path);
	/*
	* @brief ���ع��������
	* @param monsters.csv��·��
	*/
	void loadMonsterData(const string& path);
};