#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include "lib/CJsonObject.h" 
#include "util/DataUtil.h"
//#include "manager/UserManager.h"
using namespace std;

int main()
{

   // neb::CJsonObject json;
   // json.Add("name");
   // cout<<(json.IsEmpty());
   map<string,string> mp;
   mp.insert(pair<string,string>("username","admin"));
   mp.insert(pair<string,string>("userpass","123456"));
   mp.insert(pair<string,string>("usertype","1"));
   insertValuesWithMainkeyIntoTable("user",mp,"mainkey","123130");
   //updateValueIntoTable("user","age","11","mainkey","123127");
   //cout<<getValueByMainKey("user","age","mainkey","123127");
   //removeItByKey("user","mainkey","123127");
  // UserManager um;
  // um.login
}