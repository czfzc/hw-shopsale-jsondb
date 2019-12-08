#pragma once
#include<iostream>
#include<map>
#include<unistd.h>
#include"../model/User.h"
#include"../model/Product.h"
#include"../model/Order.h"
#include"../manager/ProductManager.h"
#include"../manager/UserManager.h"
#include"../manager/OrderManager.h"
using namespace std;

class ShopController{
    private:
        User *user;  /*当前正在登录的用户*/
        ProductManager productManager;
        UserManager userManager;
        OrderManager orderManager;
        vector<Product> cart; /*购物车*/
        string errormsg;
        void exitIfNotTrue(bool status,string errmsg);
        void reloginIfNotTrue(bool status,string errmsg);
        int getchoice();
        string getString(string str);

        /*下面是打印选项和用户选择的私有方法*/
        bool opt_no_login();

        bool opt_normal_logined();

        bool opt_admin_logined();

        bool opt_normal_show_product();

        bool opt_normal_show_order();
    public:
        ShopController();
        bool checkLogin();
        bool login(string username,string userpass);    /*登录*/
        void logout();        /*注销账号*/
        bool selectOption();    /*选择一个选项*/
        string getErrormsg();
};

string ShopController::getErrormsg(){
    return errormsg;
}

void ShopController::exitIfNotTrue(bool status,string errmsg){
    if(!status){
        cout<<errmsg<<endl;
        cout.flush();
        exit(0);
    }
}

bool ShopController::checkLogin(){
    return this->user != nullptr;
}

ShopController::ShopController(){
    user = nullptr;
    cart.clear();
}

bool ShopController::login(string username,string userpass){
    string mainkey;
    if(this->checkLogin())
        return true;
    if(userManager.loginUser(username,userpass,mainkey)){
        this->user = new User;
        userManager.findUserByMainkey(mainkey,*user);
        return true;
    }else{
        this->errormsg = "用户名或密码错误！";
        return false;
    }
}
void ShopController::logout(){
    if(!checkLogin()) return;
    delete user;
    user = nullptr;
}

/**
 * 用户未登录时mainkey用来传登陆成功的session 已登录时mainkey
 */
bool ShopController::selectOption(){
    clearit();
    cout<<"零食商城 v0.0"<<endl;
    if(this->checkLogin()){
        int optstatus = user->getOptionstatus();
        switch(optstatus){
            case USER_NO_LOGIN:{
                return this->opt_no_login();
            }
            case NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE:{
                return this->opt_normal_logined();
            }
            case ADMIN_USER_AFTER_LOGIN_IN_MAIN_PAGE:{
                return this->opt_admin_logined();
            }
            case NORMAL_USER_SHOW_PRODUCT:{ 
                return this->opt_normal_show_product();
            }
            case NORMAL_USER_SHOW_ORDER:{
                return this->opt_normal_show_order();
            }
            default:{
                this->errormsg = "用户状态不正常，程序关闭，errmsg：3";
                return false;
            }
        }
    }else{
        return this->opt_no_login();
    }    
}

int ShopController::getchoice(){
    cout<<"选择您的选项:";
    string choice;
    cin >> choice;
    for(int i = 0;i<choice.length();i++){
        if(!isdigit(choice[i])){
            cout<<"选项无效,请重新";
            return this->getchoice();
        }
    }
    clearit();
    return atoi(choice.c_str());
}

string ShopController::getString(string str){
    cout<<str<<":\t";
    string choice;
    cin>>choice;
    return choice;
}

bool ShopController::opt_no_login(){
    cout<<"1.登录"<<endl;
    cout<<"2.注册"<<endl;
    int option = this->getchoice();

    switch(option){
        case 1:{
            string username,userpass;
            cout<<"用户名:";
            cin>>username;
            cout<<"密码:";
            cin>>userpass;
            sleep(1);
            if(!this->login(username,userpass)){
                cout<<"登录失败"<<endl;
                return false;
            }else{
                cout<<"登录成功"<<endl;
                string usertype = this->user->getUserType();
                if(usertype == USER_TYPE_ADMIN_USER){
                    user->setOptionstatus(ADMIN_USER_AFTER_LOGIN_IN_MAIN_PAGE);
                }else if(usertype == USER_TYPE_NORMAL_USER){
                    user->setOptionstatus(NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE);
                }else{
                    this->errormsg = "用户类型无效!";
                    return false;
                }
                sleep(1);
                return true;
            }
        }
        case 2:{
            string username;
            string userpass;
            string reuserpass;
            cout<<"请输入您的用户名：";
            cin>>username;
            cout<<"请输入您的密码:";
            cin>>userpass;
            cout<<"请重复输入您的密码:";
            cin>>reuserpass;
            sleep(1);
            if(userpass!=reuserpass){
                cout<<"注册失败，您两次输入的密码不一样"<<endl;
                return false;
            }
            if(username.length()<5){
                cout<<"注册失败，用户名长度至少为5个字符"<<endl;
                return false;
            }
            if(username.length()<6){
                cout<<"注册失败，密码长度至少为6个字符"<<endl;
                return false;
            }
            string mainkey;
            if(!userManager.registNromalUser(username,userpass,mainkey)){
                this->errormsg = "注册失败，用户名已存在";
                return false;
            }
            cout<<"注册成功,请登录"<<endl;
            sleep(1);
            return true;
        }   
        default:{
            cout<<"选项无效，请重新选择"<<endl;
            return this->opt_no_login();
        }
    }
 
}

bool ShopController::opt_normal_logined(){
    if(this->checkLogin()){
        cout<<"欢迎来到我们的零食商城"<<endl;
        cout<<"1.购买商品"<<endl;
        cout<<"2.查看我的订单"<<endl;
        cout<<"3.注销"<<endl;
        int option = this->getchoice();
        switch (option){
            case 1:{
                cout<<"你选择了购买商品"<<endl;
                this->user->setOptionstatus(NORMAL_USER_SHOW_PRODUCT);
                sleep(1);
                return true;
            }
            case 2:{
                cout<<"你选择了查看订单"<<endl;
                this->user->setOptionstatus(NORMAL_USER_SHOW_ORDER);
                sleep(1);
                return true;
            }
            case 3:{
                this->logout();
                cout<<"注销成功"<<endl;
                sleep(1);
                return true;
            }
            default:{
                cout<<"选项无效，请重新选择"<<endl;
                return this->opt_normal_logined();
            }
        }
    }else{
        cout<<"登录状态异常，请重新登录"<<endl;
        return this->opt_no_login();
    }

}

bool ShopController::opt_admin_logined(){
    return true;
}

bool ShopController::opt_normal_show_product(){
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    goto retry;
    print:
    productManager.formattedPrintProductList();
    retry:
    cout<<endl<<"选项"<<endl;
    cout<<"1.加入购物车"<<endl;
    cout<<"2.付款"<<endl;
    cout<<"3.查看商品列表"<<endl;
    cout<<"4.清空购物车"<<endl;
    cout<<"5.查看购物车"<<endl;
    cout<<"6.取消并返回"<<endl;
    int option = this->getchoice();
    switch (option)
    {
        case 1:{
            string productmainkey = this->getString("请输入商品号码:");
            Product product;
            if(!productManager.findProductByMainkey(productmainkey,product)){
                cout<<"不存在此商品"<<endl;
                goto retry;
            }
            cart.push_back(product);
            cout<<"添加成功"<<endl;
        }
        case 2:{
            string orderid;
            if(orderManager.payOrder(this->user->getMainkey(),this->cart,orderid)){
                cout<<"付款成功,您的订单状态如下"<<endl;
                pauseit();
            }else{
                cout<<"付款失败"<<endl;
                pauseit();
            }
        }
        case 3:{
            goto print;
        }
        case 4:{
            productManager.formattedPrintProductList(cart);
            pauseit();
            return true;
        }
        case 5:{
            cart.clear();
            cout<<"清空成功"<<endl;
            pauseit();
            return true;
        }
        case 6:{
            this->user->setOptionstatus(NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE);
            return true;
        }
        default:{
            cout<<"您输入的选项不存在,请重新选择."<<endl;
            return this->opt_normal_show_product();
        }
    }
}

bool ShopController::opt_normal_show_order(){
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    orderManager.formattedPrintOrderListByUsermainid(this->user->getMainkey());
    cout<<endl<<"选项"<<endl;
    cout<<"1.删除订单"<<endl;
    cout<<"2.返回"<<endl;
    int choice = this->getchoice();
    switch (choice)
    {
        case 1:{
            string orderid = this->getString("请输入订单号");
            sleep(1);
            if(orderManager.delOrder(orderid)){
                cout<<"删除成功"<<endl;
                pauseit();
                return true;
            }else{
                cout<<"删除失败，您输入的订单号可能有误"<<endl;
                pauseit();
                return false;
            }
            
        }
        case 2:{
            this->user->setOptionstatus(NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE);
            return true;
        }
        default:{
            cout<<"您输入的选项不存在,请重新选择."<<endl;
            return this->opt_normal_show_order();
        }
    }
}
