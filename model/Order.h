#pragma once
#include<iostream>
#include<vector>
using namespace std;

class Order{
    private:
        string orderid;
        string usermainkey;
        vector<string> productmainids;
        double total;
    public:
        string getOrderid();
        void setOrderid(string orderid);
        string getUsermainkey();
        void setUsermainkey(string usermainkey);
        vector<string>& getProductmainids();
        void setProductmainids(vector<string> productmainids);
        double getTotal();
        void setTotal(double total);
};

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