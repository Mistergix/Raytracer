#include "Matrix.h"
Matrix::Matrix(int rows, int cols) {
    m_cols = cols;
    m_rows = rows;
    m_tab = new float[cols * rows];
    for (int i = 0; i < m_rows * m_cols; i++) {
        m_tab[i] = 1;
    }
}
Matrix::Matrix(int rows, int cols, float val) {
    m_cols = cols;
    m_rows = rows;
    m_tab = new float[cols * rows];
    for (int i = 0; i < m_rows * m_cols; i++) {
        m_tab[i] = val;
    }
}
Matrix::Matrix(const Matrix& m) {
    m_cols = m.getCols();
    m_rows = m.getRows();
    m_tab = new float[m_cols * m_rows];
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            m_tab[i * m_cols + j] = m.get(i, j);
        }
    }
}
Matrix::~Matrix() {
    delete[] m_tab;
}
Matrix& Matrix::operator=(const Matrix& m) {
    if (this == &m)
        return *this;
    m_rows = m.getRows();
    m_cols = m.getRows();
    m_tab = new float[m_cols * m_rows];
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            m_tab[i * m_cols + j] = m.get(i, j);
        }
    }
    m.~Matrix();
    return *this;
}
float& Matrix::operator[](int val)const {
    return m_tab[val];
}
float& Matrix::operator()(int val1, int val2)const {
    return m_tab[val1 * m_cols + val2];
}
int Matrix::getCols() const {
    return m_cols;
}
int Matrix::getRows() const {
    return m_rows;
}
void Matrix::set(int i, int j, float val) {
    m_tab[i * m_cols + j] = val;
}
int Matrix::get(int i, int j) const {
    return m_tab[i * m_cols + j];
}

Matrix inverse() { //TODO SURTOUT APPLIQUER POUR LES 4X4 VU QUE C'EST SURTOUT CA QU'ON VA UTILISER
    Matrix m_copy = Matrix(4,4);

    return m_copy;
}