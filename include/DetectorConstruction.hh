#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"


class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Cons;
class G4Tubs;
class G4Material;
class G4Element;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
	    DetectorConstruction();
	    virtual ~DetectorConstruction();
    	void ConstructADR();
    	void ConstructPit();
        void ConstructOuterDetectors();
        void InitializeWorld();	
    	void ConstructPbSheilding(G4double innerR, G4double outerR,G4double topthickness);	
		void ConstructPolySheilding(G4double innerR, G4double outerR,G4double topthickness);	
		virtual G4VPhysicalVolume* Construct();
		void AddConcreteFloor();
        void AddCoffinWalls();
        void AddConcreteWalls();
        void SetCrystalMaterial(G4String Material);

  	
    private:
            G4String                crystalmaterial;
  		    G4double                zeroradius;
            G4double                outerRadius1;
            G4double                outerRadius2;
            G4double                outerRadius3;
            G4double                outerRadius4;
            G4double                outerRadius5;
            G4double                outerRadius11;
            G4double                outerRadius21;
            G4double                outerRadius31;
            G4double                outerRadius41;
            G4double                outerRadius_shield;
            G4double                outerRadius_shield1;
            G4double                hz1;
            G4double                hz11;
            G4double                hz2;
            G4double                hz21;
            G4double                hz3;
			G4double                hz31;
			G4double                hz4;
            G4double                hz41;
            G4double                hz5;
			G4double                startAngle;
			G4double                startAngle90;
            G4double                endangle;
            G4double                spanningAngleFull;
            G4double                DetectorSize;
            G4double                floorWidthX;
            G4double                floorWidthZ;
            G4double                floorThickness;
            G4double                wallsHeight;
            G4double                fADRheight;
			G4double                fADRheight1;
            G4double                fADRheightaboveground;
            G4ThreeVector           fvec_roomoffset;

            G4double                pos_z;	
            G4double                pos_x;
            G4double                pos_y;		


			G4double				fWorldSize;
			G4RotationMatrix*		noRotation;

            G4VPhysicalVolume*      phystest;

	  	    G4Box*                  fSolidWorld;
	  	    G4LogicalVolume*		fLogicWorld;
	  	    G4VPhysicalVolume* 		fPhysiWorld;

			G4Tubs*                 TempTube_inner;
            G4Tubs*                 TempTube_outer;

            G4Tubs* 				fSolidDetector;
			G4LogicalVolume* 		fLogicDetector;
			G4VPhysicalVolume*		fPhysiDetector;

			G4Tubs* 				plateTube;
			G4LogicalVolume* 		plateLog;
			G4VPhysicalVolume*		platePhys;

			G4Box* 				    crystalBox_single;
			G4LogicalVolume* 		crystalLog_single;
			G4VPhysicalVolume*		crystalPhys_single;

            G4Tubs*                 TargetDetectorBox;
            G4LogicalVolume*        TargetDetectorLog;
            G4VPhysicalVolume*      TargetDetectorPhys;

			G4Tubs*                 shell1Tube;
			G4Tubs*                 shell11Tube;
			G4LogicalVolume* 		shellinnerAlTubeLog;
			G4VPhysicalVolume*		shellinnerAlTubePhys;		

			G4Tubs*                 shell2Tube;
			G4Tubs*                 shell21Tube;
			G4LogicalVolume* 		shellouterAlTubeLog;
			G4VPhysicalVolume*		shellouterAlTubePhys;
			

   		    G4Tubs*                 tungTube;
			G4Tubs*                 tung1Tube;
			G4LogicalVolume* 		fulltungTubeLog;
			G4VPhysicalVolume*		fulltungTubePhys;

   		    G4Tubs*                 scinTube;
			G4Tubs*                 scin1Tube;
			G4LogicalVolume* 		fullscinTubeLog;
			G4VPhysicalVolume*		fullscinTubePhys;
   		    
   		    G4Tubs*                 shieldTube;
			G4Tubs*                 shield1Tube;
			G4LogicalVolume* 		fullshieldTubeLog;
			G4VPhysicalVolume*		fullshieldTubePhys;

			G4Box*                  solidFloor;
            G4LogicalVolume* 		logFloor;
            G4VPhysicalVolume*		physFloor;
            
            G4SubtractionSolid*     solidWalls;
            G4LogicalVolume*        logWalls;
            G4VPhysicalVolume*      physWalls;

            G4SubtractionSolid*     PitSolid;
            G4LogicalVolume*        PitLog;
            G4VPhysicalVolume*      PitPhys;       

            G4SubtractionSolid*     GCTubeSolid;
            G4LogicalVolume*        GCTubeLog;
            G4VPhysicalVolume*      GCTubePhys;


            G4SubtractionSolid*     BufferTubeSolid;
            G4LogicalVolume*        BufferTubeLog;
            G4VPhysicalVolume*      BufferTubePhys;
			
            G4SubtractionSolid*     VetoTubeSolid;
            G4LogicalVolume*        VetoTubeLog;
            G4VPhysicalVolume*      VetoTubePhys;

            G4SubtractionSolid*     SteelTubeSolid;
            G4LogicalVolume*        SteelTubeLog;
            G4VPhysicalVolume*      SteelTubePhys;

            G4SubtractionSolid*     WaterTubeSolid;
            G4LogicalVolume*        WaterTubeLog;
            G4VPhysicalVolume*      WaterTubePhys;

            std::vector<G4Tubs*>	fSolidPolyShielding;
			std::vector<G4LogicalVolume*>	fLogicPolyShielding;
			std::vector<G4VPhysicalVolume*>	fPhysiPolyShielding;

			std::vector<G4Tubs*>	fSolidLeadShielding;
			std::vector<G4LogicalVolume*>	fLogicLeadShielding;
			std::vector<G4VPhysicalVolume*>	fPhysiLeadShielding;

			std::vector<G4Tubs*>	fSolidPolyEndcaps;
			std::vector<G4LogicalVolume*>	fLogicPolyEndcaps;
			std::vector<G4VPhysicalVolume*>	fPhysiPolyEndcaps;

			std::vector<G4Tubs*>	fSolidLeadEndcaps;
			std::vector<G4LogicalVolume*>	fLogicLeadEndcaps;
			std::vector<G4VPhysicalVolume*>	fPhysiLeadEndcaps;

			std::vector<G4Box*>	v_CrystalBoxes;
			std::vector<G4LogicalVolume*>	v_CrystalBoxesLog;
			std::vector<G4VPhysicalVolume*>	v_CrystalBoxesPhys;
            
            G4Box*             solidWallsOuterBox;
            G4Box*             solidWallsInnerVoid;
            G4Box*             coffinfloor;
            
            G4Box* solidceiling;
            G4LogicalVolume* logceiling;
            G4VPhysicalVolume* physceiling;        

            G4Box*             inbetweenwall;
            G4LogicalVolume*   inbetweenwall_logvolume;
            G4VPhysicalVolume* physinbetweenwall;            
            G4Box*             inbetweenwall_B;
            G4LogicalVolume*   inbetweenwall_logvolume_B;
            G4VPhysicalVolume* physinbetweenwall_B;
            G4Box*            inbetweenwall_C;
            G4LogicalVolume*   inbetweenwall_logvolume_C;
            G4VPhysicalVolume* physinbetweenwall_C;
            G4Box*             inbetweenwall_D;
            G4LogicalVolume*   inbetweenwall_logvolume_D;
            G4VPhysicalVolume* physinbetweenwall_D;
            G4Box*             inbetweenwall_E;
            G4LogicalVolume*   inbetweenwall_logvolume_E;
            G4VPhysicalVolume* physinbetweenwall_E;            


	  	// Material properties
	  	G4Material* 		fMaterialGalactic;
	  	G4Material*			fMaterialGe;
	  	G4Material*			fMaterialSi;
	  	G4Material*			fMaterialAir;
	  	G4Material*			fMaterialPoly;
	    G4Material*         fMaterialPoly_Borated;
        G4Material*			fMaterialLead;
        G4Material*			fMaterialOs;
        G4Material*			fMaterialAl;
        G4Material*			fMaterialW;
        G4Material*			fMaterialCu;
        G4Material*			fMaterialPb;
        G4Material*			fMaterialZn;        
        G4Material*         fMaterialZr;
        G4Material*         fMaterialGd;
        G4Material*			fMaterialScintillator;
        G4Material*         fMaterialWater;
        G4Material*			fMaterialConcrete;
        G4Material*         fMaterialQuartz;
        G4Material*         fMaterialCorundum;
        G4Material*         fMaterialIronOxide;
        G4Material*         fMaterialMgOxide;
        G4Material*         fMaterialKOxide;
        G4Material*         fMaterialChoozRock;
        G4Material*         Chrysotile;
        G4Material*         mineraloil;
        G4Material*         fMaterialSteel; 
        G4Material*         fMaterialDodecane;
        G4Material*         fMaterialPXE;
        G4Material*         fMaterialPPO;
        G4Material*         fMaterialNuTarget;
        G4Material*         fMaterialGammaCatcher;
        G4Element*         elH;
        G4Element*         elO;
        G4Element*         elSi;
        G4Element*         elB;
        G4Element*         elMg;
        G4Element*         elK;
        G4Element*         elFe;
        G4Element*         elAl;
        G4Element*         elC;

        // Visualizer attributes
	  	G4VisAttributes* 	fVisAttDetector;

	  	DetectorMessenger* fDetectorMessenger;


	private:
		void DefineMaterials();
		void ComputeGeometryParams();
        G4VPhysicalVolume* ConstructDetectors();
        G4VPhysicalVolume* ConstructSingleDetector();
        G4VPhysicalVolume*  ConstructNuDetector();
};


#endif
