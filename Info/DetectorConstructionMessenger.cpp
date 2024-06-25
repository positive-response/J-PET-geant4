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
 *  @file DetectorConstructionMessenger.cpp
 */

#include "../Info/DetectorConstructionMessenger.h"
#include "../Core/DetectorConstants.h"

DetectorConstructionMessenger::DetectorConstructionMessenger() {}

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* detector) : fDetector(detector)
{
  fDirectory = new G4UIdirectory("/jpetmc/detector/");
  fDirectory->SetGuidance("Commands for controling the geometry");

  fLoadTargetForRun = new G4UIcmdWithAnInteger("/jpetmc/detector/loadTargetForRun", this);
  fLoadTargetForRun->SetGuidance("Set RUN number to simulate given Target");
  fLoadTargetForRun->SetDefaultValue(kDefaultRunNumber);

  fLoadIdealGeometry = new G4UIcmdWithAnInteger("/jpetmc/detector/loadIdealGeom", this);
  fLoadIdealGeometry->SetGuidance("Generate ideal geometry for 1-4 layers");
  fLoadIdealGeometry->SetDefaultValue(1);

  fLoadJPetBasicGeometry = new G4UIcmdWithoutParameter("/jpetmc/detector/loadJPetBasicGeom", this);
  fLoadJPetBasicGeometry->SetGuidance("Generate standard JPet detector geometry");

  fLoadOnlyScintillators = new G4UIcmdWithoutParameter("/jpetmc/detector/loadOnlyScintillators", this);
  fLoadOnlyScintillators->SetGuidance("Generate only scintillators (for test purposes)");

  fLoadWrapping = new G4UIcmdWithABool("/jpetmc/detector/loadWrapping", this);
  fLoadWrapping->SetGuidance("Set generating wrapping of the scintillators");

  //! Bool converted into string
  fLoadModularLayer = new G4UIcmdWithAString("/jpetmc/detector/loadModularLayer", this);
  fLoadModularLayer->SetGuidance("Load additional layer made out of modules");
  
  fLoadModularLayerOnly = new G4UIcmdWithoutParameter("/jpetmc/detector/loadModularLayerOnly", this);
  fLoadModularLayerOnly->SetGuidance("Do not load other layers of detectors");

  fScinHitMergingTime = new G4UIcmdWithADoubleAndUnit("/jpetmc/detector/hitMergingTime", this);
  fScinHitMergingTime->SetGuidance("Define time range (ns) while merging hits in scintillators");
  fScinHitMergingTime->SetDefaultUnit("ns");
  fScinHitMergingTime->SetUnitCandidates("ns");

  fGeometryFileName = new G4UIcmdWithAString("/jpetmc/detector/geometryFileName", this);
  fGeometryFileName->SetGuidance("Create a JSON file for the simulated setup with a given name.");

  fCreateGeometryType = new G4UIcmdWithAString("/jpetmc/detector/createGeometryType", this);
  fCreateGeometryType->SetGuidance("Set structure of output JSON file: barrel or modular.");

  fJSONSetupFile = new G4UIcmdWithAString("/jpetmc/detector/jsonSetupFile", this);
  fJSONSetupFile->SetGuidance(
      "Name of JSON setup file with detector construction, that will be used to create scintillators instead of hardcoded procedure.");

  fJSONSetupRunNum = new G4UIcmdWithAnInteger("/jpetmc/detector/jsonSetupRunNum", this);
  fJSONSetupRunNum->SetGuidance("Set the number of Run that is to be loaded from JSON file.");

  fPressureInChamber = new G4UIcmdWithADoubleAndUnit("/jpetmc/detector/chamberPressure", this);
  fPressureInChamber->SetGuidance("Define pressure in the chamber");
  fPressureInChamber->SetDefaultUnit("Pa");
  fPressureInChamber->SetUnitCandidates("Pa");
}

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
  delete fLoadTargetForRun;
  delete fLoadIdealGeometry;
  delete fLoadJPetBasicGeometry;
  delete fLoadOnlyScintillators;
  delete fLoadWrapping;
  delete fLoadModularLayer;
  delete fLoadModularLayerOnly;
  delete fScinHitMergingTime;
  delete fGeometryFileName;
  delete fCreateGeometryType;
  delete fJSONSetupFile;
  delete fJSONSetupRunNum;
  delete fPressureInChamber;
}

// cppcheck-suppress unusedFunction
void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fLoadTargetForRun)
  {
    fDetector->LoadGeometryForRun(fLoadTargetForRun->GetNewIntValue(newValue));
    fDetector->UpdateGeometry();
  }
  else if (command == fLoadIdealGeometry)
  {
    G4Exception("DetectorConstructionMessenger", "DCM01", JustWarning, "Option is not yet implemented");
  }
  else if (command == fLoadJPetBasicGeometry)
  {
    fDetector->ConstructBasicGeometry(true);
    fDetector->LoadFrame(true);
    fDetector->UpdateGeometry();
  }
  else if (command == fLoadOnlyScintillators)
  {
    fDetector->LoadFrame(false);
    fDetector->UpdateGeometry();
  }
  else if (command == fLoadWrapping)
  {
    fDetector->LoadWrapping(fLoadWrapping->GetNewBoolValue(newValue));
  }
  else if (command == fLoadModularLayer)
  {
    fDetector->ConstructModularLayer(newValue);
    fDetector->UpdateGeometry();
  } else if (command == fLoadModularLayerOnly) 
  {
    fDetector->ConstructBasicGeometry(false);
    fDetector->UpdateGeometry();
  }
  else if (command == fScinHitMergingTime)
  {
    DetectorConstants::SetMergingTimeValueForScin(fScinHitMergingTime->GetNewDoubleValue(newValue));
    fDetector->UpdateGeometry();
  }
  else if (command == fGeometryFileName)
  {
    fDetector->CreateGeometryFileFlag(true);
    if (!newValue.contains(".json"))
    {
      newValue.append(".json");
    }
    fDetector->SetGeometryFileName(newValue);
  }
  else if (command == fCreateGeometryType)
  {
    fDetector->CreateGeometryFileFlag(true);
    fDetector->SetGeometryFileType(newValue);
  }
  else if (command == fJSONSetupFile)
  {
    std::ifstream file(newValue.c_str());
    if (file.good())
    {
      fDetector->readJSONSetup(true);
      fDetector->setJSONFileName(newValue);
      fDetector->UpdateGeometry();
    }
    else
    {
      G4Exception("DetectorConstructionMessenger", "DCM02", FatalException, "Provided JSON file does not exist.");
    }
  }
  else if (command == fJSONSetupRunNum)
  {
    fDetector->setJSONSetupRunNum(fJSONSetupRunNum->GetNewIntValue(newValue));
  }
  else if (command == fPressureInChamber)
  {
    fDetector->SetPressureInChamber(fPressureInChamber->GetNewDoubleValue(newValue));
  }
}
