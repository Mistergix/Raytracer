#pragma once
#include <iostream>	  		
#include "Vector3.h"	  				    	  
class Matrix
{
private:
    int m_rows, m_cols;
    float* m_tab;

public:
    //Constructeurs
    Matrix(); //Identity Matrix 4x4
    Matrix(int rows, int cols); // Si 4x4 alors Identity Matrix
    Matrix(int rows, int cols, float val);
    Matrix(const Matrix& m); //Constructeur par copie
    Matrix(float* t);
    //Destructeur
    ~Matrix(); 
    //Opérateur d'affectation par copie
    Matrix& operator=(const Matrix&);
    //Opérateurs d'accès aux éléments
    float& operator[](int val) const;
    float& operator()(int val1, int val2) const;
    // Accès à l'inverse de la Matrix
    Matrix inverse4x4();
    //Opérateurs de multiplications
    Matrix operator * (const Matrix& v) const;
    Matrix operator * (const float& f) const;
    Vector3 operator * (const Vector3& v) const;
    //Accesseurs
    int getCols() const;
    int getRows() const;
    void set(int i, float val);
    void set(int i, int j, float val);
    float get(int i, int j) const;    

};
std::ostream& operator<<(std::ostream& os, const Matrix& m);

