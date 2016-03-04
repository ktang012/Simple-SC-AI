#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <Windows.h>
#include <set>
#include <queue>

#include "BuildingManager.h"

class ResourceManager {
   friend class ScoutManager;
   private:
      std::set<BWAPI::Unit> resourceCenters;
      std::set<BWAPI::Unit> workers;

      unsigned totalMinerals;
      unsigned totalGas;
      unsigned totalSupply;

   protected:
      unsigned getNumBaseMineralWorkers(BWAPI::Unit rc);
      unsigned getNumBaseGasWorkers(BWAPI::Unit rc);

      unsigned getAllMineralWorkers();
      unsigned getAllGasWorkers();
      unsigned getCurrentSupply();

      void addWorker(BWAPI::Unit worker);
      void removeWorker(BWAPI::Unit worker);

      bool saturateMineralField(BWAPI::Unit rc);
      bool saturateRefinery(BWAPI::Unit rc);

      bool buildRefinery(BWAPI::Unit builder);
      bool buildExpansion(BWAPI::Unit builder, BWTA::BaseLocation* base);
      bool buildSupply(BWAPI::Unit builder);
      
   public:
      void updateResource();
      
};