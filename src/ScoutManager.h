#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <Windows.h>
#include <set>

/*
Overview:
There are two kinds of BaseLocations, those that are
candidates for starting and expansions.

Only scout over starting location since to go there, we
must go through expansions (assumption)

Our first search will always yield the main base.

We go search only when we do not have sight on enemy buildings/resource depots.
(To be implemented w/ info manager)
*/
class ScoutManager {
private:
   BWAPI::Unit scout;

   BWTA::Region *myMainRegion;
   BWTA::Region *enemyMainRegion;

   bool isSearching;
   bool stealGas;
   bool cannonRush;

protected:
   std::set<BWTA::BaseLocation*> myBases;
   std::set<BWTA::BaseLocation*> enemyBases;

   // iterate through this when searching
   std::set<BWTA::BaseLocation*> unexploredStarts;
   // delete from when upon expansion/discovery -- when to add?
   std::set<BWTA::BaseLocation*> freeBases;

   /* used for managing sets of bases */
   // if first on show, set main and add base
   // add base for subsequent on show
   void addEnemyBase(BWTA::BaseLocation *base);
   void setEnemyMain(BWTA::Region *enemyMain);

   void addMyBase(BWTA::BaseLocation *base);
   void addFreeBase(BWTA::BaseLocation *base);

   std::set<BWTA::BaseLocation*> getMyBases();
   std::set<BWTA::BaseLocation*> getEnemyBases();
   std::set<BWTA::BaseLocation*> getFreeBases();

public:
   ScoutManager();

   void setScout(BWAPI::Unit scout);
   BWAPI::Unit getScout();
   void removeScout();

   // handles scouting logic
   void updateScout();

   // used by strategy manager -- currently unused
   void setGasSteal(bool isStealing);
   void setCannonRush(bool isCannonRushing);

   // iterates over unexplored/free bases
   void goSearch();
   void foundEnemy(BWAPI::Unit u);
};