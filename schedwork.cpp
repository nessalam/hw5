#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool backtrack(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts,
DailySchedule& sched, vector<size_t>& workerShifts, size_t day, size_t workerIndex, size_t scheduledCount);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
  if(avail.size() == 0U){
      return false;
  }
  sched.clear();
  // Add your code below
  // get number of days (n) and workers (k) from matrix
  size_t n = avail.size(); //# of days
  size_t k = avail[0].size(); //# of workers 
  
  //initialize schedule
  sched.resize(n);
  for(size_t i = 0; i < n; i++){
    sched[i].resize(dailyNeed, INVALID_ID);
  }

  //vector to track how many shifts each worker has workerIndex
  vector<size_t> workerShifts(k, 0);

  //backtrack from day 0 and worker 0
  return backtrack(avail, dailyNeed, maxShifts, sched, workerShifts, 0, 0, 0);
}

bool backtrack(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts,
DailySchedule& sched, vector<size_t>& workerShifts, size_t day, size_t workerIndex, size_t scheduledCount){
  size_t n = avail.size(); //# of days
  size_t k = avail[0].size(); //# of workers

  //case 1: processed all the days
  if(day >= n){
    //check that schedule is full + no invalid IDs left
    for(size_t d = 0; d < n; d++){
      for(size_t w = 0; w < dailyNeed; w++){
        if(sched[d][w] == INVALID_ID){
          return false; //schedule incomplete
        }
      }
    }
    return true; //schedule complete and valid
  }
  
  //case 2: filled all pos for curr day -> move to next day
  if(scheduledCount >= dailyNeed){
    return backtrack(avail, dailyNeed, maxShifts, sched, workerShifts, day + 1, 0, 0);
  }

  //recursive case: trying to fill curr pos
  for(size_t worker = workerIndex; worker < k; worker++){
    //check if curr worker is available on curr day
    if(avail[day][worker]){
      //check if worker has been overworked
      if(workerShifts[worker] < maxShifts){
        sched[day][scheduledCount] = worker;
        workerShifts[worker]++;

        //recursion to fill next position
        bool found = backtrack(avail, dailyNeed, maxShifts, sched, workerShifts, day, worker + 1, scheduledCount + 1);

        //complete schedule, return success
        if(found){
          return true;
        }

        //if no schedule w the curr worker, undo
        sched[day][scheduledCount] = INVALID_ID;
        workerShifts[worker]--; 
      }
    }
  }

  //none of the workers workerIndex
  if(workerIndex == 0){
    return false;
  }

  return backtrack(avail, dailyNeed, maxShifts, sched, workerShifts, day, 0, scheduledCount);
}