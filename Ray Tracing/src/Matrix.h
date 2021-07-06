#pragma once
class Matrix
{
private:
    int m_rows, m_cols;
    float* m_tab;

public:
    //Constructeurs
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, float val);
    Matrix(const Matrix& m); //Constructeur par copie
    //Destructeur
    ~Matrix(); 
    //Opérateur d'affectation par copie
    Matrix& operator=(const Matrix&);
    //Opérateurs d'accès aux éléments
    float& operator[](int val) const;
    float& operator()(int val1, int val2) const;
    // Accès à l'inverse de la Matrix
    Matrix inverse();
    //TODO? Opérateurs de multiplications avec des Hpoints et HVectors
    //Accesseurs
    int getCols() const;
    int getRows() const;
    void set(int i, int j, float val);
    int get(int i, int j) const;    
};

