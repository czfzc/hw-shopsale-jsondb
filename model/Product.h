#pragma once
#include<iostream>
using namespace std;

/**
 * 商品模型，在MVC架构中属于M即model层
 * 虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 * 链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/model/Order.h
 * authored by 曹子帆 2019.12.9
 */

class Product{
    private:
        string mainkey;         /*商品id*/
        string name;            /*商品名*/
        int rest;               /*剩余量*/
        double price;           /*商品价格*/
        string type;            /*商品类型*/
    public:
        /*构造函数，用来初始化一个模型*/
        Product(string mainkey,string name,int rest,double price,string type);
        Product();
        /*以下是一组getter和setter接口用来交互模型数据，不在过多介绍*/
        string getMainkey();    
        void setMainkey(string mainkey);
        string getName();
        void setName(string name);
        int getRest();
        void setRest(int rest);
        double getPrice();
        void setPrice(double price);
        string getType();
        void setType(string type);
};

Product::Product(){
    
}

Product::Product(string mainkey,string name,int rest,double price,string type){
    this->mainkey = mainkey;
    this->name = name;
    this->rest = rest;
    this->price = price;
    this->type = type;
}

string Product::getMainkey(){
    return this->mainkey;
}

void Product::setMainkey(string mainkey){
    this->mainkey = mainkey;
}

string Product::getName(){
    return name;
}
void Product::setName(string name){
    this->name = name;
}
int Product::getRest(){
    return rest;
}
void Product::setRest(int rest){
    this->rest = rest;
}
double Product::getPrice(){
    return price;
}
void Product::setPrice(double price){
    this->price = price;
}
string Product::getType(){
    return this->type;
}
void Product::setType(string type){
    this->type = type;
}