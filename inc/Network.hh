#pragma once
#ifndef __NETWORK__HH__
#define __NETWORK__HH__
#include <map>
#include <list>
#include <iostream>
using namespace std;
map<string, list<string>> query;
void ClearQuery();
void AddOperation(string operation, list<string> args);
void ExecuteQuery();
#endif