// Fichero: zoom.cpp
// Crea una imagen ampliada de una zona de otra imagen principal
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){
 
  char *origen, *destino; // nombres de los ficheros
  Image image;
  int fila, col, lado;

  // Comprobar validez de la llamada
  if (argc != 6){
    cerr << "Error: Numero incorrecto de parametros.\n";
    cerr << "Uso: zoom <FichImagenOriginal> <FichImagenDestino> <fila> <col> <lado>\n";
    exit (1);
  }

  // Obtener argumentos
  origen  = argv[1];
  destino = argv[2];
  fila = atoi(argv[3]);
  col = atoi(argv[4]);
  lado = atoi(argv[5]);


  // Mostramos argumentos
  cout << endl;
  cout << "Fichero origen: " << origen << endl;
  cout << "Fichero resultado: " << destino << endl;

    // Leer la imagen del fichero de entrada
    if (!image.Load(origen)){
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }


  // Mostrar los parametros de la Imagen
  cout << endl;
  cout << "Dimensiones de " << origen << ":" << endl;
  cout << "   Imagen   = " << image.get_rows()  << " filas x " << image.get_cols() << " columnas " << endl;


  // Calculamos las dimensiones y creamos la imagen resultante del zoom
  /*
    int new_size = 2*(lado)-1;
    Image zoomed_image(new_size,new_size);
  */

  Image cropped_image = image.Crop(fila,col,lado,lado);
  Image zoomed_image = cropped_image.Zoom2X();


    // Guardar la imagen resultado en el fichero
    if (zoomed_image.Save(destino))
      cout  << "La imagen se guardo en " << destino << endl;
    else{
      cerr << "Error: No pudo guardarse la imagen." << endl;
      cerr << "Terminando la ejecucion del programa." << endl;
      return 1;
    }

  return 0;
}



      

