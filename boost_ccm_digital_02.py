# -*- coding: utf-8 -*-
#usar python3
import numpy as np
import matplotlib.pyplot as plt
from sympy import *
# import math
from scipy.signal import lti, bode, lsim, TransferFunction, step, step2
from scipy.signal import cont2discrete, dbode
from tc_udemm import sympy_to_lti, lti_to_sympy

"""
        Boost Voltage-Mode, ecuaciones segun:
        http://www.ti.com/lit/an/slva633/slva633.pdf
        Este a diferencia del Buck es un modelo de baja senial
        entonces debo estimar el Duty al que va a estar sometido
"""


#########################################################
# Transfer Function equation for the output voltage and #
# the output current.                                   #
#########################################################
L = 33e-6
Cout = 220e-6
RCout = 40e-3
Rload = 10
Vin = 12
Vout = 20
Duty = 1 - Vin/Vout

# Equivalent units
Leq = L /((1-Duty)**2)
w0 = 1 /(np.sqrt( Leq * Cout ))
Q = Rload /(Leq * w0)

# Sense probe
R1 = 10e3
R2 = 1000
alpha = R2/(R1+R2)
# alpha = 1
         

print ('Equivalent Params:')
print ('with a duty of: %.2f Leq: %f w0: %f Q: %f' %(Duty, Leq, w0, Q))

#TF equation Voltage and Lon output and voltage on Rsense
s = Symbol('s')

Plant_num = (1 + s * Cout * RCout) * (1 - s * Leq/Rload)
Plant_den = 1 + s /(w0*Q) + s**2/w0**2
Plant_out = alpha * (Vin/(1-Duty)**2) * (Plant_num/Plant_den)

Plant_out_sim = Plant_out.simplify()
print ('Plant_out: ')
print (Plant_out_sim)

#####################################################
# Desde aca utilizo ceros y polos que entrego sympy #
#####################################################
planta = sympy_to_lti(Plant_out_sim)
print ("planta con sympy:")
print (planta)

############################
# Convierto Planta Digital #
# por Tustin               #
############################
Fsampling = 24000
Tsampling = 1 / Fsampling
planta_dig_tustin_n, planta_dig_tustin_d, td = cont2discrete((planta.num, planta.den), Tsampling, method='tustin')

#normalizo con TransferFunction
print ("Planta Digital:")
planta_dig_tustin = TransferFunction(planta_dig_tustin_n, planta_dig_tustin_d, dt=td)
print (planta_dig_tustin)

################################################
# Respuesta escalon de la planta punto a punto #
# entrando con Duty propuesto como escalon     #
################################################
tiempo_de_simulacion = 2
print('td:')
print (td)
t = np.arange(0, tiempo_de_simulacion, td)

# Planta Digital por Tustin
b_planta = np.transpose(planta_dig_tustin_n)
a_planta = np.transpose(planta_dig_tustin_d)

vin_plant = np.ones(t.size) * Duty
vout_plant = np.zeros(t.size)

# for i in range(2, len(vin_plant)):        
#     ########################################
#     # aplico la transferencia de la planta #
#     ########################################
#     vout_plant[i] = b_planta[0]*vin_plant[i] \
#                     + b_planta[1]*vin_plant[i-1] \
#                     + b_planta[2]*vin_plant[i-2] \
#                     - a_planta[1]*vout_plant[i-1] \
#                     - a_planta[2]*vout_plant[i-2]

# fig, ax = plt.subplots()
# ax.set_title('Respuesta de la Planta Open Loop')
# ax.set_ylabel('Vout')
# ax.set_xlabel('Tiempo en muestras')
# ax.grid()
# ax.plot(t, vin_plant, 'y')
# ax.stem(t, vout_plant)
# plt.tight_layout()
# plt.show()

#########################################
# Realimento punto a punto con setpoint #
#########################################
############################
# PID Digital              #
# ki_dig = ki / Fsampling  #
# kp_dig = kp - ki_dig / 2 #
# kd_dig = kd * Fsampling  #
#                          #
############################
# funciona ok en 24000Hz
# kp_dig = 125 / 128
# ki_dig = 1 / 128
# kd_dig = 15

# funciona ok en 571Hz undersampling 42
kp_dig = 3000 / (128 * 3.3)
ki_dig = 42 / (128 * 3.3)
kd_dig = 0
# kp_dig = 0
# ki_dig = 41 / (128 * 3.3)
# kd_dig = 0

k1 = kp_dig + ki_dig + kd_dig
k2 = -kp_dig - 2*kd_dig
k3 = kd_dig

## este es el pid digital
b_pid = [k1, k2, k3]
a_pid = [1, -1]
print ("")
print (f"kp_dig: {kp_dig} ki_dig: {ki_dig} kd_dig: {kd_dig}")
print ("")

pid_dig = TransferFunction(b_pid, a_pid, dt=td)
print ("PID Digital:")
print (pid_dig)

############################################
# Armo la senial que quiero en el SETPOINT #
############################################
setpoint = Vout * alpha
vin_setpoint = np.ones(t.size) * setpoint
vout_plant = np.zeros(t.size)
error = np.zeros(t.size)
d = np.zeros(t.size)

max_d_pwm = 0.85
under_roof = 240
undersampling = 0

for i in range(2, len(vin_plant)):
    ###################################################
    # primero calculo el error, siempre punto a punto #
    ###################################################
    error[i] = vin_setpoint[i] - vout_plant[i-1]


    #############################################################
    # aplico lazo PID y ajusto los maximo y minimos que permito #
    #############################################################
    if undersampling < under_roof:
        #nada
        undersampling = undersampling + 1
        d[i] = d[i-1]
    else:
        undersampling = 0
        d[i] = b_pid[0] * error[i] + b_pid[1] * error[i-1] + b_pid[2] * error[i-2] - a_pid[1] * d[i-1]

    if d[i] > max_d_pwm:
        d[i] = max_d_pwm

    if d[i] < 0:
        d[i] = 0
        
    ########################################
    # aplico la transferencia de la planta #
    ########################################
    vin_plant[i] = d[i]
    vout_plant[i] = b_planta[0]*vin_plant[i] \
                    + b_planta[1]*vin_plant[i-1] \
                    + b_planta[2]*vin_plant[i-2] \
                    - a_planta[1]*vout_plant[i-1] \
                    - a_planta[2]*vout_plant[i-2]

        
fig, ax = plt.subplots()
ax.set_title('Respuesta Realimentada punto a punto')
ax.set_ylabel('Vout')
ax.set_xlabel('Tiempo en muestras')
ax.grid()
ax.plot(t, d, 'r')
ax.plot(t, error, 'g')
ax.plot(t, vin_setpoint, 'y')
# ax.stem(t, vout_plant)
ax.plot(t, vout_plant)
plt.tight_layout()
plt.show()



