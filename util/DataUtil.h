#pragma once
#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<io.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include "../lib/CJsonObject.h" 
using namespace std;

/**
 *  此为数据库操作核心代码，以下这些工具函数用来实现了一个基于json格式存储的
 *  文件型弱关系型数据库（其实它甚至不能叫做数据库，它没有任何客户端与服务端，
 *  仅仅是作为一个头文件引入的依赖形式存在，它会将要存储的数据写入文件并且从
 *  文件里取出符合条件的数据，并且这些文件的位置与主程序的位置是高度依赖的，
 *  而且它也没有实现任何事务，原子性操作，以及数据关联级联功能甚至它都没有对
 *  文件进行加锁，以及对自身的函数进行加锁操作，原因有二，一是快期末了由于时
 *  间原因我无法再实现这些，二是我对linux编程比较熟悉，这些可以用linux系统
 *  调用的函数进行实现，比如文件锁flock和互斥锁mutex，但是这些会大大降低此
 *  应用的移植性，毕竟我还想编译一份windows版本作为作业提交），以下函数包含
 *  基本的批量增删改查操作，具体可以看函数介绍
 *
 *  虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 *  链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/manager/UserManager.h
 *  authored by 曹子帆 2019.12.9
 */

 /*
	 获取数据库文件相对位置
 */
string getFilePath(string tableName) {
	return "./data/" + tableName + ".dat";
}

/*
	判断条件是否符合
	condition 条件
	jsonobject 在此jsonobject是否完全符合
*/

bool conditionFit(map<string, string> &condition, neb::CJsonObject jsonobject) {
	if (jsonobject.IsArray() || jsonobject.IsEmpty())
		return false;
	map<string, string>::iterator ite = condition.begin();
	while (ite != condition.end()) {
		if (jsonobject(ite->first) != ite->second) {
			return false;
		}
		ite++;
	}
	return true;
}

/*
	根据条件删除记录
	tableName 表名
	condition 条件
*/

bool removeValuesIntoTable(string tableName, map<string, string> &condition) {
	string file_name = getFilePath(tableName);
	if (access(file_name.c_str(), 6) != -1) {    /*判断是否有读写权限，以及不存在创建权限*/
		fstream file(file_name, ios::in);
		stringstream buf;
		buf << file.rdbuf();
		file.close();
		neb::CJsonObject json(buf.str());
		if (json.IsArray()) {
			for (int i = 0; !json[i].IsEmpty(); i++) {
				if (conditionFit(condition, json[i])) {
					json.Delete(i);
				}
			}
			fstream wri(file_name, ios::trunc | ios::out);
			wri << json.ToFormattedString();
			wri.flush();
			wri.close();
			// cout<<json.ToFormattedString();
			return true;
		}
		else {
			cout << "数据库文件格式错误" << endl;
			return false;
		}
	}
	else {
		cout << "无权限" << endl;
		return false;
	}
}

bool removeItByKey(string tableName, string key, string value) {
	map<string, string> condition;
	condition.insert(pair<string, string>(key, value));
	return removeValuesIntoTable(tableName, condition);
}

/*
	tableName 表名
	keymaps 需要选择出来的键 和其对应的值 最终选择结果在vector<string>内 如果keymaps为空 则选择全部
	condition 条件

*/

bool selectValuesIntoTable(string tableName, map<string, vector<string>*> &keymaps, map<string, string> &condition) {
	string file_name = getFilePath(tableName);
	if (access(file_name.c_str(), 4) != -1) {  /*判断是否有可读权限*/
		fstream file(file_name, ios::in);
		stringstream buf;
		buf << file.rdbuf();
		file.close();
		neb::CJsonObject json(buf.str());
		if (!json.IsArray()) {
			cout << "数据库格式错误" << endl;
			return false;
		}
		for (int i = 0; !json[i].IsEmpty(); i++) {
			if (conditionFit(condition, json[i])) {
				map<string, vector<string>*>::iterator ite = keymaps.begin();
				while (ite != keymaps.end()) {
					if (ite->second == nullptr) {
						vector<string> *ptr = new vector<string>;
						keymaps[ite->first] = ptr;
						ptr->push_back(json[i](ite->first));
						ite++;
						continue;
					}
					ite->second->push_back(json[i](ite->first));
					ite++;
				}
			}
		}
		return true;
	}
	else {
		cout << "没有权限读取文件" << endl;
		return false;
	}
}
/*
	根据一对条件查找
	tableName   表名
	key         要获取的字段名
	values      待返回的所有符合条件的字段值
	condkey     条件的键名
	condvalue   条件的键值
*/
bool selectValueIntoTable(string tableName, string key, vector<string>* &values, string condkey, string condvalue) {
	map<string, vector<string>*> mp;
	map<string, string> condition;
	mp.insert(pair<string, vector<string>*>(key, nullptr));
	condition.insert(pair<string, string>(condkey, condvalue));
	if (selectValuesIntoTable(tableName, mp, condition)) {
		values = mp.find(key)->second;
	}
	else return false;
}

/*
	根据主键查找一个记录的一个字段
	tableName   表名
	key         要获取的字段名
	mainkey     主键键名
	mainvalue   主键键值
*/

string getValueByMainKey(string tableName, string key, string mainkey, string mainvalue) {
	vector<string> *res = nullptr;
	if (selectValueIntoTable(tableName, key, res, mainkey, mainvalue)) {
		if (res == nullptr) {
			cout << "选取失败" << endl;
			return "";
		}
		string str = res->front();
		delete res;
		return str;
	}
	else {
		cout << "选取失败" << endl;
		return "";
	}
}

/*
	tableName 表名
	keymaps 字段键值对
	condition 条件键值对
*/

bool updateValuesIntoTable(string tableName, map<string, string> &keymaps, map<string, string> &condition) {
	string file_name = getFilePath(tableName);
	if (access(file_name.c_str(), 6) != -1) {
		fstream file(file_name, ios::in);
		stringstream buf;
		buf << file.rdbuf();
		neb::CJsonObject json(buf.str());
		if (!json.IsArray()) {
			cout << "数据库格式错误" << endl;
			return false;
		}
		for (int i = 0; !json[i].IsEmpty(); i++) {
			if (conditionFit(condition, json[i])) {
				map<string, string>::iterator it = keymaps.begin();
				while (it != keymaps.end()) {
					if (json[i][it->first].IsEmpty())
						json[i].Add(it->first, it->second);
					else json[i].Replace(it->first, it->second);
					it++;
				}
			}
		}
		fstream wri(file_name, ios::out | ios::trunc);
		wri << json.ToFormattedString();
		wri.flush();
		wri.close();
		return true;
	}
	else {
		cout << "没有权限修改文件" << endl;
		return false;
	}
}

/*
	批量更新
	tableName   表名
	key         更新的字段名 不存在则创建
	value       更新的字段值 不存在则插入
	condkey     条件
	condvalue   条件值
*/

bool updateValueIntoTable(string tableName, string key, string value, string condkey, string condvalue) {
	map<string, string> keymp, condition;
	keymp.insert(pair<string, string>(key, value));
	condition.insert(pair<string, string>(condkey, condvalue));
	return updateValuesIntoTable(tableName, keymp, condition);
}

/*
	有主键批量插入数据库表
	tableName   表名
	keymaps     需要插入的键值对哈希表
	mainkey     主键名
	mainkey     主键值
*/

bool insertValuesWithMainkeyIntoTable(string tableName, map<string, string> &keymaps, string mainkey, string mainvalue) {
	string file_name = getFilePath(tableName);
	if (access(file_name.c_str(), 6) != -1) {     /*判断是否有读写权限打开文件，以及文件是否存在*/
		fstream file(file_name, ios::in);
		stringstream buf;
		buf << file.rdbuf();
		file.close();
		neb::CJsonObject ojson(buf.str());
		for (int i = 0; !ojson[i].IsEmpty(); i++) {
			if (ojson[i](mainkey) == mainvalue) {
				cout << "主键不唯一" << endl;
				return false; /*主键唯一*/
			}
		}
		fstream wri(file_name, ios::trunc | ios::out);
		map<string, string>::iterator ite = keymaps.begin();
		neb::CJsonObject item;
		item.Add(mainkey, mainvalue);
		while (ite != keymaps.end()) {
			/*添加一个字段*/
			item.Add(ite->first, ite->second);
			ite++;
		}
		ojson.Add(item);
		wri << ojson.ToFormattedString();
		// cout<< ojson.ToFormattedString()<<endl;
		wri.flush();
		wri.close();
	}
	else {
		if (access(file_name.c_str(), 2) != -1) {
			cout << "没有权限修改文件" << endl;
			return false;
		}
	}
	return true;
}

/*
	有主键插入一条数据到数据库表
	tableName   表名
	key         要插入的键名
	value       要插入的键值
	mainkey     主键名
	mainvalue   主键值
*/

bool insertValueWithMainkeyIntoTable(string tableName, string key, string value, string mainkey, string mainvalue) {
	map<string, string> mp;
	mp.insert(pair<string, string>(key, value));
	return insertValuesWithMainkeyIntoTable(tableName, mp, mainkey, mainvalue);
}


/*
	无主键批量插入数据库表
	tableName   表名
	keymaps     需要插入的键值对哈希表
*/

bool insertValuesIntoTable(string tableName, map<string, string> &keymaps) {
	string file_name = getFilePath(tableName);
	if (access(file_name.c_str(), 6) != -1) {     /*判断是否有读写权限打开文件，以及文件是否存在*/
		fstream file(file_name, ios::in);
		stringstream buf;
		buf << file.rdbuf();
		file.close();
		neb::CJsonObject ojson(buf.str());
		fstream wri(file_name, ios::trunc | ios::out);
		map<string, string>::iterator ite = keymaps.begin();
		neb::CJsonObject item;
		while (ite != keymaps.end()) {
			/*添加一个字段*/
			item.Add(ite->first, ite->second);
			ite++;
		}
		ojson.Add(item);
		wri << ojson.ToFormattedString();
		// cout<< ojson.ToFormattedString()<<endl;
		wri.flush();
		wri.close();
	}
	else {
		if (access(file_name.c_str(), 2) != -1) {
			cout << "没有权限修改文件" << endl;
			return false;
		}
	}
	return true;
}

/*
	无主键插入一条数据到数据库表
	tableName   表名
	key         要插入的键名
	value       要插入的键值
*/

bool insertIt(string tableName, string key, string value) {
	map<string, string> mp;
	mp.insert(pair<string, string>(key, value));
	return insertValuesIntoTable(tableName, mp);
}

/*
	判断字段值是否存在
	tableName   表名
	key         键名
	value       键值
*/

bool existsInTable(string tableName, string key, string value) {
	string file_name = getFilePath(tableName);
	if (access(file_name.c_str(), 4) != -1) {  /*判断是否有可读权限*/
		fstream file(file_name, ios::in);
		stringstream buf;
		buf << file.rdbuf();
		file.close();
		neb::CJsonObject json(buf.str());
		if (!json.IsArray()) {
			cout << "数据库格式错误" << endl;
			return false;
		}
		map<string, string> mp;
		mp.insert(pair<string, string>(key, value));
		for (int i = 0; !json[i].IsEmpty(); i++) {
			if (conditionFit(mp, json[i])) {
				return true;
			}
		}
		return false;
	}
	else {
		cout << "没有权限读取文件" << endl;
		return false;
	}
}
