#pragma once
#include<iostream>
using namespace std;

class User{
    private:
        string mainkey;         /*用户的主键*/
        string username;        /*用户名*/
        string userpass;        /*用户密码*/
        string usertype;        /*用户类型*/
        int optionstatus = 0;   /*当前选项状态*/
    public:
        User();
        User(string mainkey,string username,string userpass,string usertype);
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
