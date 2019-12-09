/*
    用户订单管理
*/
#pragma once
#include<iostream>
#include"../util/Const.h"
using namespace std;

/**
 *  本类是用来管理订单（Order）模型的管理器，它在MVC架构中处于repository层，
 *  负责处理数据库映射模型的事务,实现了部分根据字段从数据库获取数据并且映射到
 *  模型的函数
 *  虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 *  链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/manager/OrderManager.h
 *  authored by 曹子帆 2019.12.9
 */

class OrderManager{
    private:
        void mapFromDatabase();                     /*将数据库中信息映射进成员变量Orders中*/
        vector<Order> orders;                       /*当前映射模型并且被维护的的用户列表,可以作为高速缓存*/
        vector<string>& stringToVector(string str,char c);   /*将字符串拆分成数组*/
        string vectorToString(vector<string>& vec,char c);   /*数组连成字符串*/
        UserManager userManager;                    /*用户模型管理器*/
        ProductManager productManager;              /*产品模型管理器*/
    public:
        OrderManager();                             /*构造函数，用来初始化一些变量并且调用mapFromDatabase以开始映射*/
        ~OrderManager();                            /*析构函数，暂时未做任何处理*/
                                                    /*添加订单到数据库*/
        bool addOrder(string usermainkey,double total,vector<string>& productmainids,string &orderid);
        bool delOrder(string orderid);              /*删除订单*/
        bool updateTotal(string orderid,double total);          /*更新订单总价*/
        bool findOrderByOrderid(string orderid,Order& Order);             /*以Orderid获取订单*/
        void formattedPrintOrderList();                             /*格式化打印当前数据库所有订单*/
        void formattedPrintOrderList(vector<Order> orders);         /*指定列表格式化打印*/
        void formattedPrintOrderListByUsermainkey(string mainid);    /*格式化打印指定用户的所有订单*/
        bool findOrdersByUsermainkey(string usermainkey,vector<Order> &orders);     /*根据用户id在数据库中查找订单*/
        bool payOrder(string usermainkey,vector<Product> products,string &orderid); /*下单付款*/
};

bool OrderManager::findOrdersByUsermainkey(string usermainkey,vector<Order> &orders){
    this->mapFromDatabase();
    for(int i=0;i<this->orders.size();i++){
        if(this->orders[i].getUsermainkey() == usermainkey){
            orders.push_back(this->orders[i]);
        }
    }
    return true;
}

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
        if(i!=vec.size()-1)
            str+=c;
    }
    return str;
}

void OrderManager::mapFromDatabase(){
    map<string,vector<string>*> mp;
    map<string,string> cond;
    mp.insert(pair<string,vector<string>*>("orderid",nullptr));
    mp.insert(pair<string,vector<string>*>("productmainids",nullptr));
    mp.insert(pair<string,vector<string>*>("usermainkey",nullptr));
    mp.insert(pair<string,vector<string>*>("total",nullptr));
    if(selectValuesIntoTable(DEFAULT_TABLE_ORDER,mp,cond)){
        vector<string>* orderid = mp["orderid"];
        vector<string>* productmainids = mp["productmainids"];
        vector<string>* usermainkey = mp["usermainkey"];
        vector<string>* total = mp["total"];
        vector<string> vectors[] = {*orderid,*productmainids,*usermainkey,*total};
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
                }
            }
            for(int i=0;i<orders.size();i++){
                orders[i].setOrderid((*orderid)[i]);
                orders[i].setUsermainkey((*usermainkey)[i]);
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
    srand((int)time(0));
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
        cout<<"修改总价失败"<<endl;
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

void OrderManager::formattedPrintOrderList(vector<Order> orders){
    cout<<"\t订单列表"<<endl;
    cout<<"======================"<<endl;
    for(int i=0;i<orders.size();i++){
        cout<<"订单号：\t"<<orders[i].getOrderid()<<endl;
        cout<<"订单总价：\t"<<orders[i].getTotal()<<endl;
        cout<<"用户id：\t"<<orders[i].getUsermainkey()<<endl;
        User user;
        userManager.findUserByMainkey(orders[i].getUsermainkey(),user);
        cout<<"用户名：\t"<<user.getUsername()<<endl;
        cout<<"商品列表:"<<endl;
        vector<Product> products;
        productManager.findProductsByMainkeys(orders[i].getProductmainids(),products);
        productManager.formattedPrintProductList(products);
        if(i!=orders.size()-1)
            cout<<"-----------------------"<<endl;
    }
    cout<<"======================"<<endl;
}

void OrderManager::formattedPrintOrderList(){
    this->mapFromDatabase();
    this->formattedPrintOrderList(this->orders);
}

void OrderManager::formattedPrintOrderListByUsermainkey(string usermainkey){
    vector<Order> orders; 
    this->findOrdersByUsermainkey(usermainkey,orders);
    this->formattedPrintOrderList(orders);
}

bool OrderManager::payOrder(string usermainkey,vector<Product> products,string &orderid){
    double total=0;
    vector<string> ids;
    for(int i=0;i<products.size();i++){
        total+=products[i].getPrice();
        ids.push_back(products[i].getMainkey());
    }
    return this->addOrder(usermainkey,total,ids,orderid);
}