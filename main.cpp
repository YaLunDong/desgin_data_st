#include "def_Class.h"
#include<iostream>

using namespace std;


int main(){
     init();
     cout<<"\t\t公交线路查询系统"<<endl;
     cout<<"\t+------------------------------+"<<endl;
     cout<<"\t1.查询最省时间（不考虑等待时间）"<<endl<<endl;
     cout<<"\t2.查询最省时间（考虑等待的时间）"<<endl<<endl;
     cout<<"\t3.查询最便宜的路线"<<endl<<endl;
     cout<<"\t4.退出系统"<<endl;
     cout<<"\t+------------------------------+"<<endl;
     int choice;
     char x,y;
     readRouteFromFile();
     createMap();
     
     while(1){
     cout<<"\t请选择：";
     cin>>choice;
     cout<<endl;
     switch(choice){
	     case 1:
		     cout<<"\t\t输入起点终点"<<endl<<endl<<"\t";
		     cin>>x>>y;
		     routeSearch_time(x);
		     printShortestPath_time(x,y);
		     break;
	     case 2:
		     cout<<"\t\t输入起点终点"<<endl<<endl<<"\t";
		     cin>>x>>y;
		     cpath[0]=getStation(x);
		     flag[getStation(x)]=1;
		     routeSearch_time_wait(getStation(x),getStation(y));
		     printShortestPath_time_wait();
		     break;
	     case 3:
		     cout<<"\t\t输入起点和终点"<<endl<<endl<<"\t";
		     cin>>x>>y;
		     cpath[0]=getStation(x);
		     flag[getStation(x)]=1;
		     routeSearch_cost(getStation(x),getStation(y));
		     printShortestPath_cost();
		     break;
	     case 4:
                     cout<<"\t\t退出成功"<<endl;
		     return 0;
             }
                   reset();     
     }

}
