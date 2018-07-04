// Created by James Silva on 7/1/18

#ifndef RICOCHET_ADR_H
#define RICOCHET_ADR_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class G4SubtractionSolid;

namespace detectorcomponents {

// Hard coded values of the Ricochet ADR
void ConstructADR(G4LogicalVolume* mother) noexcept;

} // namespace detectorcomponents

#endif // RICOCHET_ADR_H
