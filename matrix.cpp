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
        double* arr = NULL;
    public:
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

        matrix& operator*= (const matrix& _matrix)
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

        matrix& set_E ()
        {
            assert(width == hight);
            
            for(size_t x = 0; x < width; x++)
            {
                this->put_val(x, x, 1);
            }

            return *this;
        }

        friend double det (const matrix& _matrix);

        friend double minor (const matrix& _matrix, size_t x_m, size_t y_m);

        friend matrix inverse (const matrix& _matrix);

        ~matrix()
        {
            delete arr;
        }

        double get_val (size_t x, size_t y)
        {
            assert(x < width);
            assert(y < hight);
            return arr[y*width + x]; 
        }

        void put_val (size_t x, size_t y, double num)
        {
            assert(x < width);
            assert(y < hight);
            arr[y*width + x] = num;
        }

        friend std::ostream& operator<< (std::ostream& os, matrix& _matrix);
    };

    double det (const matrix& _matrix)
    {
        assert(_matrix.width == _matrix.hight);
        matrix buffer = _matrix;

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = 0; x < (buffer.width - 1); x++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {
            double diagonal_elem = buffer.get_val(x, x);
            for(size_t y = x + 1; x < buffer.hight; x++) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {
                double multiplier = buffer.get_val(x, y)/diagonal_elem;
                for(size_t x_buf = x; x_buf < buffer.width; x_buf++) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    buffer.put_val(x_buf, y, buffer.get_val(x_buf, y) - multiplier * buffer.get_val(x_buf, x));
                }
            }
        }

        double det = 1;
        for(size_t x = 0; x < buffer.hight; x++)
        {
            det *= buffer.get_val(x, x);
        }

        return det;

    }

    double minor (const matrix& _matrix, size_t x_m, size_t y_m)
    {
        assert(_matrix.width == _matrix.hight);
        matrix buffer = _matrix;

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = 0; x < (buffer.width - 1); x++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {
            if (x == x_m) continue;
            
            double diagonal_elem = buffer.get_val(x, x);
            for(size_t y = x + 1; y < buffer.hight; y++) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {   
                if (y == y_m) continue;
                
                double multiplier = buffer.get_val(x, y)/diagonal_elem;
                for(size_t x_buf = x; x_buf < buffer.width; x_buf++) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    buffer.put_val(x_buf, y, buffer.get_val(x_buf, y) - multiplier * buffer.get_val(x_buf, x));
                }
            }
        }

        double det = 1;
        for(size_t x = 0; x < buffer.hight; x++)
        {
            if (x == x_m) continue;

            det *= buffer.get_val(x, x);
        }

        return det;

    }

    matrix inverse (const matrix& _matrix)
    {
        assert(_matrix.width == _matrix.hight);

        matrix ans(_matrix.width, _matrix.hight);
        ans.set_E();
        matrix buffer = _matrix;

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = 0; x < (buffer.width - 1); x++) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
        {   
            double diagonal_elem = buffer.get_val(x, x);
            for(size_t y = x + 1; y < buffer.hight; y++) // Necessary to start zeroing the column from the element that is below the diagonal. (y = x + 1)
            {   
                double multiplier = buffer.get_val(x, y)/diagonal_elem;
                for(size_t x_buf = x; x_buf < buffer.width; x_buf++) //There is no need to recalculate the part of the row to the right of the zoned column, because it is zoned.
                {
                    buffer.put_val(x_buf, y, buffer.get_val(x_buf, y) - multiplier * buffer.get_val(x_buf, x));
                    ans.put_val(x_buf, y, ans.get_val(x_buf, y) - multiplier * ans.get_val(x_buf, x));
                }
            }
        }

        //This algorithm reduces the matrix to a right_triangle form
        for(size_t x = buffer.width; x > 0; x--) // The last column does not need to be recalculated based on the results of the algorithm (wight - 1)
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

        for(size_t x = 0; x < buffer.width; x++)
        {
            double multiplier = 1/buffer.get_val(x, x);
            for(size_t x_buf = 0; x_buf < buffer.width; x_buf++)
            {
                ans.put_val(x_buf, x, ans.get_val(x_buf, x)*multiplier);
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
    vec_math::matrix m(3, 3);

    std::cout << m;

    m.set_E();

    std::cout << m;

    std::cout << det(m) << "\n";

    m.put_val(1, 1, 10);

    std::cout << m;

    vec_math::matrix b(1, 3);

    b.put_val(0, 1, 13);

    std::cout << b;

    vec_math::matrix a = b;
    m *= b;

    std::cout << m;

    vec_math::string s(3);

    s.put_val(2 , 0, 10);

    std::cout << s;

    double x = det(s);

    return 0;
}