/**
 * 
 * 存储一些定义
 * 
 */
#pragma once

#define DEFAULT_TABLE_USER "user"
#define DEFAULT_TABLE_PRODUCT "product"
#define DEFAULT_TABLE_ORDER "order"

#define USER_TYPE_ADMIN_USER "0"
#define USER_TYPE_NORMAL_USER "1"

/*
    有关用户当前所在页面的定义
*/
#define USER_NO_LOGIN 0         /*用户未登录状态*/
#define NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE 1      /*普通用户登录进入主页*/
#define NORMAL_USER_REGIST 2        /*用户注册*/
#define NORMAL_USER_SHOW_PRODUCT 3  /*普通用户查看商品*/
#define NORMAL_USER_SHOW_ORDER 4    /*普通用户查看订单*/ 


#define ADMIN_USER_AFTER_LOGIN_IN_MAIN_PAGE 21
#define ADMIN_USER_MANAGE_ORDER 22
#define ADMIN_USER_MANAGE_PRODUCT 23
#define ADMIN_USER_MANAGE_USER 24


#include<vector>
#include<iostream>
#include"../model/Order.h"
#include"../model/Product.h"
#include"../model/Order.h"
#include"../util/DataUtil.h"

using namespace std;

bool equalVectors(vector<string> vectors[],int length){     /*判断这些vector是否相等*/
    if(length == 0)
        return false;
    int len = vectors[0].size();
    for(int i=0;i<length;i++){
        if(vectors[i].size()!=len)
            return false;
    }
    return true;
}

int spilt(string str,char s,string* words,int size){
    int before = 0,after = 0,num=0;
    for(int i=0;;i++){
        if(str[i]==s||str[i]=='\0'){
            after = i;
        }else continue;
        for(int j=before;j<after;j++)
            words[num]+=str[j];
        before = i+1;
        num++;
        if(num>=size||i>=str.length()){
            return num-1;
        }
    }
}

void pauseit(){
    cout<<"按任意键继续"<<endl;
  //  system("read -n 1");
    while(cin.get()!='\n');
    cin.get();
}

void clearit(){
    system("clear");
}