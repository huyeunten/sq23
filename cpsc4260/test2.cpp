#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

void mas(int *jobID, int *burst, int *arrival, int *terminate, int numJobs);
void executeQ0(queue <int>&q0, int *burst, int &time, int &prevTime, int *terminate, int &completedJobs, queue <int>&q1);
void addToQueue(queue <int>&q0, int numJobs, int *arrival, int *jobID, int time, int prevTime);
void executeQ1(queue <int>&q1, int *burst, int &time, int &prevTime, int *terminate, int &completedJobs, queue <int>&q2);
void executeQ2(queue <int>&q2,int *burst, int &time, int &prevTime, int *terminate, int &completedJobs);

int main() {
  ifstream inputFile;
  string fileName;
  cout << "Enter a file name: ";
  cin >> fileName;
  inputFile.open(fileName);
  if (inputFile.fail()) {
    cout << "File failed to open!" << endl;
    exit(1);
  }
  string line, id, b, a;
  int numJobs = 0;
  queue <int>temp;
  while (getline(inputFile, line)) {
    numJobs++;
    istringstream ss(line);
    getline(ss, id, ',');
    getline(ss, b, ',');
    getline(ss, a);
    temp.push(stoi(id));
    temp.push(stoi(b));
    temp.push(stoi(a));
  }

  int * jobID;
  jobID = new int[numJobs];
  int * burst;
  burst = new int[numJobs];
  int * arrival;
  arrival = new int[numJobs];
  int * terminate;
  terminate = new int[numJobs];

  for (int i = 0; i < numJobs; i++) {
    jobID[i] = temp.front();
    temp.pop();
    burst[i] = temp.front();
    temp.pop();
    arrival[i] = temp.front();
    temp.pop();
  }

  mas(jobID, burst, arrival, terminate, numJobs);
  
  delete [] jobID;
  delete [] burst;
  delete [] arrival;
  delete [] terminate;
  inputFile.close();
}

void mas(int *jobID, int *burst, int *arrival, int *terminate, int numJobs) {
  queue <int>q0;
  queue <int>q1;
  queue <int>q2;
  int time = 0;
  int completedJobs = 0;
  int prevTime = -1;
  addToQueue(q0, numJobs, arrival, jobID, time, prevTime);
  while (!q0.empty()) {
    executeQ0(q0, burst, time, prevTime, terminate, completedJobs, q1);
    addToQueue(q0, numJobs, arrival, jobID, time, prevTime); 
  }
  while (completedJobs < numJobs) {
    if (q0.empty() && q1.empty()) {
      executeQ2(q2, burst, time, prevTime, terminate, completedJobs);
      addToQueue(q0, numJobs, arrival, jobID, time, prevTime);
    }
    if (q0.empty() && q1.size() > 0) {
      executeQ1(q1, burst, time, prevTime, terminate, completedJobs, q2);
      addToQueue(q0, numJobs, arrival, jobID, time, prevTime);
    }
    if (!q0.empty()) {
      executeQ0(q0, burst, time, prevTime, terminate, completedJobs, q1);
      addToQueue(q0, numJobs, arrival, jobID, time, prevTime);
    }
  }

  for (int i = 0; i < numJobs; i++) {
    cout << "job: " << jobID[i] << endl;
    cout << "termination time: " << terminate[i] << endl;
  }
}

void addToQueue(queue <int>&q0, int numJobs, int *arrival, int *jobID, int time, int prevTime) {
  for (int i = 0; i < numJobs; i++) {
      if (arrival[i] <= time && arrival[i] > prevTime)
        q0.push(jobID[i]);
    }

}

void executeQ0(queue <int>&q0, int *burst, int &time, int &prevTime, int *terminate, int &completedJobs, queue <int>&q1) {
  int cJobID = q0.front();
  q0.pop();
  int job = cJobID - 1;
  int cBurst = burst[job];
  if (cBurst <= 6) {
    prevTime = time;
    time += cBurst;
    burst[job] = 0;
    terminate[job] = time;
    completedJobs++;
  }
  else {
    prevTime = time;
    time += 6;
    burst[job] = cBurst - 6;
    q1.push(cJobID);
  }
}

void executeQ1(queue <int>&q1, int *burst, int &time, int &prevTime, int *terminate, int &completedJobs, queue <int>&q2) {
  int cJobID = q1.front();
  q1.pop();
  int job = cJobID - 1;
  int cBurst = burst[job];
  if (cBurst <= 12) {
    prevTime = time;
    time += cBurst;
    burst[job] = 0;
    terminate[job] = time;
    completedJobs++;
  }
  else {
    prevTime = time;
    time += 12;
    burst[job] = cBurst - 12;
    q2.push(cJobID);
  }
}

void executeQ2(queue <int>&q2, int *burst, int &time, int &prevTime, int *terminate, int &completedJobs) {
  int cJobID = q2.front();
  q2.pop();
  int job = cJobID - 1;
  int cBurst = burst[job];
  prevTime = time;
  time += cBurst;
  burst[job] = 0;
  terminate[job] = time;
  completedJobs++;
}
