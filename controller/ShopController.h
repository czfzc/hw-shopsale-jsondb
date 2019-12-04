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
        map<string,User> session;  /*��ǰ���ڵ�¼���û�*/
        ProductManager productManager;
        UserManager userManager;
        OrderManager orderManager;
        bool checkLogin(string mainkey);
        void exitIfNotTrue(bool status,string errmsg);
        void reloginIfNotTrue(bool status,string errmsg,string mainkey);
    public:
        ShopController();
        bool login(string username,string userpass,string &mainkey);    /*��¼*/
        void logout(string mainkey);        /*ע���˺�*/
        void showOptions(string mainkey);   /*��ʾѡ��*/
        bool selectOption(string &mainkey,string option);    /*ѡ��һ��ѡ��*/
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
        cout<<"��¼�ɹ�"<<endl;
        return true;
    }else{
        cout<<"��¼ʧ��"<<endl;
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
    cout<<"��ʳ�̳� v0.0"<<endl;
    User user;
    this->exitIfNotTrue(userManager.findUserByMainkey(mainkey,user),
                    "�û�״̬������������رգ�errmsg��0");
    if(checkLogin(mainkey)){
        if(user.getUserType() == USER_TYPE_NORMAL_USER){  /*��ͨ�û�*/

        }else if(user.getUserType() == USER_TYPE_ADMIN_USER){

        }else{
            cout<<"�û�״̬�������������µ�¼��errmsg��1"<<endl;
        }
    }else{
        cout<<"1.��¼"<<endl;
        cout<<"2.ע��"<<endl;   
    }
    cout<<"��������ѡ���Լ�����"<<endl;
}

/**
 * �û�δ��¼ʱmainkey��������½�ɹ���session �ѵ�¼ʱmainkey
 */
bool ShopController::selectOption(string &mainkey,string option){
    User user;
    this->exitIfNotTrue(userManager.findUserByMainkey(mainkey,user),
                    "�û�״̬������������رգ�errmsg��0");
    if(user.getOptionstatus()==NORMAL_USER_NO_LOGIN){
        string username,userpass;
        cout<<"�û���:";
        cin>>username;
        cout<<"����";
        cin>>userpass;
        if(!this->login(username,userpass,mainkey)){
            cout<<"��¼ʧ��"<<endl;
            return false;
        }else return true;
    }else{
        cout<<"�û�״̬������������رգ�errmsg��3"<<endl;
    }
}