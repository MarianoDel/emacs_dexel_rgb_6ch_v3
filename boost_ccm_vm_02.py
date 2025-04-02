# -*- coding: utf-8 -*-
#usar python3
import numpy as np
import matplotlib.pyplot as plt
from sympy import *
# import math
from scipy.signal import lti, bode, lsim, TransferFunction, step
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
L = 215.7e-6
Cout = 940e-6
RCout = 40e-3
Rload = 24
Vi = 12
Duty = 0.67

# Equivalent units
Leq = L /((1-Duty)**2)
w0 = 1 /(np.sqrt( Leq * Cout ))
Q = Rload /(Leq * w0)

# Sense probe
R1 = 22e3
R2 = 1800
alpha = R2/(R1+R2)
# alpha = 1
         

print ('Equivalent Params:')
print ('with a duty of: %.2f Leq: %f w0: %f Q: %f' %(Duty, Leq, w0, Q))

#TF equation Voltage and Lon output and voltage on Rsense
s = Symbol('s')

Plant_num = (1 + s * Cout * RCout) * (1 - s * Leq/Rload)
Plant_den = 1 + s /(w0*Q) + s**2/w0**2
Plant_out = alpha * (Vi/(1-Duty)**2) * (Plant_num/Plant_den)

Plant_out_sim = Plant_out.simplify()
print ('Plant_out: ')
print (Plant_out_sim)

#####################################################
# Desde aca utilizo ceros y polos que entrego sympy #
#####################################################
planta = sympy_to_lti(Plant_out_sim)
print ("planta con sympy:")
print (planta)


########################################
# Respuesta en Frecuencia de la Planta #
########################################
freq = np.arange(1, 100000, 1)
w, mag, phase = bode(planta, freq)

fig, (ax1, ax2) = plt.subplots(2,1)
ax1.semilogx (w/(2*np.pi), mag, 'b-', linewidth="1")
ax1.set_title('Plant Tf - Magnitude')

ax2.semilogx (w/(2*np.pi), phase, 'r-', linewidth="1")
ax2.set_title('Phase')

plt.tight_layout()
plt.show()


"""    
        PID Analogico
        PID completo Tf = Kp + Ki/s + s Kd    Tf = 1/s * (s**2 Kd + s Kp + Ki)
        muy dificil analizar, basicamente polo en origen y dos ceros
        los dos ceros, segun los parametros elegidos, pueden llegar a ser complejos conjugados

        si fuese solo PI tengo Tf = 1/s * Kp * (s + Ki/Kp)
        esto es polo en origen w = 1; cero en w = Ki/Kp; ganancia Kp

        si fuese solo PD tengo Tf = Kd * (s + Kp/Kd)
        esto es cero en w = Kp/Kd y ganancia Kd

        Conclusion:
        elijo Kp para la ganancia media, ej 0dB Kp = 1
        elijo primer cero, ej 15.9Hz, Ki = 100
        elijo segundo cero, ej 1590Hz, Kd = 0.0001
"""
#################
# PID analogico #
#################
kp = kp_analog = 0.
ki = ki_analog = 0.3
kd = kd_analog = 0.

Pid_out = kp + ki/s + s*kd
Pid_out_sim = Pid_out.simplify()

print ('Pid_out: ')
print (Pid_out_sim)

##############################################
# Grafico de Bode con Polos y Ceros de sympy #
##############################################
pid = sympy_to_lti(Pid_out_sim)
print ("PID con sympy:")
print (pid)

freq = np.arange(1, 1000000, 1)

w, mag, phase = bode(pid, freq)

fig, (ax1, ax2) = plt.subplots(2,1)
ax1.semilogx (w/(2*np.pi), mag, 'b-', linewidth="1")
ax1.set_title('PID Tf - Magnitude')

ax2.semilogx (w/(2*np.pi), phase, 'r-', linewidth="1")
ax2.set_title('Phase')

plt.tight_layout()
plt.show()

###########################################
# Multiplico Transferencias para OpenLoop #
###########################################
c = lti_to_sympy(pid)
p = lti_to_sympy(planta)

ol = c * p

open_loop = sympy_to_lti(ol)
open_loop = TransferFunction(open_loop.num, open_loop.den)   #normalizo

w, mag, phase = bode(open_loop, freq)

fig, (ax1, ax2) = plt.subplots(2,1)
ax1.semilogx (w/(2*np.pi), mag, 'b-', linewidth="1")
ax1.set_title('Open Loop Tf - Magnitude')

ax2.semilogx (w/(2*np.pi), phase, 'r-', linewidth="1")
ax2.set_title('Phase')

plt.tight_layout()
plt.show()


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

under_roof = 25
########################
# Ecuacion PID Digital #
########################
# Parametros analogicos del PID SOLO VOLTAGE MODE
# kp = 0.01
# ki = 0.75
# kd = 0.0001
kp = kp_analog * under_roof
ki = ki_analog * under_roof
kd = kd_analog * under_roof

ki_dig = ki / Fsampling
kp_dig = kp - ki_dig / 2
kd_dig = kd * Fsampling

k1 = kp_dig + ki_dig + kd_dig
k2 = -kp_dig - 2*kd_dig
k3 = kd_dig

## este es el pid digital
b_pid = [k1, k2, k3]
a_pid = [1, -1]
print ("")
print ("kp_dig: " + str(kp_dig) + " ki_dig: " + str(ki_dig) + " kd_dig: " + str(kd_dig))
print ("")
#este es custom controller
# b_pid = [1.03925, -0.96075, 0]
# a_pid = [1, -1]

pid_dig = TransferFunction(b_pid, a_pid, dt=td)
print ("PID Digital:")
print (pid_dig)


#########################################
# Realimento punto a punto con setpoint #
#########################################

# Respuesta escalon de la planta punto a punto
tiempo_de_simulacion = 0.2
print('td:')
print (td)
t = np.arange(0, tiempo_de_simulacion, td)

# Planta Digital por Tustin
b_planta = np.transpose(planta_dig_tustin_n)
a_planta = np.transpose(planta_dig_tustin_d)

vin_plant = np.zeros(t.size)
vout_plant = np.zeros(t.size)
step_out = np.zeros(t.size)
step_in = np.zeros(t.size)


############################################
# Armo la senial que quiero en el SETPOINT #
############################################
vin_setpoint = np.ones(t.size)

d = np.zeros(t.size)
error = np.zeros(t.size)
max_d_pwm = 0.85
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

    step_in [i] = 1.0
    step_out[i] = b_planta[0]*step_in[i] \
                    + b_planta[1]*step_in[i-1] \
                    + b_planta[2]*step_in[i-2] \
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
ax.plot(t, step_out, 'c')
ax.stem(t, vout_plant)
plt.tight_layout()
plt.show()

