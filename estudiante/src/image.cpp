/**
 * @file image.cpp
 * @brief Fichero con definiciones para los métodos primitivos de la clase Image
 *
 */

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>

#include <image.h>
#include <imageIO.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/
void Image::Allocate(int nrows, int ncols, byte * buffer){
    rows = nrows;
    cols = ncols;

    img = new byte * [rows];

    if (buffer != 0)
        img[0] = buffer;
    else
        img[0] = new byte [rows * cols];

    for (int i=1; i < rows; i++)
        img[i] = img[i-1] + cols;
}

// Función auxiliar para inicializar imágenes con valores por defecto o a partir de un buffer de datos
void Image::Initialize (int nrows, int ncols, byte * buffer){
    if ((nrows == 0) || (ncols == 0)){
        rows = cols = 0;
        img = 0;
    }
    else Allocate(nrows, ncols, buffer);
}

// Función auxiliar para copiar objetos Imagen

void Image::Copy(const Image & orig){
    Initialize(orig.rows,orig.cols);
    for (int k=0; k<rows*cols;k++)
        set_pixel(k,orig.get_pixel(k));
}

// Función auxiliar para destruir objetos Imagen
bool Image::Empty() const{
    return (rows == 0) || (cols == 0);
}

void Image::Destroy(){
    if (!Empty()){
        delete [] img[0];
        delete [] img;
    }
}

LoadResult Image::LoadFromPGM(const char * file_path){
    if (ReadImageKind(file_path) != IMG_PGM)
        return LoadResult::NOT_PGM;

    byte * buffer = ReadPGMImage(file_path, rows, cols);
    if (!buffer)
        return LoadResult::READING_ERROR;

    Initialize(rows, cols, buffer);
    return LoadResult::SUCCESS;
}

/********************************
       FUNCIONES PÚBLICAS
********************************/

// Constructor por defecto

Image::Image(){
    Initialize();
}

// Constructores con parámetros
Image::Image (int nrows, int ncols, byte value){
    Initialize(nrows, ncols);
    for (int k=0; k<rows*cols; k++) set_pixel(k,value);
}

bool Image::Load (const char * file_path) {
    Destroy();
    return LoadFromPGM(file_path) == LoadResult::SUCCESS;
}

// Constructor de copias

Image::Image (const Image & orig){
    assert (this != &orig);
    Copy(orig);
}

// Destructor

Image::~Image(){
    Destroy();
}

// Operador de Asignación

Image & Image::operator= (const Image & orig){
    if (this != &orig){
        Destroy();
        Copy(orig);
    }
    return *this;
}

// Métodos de acceso a los campos de la clase

int Image::get_rows() const {
    return rows;
}

int Image::get_cols() const {
    return cols;
}

int Image::size() const{
    return get_rows()*get_cols();
}

// Métodos básicos de edición de imágenes
void Image::set_pixel (int i, int j, byte value) {
    img[i][j] = value;
}
byte Image::get_pixel (int i, int j) const {
    return img[i][j];
}

// This doesn't work if representation changes
void Image::set_pixel (int k, byte value) {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse set_pixel(i,j,value)?
    img[0][k] = value;
}

// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse get_pixel(i,j)?
    return img[0][k];
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    // TODO this makes assumptions about the internal representation
    byte * p = img[0];
    return WritePGMImage(file_path, p, rows, cols);
}


// Calcula la media de los píxeles de una imagen entera o de un fragmento de ésta.
double Image::Mean (int i, int j, int height, int width) const {

    int suma = 0;

    for (int x = i; x < i + height; x++) {
        for (int y = j; y < j + width; y++) {
            suma += get_pixel(x, y);
        }
    }

    return (suma) / (height * width);
}

// Invierte
void Image::Invert(){

// Calcular el negativo
  for (int i=0; i<size(); i++)
      set_pixel(i,255-get_pixel(i));

}

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

/*Image Image::Zoom2X() const{

    //pendiente de preguntar

}*/



