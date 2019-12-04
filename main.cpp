#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<unistd.h>
#include "lib/CJsonObject.h" 
#include "util/DataUtil.h"
#include "manager/UserManager.h"
#include "controller/ShopController.h"
using namespace std;

int main()
{

   // neb::CJsonObject json;
   // json.Add("name");
   // cout<<(json.IsEmpty());
 //  map<string,string> mp;
 //  mp.insert(pair<string,string>("username","mike"));
  // mp.insert(pair<string,string>("userpass","654321"));
  // mp.insert(pair<string,string>("usertype","1"));
  // insertValuesWithMainkeyIntoTable("user",mp,"mainkey","123131");
  // updateValueIntoTable("user","usertype","0","mainkey","123130");
   //cout<<getValueByMainKey("user","age","mainkey","123127");
   //removeItByKey("user","mainkey","123127");
  // UserManager um;
  // string mainkey;
  // cout<<um.loginNormalUser("admin","123456",mainkey);
    string mainkey;
    ShopController shop;
    string option;
    while(true){
      shop.showOptions(mainkey);
      cin>>option;
      shop.selectOption(mainkey,option);
    }
}