#include<iostream>
using namespace std;

class User{
    private:
        string mainkey;
        string username;
        string userpass;
        string usertype;
    public:
        User(string mainkey,string username,string userpass,string usertype);
        string getMainkey();
        void setMainKey(string mainkey);
        string getUsername();
        void setUsername(string username);
        string getUserpass();
        void setUserpass(string userpass);
        string getUserType();
        void setUsertype(string usertype);
}

User::User(string mainkey,string username,string userpass,string usertype){
    this->mainkey = mainkey;
    this->username = username;
    this->userpass = userpass;
    this->usertype = usertype;
}

string User::getMainkey(){
    return this->mainkey;
}

void User::setMainKey(string mainkey){
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
