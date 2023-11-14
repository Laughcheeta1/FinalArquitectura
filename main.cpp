/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <limits>
#include <vector>
#include <chrono>
#include <iostream>

AnalogIn lightDetector(A1); // Mientras mas iluminado, es mas cercano a 0

using namespace std;

UnbufferedSerial serial(USBTX, USBRX, 9600);

/*
Vamos a medir el tiempo que se demora en cada uno de las partes de la cinta (negra, blanca)
*/
vector<float> Newton(vector<float> x, vector<float> y);

int main()
{
    cout << "Hello";
  float lastIluminationHigh = lightDetector.read() + 0.1, lastIluminationLow = lightDetector.read() - 0.1;
  int i = 0;
  vector<float> times; // Solo requerimos de 3 tiempos para la ejecucion de Newton

  cout << "Listos para comenzar, presiona (1)";
  // mostrarMensaje("Listos a comenzar, presiona (1)");
  
  Timer t;
  t.start();
  cout << "Lanza la cinta";
  // mostrarMensaje("Lanza la cinta");

  int currentIlumination;
  do
  {
    currentIlumination = lightDetector.read();
  } while (currentIlumination >= lastIluminationLow && currentIlumination <= lastIluminationHigh); //This is to account for the time it takes before the first black line passes
  // Exits the while with the new ilumination (Thats why it exits)


  while (i < 3)
  {
    lastIluminationHigh = currentIlumination + 0.1;
    lastIluminationLow = currentIlumination - 0.1;

    while (currentIlumination >= lastIluminationLow && currentIlumination <= lastIluminationHigh)
      currentIlumination = lightDetector.read();
    
    times.push_back(t.elapsed_time().count());
    i++;
  }

  std::vector<float> positions = {1, 2, 3};

  std::vector<float> resultados = Newton(times, positions);

  cout << "La aceleracion es: ";
  cout << resultados[2]*2;
}

vector<float> Newton(vector<float> x, vector<float> y){

    float const1;
    float auxConst1;
    float const2;

    const1 = (y[1]-y[0])/(x[1]-x[0]);

    auxConst1 = (y[2]-y[1])/(x[2]-x[1]);

    const2 = (auxConst1-const1)/(x[2]-x[0]);

    //Resolviendo operaciones

    //Primer parentesis multiplicado

    float op1 = const1 * (-1*x[0]);

    //Encontrando segundo parentesis 

    float coeParenT= ((-1*x[1])+(-1*x[0]));

    float coeParenSolo = ((-1*x[0])*(-1*x[1]));

    //Operando distributiva

    float distributiva1 = const2 * coeParenT;
    float distributiva2 = const2 * coeParenSolo;

    //Operando semejantes

    float defi1 = y[0] + op1 +distributiva2;
    float defi2 = const1 + distributiva1;

    std::vector<float> ans;

    ans.push_back(defi1); // The one alone
    ans.push_back(defi2); // The one multiplied T
    ans.push_back(const2); // The one multiplied T^(2)

    return ans;
}