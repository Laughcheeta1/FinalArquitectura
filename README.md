# ProyectoFinalArquitectura

# Integrantes
1. Sebastian Ruiz Londoño
2. Santiago Yepes Mesa

# Hardware

Utilizamos una FRDM-KL25Z

# Descripcion del codigo
```
AnalogIn lightDetector(A1);
```
En esta parte del codigo estamos declarando el fotoresistor que vamos a usar.


## Newton
```
vector<double> Newton(vector<double> x, vector<double> y)
```
En esta funcion estamos usando el metodo de newton, el cual sirve para hacer una interpolacion de polinomios, en este caso, de un polinomia cuadrado, debido a la formula de la posicion de fisica mecanica: 
    x = (a/2)*t^(2) + Vi * t + Xi


## Main

```
double lastIluminationHigh = lightDetector.read() + 0.1, lastIluminationLow = lightDetector.read() - 0.1;
  int i = 0;
  vector<double> times;
  ```

En estas lineas de codigo estamos declarando dos variables las cuales son las que vamos a usar para poder detectar los cambios en la iluminacion.

Estas son lastIluimationHigh y lasIluminationLow, estas sirven como un rango de iluminacion en el cual se puede estar en cualquier momento, esto lo hacemos debido a que cambios naturales en la iluminacion del ambiente (por mas pequeños que sean) causarian que se detectara un cambio de iluminacion, y lo tomara cuando nosotros no lo deseabamos. En cambio tomando un rango de iluminacion, nos encargamos que estos pequeños cambios en la iluminacion ambiente no afecten el resultado de nuestro codigo.

el int i = 0, es para llevar cuenta de cuantas lineas de la regla han pasado.

el vector times es para guardar los tiempos que vamos registrando.



```
int currentIlumination;
  do
  {
    currentIlumination = lightDetector.read();
  } while (currentIlumination >= lastIluminationLow && currentIlumination <= lastIluminationHigh);
  ```

Esta linea se encarga de estar en standby en lo que tiramos la regla, es decir, debido a que es imposible saber cuanto tiempo se va a tardar el humano en tirar la regla, tenemos este while, para estar escencialmente esperando.
En el momento que este while se rompe significa que la regla empezo a pasar, por ende entre en juego el siguiente codigo.

```
while (i < 3)
  {
    lastIluminationHigh = currentIlumination + 0.1;
    lastIluminationLow = currentIlumination - 0.1;

    while (currentIlumination >= lastIluminationLow && currentIlumination <= lastIluminationHigh)
      currentIlumination = lightDetector.read();
    
    times.push_back(t.elapsed_time().count());
    i++;
  }
```

Debido a que el metodo de newton solo necesitamos 3 tiempos, entonces solo cogemos estos.
Lo que hacemos aca es que leemos la iluminacion actual, actualizamos el rango, y con el otro while es que eesperamos a que cambie esta ilumnacion, una vez cambie, registramos el tiempo de cambio en el vector.



```
std::vector<float> positions = {1, 2, 3};

  std::vector<float> resultados = Newton(times, positions);

  cout << "La aceleracion es: ";
  cout << resultados[2]*2;
```

Aqui lo que estamos haciendo es que sabemos que las posiciones cuando cambia la iluminacion si o si van a ser en 1cm, en 2cm y en 3cm, por ende los metemos en el vector de posiciones.

Ya teniendo nuestros 3 tiempos y las posiciones, las mandamos a Newton, el cual nos retorna los 3 coeficientes de la ecuacion de posicion.

De esa manera es que mostramos el tercer coeficiente, el cual corresponde a (aceleracion / 2), multiplicamos este coeficiente por 2 y lo mostramos en pantalla, para asi poder mostrar la aceleracion del objeto.