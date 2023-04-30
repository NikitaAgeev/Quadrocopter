#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <assert.h>

namespace vec_math
{

    class matrix
    {
    private:
        size_t width = 0;
        size_t hight = 0;
        double* arr = nullptr;
    public:
        matrix(): width(0), hight(0), arr(nullptr) {};

        matrix(size_t _width, size_t _hight): width(_width), hight(_hight)
        {
            arr = new double [width*hight];
        }

        matrix(const matrix& _matrix): width(_matrix.width), hight(_matrix.hight)
        {
            arr = new double [width*hight];
            std::copy(_matrix.arr, _matrix.arr + width*hight, arr);
        }

        matrix(matrix&& _matrix): width(_matrix.width), hight(_matrix.hight)
        {
            arr = _matrix.arr;
        }


        matrix& operator*= (const matrix& _matrix);

        matrix& operator*= (const double& a);

        matrix& operator+= (const matrix& _matrix);

        matrix& operator-= (const matrix& _matrix);

        matrix& operator/= (const matrix& _matrix);

        matrix& operator/= (const double& a);


        friend matrix operator* (const matrix& _matrix, const double& a);

        friend matrix operator* (const double& a, const matrix& _matrix);

        friend matrix operator/ (const matrix& _matrix, const double& a);

        friend matrix operator/ (const matrix& _matrix_a, const matrix& _matrix_b);

        friend matrix operator/ (const double& a, const matrix& _matrix);

        friend matrix operator+ (const matrix& _matrix_a, const matrix& _matrix_b);

        friend matrix operator- (const matrix& _matrix_a, const matrix& _matrix_b);
 

        size_t get_w() const;
        
        size_t get_h() const;


        static matrix get_E (size_t width, size_t hight);

        static double det (const matrix& _matrix);

        static double minor (const matrix& _matrix, size_t x_m, size_t y_m);

        static matrix inverse (const matrix& _matrix);

        static matrix transpose (const matrix& _matrix);


        ~matrix()
        {
            delete arr;
        }

        double get_val (size_t x, size_t y) const;

        void put_val (size_t x, size_t y, double num);

        friend std::ostream& operator<< (std::ostream& os, matrix& _matrix);
    };


        size_t matrix::get_w() const 
        {
            return width;
        }
        
        size_t matrix::get_h() const
        {
            return hight;
        }


        double matrix::get_val (size_t x, size_t y) const 
        {
            assert(x < width);
            assert(y < hight);
            return arr[y*width + x]; 
        }

        void matrix::put_val (size_t x, size_t y, double num)
        {
            assert(x < width);
            assert(y < hight);
            arr[y*width + x] = num;
        }


    matrix& matrix::operator*= (const matrix& _matrix)
        {
            assert(width == _matrix.hight);
            
            size_t new_width = _matrix.width;
            size_t new_hight = hight;

            double* new_arr = new double[new_width*new_hight];

            for(size_t h = 0; h < new_hight; h++)
            {
                for(size_t w = 0; w < new_width; w++)
                {
                    double mull = 0;
                    for(size_t itter = 0; itter < width; itter++)
                        mull += arr[h*width + itter] * _matrix.arr[itter*_matrix.width + w];
                    
                    new_arr[h*new_width + w] = mull;
                }
            }

            width = new_width;
            hight = new_hight;

            delete arr;
            arr = new_arr;

            return *this;
        }

    matrix& matrix::operator*= (const double& a)
    {
        for(size_t itter = 0; itter < width*hight; itter++) arr[itter] *= a;

        return *this;
    }

    matrix& matrix::operator+= (const matrix& _matrix)
    {
        assert(width == _matrix.width);
        assert(hight == _matrix.hight);
        
        for(size_t itter = 0; itter < width*hight; itter++) arr[itter] += _matrix.arr[itter];

        return *this;
    }

    matrix& matrix::operator-= (const matrix& _matrix)
    {
        assert(width == _matrix.width);
        assert(hight == _matrix.hight);
        
        for(size_t itter = 0; itter < width*hight; itter++) arr[itter] -= _matrix.arr[itter];

        return *this;
    }

    matrix& matrix::operator/= (const matrix& _matrix)
    {
        assert((width == hight) && (_matrix.width == _matrix.hight) && (width == _matrix.width) && (hight == _matrix.hight));
        
        *this *= inverse(_matrix);

        return *this;
    }

    matrix& matrix::operator/= (const double& a)
    {
        for(size_t itter = 0; itter < width*hight; itter++) arr[itter] /= a;

        return *this;
    }


    matrix operator* (const matrix& _matrix, const double& a)
    {
        matrix ans = _matrix;

        return ans *= a;
    }

    matrix operator* (const double& a, const matrix& _matrix)
    {
        matrix ans = _matrix;

        return ans *= a;
    }

    matrix operator/ (const matrix& _matrix, const double& a)
    {
        matrix ans = _matrix;

        return ans /= a;
    }

    matrix operator/ (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;

        return ans *= _matrix_b;
    }

    matrix operator/ (const double& a, const matrix& _matrix)
    {
        matrix ans = matrix::inverse(_matrix);

        return ans *= a;
    }

    matrix operator+ (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;

        return ans += _matrix_b;
    }

    matrix operator- (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;

        return ans -= _matrix_b;
    }


    matrix matrix::get_E (size_t width, size_t hight)
    {
        assert(width == hight);
        
        matrix ans(width, hight);
        
        for(size_t x = 0; x < width; x++)
        {
            ans.put_val(x, x, 1);
        }

        return ans;
    }

    double matrix::det (const matrix& _matrix)
    {
        assert(_matrix.get_w() == _matrix.get_h());
        matrix buffer = _matrix;

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = 0; x < (buffer.get_w() - 1); x++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {
            double diagonal_elem = buffer.get_val(x, x);
            for(size_t y = x + 1; y < buffer.get_h(); y++) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {
                double multiplier = buffer.get_val(x, y)/diagonal_elem;
                for(size_t x_buf = x; x_buf < buffer.get_w(); x_buf++) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    buffer.put_val(x_buf, y, buffer.get_val(x_buf, y) - multiplier * buffer.get_val(x_buf, x));
                }
            }
        }

        double det = 1;
        for(size_t x = 0; x < buffer.get_h(); x++)
        {
            det *= buffer.get_val(x, x);
        }

        return det;

    }

    double matrix::minor (const matrix& _matrix, size_t x_m, size_t y_m)
    {
        assert(_matrix.get_w() == _matrix.get_h());
        matrix buffer = _matrix;

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x_diag = 0, y_diag = 0; (x_diag < (buffer.get_w() - 1)) && (y_diag < (buffer.get_h() -1)); x_diag++, y_diag++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {
            if (x_diag == x_m) x_diag++;
            if (y_diag == y_m) y_diag++;
            
            if ((x_diag > (buffer.get_w() - 1)) || (y_diag > (buffer.get_h() - 1))) break;

            double diagonal_elem = buffer.get_val(x_diag, y_diag);
            for(size_t y = y_diag + 1; y < buffer.get_h(); y++) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {   
                if (y == y_m) continue;
                
                double multiplier = buffer.get_val(x_diag, y)/diagonal_elem;
                for(size_t x = x_diag; x < buffer.get_w(); x++) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    if (x == x_m) continue;
                    
                    buffer.put_val(x, y, buffer.get_val(x, y) - multiplier * buffer.get_val(x, y_diag));
                }
            }
        }

        double det = 1;
        for(size_t x_diag = 0, y_diag = 0; (x_diag < (buffer.get_w() - 1)) && (y_diag < (buffer.get_h() -1)); x_diag++, y_diag++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {
            if (x_diag == x_m) x_diag++;
            if (y_diag == y_m) y_diag++;
            
            if ((x_diag > (buffer.get_w() - 1)) || (y_diag > (buffer.get_h() - 1))) break;

            det *= buffer.get_val(x_diag, y_diag);
        }

        return det;

    }

    matrix matrix::inverse (const matrix& _matrix)
    {
        assert(_matrix.get_w() == _matrix.get_h());

        matrix ans = matrix::get_E(_matrix.get_w(), _matrix.get_h());
        matrix buffer = _matrix;

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = 0; x < (buffer.get_w() - 1); x++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {   
            double diagonal_elem = buffer.get_val(x, x);
            for(size_t y = x + 1; y < buffer.get_h(); y++) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {   
                double multiplier = buffer.get_val(x, y)/diagonal_elem;
                for(size_t x_buf = x; x_buf < buffer.get_w(); x_buf++) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    buffer.put_val(x_buf, y, buffer.get_val(x_buf, y) - multiplier * buffer.get_val(x_buf, x));
                    ans.put_val(x_buf, y, ans.get_val(x_buf, y) - multiplier * ans.get_val(x_buf, x));
                }
            }
        }

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = buffer.get_w() - 1; x > 0; x--) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {   
            double diagonal_elem = buffer.get_val(x, x);
            for(size_t y = x - 1; y != -1; y--) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {   
                double multiplier = buffer.get_val(x, y)/diagonal_elem;
                for(size_t x_buf = x; x_buf != -1; x_buf--) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    buffer.put_val(x_buf, y, buffer.get_val(x_buf, y) - multiplier * buffer.get_val(x_buf, x));
                    ans.put_val(x_buf, y, ans.get_val(x_buf, y) - multiplier * ans.get_val(x_buf, x));
                }
            }
        }

        for(size_t x = 0; x < buffer.get_w(); x++)
        {
            double multiplier = 1/buffer.get_val(x, x);
            for(size_t x_buf = 0; x_buf < buffer.get_w(); x_buf++)
            {
                ans.put_val(x_buf, x, ans.get_val(x_buf, x)*multiplier);
            }
        }

        return ans;

    }

    matrix matrix::transpose (const matrix& _matrix)
    {
        matrix ans(_matrix.get_h(), _matrix.get_w());

        for(size_t x = 0; x < _matrix.get_w(); x++)
        {
            for(size_t y = 0; y < _matrix.get_h(); y++)
            {
                ans.put_val(y, x, _matrix.get_val(x, y));
            }
        }

        return ans;
    }


    class string: public matrix
    {
        public:
        
        string(size_t len): matrix(len, 1) {};
        
        string(const string& _string) = default;
        string(string&& _string) = default;
        ~string() = default;
        
    };

    class column: public matrix
    {
        public:
        
        column(size_t len): matrix(len, 1) {};
        
        column(const column& _column) = default;
        column(column&& _column) = default;
        ~column() = default;
        
    };

    std::ostream& operator<< (std::ostream& os, matrix& _matrix)
    {
        os << "\n";
        //os << std::fixed;
        for(size_t y = 0; y < _matrix.hight; y++)
        {
            os << "|";
            for(size_t x = 0; x < _matrix.width; x++)
            {
                os << " " << std::fixed << std::setw(10) << _matrix.arr[y*_matrix.width + x];
            } 
            os << " |\n";
        }
        os << std::endl;

        return os;
    }

};




int main()
{
    vec_math::matrix m = vec_math::matrix::get_E(3,3);

    m.put_val(1, 0, 10);

    std::cout << m << std::endl;

    vec_math::matrix ne_m = vec_math::matrix::transpose(m);

    std::cout << ne_m << std::endl;

    vec_math::column c(3);

    vec_math::string s(3);

    return 0;
}