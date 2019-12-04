#pragma once
#include<iostream>
using namespace std;

class Product{
    private:
        string mainkey;
        string name;
        int rest;
        double price;
        string type;
    public:
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