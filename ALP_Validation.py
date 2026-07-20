# Checks for validation for ALPs
# 1] Decay width 
# 2] Cross-section 
# Parameters: 0.0167 GeV ; 0.001 1/GeV
# Vary Energy of photon

# Set different coupling constant 


# GEANT4 computed output 
# Numerical simulation


ma = 0.0167 

g_ayy = [0.001 , 0.005 , 0.01 , 0.05 , 0.1 , 0.5]

decay_width= [2.31643e-14 , 5.79108e-13 , 2.31643e-12 , 5.79108e-11 , 2.31643e-10 , 5.79108e-09 ]   ## in GeV

import numpy as np

g_ayy = np.array([0.001, 0.005, 0.01, 0.05, 0.1, 0.5])

Gamma = (g_ayy**2 * ma**3) / (64 * np.pi)

print(Gamma)

import matplotlib.pyplot as plt

plt.plot(g_ayy, decay_width, 'o', label=r'GEANT4 computed $\Gamma$')
plt.plot(g_ayy, Gamma,'x', label=r'Numerically computed $\Gamma$')
plt.yscale('log')
plt.xlabel('Coupling constant g_ayy')
plt.ylabel(r'$\Gamma$')
plt.legend()
plt.savefig("ValidationPLotG4ALPs.png", dpi=300, bbox_inches='tight')
plt.show()

### Cross-section computed





