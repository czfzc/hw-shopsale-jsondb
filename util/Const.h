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
#define NORMAL_USER_NO_LOGIN 0
#define NORMAL_USER_AFTER_LOGIN_IN_MAIN_PAGE 1


#include<vector>

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