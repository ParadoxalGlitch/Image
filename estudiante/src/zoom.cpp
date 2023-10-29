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

    int new_size = 2*(lado)-1;
    Image zoomed_image(new_size,new_size);


  // Colocamos los valores iniciales en la nueva imagen de dimension 2*n-1 x 2*n-1 

    for(int i = 0; i < new_size; i+=2)
        for(int j = 0; j < new_size; j+=2)
            zoomed_image.set_pixel(i, j, image.get_pixel(fila + (i / 2), col + (j / 2)));

  // Interpolamos sobre las columnas

    for (int i=0; i < new_size; i++)
      for (int j=1; j < new_size; j+=2)
        zoomed_image.set_pixel(i,j,round((zoomed_image.get_pixel(i,j-1) + zoomed_image.get_pixel(i,j+1))/2.0));

  // Interpolamos sobre las filas

    for (int i=1; i < new_size; i+=2)
      for (int j=0; j < new_size; j++)
        zoomed_image.set_pixel(i,j,round((zoomed_image.get_pixel(i-1,j) + zoomed_image.get_pixel(i+1,j))/2.0));



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



      

