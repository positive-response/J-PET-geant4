/**
 *  @copyright Copyright 2021 The J-PET Monte Carlo Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file PrimaryGeneratorAction.h
 */

#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H 1

#include "../Info/PrimaryGeneratorActionMessenger.h"
#include "../Core/PrimaryGenerator.h"
#include "../Core/NemaGenerator.h"
#include "../Core/HistoManager.h"
#include "../Core/SourceParams.h"
#include "../Core/BeamParams.h"

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <globals.hh>

struct IsotopeParams
{
  G4String shape;
};

enum Dimension {
  dimX, dimY  
};

/**
 * @class PrimaryGeneratorAction
 * @brief heart of simulated physics
 */
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  explicit PrimaryGeneratorAction(HistoManager* histo);
  ~PrimaryGeneratorAction();
  virtual void GeneratePrimaries(G4Event*);
  void SetSourceTypeInfo(G4String);

  BeamParams* GetBeamParams() { return fBeam; }
  SourceParams* GetIsotopeParams() { return fIsotope; }
  G4String GetSourceTypeInfo() { return fGenerateSourceType; }
  void SetNemaPoint(G4int pointID) { fNemaGenerator.SetOnePointOnly(pointID); fNemaPoint = pointID; };
  void GenerateDefaultNemaPositions();
  void SetNemaPointPosition(G4int nemaPoint, const G4ThreeVector& position);
  void SetNemaPositionWeight(G4int position, G4int weight);
  void SetNemaPointLifetime(G4int position, G4double lifetime);
  void SetNemaPoint3GOption(G4int position);
  void SetNemaPointSize(G4int position, G4double radius, G4double length);
  void SetNemaPointPromptOption(G4int position);
  void SetNemaPointPromptSize(G4int position, G4double radius, G4double length);
  void SetNemaPointOrientation(G4int position, G4double theta, G4double phi);
  void SetNemaPointShape(G4int position, Dimension dim, G4double direction, G4double power, G4double length);
  void SetEffectivePositronRadius(G4double);
  
  G4int GetNemaPoint() { return fNemaPoint; }

private:
  G4int fNemaPoint = -1;
  G4String fGenerateSourceType;
  G4String fAllowedSourceTypes[6] = {"run", "beam", "isotope", "nema", "nema-mixed", "cosmics"};
  PrimaryGeneratorActionMessenger* fMessenger = nullptr;
  PrimaryGenerator* fPrimaryGenerator = nullptr;
  HistoManager* fHistoManager = nullptr;
  BeamParams* fBeam = nullptr;
  SourceParams* fIsotope = nullptr;
  NemaGenerator fNemaGenerator;
  G4double fEffectivePositronRadius = 0.5 * cm;
};

#endif /* !PRIMARYGENERATORACTION_H */
