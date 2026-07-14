# Plot data with error bars and overlay PDG data (no explicit colors)

import matplotlib.pyplot as plt

# Original data [Pair production]
x = [0.2, 0.3, 0.4, 0.8, 1.0]

y = [0.824776786, 1.303571429, 1.777901786, 3.763392857, 4.579241071]
yerr = [0.032477679, 0.048303571, 0.063415179, 0.11953125, 0.115848214]

y_pdg = [0.755, 1.189, 1.64, 3.49, 4.438]

plt.figure()

# Plot experimental data with error bars
plt.errorbar(x, y, yerr=yerr, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^2$')

# Plot PDG data
plt.plot(x, y_pdg, 's', label='PDG data')

plt.xlabel('mCP Energy [TeV]')
plt.ylabel('Total Energy lost [MeV cm2/g ]')
plt.title('mCP energy loss in 1 m Cu Pair Production')
plt.legend()


plt.show()

# Original data [Brem]

Brem_y = [0.506584821 ,0.61171875, 1.039620536 , 1.763392857, 3.758928571]
#Brem_y_err = [0.012591299, 0.015864902 , 0.017126045 , 0.058473178 ,0.100958469]

Brem_y_err = [0.140527888, 0.177063642 , 0.191138891 , 0.652602429 ,1.126768629]


y_pdg_Brem = [0.527, 0.828 , 1.139 , 2.425, 3.086]

plt.figure()

# Plot experimental data with error bars
plt.errorbar(x, Brem_y, yerr=Brem_y_err, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^4$')

# Plot PDG data
plt.plot(x, y_pdg_Brem, 's', label='PDG data')

plt.xlabel('mCP Energy [TeV]')
plt.ylabel('Total Energy lost [MeV cm2/g ]')
plt.title('mCP energy loss in 1 m Cu via Bremsstrahlung')
plt.legend()

plt.show()

# Plot simulation data with error bars and overlay PDG data

energy = [100,140,200,267,300,400,800,1000,1400,2000,3000,4000,
          8000,10000,14000,20000,30000,40000,80000,100000,
          140000,200000,317000,400000,800000]

deposition = [
1.588169643,1.473214286,1.415178571,1.401785714,1.401785714,
1.416294643,1.496651786,1.529017857,1.584821429,1.642857143,
1.708705357,1.758928571,1.854910714,1.889508929,1.933035714,
1.984375,2.013392857,2.050223214,2.141741071,2.161830357,
2.174107143,2.253348214,2.253348214,2.2421875,2.309151786
]

error = [
0.0011181,0.0011359,0.0011646,0.001984,0.001984,
0.002652,0.00466,0.0005618,0.00766,0.01041,
0.01481,0.0195,0.03301,0.04165,0.05369,
0.06894,0.08329,0.1023,0.1682,0.2113,
0.1947,0.3321,0.344,0.35,0.5389
]

pdg = [
1.561,1.464,1.414,1.403,1.404,1.419,1.499,1.532,1.585,1.643,
1.709,1.755,1.86,1.891,1.936,1.981,2.029,2.061,2.133,2.155,
2.187,2.221,2.264,2.285,2.351
]

plt.figure()

# Simulation with error bars
plt.errorbar(energy, deposition, yerr=error, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^2$')
# PDG overlay (line)
plt.plot(energy, pdg, 's', label='PDG muon energy loss')

plt.xscale('log')

plt.xlabel('mCP Energy [MeV]')
plt.ylabel('Total Energy lost [MeV cm2/g ]')
plt.title('mCP energy loss in 1 m Cu via Ionization')

plt.legend()

plt.show()





##### Double-check ######

energy_scale_not_squared = [107.3995536, 162.3883929 , 236.2723214 , 489.6205357 ,623.7723214] 
errror_not_sqaured  = [0.279955711 ,0.000471671 ,0.000668574 ,0.001381071 ,0.00171723]


plt.figure()

plt.errorbar(x, energy_scale_not_squared, yerr=errror_not_sqaured, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^2$')
plt.plot(x, y_pdg_Brem, 's', label='PDG')

plt.xscale('log')

plt.xlabel('mCP Energy [MeV]')
plt.ylabel('Total Energy lost [MeV cm2/g ]')
plt.title('mCP energy loss in 1 m Cu via Brem with different scaling')

plt.yscale('log')

plt.legend()

plt.show()



##### plot in same plot everything 


plt.figure()

# Plot experimental data with error bars
plt.errorbar(
    x, Brem_y, yerr=Brem_y_err,
    fmt='o', capsize=5,
    label=r'GEANT4 results using Muon_Brem by $\epsilon^4$, assuming no internal scaling [scaled by $1/\epsilon^4$]'
)

# plt.errorbar(
#     x, energy_scale_not_squared, yerr=errror_not_sqaured,
#     fmt='o', capsize=5,
#     label=r'GEANT4 results using Muon_Brem by $\epsilon^2$, assuming internal scaling by $\epsilon^2$ [scaled by $1/\epsilon^4$]'
# )



y_no_scale_charged_sq = [0.010739955 , 0.016238839 ,0.023627232 , 0.048962054 ,0.062377232]
y_no_scale_charged_sq_error = [250.8403169 , 0.422616848 , 0.599042611 , 1.237440019 ,1.538638375]

#plt.errorbar(x, y_no_scale_charged_sq, yerr=y_no_scale_charged_sq_error, fmt='o', capsize=5, label=r'GEANT4 results using Muon_Brem by $\epsilon^2$, assuming internal scaling by $\epsilon^2$ [no scaling]')


# Plot PDG data
plt.plot(x, y_pdg_Brem, 's', label='PDG data')

plt.xlabel('mCP Energy [TeV]')
plt.ylabel('Total Energy lost [MeV cm2/g ]')
plt.title('mCP energy loss in 1 m Cu via Bremsstrahlung')
plt.legend()
plt.yscale('log')
plt.show()