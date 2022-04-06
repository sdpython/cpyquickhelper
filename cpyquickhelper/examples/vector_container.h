#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <string>
#include <exception>
#include <vector>
#include <functional>
#include <cstdlib>


template <typename T>
class OneTensor {
    public:
        typedef T value_type;
    protected:
        int64_t data_type_;
        std::vector<int64_t> dims_;
        std::vector<T> data_;
    public:
        OneTensor();
        OneTensor(const OneTensor<T>& copy) {
            data_type_ = copy.data_type_;
            dims_ = copy.dims_;
            data_ = copy.data_;
        }
        void set_content(size_t n_dims, std::function<size_t(size_t)>* dim_func, const T* data) {
            dims_.clear();
            dims_.reserve(n_dims);
            int64_t size = 1;
            size_t d;
            for(size_t i = 0; i < n_dims; ++i) {
                d = (*dim_func)(i);
                dims_.push_back(d);
                size *= d;
            }
            data_.resize(size);
            memcpy(data_.data(), data, sizeof(T) * size);
        }
        inline T& operator[](int64_t index) { return data_[index]; }
        inline int64_t element_type() const { return data_type_; }
        inline const std::vector<int64_t>& shape() const { return dims_; }
        inline const T* data() const { return data_.data(); }
        inline int64_t size() const { return data_.size(); }
        void reserve(int64_t size) {
            dims_.clear();
            dims_.push_back(size);
            data_.reserve(size);
        }
        inline void emplace_back(const T& v) { data_.emplace_back(v); }
};


template <>
inline OneTensor<float>::OneTensor(): data_type_(1), dims_(), data_() { }


template <typename VT>
class RandomTensorVector {
    protected:
        std::vector<VT> values_;
    public:
        RandomTensorVector(int64_t n_vectors, int64_t n_dims) {
            values_.resize(n_vectors);
            for (int64_t i = 0; i < n_vectors; ++i) {
                values_[i].reserve(n_dims);
                for (int64_t j = 0; j < n_dims; ++j) {
                    auto h = rand();
                    auto r = ((VT::value_type)h) / ((VT::value_type)RAND_MAX);
                    values_[i].emplace_back(r);
                }
            }
        }
        
        const std::vector<VT>& get() const { return values_; }
};

#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
