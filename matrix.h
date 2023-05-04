#pragma once

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <math.h>

namespace vec_math
{

    class matrix
    {
    protected:
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
            _matrix.arr = nullptr;
        }

        matrix& operator= (const matrix& _matrix)
        {
            if(this == &_matrix)
                return *this;
            
            size_t new_hight = _matrix.hight;
            size_t new_width = _matrix.width;

            if ((hight != new_hight) || (width != new_width))
            {
                double* new_arr = new double [new_width*new_hight];
                std::copy(_matrix.arr, _matrix.arr + new_width*new_hight, new_arr);

                delete[] arr;
                arr = new_arr;

                width = new_width;
                hight = new_hight;
                
                return *this;
            }

            std::copy(_matrix.arr, _matrix.arr + new_width*new_hight, arr);

            return *this;
        }

        matrix& operator= (matrix&& _matrix)
        {
            if(this == &_matrix)
                return *this;

            width = _matrix.width;
            hight = _matrix.hight; 

            if(arr != nullptr)
                delete[] arr;
            
            arr = _matrix.arr;

            _matrix.arr = nullptr;

            return *this;
        }


        matrix& operator*= (const matrix& _matrix);

        matrix& operator*= (const double& a);

        matrix& operator+= (const matrix& _matrix);

        matrix& operator-= (const matrix& _matrix);

        matrix& operator/= (const matrix& _matrix);

        matrix& operator/= (const double& a);

        
        friend matrix operator- (const matrix& _matrix);


        friend const matrix operator* (const matrix& _matrix, const double& a);

        friend const matrix operator* (const double& a, const matrix& _matrix);

        friend const matrix operator/ (const matrix& _matrix, const double& a);

        friend const matrix operator/ (const matrix& _matrix_a, const matrix& _matrix_b);

        friend const matrix operator/ (const double& a, const matrix& _matrix);

        friend const matrix operator+ (const matrix& _matrix_a, const matrix& _matrix_b);

        friend const matrix operator- (const matrix& _matrix_a, const matrix& _matrix_b);

        friend const matrix mull (const matrix& _matrix_a, const matrix& _matrix_b);


        size_t get_w() const;
        
        size_t get_h() const;

        double* get_arr() const;


        static matrix get_E (size_t width, size_t hight);

        friend double det (const matrix& _matrix);

        friend double minor (const matrix& _matrix, size_t x_m, size_t y_m);

        friend matrix inverse (const matrix& _matrix);

        friend matrix transpose (const matrix& _matrix);


        ~matrix()
        {
            delete arr;
        }

        double get_val (size_t x, size_t y) const;

        void put_val (size_t x, size_t y, double num);

        double& operator() (size_t x, size_t y);

        friend std::ostream& operator<< (std::ostream& os, const matrix& _matrix);
    };

    class string: public matrix
    {
        public:
        
        string(size_t len): matrix(len, 1) {};
        
        string(const string& _string) = default;

        string(string&& _string) = default;
        
        string& operator= (const string& _string) = default;

        string& operator= (string& _string) = default;

        ~string() = default;

        string(const matrix& _matrix)
        {
            assert(_matrix.get_h() == 1);

            width = _matrix.get_w();
            hight = 1;
            arr = new double[_matrix.get_w()];

            for(size_t itter = 0; itter < width; itter++)
            {
                arr[itter] = _matrix.get_val(itter, 1);
            }
        }

        string(matrix&& _matrix)
        {
            assert(_matrix.get_h() == 1);

            width = _matrix.get_w();
            hight = 1;
            arr = _matrix.get_arr();
        }

        //static column transpose (const string& _string);
        
    };

    class column: public matrix
    {
        public:
        
        column(size_t len): matrix(1, len) {};
        
        column(const column& _column) = default;

        column(column&& _column) = default;

        column& operator= (const column& _column) = default;

        column& operator= (column&& _column) = default;
        
        ~column() = default;

        column(const matrix& _matrix)
        {
            assert(_matrix.get_w() == 1);

            hight = _matrix.get_h();
            width = 1;
            arr = new double[_matrix.get_h()];

            for(size_t itter = 0; itter < hight; itter++)
            {
                arr[itter] = _matrix.get_val(1, itter);
            }
        }

        column(matrix&& _matrix)
        {
            assert(_matrix.get_w() == 1);

            hight = _matrix.get_h();
            width = 1;
            arr = _matrix.get_arr();
        }

        //static string transpose (const column& _string);

    };

    class vector
    {
        protected:
            column vec;

        public: 
            vector(): vec(3) {};
            vector(double x, double y, double z): vec(3) 
            {
                vec.put_val(0, 0, x);
                vec.put_val(0, 1, y);
                vec.put_val(0, 2, z);
            }
            vector(const vector& _vector): vec(_vector.vec) {};
            vector(vector&& _vector) = default;
            ~vector() = default;

            vector& operator= (const vector& _vector) = default;
            vector& operator= (vector&& _vector) = default;

            double& x ();
            double& y ();
            double& z ();

            const double get_x() const;
            const double get_y() const;
            const double get_z() const;
            
            vector& operator*= (const double& a);

            vector& operator+= (const vector& _vector);

            vector& operator-= (const vector& _vector);

            vector& operator/= (const double& a);

            vector& operator*= (const matrix& _matrix);


            friend double mod (const vector& _vector);

            friend double q_mod (const vector& _vector);

            friend vector operator- (const vector& _vector); 


            friend double operator* (const vector& _vector_a, const vector& _vector_b);

            friend vector operator* (const vector& _vector, const double& a);

            friend vector operator* (const double& a, const vector& _vector);

            friend vector operator/ (const vector& _vector, double& a);

            friend vector operator+ (const vector& _vector_a, const vector& _vector_b);

            friend vector operator- (const vector& _vector_a, const vector& _vector_b);

            friend vector vmull (const vector& _vector_a, const vector& _vector_b);


            friend double mull (const vector& _vector_a, const vector& _vector_b);

            friend vector mull (const vector& _vector, const double& a);

            friend vector mull (const double& a, const vector& _vector);

            friend vector mull (const vector& _vector, const matrix& _matrix);

            friend vector mull (const matrix& _matrix, const vector& _vector);

    };

    class qaternion
    {
        public:
        double a = 0;
        vector vec = {0, 0, 0};

        public:

        qaternion() = default;
        qaternion(const double& _a, const vector& _vec): a(_a), vec(_vec) {};
        qaternion(const double& _a, const double& _x, const double& _y, const double& _z): a(_a), vec(_x, _y, _z) {};
        qaternion(const qaternion& _qaternion) = default;
        qaternion(qaternion&& _qaternion) = default;
        ~qaternion() = default;

        qaternion& operator= (const qaternion& _qaternion) = default;
        qaternion& operator= (qaternion&& _qaternion) = default;


        friend qaternion conjugation (const qaternion& _qaternion);

        friend double mod(const qaternion& _qaternion);

        friend double q_mod(const qaternion& _qaternion);


        qaternion& operator*= (const qaternion& _qaternion);

        qaternion& operator*= (const double& _a);

        qaternion& operator+= (const qaternion& _qaternion);

        qaternion& operator-= (const qaternion& _qaternion);

        qaternion& operator/= (const qaternion& qaternion);

        qaternion& operator/= (const double& _a);

        
        friend qaternion operator* (const qaternion& _qaternion_a, const qaternion& _qaternion_b);

        friend qaternion operator* (const qaternion& _qaternion, const double& _a);

        friend qaternion operator* (const double& _a, const qaternion& _qaternion);

        friend qaternion mull (const qaternion& _qaternion_a, const qaternion& _qaternion_b);

        friend qaternion mull (const qaternion& _qaternion, const double& _a);

        friend qaternion mull (const double& _a, const qaternion& _qaternion);

        friend qaternion operator/ (const qaternion& _qaternion_a, const qaternion& _qaternion_b);

        friend qaternion operator/ (const qaternion& _qaternion, const double& _a);

        friend qaternion operator/ (const double& _a, const qaternion& _qaternion);

        friend qaternion operator+ (const qaternion& _qaternion_a, const qaternion& _qaternion_b);

        friend qaternion operator- (const qaternion& _qaternion_a, const qaternion& _qaternion_b);

    };

};