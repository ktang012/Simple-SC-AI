#include "ScoutManager.h"

using namespace std;
using namespace BWAPI;
using namespace BWTA;

#define SCOUT_TIME 200
#define APPROX_DIST 128

// need to implement better base/info tracking
ScoutManager::ScoutManager() : scout(NULL), myMainRegion(NULL),
enemyMainRegion(NULL), isSearching(false), stealGas(false), cannonRush(false) {
   BWTA::readMap();
   BWTA::analyze();

   myMainRegion = getStartLocation(Broodwar->self())->getRegion();
   myBases.insert(getStartLocation(Broodwar->self()));

   // map out free bases; remove own base from unexplored
   freeBases = getBaseLocations();
   freeBases.erase(getStartLocation(Broodwar->self()));
   unexploredStarts = getStartLocations();
   unexploredStarts.erase(getStartLocation(Broodwar->self()));
}

// only finds main base and stops... for now
void ScoutManager::updateScout() {
   if (Broodwar->elapsedTime() >= SCOUT_TIME) {
      if (enemyMainRegion == NULL && scout == NULL || !scout->exists()) {
         BWAPI::Unit new_scout = getScout();
         if (new_scout == NULL) {
            return;
         }
         else {
            Broodwar << "Successfully set scout" << std::endl;
            setScout(new_scout);
            goSearch();
         }
      }
      else if (false) {
         // will be used when info manager is implemented
         // searches again if info manager says that we do not
         // see resource center anymore
         // can be one of two things:
         // 1) enemy has no resource center (likely a vicitory for us)
         // 2) enemy has relocated
      }
   }
}

void ScoutManager::setScout(BWAPI::Unit scout) {
   this->scout = scout;
}

// sends scout back home, removes it
void ScoutManager::removeScout() {
   if (scout) {
      scout->move(myMainRegion->getCenter());
      scout = NULL;
      Broodwar << "Successfully removed scout" << std::endl;
   }
}

// picking ideal scout unit based on race
BWAPI::Unit ScoutManager::getScout() {
   // insert logic for selecting a scout
   Race myRace = Broodwar->self()->getRace();
   int fastOverlord = Broodwar->self()->getUpgradeLevel(UpgradeTypes::Pneumatized_Carapace);
   for (auto &u : Broodwar->self()->getUnits()) {
      if (u->getType() == UnitTypes::Terran_SCV) {
         return u;
      }
      else if (u->getType() == UnitTypes::Protoss_Observer) {
         return u;
      }
      else if (u->getType() == UnitTypes::Protoss_Probe) {
         return u;
      }
      else if (u->getType() == UnitTypes::Zerg_Overlord && fastOverlord > 0) {
         return u;
      }
      else if (u->getType() == UnitTypes::Zerg_Zergling) {
         Broodwar << "Got ling as scout" << std::endl;
         return u;
      }
      else if (u->getType() == UnitTypes::Zerg_Drone) {
         Broodwar << "Got drone as scout" << std::endl;
         return u;
      }
      
   }
   return NULL;
}


// queues up 'move to' commands
// first search will have scout look for possible starts and mark as main base
// subsequent searches will mark any building as a base
void ScoutManager::goSearch() {
   if (!unexploredStarts.empty() && !isSearching && scout != NULL) {
      isSearching = true;
      for (auto i : unexploredStarts) {
         // sidesteps crappy example resource manager
         // since it does not allow for queueing commands
         if (scout->getType().isWorker()) {
            if (!scout->move(i->getPosition())) {
               Broodwar << "Failed to move scout -- reset" << std::endl;
               removeScout();
               isSearching = false;
               return;
            }
         }
         if (!scout->move(i->getPosition(), true)) {
            Broodwar << "Failed to move scout -- reset" << std::endl;
            removeScout();
            isSearching = false;
            return;
         }
         else {
            Broodwar << "Successfully queued movement for scout" << std::endl;
         }
      }
   }
}

void ScoutManager::foundEnemy(BWAPI::Unit enemyResourceDepot) {
   if (!(enemyResourceDepot->getPlayer()->isEnemy(Broodwar->self()) && \
         enemyResourceDepot->getType().isResourceDepot())) {
      return;
   }
   isSearching = false;
   BWAPI::Position enemyPosition = enemyResourceDepot->getPosition();
   if (enemyMainRegion == NULL) {
      Broodwar << "Set enemy main region" << std::endl;
      enemyMainRegion = BWTA::getRegion(enemyPosition);
   }
   BaseLocation *enemyBase = getNearestBaseLocation(enemyPosition);
   if (enemyBase != NULL) {
      Broodwar << "Added an enemy base" << std::endl;
      addEnemyBase(enemyBase);
   }
   else {
      Broodwar << "Error: no nearby base to add to enemyBase" << endl;
      return;
   }
   /*
   if (stealGas) {
      // insert logic
      removeScout();
   }
   if (cannonRush) {
      // insert logic
      removeScout();
   }
   */
   removeScout();
}

void ScoutManager::addEnemyBase(BWTA::BaseLocation *base) {
   enemyBases.insert(base);
   freeBases.erase(base);
}

void ScoutManager::setEnemyMain(BWTA::Region *enemyMain) {
   enemyMainRegion = enemyMain;
}

void ScoutManager::addMyBase(BWTA::BaseLocation *base) {
   myBases.insert(base);
   freeBases.erase(base);
}

void ScoutManager::addFreeBase(BWTA::BaseLocation *base) {
   freeBases.insert(base);
}

std::set<BWTA::BaseLocation*> ScoutManager::getMyBases() {
   return myBases;
}

std::set<BWTA::BaseLocation*> ScoutManager::getEnemyBases() {
   return enemyBases;
}

std::set<BWTA::BaseLocation*> ScoutManager::getFreeBases() {
   return freeBases;
}