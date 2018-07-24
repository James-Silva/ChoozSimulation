// Created by James Silva on 6/27/18

#ifndef SHIELDINGS_H
#define SHIELDINGS_H 1

#include <string>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class G4SubtractionSolid;

namespace detectorcomponents {

// Builds Geant4 solids around the origin of a mother volume.
//
// Example:
//    LayerConstructor layerBuilder;
//    layerBuilder.AddG4Box("material", 50*mm, logicWorld;)
//
class LayerConstructor {
 public:
  LayerConstructor();

  // Places a G4Box at the origin of a given mother volume with a given
  // material, thickness. Does not produce the box if it would have a length
  // less than zero. Sets the color of each layer to orange.
  // Decreases the member variable boxLength after each call by the created thickness.
  // This makes it easier to create a lot of layers and not worry about having
  // to input the boxLength each time. The max box length is set in this class.
  // This method is called in the DetectorConstructor due to call from the
  // DetectorMessanger.
  void AddG4Box(const std::string& material, const double thickness,
                G4LogicalVolume* mother);

 private:
  void setLayerColour(G4LogicalVolume* logic, const std::string& material);
  double boxLength;
  int layerNum;
};

// Hard coded values of the Giove Shielding used by the Very Near Site Detector.
void ConstructGioveShielding(G4LogicalVolume* mother) noexcept;

// Cylindrical volume of Polyethylene centered arounf the origin of the mother.
void ConstructPolySheilding(const double innerR, const double outerR,
													  const double topthickness, G4LogicalVolume* mother)
                            noexcept;

// Cylindrical volume of Polyethylene centered arounf the origin of the mother.
void ConstructPbSheilding(const double innerR, const double outerR,
												  const double topthickness, G4LogicalVolume* mother)
                          noexcept;

} // namespace detectorcomponents

#endif // SHIELDINGS_H
