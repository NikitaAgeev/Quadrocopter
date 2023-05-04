#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <math.h>

#include <matrix.h>

namespace vec_math
{

    size_t matrix::get_w() const 
    {
        return width;
    }
    
    size_t matrix::get_h() const
    {
        return hight;
    }

    double* matrix::get_arr() const
    {
        return arr;
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


    matrix operator- (const matrix& _matrix)
    {
        matrix ans(_matrix);
        
        for(size_t itter = 0; itter < ans.hight*ans.width; itter++)
        {
            ans.arr[itter] = -ans.arr[itter]; 
        }

        return ans;
    }


    const matrix operator* (const matrix& _matrix, const double& a)
    {
        matrix ans = _matrix;

        return ans *= a;
    }

    const matrix operator* (const double& a, const matrix& _matrix)
    {
        matrix ans = _matrix;

        return ans *= a;
    }

    const matrix operator/ (const matrix& _matrix, const double& a)
    {
        matrix ans = _matrix;

        return ans /= a;
    }

    const matrix operator/ (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;

        return ans *= _matrix_b;
    }

    const matrix operator/ (const double& a, const matrix& _matrix)
    {
        matrix ans = inverse(_matrix);

        return ans *= a;
    }

    const matrix operator+ (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;

        return ans += _matrix_b;
    }

    const matrix operator- (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;

        return ans -= _matrix_b;
    }

    const matrix mull (const matrix& _matrix_a, const matrix& _matrix_b)
    {
        matrix ans = _matrix_a;
        
        return ans *= _matrix_b;
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

    double det (const matrix& _matrix)
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

    double minor (const matrix& _matrix, size_t x_m, size_t y_m)
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

    matrix inverse (const matrix& _matrix)
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

    matrix transpose (const matrix& _matrix)
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


    double& matrix::operator() (size_t x, size_t y)
    {
        assert(x < width);
        assert(y < hight);
        
        return *(arr + x + y*width);
    }


    std::ostream& operator<< (std::ostream& os, const matrix& _matrix)
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



    double& vector::x()
    {
        return vec(0, 0);
    }

    double& vector::y()
    {
        return vec(0, 1);
    }

    double& vector::z()
    {
        return vec(0, 2);
    }

    
    const double vector::get_x() const 
    {
        return vec.get_val(0, 0);
    }

    const double vector::get_y() const 
    {
        return vec.get_val(0, 1);
    }

    const double vector::get_z() const
    {
        return vec.get_val(0, 2);
    }


    vector& vector::operator*= (const double& a)
    {
        vec *= a;
        return *this;
    }

    vector& vector::operator+= (const vector& _vector)
    {
        vec += _vector.vec;
        return *this;
    } 

    vector& vector::operator-= (const vector& _vector)
    {
        vec -= _vector.vec;
        return *this;
    }

    vector& vector::operator/= (const double& a)
    {
        vec /= a;
        return *this;
    }

    vector& vector::operator*= (const matrix& _matrix)
    {
        column ans_col = inverse(mull(vec, _matrix));
        
        vec = ans_col;

        return *this;
    }

    
    double operator* (const vector& _vector_a, const vector& _vector_b)
    {
        return (mull(transpose(_vector_a.vec), _vector_b.vec)).get_val(0, 0);
    }

    vector operator* (const vector& _vector, const double& a)
    {
        vector ans = _vector;

        return ans *= a;
    }

    vector operator* (const double& a, const vector& _vector)
    {
        vector ans = _vector;

        return ans *= a;
    }

    vector operator/ (const vector& _vector, double& a)
    {
        vector ans = _vector;

        return ans /= a;
    }

    vector operator+ (const vector& _vector_a, const vector& _vector_b)
    {
        vector ans = _vector_a;

        return ans += _vector_b;
    }

    vector operator- (const vector& _vector_a, const vector& _vector_b)
    {
        vector ans = _vector_a;

        return ans -= _vector_b;
    }

    vector vmull (const vector& _vector_a, const vector& _vector_b)
    {
        vector ans(_vector_a.get_y()*_vector_b.get_z() - _vector_a.get_z()*_vector_b.get_y(),
                   _vector_a.get_z()*_vector_b.get_x() - _vector_a.get_x()*_vector_b.get_z(),
                   _vector_a.get_x()*_vector_b.get_y() - _vector_a.get_y()*_vector_b.get_x());

        return ans;
    }

    vector operator- (const vector& _vector)
    {
        vector ans(-(_vector.get_x()), -(_vector.get_y()), -(_vector.get_z()));

        return ans;
    }

    double mod (const vector& _vector)
    {
        double x = _vector.get_x();
        double y = _vector.get_y();
        double z = _vector.get_z();

        return sqrt(x*x + y*y + z*z);
    }

    double q_mod (const vector& _vector)
    {
        double x = _vector.get_x();
        double y = _vector.get_y();
        double z = _vector.get_z();

        return x*x + y*y + z*z;
    }


    double mull (const vector& _vector_a, const vector& _vector_b)
    {
        return _vector_a*_vector_b;
    }

    vector mull (const vector& _vector, const double& a)
    {
        return _vector*a;
    }

    vector mull (const double& a, const vector& _vector)
    {
        return _vector*a;
    }

    vector mull (const vector& _vector, const matrix& _matrix)
    {
        assert(_matrix.get_w() == 3);
        assert(_matrix.get_h() == 3);
        
        column str_ans(mull(inverse(_vector.vec), _matrix));
        vector ans(str_ans(0, 0), str_ans(1, 0), str_ans(2, 0));

        return ans;
    }

    vector mull (const matrix& _matrix, const vector& _vector)
    {
        column col_ans(mull(_matrix, _vector.vec));
        vector ans(col_ans(0, 0), col_ans(0, 1), col_ans(0, 2));

        return ans;
    }
            


    qaternion conjugation (const qaternion& _qaternion)
    {
        qaternion ans (_qaternion.a, -_qaternion.vec);

        return ans;
    }

    double mod(const qaternion& _qaternion)
    {
        return sqrt(_qaternion.a*_qaternion.a + q_mod(_qaternion.vec));
    }

    double q_mod(const qaternion& _qaternion)
    {
        return _qaternion.a*_qaternion.a + q_mod(_qaternion.vec);
    }


    qaternion& qaternion::operator*= (const qaternion& _qaternion)
    {
        double new_a = a*_qaternion.a - vec*_qaternion.vec;
        vec = a*_qaternion.vec + _qaternion.a*vec + vmull(vec, _qaternion.vec);
        a = new_a;
        
        return *this;
    }

    qaternion& qaternion::operator*= (const double& _a)
    {
        a *= _a;
        vec *= _a;

        return *this; 
    }

    qaternion& qaternion::operator+= (const qaternion& _qaternion)
    {
        a += _qaternion.a;
        vec += _qaternion.vec;

        return *this;
    }

    qaternion& qaternion::operator-= (const qaternion& _qaternion)
    {
        a -= _qaternion.a;
        vec -= _qaternion.vec;
        
        return *this;
    }

    qaternion& qaternion::operator/= (const qaternion& _qaternion)
    {
        qaternion conj_q = conjugation(_qaternion);
        conj_q /= q_mod(_qaternion);
        *this *= conj_q;
        
        return *this;
    }

    qaternion& qaternion::operator/= (const double& _a)
    {
        a /= _a;
        vec /= _a;

        return *this; 
    }

    
    qaternion operator* (const qaternion& _qaternion_a, const qaternion& _qaternion_b)
    {
        qaternion ans(_qaternion_a);
        ans *= _qaternion_b;

        return ans;
    }

    qaternion operator* (const qaternion& _qaternion, const double& _a)
    {
        qaternion ans(_qaternion);
        ans *= _a;

        return ans;
    }

    qaternion operator* (const double& _a, const qaternion& _qaternion)
    {
        qaternion ans(_qaternion);
        ans *= _a;

        return ans;
    }

    qaternion mull (const qaternion& _qaternion_a, const qaternion& _qaternion_b)
    {
        return _qaternion_a * _qaternion_b;
    }

    qaternion mull (const qaternion& _qaternion, const double& _a)
    {
        return _qaternion * _a;
    }

    qaternion mull (const double& _a, const qaternion& _qaternion)
    {
        return _qaternion * _a;
    }

    qaternion operator/ (const qaternion& _qaternion_a, const qaternion& _qaternion_b)
    {
        qaternion ans(_qaternion_a);
        ans /= _qaternion_b;

        return ans;
    }

    qaternion operator/ (const qaternion& _qaternion, const double& _a)
    {
        qaternion ans(_qaternion);
        ans /= _a;

        return ans;
    }

    qaternion operator/ (const double& _a, const qaternion& _qaternion)
    {
        return (_a * conjugation(_qaternion))/q_mod(_qaternion);
    }

    qaternion operator+ (const qaternion& _qaternion_a, const qaternion& _qaternion_b)
    {
        qaternion ans(_qaternion_a);
        ans += _qaternion_b;

        return ans;
    }

    qaternion operator- (const qaternion& _qaternion_a, const qaternion& _qaternion_b)
    {
        qaternion ans(_qaternion_a);
        ans -= _qaternion_b;

        return ans;
    }

};
