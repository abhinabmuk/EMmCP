import matplotlib.pyplot as plt


#below is GEANT4 simulation of ALPs simulation 

ALP_produced_ratio_to_lead = [1 , 0.335154827 , 1.58287796 , 0.361566485 , 0.661202186] #ALP produced

Error_associated_to_lead = [0, 0.061,0.19,0.06 , 0.103]

#####

ALPs_production_theortical_ratio = [1, 0.28826946 , 1.571307595 , 0.29371734 , 0.615196786, 0.43563989 ,0.332585052, 0.225539415]

Z = [82 ,26 ,74 ,30 ,47 ,50 ,40 ,56 ]


# This is updated after running 1 million events 

ALP_produced_ratio_to_lead = [1 , 0.299234579 , 1.562449347 , 0.304007204 , 0.63241783 , 0.443133724 , 0.346150383 ,0.229716344] #ALP produced

Error_associated_sim = [0 ,0.006 ,0.02 ,0.006 ,0.007 ,0.007 ,0.006 ,0.005]



plt.figure()

# Plot experimental data with error bars
plt.errorbar(Z, ALP_produced_ratio_to_lead, yerr=Error_associated_sim, fmt='o', capsize=5, label='GEANT4 simulation')
plt.errorbar(Z, ALPs_production_theortical_ratio, fmt='o', capsize=5, label='Theory')


plt.xlabel('Z of material')
plt.ylabel('Ratio of ALPs produced to lead')
plt.title('Ratio check for ALPs between theory and simulation')

plt.yscale('log')
plt.legend()

plt.savefig("Alpratio.png", dpi=300, bbox_inches='tight')

plt.show()




### -------------------------------------------------------------  ### 

ALP_sim = [
    1.1105E-08,
    3.323E-09,
    1.7351E-08,
    3.376E-09,
    7.023E-09,
    4.921E-09,
    3.844E-09,
    2.551E-09
]

ALP_sim_error = [
    1.0538E-10,
    5.76455E-11,
    1.31723E-10,
    5.81034E-11,
    8.38033E-11,
    7.01498E-11,
    6.2E-11,
    5.05074E-11
]

theory = [
    1.12502E-08,
    3.24308E-09,
    1.77005E-08,
    3.30437E-09,
    6.92106E-09,
    4.90102E-09,
    3.74164E-09,
    2.53735E-09
]


plt.figure()

# Plot experimental data with error bars
plt.errorbar(Z, ALP_sim, yerr=ALP_sim_error, fmt='o', capsize=5, label='Simulation')
plt.errorbar(Z, theory, fmt='o', capsize=5, label='Theory')


plt.xlabel('Z of material')
plt.ylabel('Probability of ALPs being produced')
plt.title('Comparison of Probability of ALPs being produced for theory and simulation')

plt.yscale('log')
plt.legend()

plt.savefig("Al_probability.png", dpi=300, bbox_inches='tight')

plt.show()
