#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// 2D Matrix Class
class Matrix {
private:
    int rows;
    int clms; 
    vector<vector<int>> data;

public:
    Matrix(int r, int c) : rows(r), clms(c) {
        data.assign(r, vector<int>(c, 0));
    }

    Matrix(const vector<vector<int>>& d) {
        rows = d.size();
        clms = d[0].size();
        data = d;
    }

    int getRows() const { return rows; }
    int getClms() const { return clms; }
    vector<vector<int>> getData() const { return data; }

    // Access operator
    int& at(int r, int c) { return data[r][c]; }
    const int& at(int r, int c) const { return data[r][c]; }

    // Addition Check and Error 
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || clms != other.clms)
            throw invalid_argument("Matrix does not match.");

        Matrix result(rows, clms);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < clms; j++)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    // Multiplication (Matrix × Matrix)  Check and error 
    Matrix operator*(const Matrix& other) const {
        if (clms != other.rows)
            throw invalid_argument("Matrix does not match");

        Matrix result(rows, other.clms);

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < other.clms; j++)
                for (int k = 0; k < clms; k++)
                    result.at(i, j) += data[i][k] * other.data[k][j];

        return result;
    }

    // Transpose Check 
    Matrix transpose() const {
        Matrix result(clms, rows);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < clms; j++)
                result.at(j, i) = data[i][j];
        return result;
    }

    // Printing 
    void print() const {
        for (auto& row : data) {
            for (auto& val : row) cout << val << " ";
            cout << endl;
        }
    }
};


//3D TENSOR 
class Tensor3D {
private:
    int dim1, dim2, dim3;
    vector<vector<vector<int>>> data;

public:
    Tensor3D(int d1, int d2, int d3) : dim1(d1), dim2(d2), dim3(d3) {
        data.assign(d1, vector<vector<int>>(d2, vector<int>(d3, 0)));
    }

    Tensor3D(const vector<vector<vector<int>>>& d) {
        dim1 = d.size();
        dim2 = d[0].size();
        dim3 = d[0][0].size();
        data = d;
    }

    int getDim1() const { return dim1; }
    int getDim2() const { return dim2; }
    int getDim3() const { return dim3; }

    // Access
    int& at(int i, int j, int k) { return data[i][j][k]; }
    const int& at(int i, int j, int k) const { return data[i][j][k]; } // const overload
    // Addition 
    Tensor3D operator+(const Tensor3D& other) const {
        if (dim1 != other.dim1 || dim2 != other.dim2 || dim3 != other.dim3)
            throw invalid_argument("Tensor does not match");

        Tensor3D result(dim1, dim2, dim3);
        for (int i = 0; i < dim1; i++)
            for (int j = 0; j < dim2; j++)
                for (int k = 0; k < dim3; k++)
                    result.data[i][j][k] = data[i][j][k] + other.data[i][j][k];
        return result;
    }

    //Elementwise multiplication
    Tensor3D elemwiseMultiply(const Tensor3D& other) const {
        if (dim1 != other.dim1 || dim2 != other.dim2 || dim3 != other.dim3)
        throw invalid_argument("Tensor does not match for elemwiseMultiply");

        Tensor3D result(dim1, dim2, dim3);
        for (int i = 0; i < dim1; i++)
            for (int j = 0; j < dim2; j++)
                for (int k = 0; k < dim3; k++)
                    result.data[i][j][k] = data[i][j][k] * other.data[i][j][k];
        return result;
    }

    // Slice extraction 
    Matrix slice(int d) const {
        if (d < 0 || d >= dim1)
            throw out_of_range("Slice index out of range");
        
        Matrix result(dim2, dim3);
        for (int i = 0; i < dim2; i++){
            for (int j = 0; j < dim3; j++){
                result.at(i,j) = data[d][i][j];
             } 
        }
        return result;
         
        }

    // Reshape
    void reshape(int newD1, int newD2, int newD3) {
        int oldTotal = dim1 * dim2 * dim3;
        int newTotal = newD1 * newD2 * newD3;
        if (oldTotal != newTotal)
            throw invalid_argument("reshape: total number pf elements must stay the same");

     //Flatten current data in order 
     vector<int> flat;
     flat.reserve(oldTotal);
     for (int i = 0; i < dim1; i++)
        for (int j = 0; j < dim2; j++)
            for (int k= 0; k < dim3; k++)
                flat.push_back(data[i][j][k]);
    dim1 = newD1;
    dim2 = newD2;
    dim3 = newD3;

    data.assign(dim1, vector<vector<int>>(dim2, vector<int>(dim3)));
    int idx = 0;
    for (int i = 0; i < dim1; i++)
        for (int j = 0; j < dim2; j++)
            for (int k = 0; k < dim3; k++)
                data[i][j][k] = flat[idx++];
    }

    // Channel-wise multiplication with a matrix
    Tensor3D operator*(const Matrix& other) const {
        if (dim3 != other.getRows())
            throw invalid_argument("Tensor 3D * Matrix: dim3 must equal matrix rows");
        
        int K = other.getClms();
        Tensor3D result(dim1, dim2, K);

        for (int d = 0; d < dim1; d++) {
            for (int i = 0; i < dim2; i++) {
               for (int outCol = 0; outCol < K; outCol++){
                int sum = 0;
                for (int c = 0; c < dim3; c++) {
                    sum += data[d][i][c] * other.at(c, outCol);
                }
                result.at(d, i, outCol) = sum;
             } 
        }                
            
    }
    return result;
}

    // Printing 
    void print() const {
        for (int i = 0; i < dim1; i++) {
            cout << "Slice " << i << ":" << endl;
            for (int j = 0; j < dim2; j++) {
                for (int k = 0; k < dim3; k++)
                    cout << data[i][j][k] << " ";
                cout << endl;
            }
            cout << endl;
        }
    }
};


   
int main() {
    cout << "Matric Tests\n";

    // Example: adding [0,0,0,0] + [1,1,1,1] 
    Matrix A({{0,0,0,0}});
    Matrix B({{1,1,1,1}});
    Matrix C = A + B;

    cout << "A + B = ";
    C.print();   // Should be 1 1 1 1 

    // Test error handling 
    try {
        Matrix X(2,2);
        Matrix Y(3,3);
        Matrix Z = X + Y;  // should throw error
    } catch (const exception& e) {
        cout << "Error correctly caught " << e.what() << endl;
    }

    // Test multiplication
    Matrix M1({{1,2,3},
               {4,5,6}});   // 2×3

    Matrix M2({{1,2},
               {3,4},
               {5,6}});     // 3×2

    cout << "\nM1 * M2 = \n";
    Matrix M3 = M1 * M2;
    M3.print();  // Should be 2×2 

    // Test transpose
    cout << "\nTranspose of M1:\n";
    Matrix T = M1.transpose();
    T.print();

    cout << "\nTENSOR TESTS \n";

    // 2×2×2 tensor addition 
    Tensor3D T1({{{1,2},{3,4}},
                 {{5,6},{7,8}}});

    Tensor3D T2({{{1,1},{1,1}},
                 {{2,2},{2,2}}});

    Tensor3D T3 = T1 + T2;

    cout << "T1 + T2 = \n";
    T3.print();

    return 0;
}