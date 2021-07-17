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
void Matrix::set(int i, float val) {
    m_tab[i] = val;
}
void Matrix::set(int i, int j, float val) {
    m_tab[i * m_cols + j] = val;
}
float Matrix::get(int i, int j) const {
    return m_tab[i * m_cols + j];
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (int i = 0; i < m.getRows(); i++){
        for (int j = 0; j < m.getCols(); j++) {
            os << m.get(i, j) << " ";
        }
        os << std::endl;
    }
    //os << "test test test";
    return os;
}


Matrix Matrix::inverse4x4() { //A Appliquer uniquement sur les Matrix 4x4

    if (getRows() != 4 && getCols() != 4) {
        throw std::invalid_argument("Ce n'est pas une matrix 4x4");
    }

    float tmp[16], inv[16], det;
    int i;

    tmp[0] = m_tab[5] * m_tab[10] * m_tab[15] -
        m_tab[5] * m_tab[11] * m_tab[14] -
        m_tab[9] * m_tab[6] * m_tab[15] +
        m_tab[9] * m_tab[7] * m_tab[14] +
        m_tab[13] * m_tab[6] * m_tab[11] -
        m_tab[13] * m_tab[7] * m_tab[10];

    tmp[4] = -m_tab[4] * m_tab[10] * m_tab[15] +
        m_tab[4] * m_tab[11] * m_tab[14] +
        m_tab[8] * m_tab[6] * m_tab[15] -
        m_tab[8] * m_tab[7] * m_tab[14] -
        m_tab[12] * m_tab[6] * m_tab[11] +
        m_tab[12] * m_tab[7] * m_tab[10];

    tmp[8] = m_tab[4] * m_tab[9] * m_tab[15] -
        m_tab[4] * m_tab[11] * m_tab[13] -
        m_tab[8] * m_tab[5] * m_tab[15] +
        m_tab[8] * m_tab[7] * m_tab[13] +
        m_tab[12] * m_tab[5] * m_tab[11] -
        m_tab[12] * m_tab[7] * m_tab[9];

    tmp[12] = -m_tab[4] * m_tab[9] * m_tab[14] +
        m_tab[4] * m_tab[10] * m_tab[13] +
        m_tab[8] * m_tab[5] * m_tab[14] -
        m_tab[8] * m_tab[6] * m_tab[13] -
        m_tab[12] * m_tab[5] * m_tab[10] +
        m_tab[12] * m_tab[6] * m_tab[9];

    tmp[1] = -m_tab[1] * m_tab[10] * m_tab[15] +
        m_tab[1] * m_tab[11] * m_tab[14] +
        m_tab[9] * m_tab[2] * m_tab[15] -
        m_tab[9] * m_tab[3] * m_tab[14] -
        m_tab[13] * m_tab[2] * m_tab[11] +
        m_tab[13] * m_tab[3] * m_tab[10];

    tmp[5] = m_tab[0] * m_tab[10] * m_tab[15] -
        m_tab[0] * m_tab[11] * m_tab[14] -
        m_tab[8] * m_tab[2] * m_tab[15] +
        m_tab[8] * m_tab[3] * m_tab[14] +
        m_tab[12] * m_tab[2] * m_tab[11] -
        m_tab[12] * m_tab[3] * m_tab[10];

    tmp[9] = -m_tab[0] * m_tab[9] * m_tab[15] +
        m_tab[0] * m_tab[11] * m_tab[13] +
        m_tab[8] * m_tab[1] * m_tab[15] -
        m_tab[8] * m_tab[3] * m_tab[13] -
        m_tab[12] * m_tab[1] * m_tab[11] +
        m_tab[12] * m_tab[3] * m_tab[9];

    tmp[13] = m_tab[0] * m_tab[9] * m_tab[14] -
        m_tab[0] * m_tab[10] * m_tab[13] -
        m_tab[8] * m_tab[1] * m_tab[14] +
        m_tab[8] * m_tab[2] * m_tab[13] +
        m_tab[12] * m_tab[1] * m_tab[10] -
        m_tab[12] * m_tab[2] * m_tab[9];

    tmp[2] = m_tab[1] * m_tab[6] * m_tab[15] -
        m_tab[1] * m_tab[7] * m_tab[14] -
        m_tab[5] * m_tab[2] * m_tab[15] +
        m_tab[5] * m_tab[3] * m_tab[14] +
        m_tab[13] * m_tab[2] * m_tab[7] -
        m_tab[13] * m_tab[3] * m_tab[6];

    tmp[6] = -m_tab[0] * m_tab[6] * m_tab[15] +
        m_tab[0] * m_tab[7] * m_tab[14] +
        m_tab[4] * m_tab[2] * m_tab[15] -
        m_tab[4] * m_tab[3] * m_tab[14] -
        m_tab[12] * m_tab[2] * m_tab[7] +
        m_tab[12] * m_tab[3] * m_tab[6];

    tmp[10] = m_tab[0] * m_tab[5] * m_tab[15] -
        m_tab[0] * m_tab[7] * m_tab[13] -
        m_tab[4] * m_tab[1] * m_tab[15] +
        m_tab[4] * m_tab[3] * m_tab[13] +
        m_tab[12] * m_tab[1] * m_tab[7] -
        m_tab[12] * m_tab[3] * m_tab[5];

    tmp[14] = -m_tab[0] * m_tab[5] * m_tab[14] +
        m_tab[0] * m_tab[6] * m_tab[13] +
        m_tab[4] * m_tab[1] * m_tab[14] -
        m_tab[4] * m_tab[2] * m_tab[13] -
        m_tab[12] * m_tab[1] * m_tab[6] +
        m_tab[12] * m_tab[2] * m_tab[5];

    tmp[3] = -m_tab[1] * m_tab[6] * m_tab[11] +
        m_tab[1] * m_tab[7] * m_tab[10] +
        m_tab[5] * m_tab[2] * m_tab[11] -
        m_tab[5] * m_tab[3] * m_tab[10] -
        m_tab[9] * m_tab[2] * m_tab[7] +
        m_tab[9] * m_tab[3] * m_tab[6];

    tmp[7] = m_tab[0] * m_tab[6] * m_tab[11] -
        m_tab[0] * m_tab[7] * m_tab[10] -
        m_tab[4] * m_tab[2] * m_tab[11] +
        m_tab[4] * m_tab[3] * m_tab[10] +
        m_tab[8] * m_tab[2] * m_tab[7] -
        m_tab[8] * m_tab[3] * m_tab[6];

    tmp[11] = -m_tab[0] * m_tab[5] * m_tab[11] +
        m_tab[0] * m_tab[7] * m_tab[9] +
        m_tab[4] * m_tab[1] * m_tab[11] -
        m_tab[4] * m_tab[3] * m_tab[9] -
        m_tab[8] * m_tab[1] * m_tab[7] +
        m_tab[8] * m_tab[3] * m_tab[5];

    tmp[15] = m_tab[0] * m_tab[5] * m_tab[10] -
        m_tab[0] * m_tab[6] * m_tab[9] -
        m_tab[4] * m_tab[1] * m_tab[10] +
        m_tab[4] * m_tab[2] * m_tab[9] +
        m_tab[8] * m_tab[1] * m_tab[6] -
        m_tab[8] * m_tab[2] * m_tab[5];

    det = m_tab[0] * tmp[0] + m_tab[1] * tmp[4] + m_tab[2] * tmp[8] + m_tab[3] * tmp[12];

    if (det == 0)
      throw std::invalid_argument("Non Inversible");
    ////TODO return error

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        inv[i] = tmp[i] * det;

    Matrix m_inverse = Matrix(4, 4);

    for (i = 0; i < 16; i++) {
        m_inverse.set(i, inv[i]);
    }

    return m_inverse;
}