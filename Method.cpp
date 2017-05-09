#include "def_Class.h"
#include<string>
#include<math.h>
#include<fstream>
#include<algorithm>
#include<iostream>
using namespace std;

//some global varible;
station st[50];//公交站
route r[100];//线路

const int LargeNumber=1000000;
int st_count = 0;
int r_count =0;

bool edge[50][50];//两个站点之间是否有线路
int edge_r_rid[50][50];//两点之间时间最短线路ｉｄ
int edge_rid[100][50][50];//两点之间所有的线路的id

double dist_time[50];

double edge_time_min[50][50];

int bpath[50],cpath[50];
int flag[50];
int pre[50];

int m=1;
int btime=LargeNumber, c_time=LargeNumber;
int croute[50],broute[50];
int time1=0;
int rout[50];
int ccost=LargeNumber;
int bcost=LargeNumber;
int cost=0;
int rout1[50];

//get the station index
int getStation(char s_id){
	int i;
	for(i = 0;i < st_count;i++){
		if(st[i].s_id == s_id)
		return i;
	}
	return -1;
}

//get the distance between two
double  distanceBetween(char s_a,char s_b){

	int x = st[getStation(s_a)].x - st[getStation(s_b)].x;
	int y = st[getStation(s_a)].y - st[getStation(s_b)].y;

	return (double)sqrt(x*x+y*y);
}

//check the duplicate
bool checkForEcho(char c){
	int i;
	for(i = 0;i < st_count;i++){
		if(st[i].s_id == c)
		return true;
	}
	return false;
}

//add the bus line
void readRouteFromFile(){

	string info[10];
	ifstream fin("data.txt");
    int n = 0;
	while(!fin.eof()){
        getline(fin,info[n]);
        n++;
	}
	int i,j,count,k,m;
	for(i = 0;i < n;i++){
		r_count++;

		count = 0;
		for(j = 0;j < info[i].length();j++){
			//get the char we need
			if(info[i][j] == ':')//getR_id
			r[i].r_id = atoi(info[i].substr(0,j).c_str());

			if(info[i][j] == 'y')//getCost
				r[i].price = atoi(info[i].substr(j-1,1).c_str());
			if(info[i][j] == 'm')//getWaitTime
				r[i].waitTime = atoi(info[i].substr(j-1,1).c_str());
			if(info[i][j] == '/')//getSpeed
				r[i].speed = atoi(info[i].substr(j-1,1).c_str());

			//get the station info
			if(info[i][j] == '('){
				r[i].statpoint[count++] = info[i][j-1];
				//check before valuation
				if(!checkForEcho(info[i][j-1])){
				st[st_count].s_id = info[i][j-1];

				for(k = j;info[i][k] != ',';k++);
				st[st_count].x = atoi(info[i].substr(j+1,k-1-j).c_str());


				for(m = k;info[i][m] != ')';m++);
				st[st_count++].y = atoi(info[i].substr(k+1,m-1-k).c_str());
				}
			}
		}
	}
}

//get the path using Dijkstra
void routeSearch_time(char start){
	bool S[st_count];
	int v0 = getStation(start);

	int i,j;
	for(i = 0;i < st_count;i++){
		dist_time[i] = edge_time_min[v0][i];
		S[i] = false;
		if(dist_time[i] == LargeNumber)
			pre[i] = -1;
		else
			pre[i] = v0;
	}

	dist_time[v0] = 0;
	S[v0] = true;
	for(i = 0;i < st_count;i++){

		double mindist = LargeNumber;
		int u = v0;
		for(j = 0;j < st_count;j++)
			if((!S[j]) && dist_time[j] < mindist){
				u = j;
				mindist = dist_time[j];
			}
		S[u] = true;
		for(j = 0;j < st_count;j++){

			if((!S[j]) && edge_time_min[u][j] < LargeNumber)
			if(dist_time[u] + edge_time_min[u][j] < dist_time[j]){
				dist_time[j] = dist_time[u] + edge_time_min[u][j];
				pre[j] = u;
			}

		}
	}
}



//clean some value for next calculation
void cleanTime(){

    for(int i= 0 ;i < 50;i++){
    rout[i] = -1;
}
    time1 = 0;
    c_time = LargeNumber;
}

//calculate the min time of current path
void calcuTime(int p){
    if(p == m){
        int intime = time1;
        int a = LargeNumber;
        int temp = 0;
        for(int i = 0;i < r_count; i++){
            if(edge_rid[i][cpath[m-1]][cpath[m]] != -1 ){
                    if(i == rout[m-2]){
                        temp = edge_rid[i][cpath[m-1]][cpath[m]];
                    }else{
                        temp = edge_rid[i][cpath[m-1]][cpath[m]] + r[i].waitTime;
                    }
                    if(temp < a){
                        a = temp;
                        rout[m-1] = i;
                    }
            }
        }
        time1 += a;
        if(time1 < c_time){
            c_time = time1;
            for(int i = 0;i < m;i++)
                croute[i] = rout[i];
        }
        time1 = intime;
    }
    else{
        for(int j = 0;j < r_count;j++){
           if(edge_rid[j][cpath[p-1]][cpath[p]] != -1){
                if(p > 1){
                    if(j == rout[p-2]){
                    rout[p-1] = j;
                    time1 += edge_rid[j][cpath[p-1]][cpath[p]];
                    calcuTime(p+1);
                }
                else{
                    rout[p-1] = j;
                    time1 += r[j].waitTime + edge_rid[j][cpath[p-1]][cpath[p]];
                    calcuTime(p+1);
                    time1 -= r[j].waitTime + edge_rid[j][cpath[p-1]][cpath[p]];
                }
                }else{
                    rout[p-1] = j;
                    time1 += edge_rid[j][cpath[p-1]][cpath[p]];
                    calcuTime(p+1);
                    time1 -= edge_rid[j][cpath[p-1]][cpath[p]];
                }
           }
        }
    }
}

//get the min path by traversing using DFS
void routeSearch_time_wait(int x,int y){
    int i, j, k;
    for(i=0; i<st_count; i++)
    {
        if(edge[x][i]== 1 && flag[i] ==0 )
        {//get the end
            if(i == y)
            {
                cpath[m] = y;
                calcuTime(1);
                if(c_time < btime){
                    btime = c_time;
                    for(j = 0;j < 50;j++)
                        bpath[j] = -1;
                    for(k = 0;k <= m;k++){
                        bpath[k] = cpath[k];
                        if(k < m)
                        broute[k] = croute[k];
                    }
                }
                cleanTime();
            }
            else///if it is not the end
            {
                cpath[m] = i;//record it
                flag[i] = 1;
                m++;
                routeSearch_time_wait(i,y);//DFS
                m--;
                flag[i] = 0;
            }
        }
    }
}


//clean for next calling
void cleanCost(){
    for(int i= 0 ;i < 50;i++){
        rout1[i] = -1;
    }
    cost = 0;
    ccost = LargeNumber;
}
//calculate the min cost of current path
void calcuCost(int p){

    if(p == m){
        int incost = cost;
        int a = LargeNumber;
        if(edge_rid[rout1[m-2]][cpath[m-1]][cpath[m]] != -1)
            rout1[m-1] = rout1[m-2];
        else{
            for(int i = 0;i <r_count; i++){
                if(edge_rid[i][cpath[m-1]][cpath[m]] != -1 && edge_rid[i][cpath[m-1]][cpath[m]] < a){
                    a = edge_rid[i][cpath[m-1]][cpath[m]];
                    rout1[m-1] = i;
                }
            }
            cost += r[rout1[m-1]].price;
        }
        if(cost < ccost){
            ccost = cost;
            for(int i = 0;i < m;i++)
                croute[i] = rout1[i];
        }
        cost = incost;
    }
    else{
        for(int j = 0;j < r_count;j++){
           if(edge_rid[j][cpath[p-1]][cpath[p]] != -1){
                if(p > 1){
                    if(j == rout1[p-2]){
                    rout1[p-1] = j;
                    calcuCost(p+1);
                }
                else{
                    rout1[p-1] = j;
                    cost += r[j].price;
                    calcuCost(p+1);
                    cost -= r[j].price;
                }
                }else{
                    rout1[p-1] = j;
                    cost += r[j].price;
                    calcuCost(p+1);
                    cost -= r[j].price;
                }

           }
        }
    }
}

//traversing using DFS
void routeSearch_cost(int x,int y){
    int i, j, k;
    for(i=0; i<st_count; i++)
    {
        if(edge[x][i] == 1 && flag[i] == 0 )
        {
            if(i == y)
            {
                cpath[m] = y;
                calcuCost(1);

                if(ccost < bcost){
                    bcost = ccost;
                    for(j = 0;j < 50;j++)
                        bpath[j] = -1;
                    for(k = 0;k <= m;k++){
                        bpath[k] = cpath[k];
                        if(k < m)
                        broute[k] = croute[k];
                    }

                }
                cleanCost();
            }
            else
            {
                cpath[m] = i;
                flag[i] = 1;
                m++;
                routeSearch_cost(i,y);
                m--;
                flag[i] = 0;
            }
        }
    }
}
//print the path and time for path of min time without waiting
void printShortestPath_time(char x,char y){

	int path[st_count];//record the ling in pre array
	int count_p = 0;//record the capacity of array path

	int v = getStation(y);
	while(v != getStation(x)){
		path[count_p++] = v;
		v = pre[v];
	}
	path[count_p] = v;

	int c = count_p;
	while(c != -1){
		if(c == count_p)
		cout<<"\n线路"<<edge_r_rid[path[c]][path[c-1]]<<":";
		if((c > 0 && c < count_p) && (edge_r_rid[path[c]][path[c-1]] != edge_r_rid[path[c+1]][path[c]]))
		cout<<st[path[c]].s_id<<"\n\n换乘线路"<<edge_r_rid[path[c]][path[c-1]]<<":";

		if(st[path[c]].s_id != y)
			cout<<st[path[c]].s_id<<"->";
		else
			cout<<st[path[c]].s_id;
		c--;
	}
	cout<<"\n\n共用"<<dist_time[getStation(y)]<<"分钟\n"<<endl;
}
void printShortestPath_time_wait(){//print the path considering the wait time
    int i;
    for(i = 0;bpath[i] != -1;i++){
        if(i == 0) cout<<"\n线路"<<r[broute[0]].r_id<<":"<<st[bpath[0]].s_id<<"->"<<st[bpath[1]].s_id;
        else if(i > 1 && broute[i-2] == broute[i-1])
            cout<<"->"<<st[bpath[i]].s_id;
        else if(i > 1 && broute[i-2] != broute[i-1])
            cout<<"\n\n换乘线路"<<r[broute[i-1]].r_id<<":"<<st[bpath[i-1]].s_id<<"->"<<st[bpath[i]].s_id;
    }
    cout<<"\n\n共用"<<btime<<"分钟\n"<<endl;
}

void printShortestPath_cost(){//print the path and the cost for min cost
    int i;
    for(i = 0;bpath[i] != -1;i++){
        if(i == 0) cout<<"\n线路"<<r[broute[0]].r_id<<":"<<st[bpath[0]].s_id<<"->"<<st[bpath[1]].s_id;
        else if(i > 1 && broute[i-2] == broute[i-1])
            cout<<"->"<<st[bpath[i]].s_id;
        else if(i > 1 && broute[i-2] != broute[i-1])
            cout<<"\n\n换乘线路"<<r[broute[i-1]].r_id<<":"<<st[bpath[i-1]].s_id<<"->"<<st[bpath[i]].s_id;
    }
    cout<<"\n\n共用"<<bcost<<"元\n"<<endl;

}

//crate the map
void createMap(){
	int i,j;
	double tmp;
	for(i = 0;i < r_count;i++){
		for(j = 1;j < 20;j++)

		if(r[i].statpoint[j] != '0' && r[i].statpoint[j-1] != '0'){
			edge[getStation(r[i].statpoint[j-1])][getStation(r[i].statpoint[j])] = 1;//mark there is a road
			edge[getStation(r[i].statpoint[j])][getStation(r[i].statpoint[j-1])] = 1;

			if((tmp = (distanceBetween(r[i].statpoint[j-1],r[i].statpoint[j])/r[i].speed)) <
				edge_time_min[getStation(r[i].statpoint[j-1])][getStation(r[i].statpoint[j])]){	//traverse to get min time edge

                edge_time_min[getStation(r[i].statpoint[j-1])][getStation(r[i].statpoint[j])] = tmp;
                edge_time_min[getStation(r[i].statpoint[j])][getStation(r[i].statpoint[j-1])] = tmp;

                edge_r_rid[getStation(r[i].statpoint[j])][getStation(r[i].statpoint[j-1])] = r[i].r_id;
                edge_r_rid[getStation(r[i].statpoint[j-1])][getStation(r[i].statpoint[j])] = r[i].r_id;
			}

			edge_rid[i][getStation(r[i].statpoint[j])][getStation(r[i].statpoint[j-1])] =
                (distanceBetween(r[i].statpoint[j-1],r[i].statpoint[j])/r[i].speed);
            edge_rid[i][getStation(r[i].statpoint[j-1])][getStation(r[i].statpoint[j])] =
                (distanceBetween(r[i].statpoint[j-1],r[i].statpoint[j])/r[i].speed);

	}
}
}
//initialize
void init(){
	int i,j,k;
	for(i = 0;i < 100;i++)
	for(j = 0;j < 20;j++)
	r[i].statpoint[j] = '0';

	for(i = 0;i < 50;i++)
	for(j = 0;j < 50;j++){
		edge[i][j] = 0;

		if(i == j){
			edge_time_min[i][j] = 0;
		}
		else{
			edge_time_min[i][j] = LargeNumber;
		}
		edge_r_rid[i][j] = -1;
	}
	for(i = 0;i < 50;i++){
        cpath[i] = -1;
        bpath[i] = -1;
        flag[i] = 0;
        broute[i] = -1;
        croute[i] = -1;
        rout[i] = -1;
        rout1[i] = -1;
	}
	for(i = 0;i < 100;i++){
        for(j = 0;j < 50;j++){
            for(k = 0;k < 50;k++)
                edge_rid[i][j][k] = -1;
        }
	}
}
//reset some value for next calling
void reset(){
    m = 1;
    btime = LargeNumber,c_time = LargeNumber;
    time1 = 0;
    ccost = LargeNumber,bcost = LargeNumber;
    cost = 0;
    for(int i = 0;i < 50;i++){
        cpath[i] = -1;
        bpath[i] = -1;
        flag[i] = 0;
        broute[i] = -1;
        croute[i] = -1;
        rout[i] = -1;
        rout1[i] = -1;
	}

}

