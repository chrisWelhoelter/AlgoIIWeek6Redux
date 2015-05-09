//
//  main.cpp
//  AlgoIIWeek6Redux
//
//  Created by Chris Welhoelter on 5/8/15.
//  Copyright (c) 2015 Chris Welhoelter. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <math.h>
#include <random>

int main(int argc, const char * argv[]) {
    std::string fileName;
    
    std::cout << "enter file name: ";
    std::cin >> fileName;
    std::cout << std::endl;
    
    std::fstream fsIn;
    fsIn.open(fileName);
    if (!fsIn.is_open()){
        std::cout << "could not open " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    
    size_t clauseCount;
    fsIn >> clauseCount;
    
    std::set<std::pair<int, int>> clauses;
    for (size_t i = 0; i < clauseCount; i++){
        std::pair<int, int> newClause;
        fsIn >> newClause.first;
        fsIn >> newClause.second;
        clauses.insert(newClause);
    }
    
    // *** preprocessing!!! ***
    
    size_t oldClauseCount = 0;
    
    while (oldClauseCount != clauseCount){
        
        oldClauseCount = clauseCount;
        
        std::set<int> curVars;
        std::set<int> relevantVars;
        
        for (std::set<std::pair<int, int>>::iterator it = clauses.begin(); it != clauses.end(); it++){
            curVars.insert(it->first);
            curVars.insert(it->second);
            if (curVars.count(0 - it->first) == 1){
                relevantVars.insert(abs(it->first));
            }
            if (curVars.count(0 - it->second) == 1){
                relevantVars.insert(abs(it->second));
            }
        }
        for (std::set<std::pair<int, int>>::iterator it = clauses.begin(); it != clauses.end();){
            if (relevantVars.count(abs(it->first)) == 1 && relevantVars.count(abs(it->second)) == 1){
                it++;
            }
            else {
                std::set<std::pair<int, int>>::iterator eraseIt = it;
                it++;
                clauses.erase(eraseIt);
                clauseCount--;
            }
        }
    }
    
    if (clauses.size() == 0){
        std::cout << "satisfiable" << std::endl;
        return 0;
    }
    
    std::set<int> vars;
    for (std::set<std::pair<int, int>>::iterator it = clauses.begin(); it != clauses.end(); it++){
        vars.insert(abs(it->first));
        vars.insert(abs(it->second));
    }
    
    // *** /preprocessing!!! ***
    
    // loop parameters
    
    size_t OUTERLOOPCOUNT = log2(vars.size()); // log2(varCount) in lecture
    size_t INNERLOOPCOUNT = 2*pow(vars.size(), 2); // 2*pow(varCount, 2) in lecture
    
    // seed rand
    srand((int)time(NULL));
    
    // main loop
    
    for (size_t i = 0; i < OUTERLOOPCOUNT; i++){
        std::map<int, bool> curAssignment;
        for (std::set<int>::iterator it = vars.begin(); it != vars.end(); it++){
            curAssignment.insert(std::pair<int, bool>(*it, rand()%2));
        }
        for (size_t j = 0; j < INNERLOOPCOUNT; j++){
            std::vector<std::pair<int, int>> unsatisfiedClauses;
            for (std::set<std::pair<int, int>>::iterator it = clauses.begin(); it != clauses.end(); it++){
                if (curAssignment[abs(it->first)] != (it->first > 0) && curAssignment[abs(it->second)] != (it->second > 0)){
                    unsatisfiedClauses.push_back(*it);
                }
            }
            if (unsatisfiedClauses.size() == 0){
                std::cout << "satisfiable" << std::endl;
                return 0;
            }
            std::pair<int, int> randomUnsatisfiedClause = unsatisfiedClauses[rand()%unsatisfiedClauses.size()];
            bool coinFlip = rand()%2;
            if (coinFlip){
                curAssignment[abs(randomUnsatisfiedClause.first)] ^= 1;
            }
            else {
                curAssignment[abs(randomUnsatisfiedClause.second)] ^= 1;
            }
        }
    }
    
    std::cout << "unsatisfiable" << std::endl;
    return 0;
}
