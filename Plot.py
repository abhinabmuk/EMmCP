import matplotlib.pyplot as plt

# Original data [Pair production]
x = [0.2, 0.3, 0.4, 0.8, 1.0]
y = [0.824776786, 1.303571429, 1.777901786, 3.763392857, 4.579241071]
yerr = [0.032477679, 0.048303571, 0.063415179, 0.11953125, 0.115848214]
y_pdg = [0.755, 1.189, 1.64, 3.49, 4.438]

# Ionization data
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

# --- Subplots ---
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# --- Left: Pair Production ---
ax1.errorbar(x, y, yerr=yerr, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^2$')
ax1.plot(x, y_pdg, 's', label='PDG data')
ax1.set_xlabel('mCP Energy [TeV]')
ax1.set_ylabel(r'Total Energy lost [MeV cm$^2$/g]')
ax1.set_title('Pair Production')
ax1.legend()
ax1.grid(True, alpha=0.3)

# --- Right: Ionization ---
ax2.errorbar(energy, deposition, yerr=error, fmt='o', capsize=5, label=r'GEANT4 scaled by $1/\epsilon^2$')
ax2.plot(energy, pdg, 's', label='PDG muon energy loss')
ax2.set_xscale('log')
ax2.set_xlabel('mCP Energy [MeV]')
ax2.set_ylabel(r'Total Energy lost [MeV cm$^2$/g]')
ax2.set_title('Ionization')
ax2.legend()
ax2.grid(True, alpha=0.3)

fig.suptitle('mCP energy loss in 1 m Cu', fontsize=14, fontweight='bold')
plt.savefig("mCP.png", dpi=300, bbox_inches='tight')
plt.tight_layout()
plt.show()