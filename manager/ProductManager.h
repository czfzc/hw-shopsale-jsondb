#pragma once
#include<iostream>
#include "../util/DataUtil.h"
#include "../model/Product.h"
#include"../util/Const.h"
using namespace std;

/**
 *  本类是用来管理商品（Product）模型的管理器，它在MVC架构中处于repository层，
 *  负责处理数据库映射模型的事务,实现了部分根据字段从数据库获取数据并且映射到
 *  模型的函数
 *  虽然已被提交作业，但是代码可能被更新在github，您可以在主目录下使用git pull origin master命令拉取以更新代码
 *  链接https://github.com/czfzc/hw-shopsale-jsondb/blob/master/manager/ProductManager.h
 *  authored by 曹子帆 2019.12.9
 */

class ProductManager{
    private:
        void mapFromDatabase();             /*从数据库映射到模型*/
        vector<Product> products;        /*当前映射并且被维护的的用户列表*/
    public:
        ProductManager();                   /*构造函数，用来初始化一些成员变量以及调用mapFromDatabase函数以映射模型到高速缓存*/
        ~ProductManager();                  /*析构函数*/
        bool addProduct(string name,int rest,double price,string type,string &mainkey); /*添加商品*/
        bool delProduct(string mainkey);                            /*根据商品id删除商品*/
        bool updateName(string mainkey,string name);                /*更新商品名称*/
        bool updateRest(string mainkey,int rest);                   /*更新商品剩余量*/
        bool updatePrice(string mainkey,double price);              /*更新商品价格*/
        bool updateType(string mainkey,string type);                /*更新商品类型*/
        bool findProductByMainkey(string mainkey,Product &product);         /*查找商品*/
        bool findProductsByNameLike(string name,vector<Product> &products);     /*根据名称模糊查找*/
        void formattedPrintProductList();                           /*格式化输出所有商品*/
        static void formattedPrintProductList(vector<Product> products);    /*根据列表格式化输出商品*/
        bool findProductsByMainkeys(vector<string> mainkeys,vector<Product> &products); /*批量查找商品*/
};

void ProductManager::mapFromDatabase(){
    map<string,vector<string>*> mp;
    map<string,string> cond;
    mp.insert(pair<string,vector<string>*>("mainkey",nullptr));
    mp.insert(pair<string,vector<string>*>("name",nullptr));
    mp.insert(pair<string,vector<string>*>("price",nullptr));
    mp.insert(pair<string,vector<string>*>("rest",nullptr));
    mp.insert(pair<string,vector<string>*>("type",nullptr));
    if(selectValuesIntoTable(DEFAULT_TABLE_PRODUCT,mp,cond)){
        vector<string>* mainkey = mp["mainkey"];
        vector<string>* name = mp["name"];
        vector<string>* price = mp["price"];
        vector<string>* rest = mp["rest"];
        vector<string>* type = mp["type"];
        vector<string> vectors[] = {*mainkey,*name,*price,*rest,*type};
        if(equalVectors(vectors,5)){
            if(products.size()==mainkey->size()){
                //do nothing
            }else if(products.size()<mainkey->size()){
                int c = mainkey->size()-products.size();
                for(int i=0;i<c;i++){
                    products.push_back(*(new Product));
                }
            }else{
                int c = products.size()-mainkey->size();
                for(int i=0;i<c;i++){
                    Product del = products.back();
                    products.pop_back();
                    delete &del;
                }
            }
            for(int i=0;i<products.size();i++){
                products[i].setMainkey((*mainkey)[i]);
                products[i].setName((*name)[i]);
                double pricedouble = atof((*price)[i].c_str());
                products[i].setPrice(pricedouble);
                int restint = atoi((*rest)[i].c_str());
                products[i].setRest(restint);
                products[i].setType((*type)[i]);
            }
        }else{
            cout<<"失败,有空字段"<<endl;
        }
    }else{
        cout<<"数据库获取失败"<<endl;
    }
}

ProductManager::ProductManager(){
    this->mapFromDatabase();
}
ProductManager::~ProductManager(){

}

bool ProductManager::addProduct(string name,int rest,double price,string type,string &mainkey){
    mainkey = to_string(rand());
    map<string,string> mp;
    mp.insert(pair<string,string>("name",name));
    mp.insert(pair<string,string>("price",to_string(price)));
    mp.insert(pair<string,string>("rest",to_string(rest)));
    mp.insert(pair<string,string>("type",type));
    if(insertValuesWithMainkeyIntoTable(DEFAULT_TABLE_PRODUCT,mp,"mainkey",mainkey)){
        this->mapFromDatabase();
        return true;
    }else{
        return false;
    }
}
bool ProductManager::delProduct(string mainkey){
    if(removeItByKey(DEFAULT_TABLE_PRODUCT,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        return false;
    }
}
bool ProductManager::updateName(string mainkey,string name){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"name",name,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户密码失败"<<endl;
        return false;
    }
}
bool ProductManager::updateRest(string mainkey,int rest){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"rest",to_string(rest),"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户密码失败"<<endl;
        return false;
    }
}
bool ProductManager::updatePrice(string mainkey,double price){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"price",to_string(price),"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户密码失败"<<endl;
        return false;
    }
}
bool ProductManager::updateType(string mainkey,string type){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"type",type,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"修改用户密码失败"<<endl;
        return false;
    }
}
bool ProductManager::findProductByMainkey(string mainkey,Product &product){
    for(int i=0;i<products.size();i++){
        if(products[i].getMainkey() == mainkey){
            product = products[i];
            return true;
        }
    }
    return false;
}

void ProductManager::formattedPrintProductList(vector<Product> products){
    cout<<"======================"<<endl;
    for(int i=0;i<products.size();i++){
        cout<<"品名：\t"<<products[i].getName()<<endl;
        cout<<"价格：\t"<<products[i].getPrice()<<endl;
        cout<<"余量：\t"<<products[i].getRest()<<endl;
        cout<<"种类：\t"<<products[i].getType()<<endl;
        cout<<"商品号：\t"<<products[i].getMainkey()<<endl;
        if(i!=products.size()-1)
            cout<<"-----------------------"<<endl;
    }
    cout<<"======================"<<endl;
}

bool ProductManager::findProductsByMainkeys(vector<string> mainkeys,vector<Product> &products){
    for(int i=0;i<mainkeys.size();i++){
        Product product;
        if(!findProductByMainkey(mainkeys[i],product)){
            cout<<"获取商品id为"<<mainkeys[i]<<"失败"<<endl;
            return false;
        }
        products.push_back(product);
    }
}

bool ProductManager::findProductsByNameLike(string name,vector<Product> &products){
    this->mapFromDatabase();
    bool status = false;
    for(int i=0;i<this->products.size();i++){
        if(this->products[i].getName().find(name)!=-1){
            products.push_back(this->products[i]);
            status = true;
        }
    }
    return status;
}

void ProductManager::formattedPrintProductList(){
    this->mapFromDatabase();
    cout<<"\t商品列表"<<endl;
    this->formattedPrintProductList(this->products);
}
