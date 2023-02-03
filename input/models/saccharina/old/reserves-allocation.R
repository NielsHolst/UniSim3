rm(list=ls(all=TRUE))
  massCStruct = 37.5537
  massNStruct = 3.57654
  massCRes = 79.8106
  massNRes = 4.4209
  propCStruct = 0.21
  propNStruct = 0.02
  propCRes = 0.47
  propNRes = 0.37
  optCConc = 0.36
  optNConc = 0.052
  supplyC = 5.74513
  supplyN = 0.116028
  demandRespirationC = 0.226504
  demandGrowthC = 1.22049
  demandGrowthN = 0.116238
  demandReservesC = 236.249
  demandReservesN = 43.1401
  demandCostC = 97.4678
  costStruct = 0.571
  costCRes = 0.267
  costNRes = 0.781
  
M0C = massCRes + supplyC;
M0N = massNRes + supplyN;

# Step 1. Maintenance respiration
supplyRespirationC = demandRespirationC;
M1C = M0C - supplyRespirationC;
M1N = M0N;

# Step 2. Structural growth
sdGrowthC = min(M1C/demandGrowthC/(1. + costStruct), 1.);
sdGrowthN = min(M1N/demandGrowthN, 1.);
supplyGrowthC = min(sdGrowthC, sdGrowthN)*demandGrowthC;
supplyGrowthN = propNStruct/propCStruct*supplyGrowthC;
structuralBiomassGain = supplyGrowthC/propCStruct;
M2C = M1C - supplyGrowthC*(1. + costStruct);
M2N = M1N - supplyGrowthN;


#
#
#


biomass = function(supplyReservesC, supplyReservesN) {
  (massCStruct + supplyGrowthC)/propCStruct + (massCRes + supplyReservesC)/propCRes + (massNRes + supplyReservesN)/propNRes
}

concC = function(supplyReservesC, supplyReservesN) {
  massC = (massCStruct + supplyGrowthC) + (massCRes + supplyReservesC)
  massC/biomass(supplyReservesC, supplyReservesN)
}

concN = function(supplyReservesC, supplyReservesN) {
  massN = (massNStruct + supplyGrowthN) + (massNRes + supplyReservesN)
  massN/biomass(supplyReservesC, supplyReservesN)
}

step = 0.9
repeat {
  # Step 3. Allocate N reserves
  # Return original reserves
  M3N = M2N - massNRes;
  # Subtract C costs for additional N reserves
  if (M3N > 0.) {
      M3C = M2C - costNRes*M3N;
      # Reduce N allocation when short of C
      if (M3C < 0.) {
          M3N = M2C/costNRes;
          M3C = 0.;
      }
  }
  # Supply cannot exceed demand
  supplyReservesN = min(M3N, demandReservesN);

  # Step 4. Allocate C reserves
  # Return original reserves
  M4C = M3C - massCRes;
  # Subtract C costs for additional C reserves
  supplyReservesC = if (M4C > 0.)  M4C/(1. + costCRes) else M4C;
  # Supply cannot exceed demand
  supplyReservesC = min(supplyReservesC, demandReservesC);

  # Check optima
  C = concC(supplyReservesC, supplyReservesN)
  N = concN(supplyReservesC, supplyReservesN)
  M = data.frame(
    supplyReservesC = supplyReservesC,
    supplyReservesN = supplyReservesN,
    C = C,
    N = N
  )
  print(M)
  if (C > optCConc)
    M2C = step*M2C
  else if (N > optNConc)
    M2N = step*M2N
  else 
    break
}

