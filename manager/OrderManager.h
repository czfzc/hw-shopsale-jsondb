/*
    用户订单管理
*/
#pragma once
#include<iostream>
#include"../util/DataUtil.h"
#include"../util/Const.h"
#include"../model/Order.h"
using namespace std;

class OrderManager{
    private:
        void mapFromDatabase();     /*将数据库中信息映射进成员变量Orders中*/
        vector<Order> orders;        /*当前映射并且被维护的的用户列表*/
        vector<string>& stringToVector(string str,char c);   /*将字符串拆分成数组*/
        string vectorToString(vector<string>& vec,char c);   /*数组连成字符串*/
    public:
        OrderManager();
        ~OrderManager();
        bool addOrder(string usermainkey,double total,vector<string>& productmainids,string &orderid);
        bool delOrder(string orderid);
        bool updateTotal(string orderid,double total);
        bool findOrderByOrderid(string orderid,Order& Order);             /*以Orderid获取订单*/
};

vector<string>& OrderManager::stringToVector(string str,char c){
    int size = 100;
    string words[100];
    int num = spilt(str,c,words,size);
    vector<string> *word=new vector<string>;
    for(int i = 0;i<num;i++){
        word->push_back(words[i]);
    }
    return *word;
}

string OrderManager::vectorToString(vector<string>& vec,char c){
    string str = "";
    for(int i=0;i<vec.size();i++){
        str+=vec[i];
        if(i==vec.size()-1)
            str+=c;
    }
    return str;
}

void OrderManager::mapFromDatabase(){
    map<string,vector<string>*> mp;
    map<string,string> cond;
    mp.insert(pair<string,vector<string>*>("orderid",nullptr));
    mp.insert(pair<string,vector<string>*>("productmainids",nullptr));
    mp.insert(pair<string,vector<string>*>("usermainid",nullptr));
    mp.insert(pair<string,vector<string>*>("total",nullptr));
    if(selectValuesIntoTable(DEFAULT_TABLE_ORDER,mp,cond)){
        vector<string>* orderid = mp["orderid"];
        vector<string>* productmainids = mp["productmainids"];
        vector<string>* usermainid = mp["usermainid"];
        vector<string>* total = mp["total"];
        vector<string> vectors[] = {*orderid,*productmainids,*usermainid,*total};
        if(equalVectors(vectors,4)){
            if(orders.size()==orderid->size()){
                //do nothing
            }else if(orders.size()<orderid->size()){
                int c = orderid->size()-orders.size();
                for(int i=0;i<c;i++){
                    orders.push_back(*(new Order));
                }
            }else{
                int c = orders.size()-orderid->size();
                for(int i=0;i<c;i++){
                    Order del = orders.back();
                    orders.pop_back();
                    delete &del;
                }
            }
            for(int i=0;i<orders.size();i++){
                orders[i].setOrderid((*orderid)[i]);
                orders[i].setUsermainid((*usermainid)[i]);
                vector<string> ids = stringToVector((*productmainids)[i],',');
                orders[i].setProductmainids(ids);
                orders[i].setTotal(atof((*total)[i].c_str()));
            }
        }else{
            cout<<"失败,有空字段"<<endl;
        }
    }else{
        cout<<"数据库获取失败"<<endl;
    }
}
OrderManager::OrderManager(){
    this->mapFromDatabase();
}
OrderManager::~OrderManager(){

}
bool OrderManager::addOrder(string usermainkey,double total,vector<string>& productmainids,string &orderid){
    orderid = to_string(rand());
    map<string,string> mp;
    mp.insert(pair<string,string>("usermainkey",usermainkey));
    mp.insert(pair<string,string>("total",to_string(total)));
    mp.insert(pair<string,string>("productmainids",vectorToString(productmainids,',')));
    if(insertValuesWithMainkeyIntoTable(DEFAULT_TABLE_ORDER,mp,"orderid",orderid)){
        this->mapFromDatabase();
        return true;
    }else{
        cout<<"添加订单失败"<<endl;
        return false;
    }
}
bool OrderManager::delOrder(string orderid){
    if(removeItByKey(DEFAULT_TABLE_ORDER,"orderid",orderid)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"删除商品失败"<<endl;
        return false;
    }
}
bool OrderManager::updateTotal(string orderid,double total){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"total",to_string(total),"orderid",orderid)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户密码失败"<<endl;
        return false;
    }
}
bool OrderManager::findOrderByOrderid(string orderid,Order& order){
    for(int i=0;i<orders.size();i++){
        if(orders[i].getOrderid() == orderid){
            order = orders[i];
            return true;
        }
    }
    return false;
}