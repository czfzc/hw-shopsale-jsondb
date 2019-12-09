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

/**
 *  此类用来作为与用户交互操作的主控制器 在MVC架构中属于controller层 
 *  提供登录注销和获取选项的公有接口，使用简单，只需要创建对象，
 *  调用获取选项的接口并且互动即可    
 *  虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 *  链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/controller/ShopController.h
 *  authored by 曹子帆 2019.12.9
 */ 

class ShopController{
    private:
        User *user;  /*当前正在登录的用户*/
        ProductManager productManager;  /*商品模型的数据库中间层 用来管理商品*/
        UserManager userManager;        /*用户模型的数据库中间层 用来管理商品*/
        OrderManager orderManager;      /*订单模型的数据库中间层 用来管理商品*/
        vector<Product> cart;           /*普通用户的购物车*/
        string errormsg;                /*错误消息，实际上未使用*/
        static void exitIfNotTrue(bool status,string errmsg);       /*工具函数，实际未使用*/
        static void reloginIfNotTrue(bool status,string errmsg);    /*工具函数，实际未使用*/
        static int getchoice();                /*工具函数，用来获取选项*/
        static string getString(string str);    /*工具函数，用来获取输入字符串*/

        bool opt_no_login();    /*处理未登录的交互操作*/
        bool opt_normal_logined();  /*处理普通用户登陆后的交互操作*/
        bool opt_admin_logined();   /*处理管理员用户登陆后的交互操作*/
        bool opt_normal_show_product(); /*处理普通用户获取可以购买的商品的交互操作*/
        bool opt_normal_show_order();   /*处理普通用户获取订单的交互操作*/
        bool opt_admin_manage_order();  /*处理管理员用户管理订单的交互操作*/
        bool opt_admin_manage_user();   /*处理管理员用户管理用户的交互操作*/
        bool opt_admin_manage_product();    /*处理管理员用户管理商品的交互操作*/
    public:
        ShopController();                               /*构造函数*/
        bool checkLogin();                              /*检查是否已经登录*/
        bool login(string username,string userpass);    /*登录*/
        void logout();                                  /*注销账号*/
        bool selectOption();                            /*选择一个选项*/
        string getErrormsg();                           /*获取错误消息*/
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

bool ShopController::selectOption(){
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
            case ADMIN_USER_MANAGE_ORDER:{
                return this->opt_admin_manage_order();
            }
            case ADMIN_USER_MANAGE_PRODUCT:{
                return this->opt_admin_manage_product();
            }
            case ADMIN_USER_MANAGE_USER:{
                return this->opt_admin_manage_user();
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
            return getchoice();
        }
    }
    clearit();
    return atoi(choice.c_str());
}

string ShopController::getString(string str){
    cout<<str<<": ";
    string choice;
    cin>>choice;
    return choice;
}

bool ShopController::opt_no_login(){
    clearit();
    cout<<"欢迎来到零食商城，登录以继续"<<endl;
    cout<<"1.登录"<<endl;
    cout<<"2.注册"<<endl;
    cout<<"0.退出"<<endl;
    int option = this->getchoice();
    switch(option){
        case 0:{
            pauseit();
            exit(0);
        }
        case 1:{
            clearit();
            string username,userpass;
            cout<<"用户名:";
            cin>>username;
            cout<<"密码:";
            cin>>userpass;
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
                pauseit();
                return true;
            }
        }
        case 2:{
            clearit();
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
    clearit();
    if(this->user->getUserType()!=USER_TYPE_NORMAL_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    if(this->checkLogin()){
        cout<<this->user->getUsername()<<",欢迎来到我们的零食商城"<<endl;
        cout<<"1.购买商品"<<endl;
        cout<<"2.查看我的订单"<<endl;
        cout<<"3.注销"<<endl;
        int option = this->getchoice();
        switch (option){
            case 1:{
                this->user->setOptionstatus(NORMAL_USER_SHOW_PRODUCT);
                return true;
            }
            case 2:{
                this->user->setOptionstatus(NORMAL_USER_SHOW_ORDER);
                return true;
            }
            case 3:{
                this->logout();
                cout<<"注销成功"<<endl;
                pauseit();
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
    clearit();
    if(this->user->getUserType()!=USER_TYPE_ADMIN_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    if(this->checkLogin()){
        cout<<this->user->getUsername()<<",欢迎来到零食商城管理页面"<<endl;
        cout<<"1.订单管理"<<endl;
        cout<<"2.商品管理"<<endl;
        cout<<"3.用户管理"<<endl;
        cout<<"4.注销"<<endl;
        int option = this->getchoice();
        switch (option){
            case 1:{
                this->user->setOptionstatus(ADMIN_USER_MANAGE_ORDER);
                return true;
            }
            case 2:{
                this->user->setOptionstatus(ADMIN_USER_MANAGE_PRODUCT);
                return true;
            }
            case 3:{
                this->user->setOptionstatus(ADMIN_USER_MANAGE_USER);
                return true;
            }
            case 4:{
                this->logout();
                cout<<"注销成功"<<endl;
                pauseit();
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

bool ShopController::opt_normal_show_product(){
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    if(this->user->getUserType()!=USER_TYPE_NORMAL_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    goto retry;
    print:
    clearit();
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
            string productmainkey = this->getString("请输入商品号码");
            Product product;
            if(!productManager.findProductByMainkey(productmainkey,product)){
                cout<<"不存在此商品"<<endl;
                goto retry;
            }
            cart.push_back(product);
            cout<<"添加成功"<<endl;
            return true;
        }
        case 2:{
            string orderid;
            if(orderManager.payOrder(this->user->getMainkey(),this->cart,orderid)){
                cout<<"付款成功,您的订单状态如下"<<endl;
                Order order;
                vector<Order> orders;
                if(orderManager.findOrderByOrderid(orderid,order)){
                    orders.push_back(order);
                    orderManager.formattedPrintOrderList(orders);
                }
                pauseit();
            }else{
                cout<<"付款失败"<<endl;
                pauseit();
            }
            return true;
        }
        case 3:{
            clearit();
            goto print;
        }
        case 4:{
            cart.clear();
            cout<<"清空成功"<<endl;
            pauseit();
            return true;
        }
        case 5:{
            productManager.formattedPrintProductList(cart);
            pauseit();
            return true;
        }
        case 6:{
            this->user->setOptionstatus(NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE);
            clearit();
            return true;
        }
        default:{
            cout<<"您输入的选项不存在,请重新选择."<<endl;
            return this->opt_normal_show_product();
        }
    }
}

bool ShopController::opt_normal_show_order(){
    clearit();
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    if(this->user->getUserType()!=USER_TYPE_NORMAL_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    orderManager.formattedPrintOrderListByUsermainkey(this->user->getMainkey());
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

bool ShopController::opt_admin_manage_order(){
    clearit();
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    if(this->user->getUserType()!=USER_TYPE_ADMIN_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    cout<<"1.查看所有订单"<<endl;
    cout<<"2.删除用户订单"<<endl;
    cout<<"3.查找某位用户所有订单"<<endl;
    cout<<"4.查找某个订单"<<endl;
    cout<<"0.返回"<<endl;
    int option = this->getchoice();
    switch(option){
        case 0:{
            this->user->setOptionstatus(ADMIN_USER_AFTER_LOGIN_IN_MAIN_PAGE);
            return true;
        }
        case 1:{
            clearit();
            orderManager.formattedPrintOrderList();
            pauseit();
            return true;
        }
        case 2:{
            string orderid = this->getString("请输入订单号");
            if(orderManager.delOrder(orderid)){
                cout<<"删除成功"<<endl;
            }else{
                cout<<"不存在此订单"<<endl;
            }
            pauseit();
            return true;
        }
        case 3:{
            clearit();
            string usermainkey = this->getString("请输入用户id");
            vector<Order> orders;
            if(orderManager.findOrdersByUsermainkey(usermainkey,orders)){
                cout<<"该用户的所有订单如下:"<<endl;
                orderManager.formattedPrintOrderList(orders);
            }else{
                cout<<"未查找到结果"<<endl;
            }
            pauseit();
            return true;
        }
        case 4:{
            clearit();
            string orderid = this->getString("请输入订单号");
            Order order;
            if(orderManager.findOrderByOrderid(orderid,order)){
                cout<<"此订单如下"<<endl;
                vector<Order> odrs;
                odrs.push_back(order);
                orderManager.formattedPrintOrderList(odrs);
            }else{
                cout<<"未查找到结果"<<endl;
            }
            pauseit();
            return true;
        }
        default:{
            cout<<"您输入的选项不存在，请重新输入"<<endl;
            return this->opt_admin_manage_order();
        }
    }
    orderManager.formattedPrintOrderList();
}

bool ShopController::opt_admin_manage_product(){
    clearit();
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    if(this->user->getUserType()!=USER_TYPE_ADMIN_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    cout<<"1.查看所有商品"<<endl;
    cout<<"2.删除商品"<<endl;
    cout<<"3.添加商品"<<endl;
    cout<<"4.查询商品"<<endl;
    cout<<"0.返回"<<endl;
    int option = this->getchoice();
    switch(option){
        case 1:{
            clearit();
            productManager.formattedPrintProductList();
            pauseit();
            return true;
        }
        case 2:{
            clearit();
            string productmainkey = this->getString("请输入商品id");
            if(productManager.delProduct(productmainkey)){
                cout<<"删除成功"<<endl;
            }else{
                cout<<"删除失败,您输入的商品不存在"<<endl;
            }
            pauseit();
            return true;
        }
        case 3:{
            clearit();
            string name = this->getString("请输入商品名:");
            string rest = this->getString("请输入商品余量:");
            string price = this->getString("请输入商品价格");
            string type = this->getString("请输入商品类型");
            string mainkey;
            if(productManager.addProduct(name,atoi(rest.c_str()),atof(price.c_str()),type,mainkey)){
                Product product;
                if(productManager.findProductByMainkey(mainkey,product)){
                    cout<<"添加商品成功，以下是商品信息:"<<endl;
                    vector<Product> pros;
                    pros.push_back(product);
                    productManager.formattedPrintProductList(pros);
                }else{
                    cout<<"添加商品失败,err 2"<<endl;
                }
            }else{
                cout<<"添加商品失败,err 1"<<endl;
            }
            pauseit();
            return true;
        }
        case 4:{
            clearit();
            string name = this->getString("请输入商品名");
            vector<Product> pros;
            if(productManager.findProductsByNameLike(name,pros)){
                cout<<"以下是符合此商品名的商品:"<<endl;
                productManager.formattedPrintProductList(pros);
            }else{
                cout<<"未查找到此商品"<<endl;
            }
            pauseit();
            return true;
        }
        case 0:{
            this->user->setOptionstatus(ADMIN_USER_AFTER_LOGIN_IN_MAIN_PAGE);
            return true;
        }
    }
}

bool ShopController::opt_admin_manage_user(){
    clearit();
    if(!this->checkLogin()){
        cout<<"用户未登录"<<endl;
        return false;
    }
    if(this->user->getUserType()!=USER_TYPE_ADMIN_USER){
        cout<<"用户状态异常"<<endl;
        return false;
    }
    clearit();
    cout<<"1.获取所有用户"<<endl;
    cout<<"2.删除用户"<<endl;
    cout<<"3.搜索用户"<<endl;
    cout<<"0.返回"<<endl;
    int option = this->getchoice();
    switch(option){
        case 0:{
            this->user->setOptionstatus(ADMIN_USER_AFTER_LOGIN_IN_MAIN_PAGE);
            return true;
        }
        case 1:{
            clearit();
            userManager.formattedPrintProductList();
            pauseit();
            return true;
        }
        case 2:{
            string mainkey = this->getString("请输入用户id");
            if(userManager.delUser(mainkey)){
                cout<<"删除用户成功"<<endl;
            }else{
                cout<<"删除用户失败"<<endl;
            }   
            pauseit();
            return true;
        }
        case 3:{
            string username = this->getString("请输入用户名称");
            vector<User> users;
            if(userManager.findUsersByUserLike(username,users)){
                cout<<"以下是搜索结果"<<endl;
                userManager.formattedPrintProductList(users);
            }else{
                cout<<"搜索不到此用户"<<endl;
            }
            pauseit();
            return true;
        }
    }
}