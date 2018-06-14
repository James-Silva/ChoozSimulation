#Created 6/14/18 by James Silva
#Create 40 macros for joe_10Mev-50GeV_neutrons

for i in range(0,40):
    fileName = 'run_mei10MeV_'+ str(i+1) +'.mac'
    with open(fileName,'w') as f:
        f.write('/control/verbose 0\n')
        f.write('/run/verbose 0\n')
        f.write('/event/verbose 0\n')
        f.write('/tracking/verbose 0\n')
        f.write('/ricochetchoozsim/tree/setOutputFileName ChoozWithAirMei10MeV_'+str(i+1)+'.root\n')
        f.write('/ricochetchoozsim/generator/SetNeutronSource true\n')
        f.write('/ricochetchoozsim/generator/setsourceradius 4.25 m\n')
        f.write('/ricochetchoozsim/generator/setsourceheight 8.4 m\n')
        f.write('/ricochetchoozsim/generator/setneutronsourcespectrum input_spectra/fast_neutron_spectrum_Mei-32GeV-10MeVthreshold.txt\n')
        f.write('/run/beamOn 250000\n')
