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
 *  @file NemaGenerator.h
 */

#ifndef NEMAGENERATOR_H
#define NEMAGENERATOR_H 1

#include <G4PrimaryVertex.hh>
#include <TVector3.h>
#include <TGraph.h>
#include <vector>
#include <TF1.h>
#include <map>

struct NemaPoint
{
  NemaPoint();
  
  G4ThreeVector position;
  G4double lifetime = 2;
  bool is3GAllowed = false;
  G4ThreeVector sizeOfPoint;
  bool isPromptAllowed = true;
  G4ThreeVector sizeOfPointPrompt;
  G4ThreeVector orientationOfPoint;
  G4ThreeVector shapeOfPointInY;
  TF1* elipseYNorm = nullptr;
};

class NemaGenerator
{
public:
  NemaGenerator() {};
  ~NemaGenerator() {
    fGeneratedPoints.clear();
    fWeightPositions.clear();
    fIDPointsConnection.clear();
  };
  void Clear() {
    fGeneratedPoints.clear();
    fWeightPositions.clear();
    fIDPointsConnection.clear();
  };
  void AddPoint(G4int pointID);
  void AddPointWeight(G4int pointID, G4int weight);
  void SetOnePointOnly(G4int pointID);
  void SetPointPosition(G4int pointID, const G4ThreeVector& pos) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).position = pos; };
  void SetPointLifetime(G4int pointID, G4double lf) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).lifetime = lf; };
  void SetPoint3GOption(G4int pointID, bool is3G) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).is3GAllowed = is3G; };
  void SetPointSize(G4int pointID, const G4ThreeVector& size) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).sizeOfPoint = size; };
  void SetPointPromptOption(G4int pointID, bool isPrompt) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).isPromptAllowed = isPrompt; };
  void SetPointPromptSize(G4int pointID, const G4ThreeVector& size) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).sizeOfPointPrompt = size; };
  void SetPointOrientation(G4int pointID, const G4ThreeVector& orient) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).orientationOfPoint = orient; };
  void SetNemaPointShapeInY(G4int pointID, const G4ThreeVector& shape) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).shapeOfPointInY = shape; };
  bool DoesPointExistAlready(G4int pointID) const;
  NemaPoint GetPoint(G4int pointID) const;
  NemaPoint GetRandomPoint() const;

  void GenerateElipseYNorm(G4int pointID);
  static G4ThreeVector GetPointShapedInY(G4ThreeVector vtxPosition, NemaPoint nemaPoint);
  static G4ThreeVector GetRotatedPoint(G4ThreeVector vtxPosition, NemaPoint nemaPoint);
  
private:
  std::vector<NemaPoint> fGeneratedPoints;
  std::vector<G4int> fWeightPositions;
  std::map<G4int, G4int> fIDPointsConnection;
};

#endif /* !NEMAGENERATOR_H */
