/**
 * @file imageop.cpp
 * @brief Fichero con definiciones para el resto de métodos de la clase Image
 */

#include <iostream>
#include <cmath>
#include <image.h>

#include <cassert>

/********************************
       FUNCIONES PÚBLICAS
********************************/

double Image::Mean (int nrow, int ncol, int height, int width) const {

    int suma = 0;

    for (int x = nrow; x < nrow + height; x++) {
        for (int y = ncol; y < ncol + width; y++) {
            suma += get_pixel(x, y);
        }
    }

    return (suma) / (height * width);
}

// Invierte los tonos de grises de una imagen

void Image::Invert(){

  // Calcular el negativo
  for (int i=0; i<size(); i++)
      set_pixel(i,255-get_pixel(i));

}

// Ajusta el contraste de una imagen según los parámetros dados

void Image::AdjustContrast (byte in1, byte in2, byte out1, byte out2){

    double scalling_factor = (out2-out1)/(in2-in1);

    for (size_t i = 0; i<size(); i++){
        byte p = get_pixel(i);
        if (p > in1 && p < in2)
            set_pixel(i, round(out1+scalling_factor*(p-in1)));
    }
}

// Genera un icono como reducción de una imagen.

Image Image::Subsample(int factor) const{

    // Calculamos la dimensión de la nueva imagen

    int new_rows = floor((get_rows())/(factor));
    int new_cols = floor((get_cols())/(factor));

    Image icon(new_rows,new_cols);
    int suma;

    int i=0, j=0;
    while (i<get_rows()-factor+1){
        while(j<get_cols()-factor+1){
            // Calculamos la media del sector
            icon.set_pixel((i/factor),(j/factor),Mean(i,j,factor,factor));
            j+=factor;
        }
        i+=factor;
        j=0;
        
    }

    return icon;

}


// Genera una subimagen.
Image Image::Crop(int nrow, int ncol, int height, int width) const{

    // Creamos una imagen auxiliar donde alojaremos la imagen recortada
    Image aux = Image(height, width, 0);

    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){

            aux.set_pixel(i,j,get_pixel(nrow+i,ncol+j));

        }
    }

    return aux;

}

Image Image::Zoom2X() const{

    // Calculamos el tamaño de la imagen con el zoom realizado

    int new_size = (2*get_rows())-1;
    Image zoomed_image(new_size,new_size);


  // Colocamos los píxeles que no se van a alterar en la foto final
    for(int i = 0; i < new_size; i+=2)
        for(int j = 0; j < new_size; j+=2)
            zoomed_image.set_pixel(i, j, get_pixel((i / 2),(j / 2)));
  // Interpolamos sobre las columnas

    for (int i=0; i < new_size; i++)
      for (int j=1; j < new_size; j+=2)
        zoomed_image.set_pixel(i,j,round((zoomed_image.get_pixel(i,j-1) + zoomed_image.get_pixel(i,j+1))/2.0));

  // Interpolamos sobre las filas

    for (int i=1; i < new_size; i+=2)
      for (int j=0; j < new_size; j++)
        zoomed_image.set_pixel(i,j,round((zoomed_image.get_pixel(i-1,j) + zoomed_image.get_pixel(i+1,j))/2.0));

    return zoomed_image;

}