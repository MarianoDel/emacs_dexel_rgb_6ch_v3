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

Show_Plant = True
Show_PID = True
Show_Open_Closed = True
Show_Step = True

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


########################################
# Respuesta en Frecuencia de la Planta #
########################################
if Show_Plant == True:
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
#########################
# Controlador analogico #
#########################
PID_controller = False
PI_controller = True

gain_c = 10
fz1 = 500
fz2 = 5000

wz1 = fz1 * 2 * np.pi
wz2 = fz2 * 2 * np.pi
wpc = wz2 * 10

if PID_controller:
    controller = 1/s * (s + wz1) * (s + wz2) * 1 / (s + wpc)

if PI_controller:
    controller = 1/s * (s + wz1) * 1/wz1
    
# if integrator == 1:
#     den_controller = s * (s + wp1) * (s + wp2)
# else:
#     den_controller = (s + wp1) * (s + wp2)
    
gain_controller = gain_c    

Controller_out = gain_controller * controller
Controller_out_sim = Controller_out.simplify()
print ('Controller_out: ')
print (Controller_out_sim)

##############################################
# Grafico de Bode con Polos y Ceros de sympy #
##############################################
controller = sympy_to_lti(Controller_out_sim)
print ("Controller con sympy:")
print (controller)

if Show_PID == True:
    freq = np.arange(1, 1000000, 1)
    w, mag, phase = bode(controller, freq)

    fig, (ax1, ax2) = plt.subplots(2,1)
    ax1.semilogx (w/(2*np.pi), mag, 'b-', linewidth="1")
    ax1.set_title('PID Tf - Magnitude')

    ax2.semilogx (w/(2*np.pi), phase, 'r-', linewidth="1")
    ax2.set_title('Phase')

    plt.tight_layout()
    plt.show()

#######################################################
# Multiplico Transferencias para OpenLoop y CloseLoop #
#######################################################
c = lti_to_sympy(controller)
p = lti_to_sympy(planta)

ol = c * p
cl = ol/(1+ol)

open_loop = sympy_to_lti(ol)
open_loop = TransferFunction(open_loop.num, open_loop.den)   #normalizo ol
close_loop = sympy_to_lti(cl)
close_loop = TransferFunction(close_loop.num, close_loop.den)   #normalizo cl

w, mag_ol, phase_ol = bode(open_loop, freq)
w, mag_cl, phase_cl = bode(close_loop, freq)

if Show_Open_Closed == True:
    fig, (ax1, ax2) = plt.subplots(2,1)
    ax1.semilogx(w/(2*np.pi), mag_ol, 'b')
    ax1.semilogx(w/(2*np.pi), mag_cl, 'y')
    ax1.set_title('Analog OpenLoop Blue, CloseLoop Yellow')
    ax1.set_ylabel('Amplitude P D2 [dB]', color='b')
    ax1.set_xlabel('Frequency [Hz]')
    ax1.set_ylim([-40, 40])

    ax2.semilogx(w/(2*np.pi), phase_ol, 'b')
    ax2.semilogx(w/(2*np.pi), phase_cl, 'y')
    ax2.set_ylabel('Phase', color='r')
    ax2.set_xlabel('Frequency [Hz]')

    plt.tight_layout()
    plt.show()

######################################
# Realimento y veo Respuesta escalon #
######################################
t = np.linspace(0, 0.2, num=2000)
t, y = step2(close_loop, T=t)

if Show_Step == True:
    fig.clear()
    fig, ax = plt.subplots()
    ax.set_title('Respuesta escalon Close Loop')
    ax.set_ylabel('Vout')
    ax.set_xlabel('Tiempo [s]')
    ax.grid()
    ax.plot(t, y)

    plt.tight_layout()
    plt.show()

