// Created by James Silva on 7/1/18

#ifndef RICOCHET_ADR_H
#define RICOCHET_ADR_H 1

class G4LogicalVolume;

namespace detectorcomponents {

// Hard coded values of the Ricochet ADR
void ConstructADR(G4LogicalVolume* mother) noexcept;

} // namespace detectorcomponents

#endif // RICOCHET_ADR_H
