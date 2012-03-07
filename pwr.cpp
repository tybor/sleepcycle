//SleepCycle -- A rtcwake interface written in C++, soon to include GUI functionality.
//Version 0.1
//Must run as root!
//rsmitty1025@gmail.com
//3/6/2012

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]){
  int schedule[7][2];
  bool wakeLock;
  cout << "Retrieving crontab for editing..." << endl;
  system("sudo crontab -l > cron.temp");

  fstream cronTemp;
  cronTemp.open("cron.temp");
  vector<string> cronTempContents;
  string line;

  cout << "Removing previous rtcwake commands from crontab." << endl;
  while(cronTemp.good()){
  
    getline(cronTemp,line);

    if(line.find("rtcwake") == -1){
      cronTempContents.push_back(line);
    }	
  }

  cronTemp.close();

  remove("cron.temp");

  fstream confFile;
  confFile.open("pwr.conf");
  int pos;

  cout << "Retrieving desired sleep cycles from config file." << endl;

 while(confFile.good()){

    getline(confFile,line);

    pos = line.find("=");
    //if(line.substr(0,pos) == "Lock_On_Wake"){ wakeLock =  atoi(line.substr(pos+1).c_str()); }       
    if(line.substr(0,pos) == "Sunday_Start"){ schedule[0][0] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Sunday_End"){ schedule[0][1] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Monday_Start"){ schedule[1][0] = atoi(line.substr(pos+1).c_str()); }
    if(line.substr(0,pos) == "Monday_End"){ schedule[1][1] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Tuesday_Start"){ schedule[2][0] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Tuesday_End"){ schedule[2][1] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Wednesday_Start"){ schedule[3][0] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Wednesday_End"){ schedule[3][1] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Thursday_Start"){ schedule[4][0] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Thursday_End"){ schedule[4][1] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Friday_Start"){ schedule[5][0] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Friday_End"){ schedule[5][1] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Saturday_Start"){ schedule[6][0] =  atoi(line.substr(pos+1).c_str()); }    
    if(line.substr(0,pos) == "Saturday_End"){ schedule[6][1] =  atoi(line.substr(pos+1).c_str()); }
  }

  confFile.close();

  system("touch cron.temp");
  ofstream cronWrite;
  cronWrite.open("cron.temp");

  cout << "Restoring old cron entries and appending new sleep schedules" << endl;

  for(int i = 0; i < cronTempContents.size(); i++){
    cronWrite << cronTempContents[i];
    if((i+1) == cronTempContents.size()){cronWrite<<endl;}
  }

  for(int i = 0; i < 7; i++){
	int hours = (schedule[i][1]/100)-(schedule[i][0]/100);
	int minutes = (schedule[i][1]%100) - (schedule[i][0]%100);
	int total = (((hours*60)+minutes)*60);
	if(total > 0){
	  stringstream cronEntry;
	  cronEntry << schedule[i][0]%100;
          cronEntry << " ";
	  cronEntry << schedule[i][0]/100;
	  cronEntry << " * * ";
	  cronEntry << i;
	  cronEntry << " sudo rtcwake -m mem -s ";
	  cronEntry << total;
	  cronWrite<<cronEntry.str()<<endl;
	}
  }


  cronWrite.close();

  cout << "Installing new crontab" << endl;

  system("sudo crontab < cron.temp");

  cout << "Removing temporary cron backup" << endl;
  remove("cron.temp");

  return 0;
}
