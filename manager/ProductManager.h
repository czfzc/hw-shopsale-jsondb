#pragma once
#include<iostream>
#include "../util/DataUtil.h"
#include "../model/Product.h"
#include"../util/Const.h"
using namespace std;

class ProductManager{
    private:
        void mapFromDatabase();
        vector<Product> products;        /*��ǰӳ�䲢�ұ�ά���ĵ��û��б�*/
    public:
        ProductManager();
        ~ProductManager();
        bool addProduct(string name,int rest,double price,string type,string &mainkey);
        bool delProduct(string mainkey);
        bool updateName(string mainkey,string name);
        bool updateRest(string mainkey,int rest);
        bool updatePrice(string mainkey,double price);
        bool updateType(string mainkey,string type);
        bool findProductByMainkey(string mainkey,Product &product);
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
            cout<<"ʧ��,�п��ֶ�"<<endl;
        }
    }else{
        cout<<"���ݿ��ȡʧ��"<<endl;
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
        cout<<"�����Ʒʧ��"<<endl;
        return false;
    }
}
bool ProductManager::delProduct(string mainkey){
    if(removeItByKey(DEFAULT_TABLE_PRODUCT,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"ɾ����Ʒʧ��"<<endl;
        return false;
    }
}
bool ProductManager::updateName(string mainkey,string name){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"name",name,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"�޸��û�����ʧ��"<<endl;
        return false;
    }
}
bool ProductManager::updateRest(string mainkey,int rest){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"rest",to_string(rest),"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"�޸��û�����ʧ��"<<endl;
        return false;
    }
}
bool ProductManager::updatePrice(string mainkey,double price){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"price",to_string(price),"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"�޸��û�����ʧ��"<<endl;
        return false;
    }
}
bool ProductManager::updateType(string mainkey,string type){
    if(updateValueIntoTable(DEFAULT_TABLE_PRODUCT,"type",type,"mainkey",mainkey)){
        mapFromDatabase();
        return true;
    }else{
        cout<<"�޸��û�����ʧ��"<<endl;
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