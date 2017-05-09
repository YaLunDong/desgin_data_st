//define the station
class station{
   
   public:
          int x;//the row
          int y;//the col

          char s_id;//the name 
};

//define the bus line
class route{
   public:
 	  int r_id;//NO. of the line
	  char statpoint[20];//station of line

	  int price;//price RMB
	  int waitTime;//wait time for next bus /minute
	  double speed;// km/min
};
//GLBOAL
extern int cpath[50];
extern int flag[50];


//end;

int getStation(char s_id);
double distanceBetween(char s_a,char s_b);
bool checkForEcho(char c);
void readRouteFromFile();
void routeSearch_time(char start);
void cleanTime(int p);
void routeSearch_time_wait(int x,int y);
void cleanCost();
void calcuCost(int p);
void routeSearch_cost(int x,int y);
void printShortestPath_time(char x,char y);
void printShortestPath_time_wait();
void printShortestPath_cost();
void createMap();
void init();
void reset();
