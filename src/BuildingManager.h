#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <Windows.h>
#include <set>
#include <queue>

/* 
   Quick lesson: maps vs sets (ordered)
   they are both associative containers e.g. key to value
   not numeric/sequential index
   map <key, val> is used if val is not contained in key
   set <key> is used if val is contained in key (key == val)

   examples:
   want to know which words appear in a paragraph
      use a set -- set<word>
   want to know how many times a word appears in a paragraph
      use a map -- map<word, count>
*/




#endif