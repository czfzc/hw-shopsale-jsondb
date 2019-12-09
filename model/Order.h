#pragma once
#include<iostream>
#include<vector>
using namespace std;

/**
 * ����ģ�ͣ���MVC�ܹ�������M��model��
 * ��Ȼ�ѱ��ύ��ҵ�����Ǵ�����ܱ�������github������������Ŀ¼��ʹ��git pull origin master������ȡ�Ը��´���
 * ����https://github.com/czfzc/hw-shopsale-jsondb/blob/master/model/Order.h
 * authored by ���ӷ� 2019.12.9
 */

class Order{
    private:
        string orderid;
        string usermainkey;
        vector<string> productmainids;
        double total;
    public:
        /*���캯����������ʼ��һ��ģ��*/
        Order(string orderid,string usermainkey,vector<string> productmainids,double total);
        Order();
        /*������һ��getter��setter�ӿ���������ģ�����ݣ����ڹ������*/
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