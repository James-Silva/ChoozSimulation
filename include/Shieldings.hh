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
  // material, thickness. Sets the layer color using an internal method.
  // Increases the member variable boxLength after each call by the created thickness.
  // This makes it easier to create a lot of layers and not worry about having
  // to input the boxLength each time.
  // This method is called in the DetectorConstructor due to call from the
  // DetectorMessenger. Look in Detector Messenger for example.
  void AddLayer(const std::string& material, G4LogicalVolume* mother);

  // Produces a layer just like AddLayer(~), but it cuts out the top of the layer
  void AddULayer(const std::string& material, G4LogicalVolume* mother);

private:
  // Sets various colors for different materials
  void setLayerColour(G4LogicalVolume* logic, const std::string& material);
  // Used in naming shieldings that are added
  int layerNum;
  // set in Detec. Messenger, new layers are created using this length
  double boxLength;
  // Inner Cavity around the crystal that is not filled with a material
  double cavityLength;
  // Thickness of the layer that is added
  double layerThickness;

public:
  void SetCavityLength(double clength) {
    cavityLength = clength;
    if (boxLength == -1) boxLength = clength;
  }
  // TODO: Add messenger command to be able to manually set this
  void SetBoxLength(double blength) { boxLength = blength; }
  void SetLayerThickness(double thick) { layerThickness = thick; }
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
