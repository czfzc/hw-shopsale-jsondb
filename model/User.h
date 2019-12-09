#pragma once
#include<iostream>
using namespace std;

/**
 * 用户模型，在MVC架构中属于M即model层
 * 虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 * 链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/model/Order.h
 * authored by 曹子帆 2019.12.9
 */

class User{
    private:
        string mainkey;         /*用户的主键*/
        string username;        /*用户名*/
        string userpass;        /*用户密码*/
        string usertype;        /*用户类型*/
        int optionstatus = 0;   /*当前选项状态*/
    public:
        /*构造函数，用来初始化一个模型*/
        User();
        User(string mainkey,string username,string userpass,string usertype);
        /*以下是一组getter和setter接口用来交互模型数据，不在过多介绍*/
        string getMainkey();
        void setMainkey(string mainkey);
        string getUsername();
        void setUsername(string username);
        string getUserpass();
        void setUserpass(string userpass);
        string getUserType();
        void setUsertype(string usertype);
        int getOptionstatus();
        void setOptionstatus(int optionstatus);
};

User::User(){
    
}

User::User(string mainkey,string username,string userpass,string usertype){
    this->mainkey = mainkey;
    this->username = username;
    this->userpass = userpass;
    this->usertype = usertype;
}

int User::getOptionstatus(){
    return optionstatus;
}

void User::setOptionstatus(int optionstatus){
    this->optionstatus = optionstatus;
}

string User::getMainkey(){
    return this->mainkey;
}

void User::setMainkey(string mainkey){
    this->mainkey = mainkey;
}

string User::getUsername(){
    return this->username;
}

void User::setUsername(string username){
    this->username = username;
}

string User::getUserpass(){
    return this->userpass;
}

void User::setUserpass(string userpass){
    this->userpass = userpass;
}

string User::getUserType(){
    return this->usertype;
}

void User::setUsertype(string usertype){
    this->usertype = usertype;
}
