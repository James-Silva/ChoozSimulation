#ifndef MATERIALS_HH
#define MATERIALS_HH 1

#include <G4Material.hh>

class G4String;

class Materials
{
public:

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

#endif // MATERIALS_HH
