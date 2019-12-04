#pragma once
#include<iostream>
#include<map>
#include"../model/User.h"
#include"../model/Product.h"
#include"../model/Order.h"
#include"../manager/ProductManager.h"
#include"../manager/UserManager.h"
#include"../manager/OrderManager.h"
using namespace std;

class ShopController{
    private:
        map<string,User> session;  /*当前正在登录的用户*/
        ProductManager productManager;
        UserManager userManager;
        OrderManager orderManager;
        bool checkLogin(string mainkey);
        void exitIfNotTrue(bool status,string errmsg);
        void reloginIfNotTrue(bool status,string errmsg,string mainkey);
    public:
        ShopController();
        bool login(string username,string userpass,string &mainkey);    /*登录*/
        void logout(string mainkey);        /*注销账号*/
        void showOptions(string mainkey);   /*显示选项*/
        bool selectOption(string &mainkey,string option);    /*选择一个选项*/
};

void ShopController::exitIfNotTrue(bool status,string errmsg){
    if(!status){
        cout<<errmsg<<endl;
        cout.flush();
        exit(0);
    }
}

void ShopController::reloginIfNotTrue(bool status,string errmsg,string mainkey){
    if(session.count(mainkey)==0)
        return;
    User user = session[mainkey];
    if(!checkLogin(mainkey)){
        user.setOptionstatus(NORMAL_USER_NO_LOGIN);
        return;
    }
    if(!status){
        cout<<errmsg<<endl;
        cout.flush();
        user.setOptionstatus(NORMAL_USER_NO_LOGIN);
        return;
    }
}

bool ShopController::checkLogin(string mainkey){
    return session.count(mainkey) == 1;
}

ShopController::ShopController(){

}

bool ShopController::login(string username,string userpass,string &mainkey){
    if(userManager.loginUser(username,userpass,mainkey)){
        cout<<"登录成功"<<endl;
        return true;
    }else{
        cout<<"登录失败"<<endl;
        return false;
    }
    User user;
    userManager.findUserByMainkey(mainkey,user);
    session.insert(pair<string,User>(mainkey,user));
}
void ShopController::logout(string mainkey){
    if(!checkLogin(mainkey)) return;
    session.erase(session.find(mainkey));
}
void ShopController::showOptions(string mainkey){
    cout<<"零食商城 v0.0"<<endl;
    User user;
    this->exitIfNotTrue(userManager.findUserByMainkey(mainkey,user),
                    "用户状态不正常，程序关闭，errmsg：0");
    if(checkLogin(mainkey)){
        if(user.getUserType() == USER_TYPE_NORMAL_USER){  /*普通用户*/

        }else if(user.getUserType() == USER_TYPE_ADMIN_USER){

        }else{
            cout<<"用户状态不正常，请重新登录，errmsg：1"<<endl;
        }
    }else{
        cout<<"1.登录"<<endl;
        cout<<"2.注册"<<endl;   
    }
    cout<<"输入您的选项以继续："<<endl;
}

/**
 * 用户未登录时mainkey用来传登陆成功的session 已登录时mainkey
 */
bool ShopController::selectOption(string &mainkey,string option){
    User user;
    this->exitIfNotTrue(userManager.findUserByMainkey(mainkey,user),
                    "用户状态不正常，程序关闭，errmsg：0");
    if(user.getOptionstatus()==NORMAL_USER_NO_LOGIN){
        string username,userpass;
        cout<<"用户名:";
        cin>>username;
        cout<<"密码";
        cin>>userpass;
        if(!this->login(username,userpass,mainkey)){
            cout<<"登录失败"<<endl;
            return false;
        }else return true;
    }else{
        cout<<"用户状态不正常，程序关闭，errmsg：3"<<endl;
    }
}