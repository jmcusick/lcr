#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
//#include <thread>
#include <unistd.h>

void plotProbability(){
  system("xterm -e gnuplot 'plotProb.txt' -");
  //system("gnuplot 'plotProb.txt' -");
}

void plotDistribution(){
  system("xterm -e gnuplot 'plotDistr.txt' -");
  //system("gnuplot 'plotDistr.txt' -");
}

//to add to this:
//-over the aggregate show the money distrubution after everyone gets a turn

//roll rand stuff
//0 = center
//1 = left
//2 = right
//3-5 = dot

void passRight(std::vector<int> &money, int player){
  money[player]--;
  money[(player+1)%money.size()]++;
}
void passLeft(std::vector<int> &money, int player){
  money[player]--;
  money[(player-1)%money.size()]++;
}
void passCenter(std::vector<int> &money, int player){
  money[player]--;
}

bool gameNotOver(const std::vector<int> &money){
  int numPeoplePossessingMoney = 0;
  for(int i = 0; i < money.size(); i++){
    if(money[i] > 0){
      numPeoplePossessingMoney++;
    }
  }
  if(numPeoplePossessingMoney > 1){
    return true;
  }
  return false;
}

void printGameStatus(const std::vector<int> &money, int pot){
  //std::cout<<"=====================print===================="<<std::endl;
  std::cout<<"Pot: "<<pot<<std::endl;
  for(int i = 0; i <money.size(); i++){
    std::cout<<money[i]<<" ";
  }
  std::cout<<std::endl;
  //std::cout<<"-------------------end print-----------------"<<std::endl;
}



int main(){
  srand(time(NULL));
  int numPeople = 0;
  double numTrials = 10000;
  std::string debugStr = "";
  bool debug = false;

  std::cout<<"Welcome to the lcr game!"<<std::endl;
  
  std::cout<<"How many people?"<<std::endl;
  std::cin>>numPeople;

  //std::cout<<"How many trials?"<<std::endl;
  //std::cin>>numTrials;

  std::cout<<"Debug mode?(y/n)"<<std::endl;
  std::cin>>debugStr;
  if(debugStr == "y"){
    debug = true;
    numTrials = 1;
  }
  
  std::vector<int> totalWins;
  std::vector<std::vector<int> > totalDistribution;

  for(int i = 0; i < numPeople; i++){
    totalWins.push_back(0);
  }

  //each game
  for(int i = 0; i<numTrials; i++){
    //new game
    int pot = 0;
    std::vector<int> money;
    for(int j = 0; j < numPeople; j++){
      money.push_back(3);
    }
    if(debug == true){
      std::cout<<"~~~~~~~~~~~~~~~~~~~ Begin Game: "<<numTrials<<" ~~~~~~~~~~~~~~~~~~~"<<std::endl;
      printGameStatus(money, pot);
    }
    
    //start game
    int turn = 0;
    int roundCounter = 0;
    while(gameNotOver(money)){  //problem here, potential problem if two centers are rolled, but maybe not
      turn = turn % numPeople;  //keeps track of whose turn it is
      
      //MONEY DISTRIBUTION
      if(turn == 0){  //each rotation (will still work if 0 doesnt have a dollar)
	//pushback or change or something
	if(roundCounter >= totalDistribution.size()){
	  totalDistribution.push_back(money);
	}
	else{
	  for(int i = 0; i < numPeople; i++){
	    totalDistribution[roundCounter][i]+=money[i];
	  }
	}
	roundCounter++;
      }
      //roll proper # of times
      std::string rollText = "";
      if(debug == true)
	std::cout<<"=================Player: "<<turn<<"'s turn================="<<std::endl;
      int numRolls = 0;
      if(money[turn]<3){
	numRolls = money[turn];
      }
      else{
	numRolls = 3;
      }
      if(debug == true)
	std::cout<<"numRolls: " <<numRolls<<std::endl;
      for(int roller = 0 ; roller < numRolls; roller++){
        int roll = rand() % 6;
	if(roll >= 3){
	  rollText+="-";
	  continue;
	}
	else if(roll == 0){
          rollText+="C";
          passCenter(money, turn);
	  pot++;
        }
        else if(roll == 1){
	  rollText+="L";
          passLeft(money, turn);
        }
        else if(roll == 2){
	  rollText+="R";
          passRight(money, turn);
        }
      }
      if(debug == true){
	std::cout<<"Player "<<turn<<" rolled: "<<rollText<<std::endl;
	std::cout<<"New Distribution of money: "<<std::endl;
	printGameStatus(money, pot);
      }
      turn++;
    }
    for(int end = 0; end < money.size(); end++){
      if(money[end]>0){
        totalWins[end]++;
      }
    }
  }

  
  std::cout<<"End Testing"<<std::endl;
  std::cout<<"Winning Stats:"<<std::endl;
  for(int i = 0; i < totalWins.size(); i++){
    std::cout<<"Player "<<i+1<<": "<<totalWins[i]<<std::endl;
  }
  std::cout<<"Probabilities:"<<std::endl;
  for(int i = 0; i < totalWins.size(); i++){
    std::cout<<"Player "<<i+1<<": "<<totalWins[i]/numTrials<<std::endl;
  }

  //output the probabilities to prob.txt
  std::ofstream myfile ("prob.txt");
  if (!myfile.is_open()){
    std::cout<<"can't open output file"<<std::endl;
    return 0;
  }
  for(int i = 0; i < totalWins.size(); i++){
    myfile<<i+1<<" "<<totalWins[i]/numTrials<<std::endl;
  }
  
  myfile.close();
  /*
  std::thread thread1 (plotProbability);
  std::thread thread2 (plotDistribution);
  
  thread1.join();
  thread2.join();
  */

  //print total distribution
  for(int i = 0; i < totalDistribution.size();i++){
    std::cout<<"Turn "<<i<<": ";
    for(int j = 0; j < numPeople; j++){
      std::cout<<totalDistribution[i][j]<<" ";
    }
    std::cout<<std::endl;
  }

  //create ave distribution
  std::vector<std::vector<double> > aveDistribution;

  for(int i = 0; i < totalDistribution.size();i++){
    std::vector<double> tmp;
    for(int j = 0; j < numPeople; j++){
      tmp.push_back(totalDistribution[i][j]/numTrials);
    }
    aveDistribution.push_back(tmp);
  }

  //print ave distribution
  for(int i = 0; i < aveDistribution.size();i++){
    std::cout<<"Turn "<<i<<": ";
    for(int j = 0; j < numPeople; j++){
      std::cout<<aveDistribution[i][j]<<" ";
    }
    std::cout<<std::endl;
  }

  //output the probabilities to prob.txt
  
  std::ofstream myfile2 ("distr.txt");
  if (!myfile2.is_open()){
    std::cout<<"can't open output file"<<std::endl;
    return 0;
  }
  for(int i = 0; i < aveDistribution.size(); i++){
    for(int j = 0; j < numPeople; j++){
      myfile2<<j+1<<" "<<i<<" "<<aveDistribution[i][j]<<std::endl;
    }  
  }
  
  myfile2.close();


  pid_t pid = fork();
  
  if (pid == 0){
    // child process
    plotProbability();
  }
  else if (pid > 0){
    // parent process
    plotDistribution();
  }
  else{
    // fork failed
    std::cout<<"fork() failed!\n";
    return 1;
  }

}
