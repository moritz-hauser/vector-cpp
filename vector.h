#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <stdexcept>

// utest3 passed, template test ausgabe korrekt
template<typename T>
class Vector {
    public:
    class ConstIterator;
    class Iterator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    private:
    // instanzvariablen
    static constexpr size_type min_sz = 10;
    size_type max_sz;
    size_type sz;
    pointer values;

    void realloc(size_type new_sz) {
        pointer new_values{new value_type[new_sz]};
        for(size_type i {0}; i<sz; ++i)
            new_values[i]=values[i];
        delete[] values;
        values = new_values;
        max_sz = new_sz;
    }

    size_type grow() {
        return max_sz*2;
    }

    public:
    // Konstruktoren
    Vector(): max_sz{min_sz}, sz{0}, values{new value_type[min_sz]} {}
    Vector(size_type max_sz): max_sz{max_sz<min_sz?min_sz:max_sz}, sz{0}, values{new value_type[this->max_sz]} {}
    Vector(std::initializer_list<value_type> ilist): Vector(ilist.size()) {
        for(const auto& elem : ilist)
            values[sz++]=elem;
    }
    // Kopierkonstruktor
    Vector(const Vector& src): Vector(src.sz) {
        for(; sz<src.sz; ++sz)
            values[sz]=src.values[sz];
    }
    // Destruktor
    ~Vector() {
        delete[] values;
    }

    // operator=
    Vector& operator=(Vector src) {
        std::swap(max_sz,src.max_sz);
        std::swap(sz,src.sz);
        std::swap(values,src.values);
        return *this;
    }

    // push_back
    void push_back(const_reference val) {
        if(sz>=max_sz)
            realloc(grow());
        values[sz++]=val;
    }
    
    // operator []
    reference operator[](size_type index) {
        if(index>=sz)
            throw std::runtime_error{"index out of bounds"};
        return values[index];
    }

    // operator[] für const vektoren
    const_reference operator[] (size_type index) const {
        if(index>=sz)
            throw std::runtime_error{"index out of bounds"};
        return values[index];
    }

    // size()
    size_t size() const {
        return sz;
    }

    // empty()
    bool empty() const {
        return sz==0;
    }

    // clear()
    void clear() {
        sz=0;
        // alten vektor komplett löschen und neuen erstellen? bzw shrink_to_fit()
    }

    // reserve()
    void reserve(size_t n) {
        if(max_sz<n)
            realloc(n);
    }

    // shrink_to_fit()
    void shrink_to_fit() {
        // auf sz reduzieren
        if(sz<min_sz) realloc(min_sz);
        else realloc(sz);
    }

    // pop_back
    void pop_back() {
        if(sz==0) 
            throw std::runtime_error{"vector already empty"};
        else --sz;
    }

    // capacity()
    size_t capacity() const {
        return max_sz;
    }

    // print()
    // [0, 1, 2, 3]
    std::ostream& print(std::ostream& o) const {
        if(empty()) {
            o << "[]";
            return o;
        }
        o << "[";
        for(size_type i {0}; i<(sz-1); ++i) {
            o << values[i] << ", ";
        }
        o << values[sz-1] << "]";
        return o;
    }

    // iterator begin()
    iterator begin() {
        return (empty()?end():iterator(values, values+sz)); //end mitgeben
    }

    const_iterator begin() const {
        return (empty()?end():const_iterator(values, values+sz));
    }

    // iterator end()
    iterator end() {
        return iterator(values+sz, values+sz); 
    }

    const_iterator end() const {
        return const_iterator(values+sz, values+sz);
    }

    // kopierte Methoden
    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
         throw std::runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
    if (sz >= max_sz)
        reserve(max_sz * 2); // Achtung Sonderfall, wenn keine Mindestgroesze definiert ist
    for (auto i{sz}; i-- > current;)
        values[i + 1] = values[i];
    values[current] = val;
    ++sz;
    return iterator{values + current, values+sz}; //korrekt? values+sz???
}

iterator erase(const_iterator pos) {
    auto diff = pos - begin();
    if (diff < 0 || static_cast<size_type>(diff) >= sz)
        throw std::runtime_error("Iterator out of bounds");
    size_type current{static_cast<size_type>(diff)};
    for (auto i{current}; i < sz - 1; ++i)
        values[i] = values[i + 1];
    --sz;
    return iterator{values + current, values+sz}; //korrekt? values+sz???
}

    
    // iteratoren
    class Iterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::reference;
      using pointer = Vector::pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
        // Instanzvariablen
        pointer ptr;
        pointer end = nullptr;
    public:
        // Konstruktor
        Iterator(): ptr{nullptr} {}
        Iterator(pointer ptr, pointer end): ptr{ptr}, end{end} {}
        // Methoden
        reference operator*() {
            if(ptr==end) throw std::runtime_error("end pointer kann nicht dereferenziert werden");
            return *ptr;
        }

        pointer operator->() {
            return ptr;
        }

        bool operator==(const const_iterator& rop) const {
            return ptr==rop.ptr;
        }

        bool operator!=(const const_iterator& rop) const {
            return ptr!=rop.ptr;
        }

        iterator& operator++() {
            //für secure (auch in const):
            if (ptr == end) throw std::runtime_error("iterator ist bereits am ende angelangt");
            //standard:
            ++ptr;
            return *this;
        }

        iterator operator++ (int) {
            //für secure (auch in const):
            if (ptr == end) throw std::runtime_error("iterator ist bereits am ende angelangt");
            //standard:
            Iterator ret(ptr, end);
            ++ptr;
            return ret;
        }

        operator const_iterator() const {
            return const_iterator(ptr, end);
        }

        difference_type operator-(const const_iterator& rop) const {
            return ptr-rop.ptr;
        }
  };
  class ConstIterator {
    public:
      friend class Iterator; // erlaubt??? sonst bei zugriff auf ptr static casten
      using value_type = Vector::value_type;
      using reference = Vector::const_reference;
      using pointer = Vector::const_pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
        // Instanzvariablen
        pointer ptr;
        pointer end = nullptr;
    public:
        // Konstruktor
        ConstIterator(): ptr{nullptr} {}
        ConstIterator(pointer ptr, pointer end): ptr{ptr}, end{end}  {}
        // Methoden
        reference operator*() {
            if(ptr==end) throw std::runtime_error("end pointer kann nicht dereferenziert werden");
            return *ptr;
        }

        pointer operator->() {
            return ptr;
        }

        bool operator==(const const_iterator& rop) const {
            return ptr==rop.ptr;
        }

        bool operator!=(const const_iterator& rop) const {
            return ptr!=rop.ptr;
        }

        const_iterator& operator++() {
            //für secure:
            if (ptr == end) throw std::runtime_error("iterator ist bereits am ende angelangt");
            //standard:
            ++ptr;
            return *this;
        }

        const_iterator operator++ (int) {
            //für secure:
            if (ptr == end) throw std::runtime_error("iterator ist bereits am ende angelangt");
            //standard:
            const_iterator ret(ptr, end);
            ++ptr;
            return ret;
        }

        difference_type operator-(const const_iterator& rop) const {
            return ptr-rop.ptr;
        }
  };
};

// operator<<
template <typename T>
std::ostream& operator<<(std::ostream &o,const Vector<T> v) {
   return v.print(o);
}
#endif