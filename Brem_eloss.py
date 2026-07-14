import matplotlib.pyplot as plt


# Original data [Brem]
x = [0.2, 0.3, 0.4, 0.8, 1.0]  #Energy

Brem_y = [0.506584821 ,0.61171875, 1.039620536 , 1.763392857, 3.758928571]
#Brem_y_err = [0.012591299, 0.015864902 , 0.017126045 , 0.058473178 ,0.100958469]

Brem_y_err = [0.140527888, 0.177063642 , 0.191138891 , 0.652602429 ,1.126768629]


y_pdg_Brem = [0.527, 0.828 , 1.139 , 2.425, 3.086]

plt.figure()

# Plot experimental data with error bars
plt.errorbar(x, Brem_y, yerr=Brem_y_err, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^4$')
# Plot PDG data
#plt.plot(x, y_pdg_Brem, 's', label='PDG data')
##### Double-check ######
energy_scale_not_squared = [107.3995536, 162.3883929 , 236.2723214 , 489.6205357 ,623.7723214] 
errror_not_sqaured  = [0.279955711 ,0.000471671 ,0.000668574 ,0.001381071 ,0.00171723]


y_energy_unscale = [0.010739955 , 0.016238839 , 0.023627232 ,0.048962054 ,0.062377232]


plt.errorbar(x, energy_scale_not_squared, yerr=errror_not_sqaured, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^2$')
plt.plot(x, y_pdg_Brem, 's', label='PDG')


plt.plot(x, y_energy_unscale, 's', label=r'Raw output from GEANT4 scaling $\epsilon^2$')


plt.xscale('log')

plt.xlabel('mCP Energy [TeV]')
plt.ylabel('Total Energy lost [MeV cm2/g ]')
plt.title('mCP energy loss in 1 m Cu via Brem with different scaling')

plt.yscale('log')

plt.legend()

plt.show()

### Assuming no scaling 

