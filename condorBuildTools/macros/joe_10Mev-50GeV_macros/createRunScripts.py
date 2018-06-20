#Created 6/14/18 by James Silva
#Create 40 macros for joe_10Mev-50GeV_neutrons

for i in range(0,20):
    fileName = 'run_joe10MeV_'+ str(i) +'.mac'
    with open(fileName,'w') as f:
        f.write('/control/verbose 0\n')
        f.write('/run/verbose 0\n')
        f.write('/event/verbose 0\n')
        f.write('/tracking/verbose 0\n')
        f.write('/ricochetchoozsim/tree/setOutputFileName ChoozWithAirJoe10MeV_'+str(i)+'.root\n')
        f.write('/ricochetchoozsim/generator/SetNeutronSource true\n')
        f.write('/ricochetchoozsim/generator/setsourceradius 4.25 m\n')
        f.write('/ricochetchoozsim/generator/setsourceheight 8.4 m\n')
        f.write('/ricochetchoozsim/generator/setneutronsourcespectrum /home/silvaj/fast_neutrons_10_000_000/build/input_spectra/fast_neutron_spectrum_Joe-32GeV-10MeVthreshold.txt\n')
        f.write('/run/beamOn 500000\n')