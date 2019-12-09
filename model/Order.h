#pragma once
#include<iostream>
#include<vector>
using namespace std;

/**
 * 订单模型，在MVC架构中属于M即model层
 * 虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 * 链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/model/Order.h
 * authored by 曹子帆 2019.12.9
 */

class Order{
    private:
        string orderid;
        string usermainkey;
        vector<string> productmainids;
        double total;
    public:
        /*构造函数，用来初始化一个模型*/
        Order(string orderid,string usermainkey,vector<string> productmainids,double total);
        Order();
        /*以下是一组getter和setter接口用来交互模型数据，不在过多介绍*/
        string getOrderid();
        void setOrderid(string orderid);
        string getUsermainkey();
        void setUsermainkey(string usermainkey);
        vector<string>& getProductmainids();
        void setProductmainids(vector<string> productmainids);
        double getTotal();
        void setTotal(double total);
};

Order::Order(){
    
}

Order::Order(string orderid,string usermainkey,vector<string> productmainids,double total){
    this->orderid = orderid;
    this->usermainkey = usermainkey;
    this->productmainids = productmainids;
    this->total = total;
}

string Order::getOrderid(){
    return this->orderid;
}
void Order::setOrderid(string orderid){
    this->orderid = orderid;
}
string Order::getUsermainkey(){
    return this->usermainkey;
}
void Order::setUsermainkey(string usermainkey){
    this->usermainkey = usermainkey;
}
vector<string>& Order::getProductmainids(){
    return this->productmainids;
}
void Order::setProductmainids(vector<string> productmainids){
    this->productmainids = vector<string>(productmainids);
}
double Order::getTotal(){
    return this->total;
}
void Order::setTotal(double total){
    this->total = total;
}