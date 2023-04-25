#include <iostream>
#include <cmath>
#ifdef WIN32
#define GNUPLOT_NAME "C:\\gnuplot\\bin\\gnuplot -persist"
#else
#define GNUPLOT_NAME "gnuplot -persist"
#endif
using namespace std;
class Matrix {
public:
    int rows, columns;
    double **matr;
    Matrix(){};
    Matrix(int rows, int columns) {
        this->rows = rows;
        this->columns = columns;
        matr = new double*[rows];
        for(int i = 0; i < rows; i++){
            matr[i] = new double[columns];
        }
    }
    friend istream& operator >> (istream& in, Matrix& matrix1) {
        int n;
        in >> n;
        matrix1 = Matrix(n, 2);
        for(int i = 0; i < n; i++){
            for(int k = 0; k < 2; k++){
                cin >> matrix1.matr[i][k];
            }
        }
        return in;
    }
    friend ostream& operator << (ostream& out, Matrix& matrix1) {
        for(int i = 0; i < matrix1.rows; i++){
            for(int k = 0; k < matrix1.columns; k++){
                cout.setf(ios::fixed);
                cout.precision(4);
                out << matrix1.matr[i][k] << " ";
            }
            out << endl;
        }
        return out;
    }
    Matrix operator =(const Matrix matrix1) {
        this->rows = matrix1.rows;
        this->columns = matrix1.columns;
        this->matr = new double*[this->rows];
        for(int i = 0; i < this->rows; i++){
            matr[i] = new double[this->columns];
        }
        for(int i = 0; i < this->rows; i++){
            for(int k = 0; k < this->columns; k++){
                this->matr[i][k] = matrix1.matr[i][k];
            }
        }
        return *this;
    }
    Matrix& operator +(const Matrix& matrix1) {
        if(this->rows == matrix1.rows && this->columns == matrix1.columns){
            for(int i = 0; i < this->rows; i++){
                for(int k = 0; k < this->columns; k++){
                    this->matr[i][k] += matrix1.matr[i][k];
                }
            }
            //cout << *this;
        } else {
            cout << "Error: the dimensional problem occurred" << endl;
        }
        return *this;
    }
    Matrix& operator -(const Matrix& matrix1) {
        if(this->rows == matrix1.rows && this->columns == matrix1.columns){
            for(int i = 0; i < this->rows; i++){
                for(int k = 0; k < this->columns; k++){
                    this->matr[i][k] -= matrix1.matr[i][k];
                }
            }
            //cout << *this;
        } else {
            cout << "Error: the dimensional problem occurred" << endl;
        }
        return *this;
    }
    Matrix& operator *(const Matrix& matrix1) {
        if(this->columns == matrix1.rows){
            double **matrix = new double*[this->rows];
            for(int i = 0; i < this->rows; i++){
                matrix[i] = new double[matrix1.columns];
            }
            for(int i =0; i < this->rows; i++){
                for(int j = 0; j < matrix1.columns; j++){
                    matrix[i][j] = 0;
                    for(int k =0; k < this->columns; k++){
                        matrix[i][j] += this->matr[i][k]*matrix1.matr[k][j];
                    }
                }
            }
            this->columns = matrix1.columns;
            matr = new double*[this->rows];
            for(int i = 0; i < this->rows; i++){
                matr[i] = new double[this->columns];
            }
            for(int i =0; i < this->rows; i++){
                for(int j = 0; j < this->columns; j++){
                    matr[i][j] = matrix[i][j];
                }
            }
            //cout << *this;
        } else {
            cout << "Error: the dimensional problem occurred" << endl;
        }
        return *this;
    }

    virtual void transpose(){
        if(this->rows != this->columns){
            int temporary = this->rows;
            this->rows = this->columns;
            this->columns = temporary;
            double **matrix = new double*[this->rows];
            for(int i = 0; i < this->rows; i++){
                matrix[i] = new double[this->columns];
            }
            for(int i = 0; i < this->rows; i++){
                for(int k = 0; k < this->columns; k++){
                    matrix[i][k] = this->matr[k][i];
                }
            }
            this->matr = matrix;
        } else{
            for(int i = 0; i < this->rows; i++){
                for(int k = 0; k < i; k++){
                    double temp = this->matr[i][k];
                    this->matr[i][k] = this->matr[k][i];
                    this->matr[k][i] = temp;
                }
            }
        }

    }
};
Matrix augmentation(Matrix matrix1, Matrix matrix2) {
    Matrix temp = Matrix(matrix1.rows, matrix1.columns + matrix2.columns);
    for(int i = 0; i < matrix1.rows; i++) {
        for(int k = 0; k < matrix1.columns; k++) {
            temp.matr[i][k] = matrix1.matr[i][k];
        }
        for(int k = matrix1.columns; k < matrix1.columns + matrix2.columns; k++) {
            temp.matr[i][k] = matrix2.matr[i][k - matrix1.columns];
        }
    }

    return temp;
}
void normalisation(Matrix matrix1) {
    for(int k = 0; k < matrix1.rows; k++) {
        double norm = matrix1.matr[k][k];
        if(abs(norm) > pow(10,-10)) {
            for(int i = k; i < matrix1.columns; i++){
                matrix1.matr[k][i] = matrix1.matr[k][i]/norm;
                if(abs(matrix1.matr[k][i]) < pow(10,-10)) matrix1.matr[k][i] = 0;
            }
        }
    }
}
Matrix Half(Matrix matrix1) {
    Matrix matrix2 = Matrix(matrix1.rows, matrix1.rows);
    for(int i = 0; i < matrix1.rows; i++){
        for(int k = matrix1.columns/2; k < matrix1.columns; k++){
            matrix2.matr[i][k - matrix1.columns/2] = matrix1.matr[i][k];
        }
    }
    return matrix2;
}
class SquareMatrix : public Matrix {
public:
    SquareMatrix(){}
    SquareMatrix(int rows) : Matrix(rows, rows){}
    friend istream& operator >> (istream& in, SquareMatrix& matrix1) {
        int n;
        in >> n;
        matrix1 = SquareMatrix(n);
        for(int i = 0; i < n; i++){
            for(int k = 0; k < n; k++){
                cin >> matrix1.matr[i][k];
            }
        }
        return in;
    }

    friend SquareMatrix operator+(SquareMatrix& left, SquareMatrix& right) {
        auto temporary = *(Matrix*)&right + *(Matrix*)&left;
        return *(SquareMatrix*)&temporary;
    }
    friend SquareMatrix operator-(SquareMatrix& left, SquareMatrix& right) {
        auto temporary = *(Matrix*)&left - *(Matrix*)&right;
        return *(SquareMatrix*)&temporary;
    }
    friend SquareMatrix operator*(SquareMatrix& left, SquareMatrix& right) {
        auto temporary = *(Matrix*)&left * *(Matrix*)&right;
        return *(SquareMatrix*)&temporary;
    }
    double determinant(){

        double det = 1;
        for(int i = 0; i < this->rows; i++){
            det *= this->matr[i][i];
        }
        return det;
    }
};
class IdentityMatrix : public SquareMatrix {
public:
    IdentityMatrix(){}
    IdentityMatrix(int n) : SquareMatrix(n){
        for(int i = 0; i < n; i++){
            for(int k = 0; k < n; k++) {
                if(i == k) this->matr[i][k] = 1;
                else this->matr[i][k] = 0;
            }
        }
    }
};
class PermutationMatrix : public IdentityMatrix {
public:
    PermutationMatrix(){}
    PermutationMatrix(int dim, int row1, int row2) :IdentityMatrix(dim){
        for(int i = 0; i < dim; i++) {
            int temp = this->matr[row1][i];
            this->matr[row1][i] = this->matr[row2][i];
            this->matr[row2][i] = temp;
        }
    }
};
class EliminationMatrix : public IdentityMatrix {
public:
    EliminationMatrix(){}
    EliminationMatrix(int dim, int rowTo, int rowFrom, double coef) : IdentityMatrix(dim) {
        this->matr[rowTo][rowFrom] = -coef;
    }
};
Matrix inverse(Matrix matrix1){
    Matrix AI = augmentation(matrix1, IdentityMatrix(matrix1.rows));
    int k = 0, index = 0, step = 0;
    //Step 1: forward elimination
    while(k < AI.rows) {
        double max = abs(AI.matr[k][k]);
        index = k;
        bool flag = false;
        //finding maximum pivot element and permutating the rows
        for(int i = k + 1; i < AI.rows; i++) {
            if(abs(AI.matr[i][k]) > max) {
                max = abs(AI.matr[i][k]);
                index = i;
                flag = true;
            }
        }
        //if the maximum element is zero then skip the column
        if(max < pow(10,-10)){
            k++;
            continue;
        }
        //permutating rows
        if(flag){
            PermutationMatrix P = PermutationMatrix(AI.rows, index, k);
            AI = P*AI;
            flag = false;
        }
        //forward elimination
        for(int i = k+1; i < AI.rows; i++) {
            bool check = false;
            if(abs(AI.matr[i][k])>pow(10,-10)) check = true;
            if(check){
                EliminationMatrix E = EliminationMatrix(matrix1.rows, i, k, AI.matr[i][k]/AI.matr[k][k]);
                AI = E*AI;
            }
        }
        k++;
    }
    //Step2: backward elimination
    for(int i = k - 1; i > 0; i--){
        //backwards elimination
        for(int h = i - 1; h >=0; h--){
            bool check = false;
            if(abs(AI.matr[h][i])>pow(10,-10)) check = true;
            if(check) {
                EliminationMatrix E = EliminationMatrix(matrix1.rows, h, i, AI.matr[h][i]/AI.matr[i][i]);
                AI = E*AI;
            }
        }
    }
    normalisation(AI);
    return Half(AI);
}
Matrix leastSquareMatrix(Matrix matrix1, int deg) {
    deg++;
    Matrix matrix2 = Matrix(matrix1.rows, deg);
    for(int i = 0; i < matrix1.rows; i++){
        for(int k = 0; k < deg; k++) {
            matrix2.matr[i][k] = pow(matrix1.matr[i][0], k);
        }
    }
    return matrix2;
}
Matrix Vector(Matrix matrix1, int column){
    Matrix vector = Matrix(matrix1.rows, 1);
    for(int i = 0; i < matrix1.rows; i++){
        vector.matr[i][0] = matrix1.matr[i][column];
    }
    return vector;
}
int main() {
    #ifdef WIN32
        FILE* pipe = _popen(GNUPLOT_NAME, "w");
    #else
        FILE* pipe = popen(GNUPLOT_NAME, "w");
    #endif
    Matrix A = Matrix();
    cin >> A;
    int n;
    cin >> n;

    Matrix M = leastSquareMatrix(A, n);
    cout << "A:\n" << M;
    Matrix MT = leastSquareMatrix(A, n);
    M.transpose();
    M*MT;
    cout << "A_T*A:\n" << M;
    M = inverse(M);
    cout << "(A_T*A)^-1:\n"<< M;
    Matrix b = Vector(A, 1);
    MT.transpose();
    MT*b;
    cout << "A_T*b:\n" << MT;
    M*MT;
    cout << "x~:\n" << M;
    fprintf(pipe,
            "plot [-10 : 10] [-10 : 25] %lf*x**3 + %lf*x**2 + %lf*x**1 + %lf*x**0 , '-' using 1:2 with points\n",
            M.matr[3][0], M.matr[2][0], M.matr[1][0], M.matr[0][0]);
    for (int i = 0; i < A.rows; ++i) {
        fprintf(pipe, "%f\t%f\n", A.matr[i][0], A.matr[i][1]);
    }
    fprintf(pipe, "e\n");
    fflush(pipe);
    #ifdef WIN32
        _pclose(pipe);
    #else
        pclose(pipe);
    #endif
    return 0;
}