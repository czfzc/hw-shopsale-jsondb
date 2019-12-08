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
    ShopController shop;
    system("clear");
    while(shop.selectOption());

}