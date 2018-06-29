#ifndef MATERIALS_HH
#define MATERIALS_HH 1

#include <G4Material.hh>

class G4String;

namespace detectorcomponents
{

// Builds a list of materials that Geant stores and are available for access.
//
// Example:
//    // It is crucial that CreateMaterials() is called before any materials are
//    //acessed.
//    Materials::CreateMaterials(); // Only needs to be called once
//    auto log = new G4LogicalVolume(solid, G4Material::GetMaterial("G4_Cu"),...
//
class Materials
{
public:
  //Builds materials for the entire program
  static void CreateMaterials() {
    if (materials == nullptr) { materials = new Materials(); }
  }

  static Materials* Get() {
    CreateMaterials();
    return materials;
  }

  static G4Material* GetMaterial(const G4String& material) {
    CreateMaterials();
    return G4Material::GetMaterial(material);
  }

private:
  Materials();
  Materials(const Materials& ) = delete;

  static Materials* materials;
};

} // namespace detectorcomponents

#endif // MATERIALS_HH
