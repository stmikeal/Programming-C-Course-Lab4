#pragma once

#include <map>
#include <cstddef>
#include <concepts>
#include <iostream>
#include <stdexcept>
#include <set>
#include <functional>

template <typename _Tp = double>
class SparseVector
{
public:
    typedef _Tp value_type;

    SparseVector() = default;
    SparseVector(size_t n) : vector_size({n, 1}) {};
    SparseVector(size_t n, size_t m) : vector_size({n, m}) {};
    SparseVector(std::pair<size_t, size_t> size) : vector_size(size) {};
    SparseVector(const SparseVector &other) : data(other.data), vector_size(other.vector_size) {};
    SparseVector(SparseVector &&other) : data(std::move(other.data)), vector_size(other.vector_size) {};
    ~SparseVector() = default;

    SparseVector &operator=(const SparseVector &other)
    {
        if (this != &other)
        {
            vector_size = other.vector_size;
            data = other.data;
        }
        return *this;
    }

    SparseVector &operator=(SparseVector &&other)
    {
        if (this != &other)
        {
            vector_size = other.vector_size;
            data = std::move(other.data);
        }
        return *this;
    }

    bool contains(size_t n, size_t m = 1)
    {
        checkBorder(n, m);
        return data.contains({n, m});
    }

    bool contains(std::pair<size_t, size_t> key)
    {
        checkBorder(key.first, key.second);
        return data.contains(key);
    }

    value_type get(size_t n, size_t m = 1)
    {
        checkBorder(n, m);
        return data[{n, m}];
    }

    void put(value_type val, size_t n, size_t m = 1)
    {
        checkBorder(n, m);
        data[{n, m}] = val;
    }

    void resize(size_t n, size_t m = 1)
    {
        vector_size = {n, m};
    }

    std::pair<size_t, size_t> getSize()
    {
        return vector_size;
    }

    void clear()
    {
        data.clear();
    }

    void erase(std::pair<size_t, size_t> key)
    {
        if (data.contains(key))
        {
            data.erase(key);
        }
    }

    SparseVector &T()
    {
        std::set<std::pair<size_t, size_t>> seen;
        std::set<std::pair<size_t, size_t>> eraser;
        vector_size = {vector_size.second, vector_size.first};
        for (auto &[key, value] : data)
        {
            if (seen.count(key))
                continue;
            seen.insert(key);
            seen.insert({key.second, key.first});
            std::pair swapped = {key.second, key.first};
            if (data.contains(swapped))
            {
                auto swap_val = data[swapped];
                data[swapped] = value;
                value = swap_val;
            }
            else
            {
                data[swapped] = value;
                eraser.insert(key);
            }
        }
        for (auto &key : eraser)
        {
            data.erase(key);
        }
        return *this;
    }

    SparseVector &map(std::function<value_type(value_type)> exec)
    {
        for (auto &[key, value] : data)
        {
            value = exec(value);
        }
        return *this;
    }

    value_type &operator[](const std::pair<size_t, size_t> &__k)
    {
        return data[__k];
    }

    SparseVector &operator^(int pow)
    {
        if (vector_size.first != vector_size.second)
            throw std::length_error("Wrong sizes to power");
        if (pow <= 0)
            throw std::invalid_argument("Pow must be greater than 0");
        SparseVector copy(*this);
        for (size_t i = 1; i < pow; ++i)
            copy *(*this);
        return *this;
    }

    SparseVector &operator+(SparseVector &other)
    {
        if (vector_size != other.vector_size)
            throw std::length_error("Wrong sizes to add");
        for (auto &[key, value] : data)
        {
            other[key] = (other.contains(key) ? other[key] : 0) + value;
        }
        return other;
    };

    SparseVector &operator/(value_type del)
    {
        for (auto &[key, value] : data)
        {
            value /= del;
        }
        return *this;
    };

    SparseVector &operator-()
    {
        for (auto &[key, value] : data)
        {
            value = -value;
        }
        return *this;
    }

    SparseVector &operator-(SparseVector &other)
    {
        if (vector_size != other.vector_size)
            throw std::length_error("Wrong sizes to sub");
        other = -other;
        for (auto &[key, value] : data)
        {
            other[key] = (other.contains(key) ? other[key] : 0) + value;
        }
        return other;
    };

    SparseVector &operator*(SparseVector &other)
    {
        if (vector_size.second != other.vector_size.first)
            throw std::length_error("Wrong sizes to multiply");
        std::map<std::pair<size_t, size_t>, value_type> inserter;
        for (auto &[key, value] : data)
        {
            for (size_t i = 0; i < other.vector_size.second; ++i)
            {
                if (other.contains({key.second, i}))
                {
                    inserter[{key.first, i}] = other[{key.second, i}] * value + (inserter.contains({key.first, i}) ? inserter[{key.first, i}] : 0);
                }
            }
        }
        other.clear();
        other.resize(vector_size.first, other.vector_size.second);
        for (auto &[key, value] : inserter)
        {
            other.data[key] = value;
        }
        return other;
    };

    void print()
    {
        std::cout << "Matrix of size " << std::to_string(vector_size.first) << ":" << std::to_string(vector_size.second) << std::endl;
        for (size_t i = 0; i < vector_size.first; ++i)
        {
            for (size_t j = 0; j < vector_size.second; ++j)
            {
                std::cout << std::to_string(data.contains({i, j}) ? data[{i, j}] : 0) << " ";
            }
            std::cout << std::endl;
        }
    }

protected:
    std::pair<size_t, size_t> vector_size;
    std::map<std::pair<size_t, size_t>, value_type> data;

    void checkBorder(size_t n, size_t m)
    {
        if (n >= vector_size.first || m >= vector_size.second)
            throw std::out_of_range("Index is out of border");
    }
};
