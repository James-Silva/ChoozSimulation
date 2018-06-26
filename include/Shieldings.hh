#ifndef DETECTORCOMPONENTS_H
#define DETECTORCOMPONENTS_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class G4SubtractionSolid;

namespace DetectorComponents {

  void ConstructGioveShielding(G4LogicalVolume* mother) noexcept;

  void ConstructPolySheilding(const double innerR, const double outerR,
														  const double topthickness, G4LogicalVolume* mother)
                              noexcept;

  void ConstructPbSheilding(const double innerR, const double outerR,
													  const double topthickness, G4LogicalVolume* mother)
                            noexcept;

}

#endif // DETECTORCOMPONENTS_H
