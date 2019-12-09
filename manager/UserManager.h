#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<cstdlib>
#include <ctime>
#include"../util/DataUtil.h"
#include"../util/Const.h"
#include"../model/User.h"
using namespace std;

class UserManager{
    private:
        void mapFromDatabase();     /*将数据库中信息映射进成员变量users中*/
        vector<User> users;        /*当前映射并且被维护的的用户列表*/
    public:
        UserManager();
        ~UserManager();
        bool addUser(string username,string userpass,string usertype,string &mainkey); /*添加用户*/
        bool delUser(string mainkey);                                    /*删除用户*/
        bool updateUsertype(string mainkey,string usertype);             /*更新用户类型*/
        bool updateUserpass(string mainkey,string userpass);             /*更新用户密码*/
        bool updateUsername(string mainkey,string username);             /*更新用户名*/
        bool findUserByUsername(string username,User& user);             /*以username获取用户*/
        bool findUserByMainkey(string mainkey,User& user);               /*以mainkey获取用户*/
        bool loginUser(string username,string userpass,string &mainkey);   /*普通用户登录*/
        bool registNromalUser(string username,string userpass,string &mainkey); /*注册普通用户*/
        static void formattedPrintProductList(vector<User> users);             /*根据列表格式化打印用户信息*/
        void formattedPrintProductList();                                 /*打印所有用户信息*/
        bool findUsersByUserLike(string name,vector<User> &users);
};


UserManager::UserManager(){
    this->mapFromDatabase();    /*从数据库映射到模型*/
}

UserManager::~UserManager(){
    
}

bool UserManager::registNromalUser(string username,string userpass,string &mainkey){
    return this->addUser(username,userpass,USER_TYPE_NORMAL_USER,mainkey);
}

/**
 * 登录功能 返回mainkey用于之后换取其他信息 也就是session 但是此处不再实现session了
 */

bool UserManager::loginUser(string username,string userpass,string &mainkey){
    User user;
    if(findUserByUsername(username,user)){
        if(user.getUserpass() == userpass){
            mainkey = user.getMainkey();
            return true;
        }
        return false;
    }else return false;
}

bool UserManager::findUserByUsername(string username,User& user){
    for(int i=0;i<users.size();i++){
        if(users[i].getUsername() == username){
            user = users[i];
            return true;
        }
    }
    return false;
}

bool UserManager::findUserByMainkey(string mainkey,User& user){
    for(int i=0;i<users.size();i++){
        if(users[i].getMainkey() == mainkey){
            user = users[i];
            return true;
        }
    }
    return false;
}

bool UserManager::updateUsername(string mainkey,string username){
    if(updateValueIntoTable(DEFAULT_TABLE_USER,"username",username,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户名失败"<<endl;
        return false;
    }
}

bool UserManager::updateUserpass(string mainkey,string userpass){
    if(updateValueIntoTable(DEFAULT_TABLE_USER,"userpass",userpass,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户密码失败"<<endl;
        return false;
    }
}

bool UserManager::updateUsertype(string mainkey,string usertype){
    if(updateValueIntoTable(DEFAULT_TABLE_USER,"usertype",usertype,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户类型失败"<<endl;
        return false;
    }
}

bool UserManager::delUser(string mainkey){
    if(removeItByKey(DEFAULT_TABLE_USER,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"删除用户失败"<<endl;
        return false;
    }
}

bool UserManager::addUser(string username,string userpass,string usertype,string &mainkey){
    srand((int)time(0));
    mainkey = to_string(rand());
    //User user(mainky,username,userpass,usertype);
    map<string,string> mp;
    mp.insert(pair<string,string>("username",username));
    mp.insert(pair<string,string>("userpass",userpass));
    mp.insert(pair<string,string>("usertype",usertype));
    if(existsInTable(DEFAULT_TABLE_USER,"username",username)){
        cout<<"用户名已存在"<<endl;
        return false;
    }
    if(insertValuesWithMainkeyIntoTable(DEFAULT_TABLE_USER,mp,"mainkey",mainkey)){
        this->mapFromDatabase();
        return true;
    }else{
        cout<<"添加用户失败"<<endl;
        return false;
    }
}


void UserManager::mapFromDatabase(){
    map<string,vector<string>*> mp;
    map<string,string> cond;
    mp.insert(pair<string,vector<string>*>("mainkey",nullptr));
    mp.insert(pair<string,vector<string>*>("username",nullptr));
    mp.insert(pair<string,vector<string>*>("userpass",nullptr));
    mp.insert(pair<string,vector<string>*>("usertype",nullptr));
    if(selectValuesIntoTable(DEFAULT_TABLE_USER,mp,cond)){
        vector<string>* mainkey = mp["mainkey"];
        vector<string>* username = mp["username"];
        vector<string>* userpass = mp["userpass"];
        vector<string>* usertype = mp["usertype"];
        vector<string> vectors[] = {*mainkey,*username,*userpass,*usertype};
        if(equalVectors(vectors,4)){
            if(users.size()==mainkey->size()){
                //do nothing
            }else if(users.size()<mainkey->size()){
                int c = mainkey->size()-users.size();
                for(int i=0;i<c;i++){
                    users.push_back(*(new User));
                }
            }else{
                int c = users.size()-mainkey->size();
                for(int i=0;i<c;i++){
                    User del = users.back();
                    users.pop_back();
                }
            }
            for(int i=0;i<users.size();i++){
                users[i].setMainkey((*mainkey)[i]);
                users[i].setUsername((*username)[i]);
                users[i].setUserpass((*userpass)[i]);
                users[i].setUsertype((*usertype)[i]);
            }
        }else{
            cout<<"失败,有空字段"<<endl;
        }
    }else{
        cout<<"数据库获取失败"<<endl;
    }
}

void UserManager::formattedPrintProductList(vector<User> users){
    cout<<"======================"<<endl;
    for(int i=0;i<users.size();i++){
        cout<<"用户id：\t"<<users[i].getMainkey()<<endl;
        cout<<"用户名：\t"<<users[i].getUsername()<<endl;
        if(users[i].getUserType()==USER_TYPE_ADMIN_USER){
            cout<<"用户类型：\t管理员"<<endl;
        }else if(users[i].getUserType()==USER_TYPE_NORMAL_USER){
            cout<<"用户类型：\t普通用户"<<endl;
        }
        cout<<"用户密码：\t"<<users[i].getUserpass()<<endl;
        if(i!=users.size()-1)
            cout<<"-----------------------"<<endl;
    }
    cout<<"======================"<<endl;
}

bool UserManager::findUsersByUserLike(string username,vector<User> &users){
    this->mapFromDatabase();
    bool status = false;
    for(int i=0;i<this->users.size();i++){
        if(this->users[i].getUsername().find(username)!=-1){
            users.push_back(this->users[i]);
            status = true;
        }
    }
    return status;
}


void UserManager::formattedPrintProductList(){
    this->mapFromDatabase();
    this->formattedPrintProductList(this->users);
}