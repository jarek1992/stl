#pragma once

#include <iostream>
#include <map>
#include <string>
#include <deque>
#include <algorithm>
#include <list>

std::map<int, std::string> removeDuplicateAndTranformToMap(
	std::list<std::string>& values, 
	std::deque<int>& keys);