#ifndef __ForwardList__
#define __ForwardList__

#include <iostream>
#include <iterator>

namespace LAZ {
    template<typename T>
    class ForwardList {
    private:
        struct Node {
        public:
            Node() : _next{nullptr}, _value{} {}
            Node(const T& value) : _next{nullptr}, _value{value} {}
        public:
            Node* _next;
            T _value;
        };
    public:
        class Iterator : public std::iterator<std::forward_iterator_tag, T> {
        public:
            Iterator() : _iter{nullptr} {}
            Iterator(Node* iter) : _iter{iter} {}
            Iterator(const Iterator& oth) : _iter{oth._iter} {}
        public:
            Iterator& operator=(const Iterator& rhs) { _iter = rhs._iter; return *this; }
            Iterator& operator=(Node* rhs) { _iter = rhs; return *this; }
            Iterator operator++() { _iter = _iter->_next; return *this; }
            Iterator operator++(int) { auto tmp = *this; _iter = _iter->_next; return tmp; }
            T& operator*() { return _iter->_value; }
            T* operator->() { return &(_iter->_value); }
            bool operator==(const Iterator& rhs) { return (_iter == rhs._iter); }
            bool operator!=(const Iterator& rhs) { return (_iter != rhs._iter); }
            Node* getIter() { return _iter; }
        private:
            Node* _iter;
        };
    public:
        ~ForwardList() { clear(); }
        ForwardList() : _head{nullptr}, _begin{nullptr}, _end{nullptr} {}
        ForwardList(int n);
        ForwardList(int n, const T& value);
        ForwardList(std::initializer_list<T> list);
        ForwardList(const ForwardList<T>& oth);
        ForwardList(ForwardList<T>&& oth);
        template<typename U>
        ForwardList(const ForwardList<U>& oth);
    public:
        ForwardList<T>& operator=(const ForwardList<T>& rhs);
        ForwardList<T>& operator=(ForwardList<T>&& rhs);
        template<typename U>
        ForwardList<T>& operator=(const ForwardList<U>& rhs);
        bool operator==(const ForwardList<T>& rhs);
        bool operator!=(const ForwardList<T>& rhs);
        bool operator<(const ForwardList<T>& rhs);
        bool operator<=(const ForwardList<T>& rhs);
        bool operator>(const ForwardList<T>& rhs);
        bool operator>=(const ForwardList<T>& rhs);
    public:
        bool empty() const { return (_head == nullptr); }
        int size() const;
        void swap(ForwardList<T>& ob2);
        void assign(int n, const T& value);
        void assign(std::initializer_list<T> list);
        template<typename Iter>
        void assign(Iter begin, Iter end);
        void pushFront(const T& value);
        void popFront();
        Iterator insert(Iterator pos, const T& value);
        template<typename Iter>
        Iterator insert(Iterator pos, Iter begin, Iter end);
        Iterator insert(Iterator pos, int n, const T& value);
        Iterator insert(Iterator pos, std::initializer_list<T> list);
        template<typename... U>
        Iterator emplace(Iterator pos, U&&... args) { insert(pos, T(args...)); }
        template<typename... U>
        Iterator emplaceFront(U&&... args) { pushFront(T(args...)); }
        void erase(Iterator pos);
        void remove(const T& value);
        template<typename Operation>
        void removeIf(Operation op);
        void clear();
        void resize(int n);
        void resize(int n, const T& value);
        void reverse();
    public:
        Iterator begin() const { return _begin; }
        Iterator end() const { return _end; }
    private:
        Node* _head;
        Iterator _begin;
        Iterator _end;
    };

    // CONSTRUCTORS
    template<typename T>
    ForwardList<T>::ForwardList(int n) {
        _head = new Node();
        Node* tmp = _head;
        for(int i = 0; i < n - 1; ++i) {
            tmp->_next = new Node();
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }

    template<typename T>
    ForwardList<T>::ForwardList(int n, const T& value) {
        _head = new Node(value);
        Node* tmp = _head;
        for(int i = 0; i < n - 1; ++i) {
            tmp->_next = new Node(value);
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }
    
    template<typename T>
    ForwardList<T>::ForwardList(std::initializer_list<T> list) {
        auto j = list.begin();
        _head = new Node(*j);
        ++j;
        Node* tmp = _head;
        for(int i = 1; i < list.size(); ++i, ++j) {
            tmp->_next = new Node(*j);
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }
    
    template<typename T>
    ForwardList<T>::ForwardList(const ForwardList<T>& oth) {
        Node* tmp = oth._head;
        _head = new Node(tmp->_value);
        Node* tmp1 = _head;
        while(tmp->_next != nullptr) {
            tmp = tmp->_next;
            tmp1->_next = new Node(tmp->_value);
            tmp1 = tmp1->_next;
        }
        _begin = _head;
        _end = tmp1;
    }
    
    template<typename T>
    ForwardList<T>::ForwardList(ForwardList<T>&& oth) {
        _head = oth._head;
        _begin = oth._begin;
        _end = oth._end;
        oth._head = nullptr;
        oth._begin = nullptr;
        oth._end = nullptr;
    }

    template<typename T>
    template<typename U>
    ForwardList<T>::ForwardList(const ForwardList<U>& oth) {
        auto it = oth.begin();
        _head = new Node(*it);
        Node* tmp = _head;
        while(++it != oth.end()) {
            tmp->_next = new Node(*it);
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }

    // OPERATORS
    template<typename T>
    ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& rhs) {
        if(this == &rhs) {
            return *this;
        }
        clear();
        _head = new Node(rhs._head->_value);
        Node* tmp = rhs._head;
        Node* tmp1 = _head;
        while(tmp->_next != nullptr) {
            tmp = tmp->_next;
            tmp1->_next = new Node(tmp->_value);
            tmp1 = tmp1->_next;
        }
        _begin = _head;
        _end = tmp1;
        return *this;
    }

    template<typename T>
    ForwardList<T>& ForwardList<T>::operator=(ForwardList<T>&& rhs) {
        _head = rhs._head;
        _begin = rhs._begin;
        _end = rhs._end;
        rhs._head = nullptr;
        rhs._begin = nullptr;
        rhs._end = nullptr;
        return *this;
    }
    
    template<typename T>
    template<typename U>
    ForwardList<T>& ForwardList<T>::operator=(const ForwardList<U>& rhs) {
        clear();
        auto it = rhs.begin();
        _head = new Node(*it);
        Node* tmp = _head;
        while(++it != rhs.end()) {
            tmp->_next = new Node(*it);
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
        return *this;
    }

    template<typename T>
    bool ForwardList<T>::operator==(const ForwardList<T>& rhs) {
        auto it = _begin;
        auto iter = rhs.begin();
        while(it != _end) {
            if(*it != *iter) {
                return false;
            }
            ++it;
            ++iter;
        }
        return true;
    }

    template<typename T>
    bool ForwardList<T>::operator!=(const ForwardList<T>& rhs) {
        return !(*this == rhs);
    }

    template<typename T>
    bool ForwardList<T>::operator<(const ForwardList<T>& rhs) {
        return std::lexicographical_compare(_begin, _end, rhs._begin, rhs._end);
    }

    template<typename T>
    bool ForwardList<T>::operator<=(const ForwardList<T>& rhs) {
        return !(rhs < *this);
    }

    template<typename T>
    bool ForwardList<T>::operator>(const ForwardList<T>& rhs) {
        return (rhs < *this);
    }

    template<typename T>
    bool ForwardList<T>::operator>=(const ForwardList<T>& rhs) {
        return !(*this < rhs);
    }

    // FUNCTIONS
    template<typename T>
    int ForwardList<T>::size() const {
        if(empty()) {
            return 0;
        }
        int size{};
        auto it = _begin;
        while(it++ != _end) {
            ++size;
        }
        return size;
    }

    template<typename T>
    void ForwardList<T>::swap(ForwardList<T>& ob2) {
        Node* tmp = _head;
        _head = ob2._head;
        ob2._head = tmp;
        Iterator it = _begin;
        _begin = ob2._begin;
        ob2._begin = it;
        it = _end;
        _end = ob2._end;
        ob2._end = it;
    }

    template<typename T>
    void ForwardList<T>::assign(int n, const T& value) {
        clear();
        _head = new Node(value);
        Node* tmp = _head;
        for(int i = 0; i < n - 1; ++i) {
            tmp->_next = new Node(value);
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }

    template<typename T>
    void ForwardList<T>::assign(std::initializer_list<T> list) {
        clear();
        auto j = list.begin();
        _head = new Node(*j);
        ++j;
        Node* tmp = _head;
        for(int i = 1; j != list.end(); ++i, ++j) {
            tmp->_next = new Node(*j);
            tmp = tmp->_next;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }

    template<typename T>
    template<typename Iter>
    void ForwardList<T>::assign(Iter begin, Iter end) {
        clear();
        _head = new Node(*begin);
        Node* tmp = _head;
        ++begin;
        while(begin != end) {
            tmp->_next = new Node(*begin);
            tmp = tmp->_next;
            ++begin;
        }
        _begin = _head;
        tmp->_next = new Node();
        _end = tmp->_next;
    }

    template<typename T>
    void ForwardList<T>::pushFront(const T& value) {
        Node* tmp = new Node(value);
        tmp->_next = _head;
        _head = tmp;
        _begin = _head;
    }

    template<typename T>
    void ForwardList<T>::popFront() {
        if(!empty()) {
            Node* tmp = _head;
            _head = _head->_next;
            _begin = _head;
            tmp->_value = {};
            delete tmp;
        }
    }

    template<typename T>
    typename ForwardList<T>::Iterator ForwardList<T>::insert(Iterator pos, const T& value) {
        Node* tmp = pos.getIter()->_next;
        pos.getIter()->_next = new Node(value);
        pos.getIter()->_next->_next = tmp;
        return Iterator(tmp->_next);
    }

    template<typename T>
    template<typename Iter>
    typename ForwardList<T>::Iterator ForwardList<T>::insert(Iterator pos, Iter begin, Iter end) {
        Node* tmp = new Node(*begin);
        Node* tmp1 = tmp;
        ++begin;
        while(begin != end) {
            tmp->_next = new Node(*begin);
            tmp = tmp->_next;
            ++begin;
        }
        Node* a = pos.getIter()->_next;
        pos.getIter()->_next = tmp1;
        tmp->_next = a;
        return Iterator(tmp1);
    }

    template<typename T>
    typename ForwardList<T>::Iterator ForwardList<T>::insert(Iterator pos, int n, const T& value) {
        Node* tmp = new Node(value);
        Node* tmp1 = tmp;
        --n;
        while(n) {
            tmp->_next = new Node(value);
            tmp = tmp->_next;
            --n;
        }
        Node* a = pos.getIter()->_next;
        pos.getIter()->_next = tmp1;
        tmp->_next = a;
        return Iterator(tmp1);
    }

    template<typename T>
    typename ForwardList<T>::Iterator ForwardList<T>::insert(Iterator pos, 
                                                             std::initializer_list<T> list)
    {
        auto begin = list.begin();
        Node* tmp = new Node(*begin);
        Node* tmp1 = tmp;
        ++begin;
        while(begin != list.end()) {
            tmp->_next = new Node(*begin);
            tmp = tmp->_next;
            ++begin;
        }
        Node* a = pos.getIter()->_next;
        pos.getIter()->_next = tmp1;
        tmp->_next = a;
        return Iterator(tmp1);
    }

    template<typename T>
    void ForwardList<T>::erase(Iterator pos) {
        Node* tmp = pos.getIter()->_next->_next;
        delete pos.getIter()->_next;
        Node* tmp1 = pos.getIter();
        tmp1->_next = tmp;
    }

    template<typename T>
    void ForwardList<T>::remove(const T& value) {
        while(*_begin == value) {
            popFront();
        }
        Iterator tmp = _begin;
        Iterator tmp1;
        while(tmp != _end) {
            if(*tmp == value) {
                ++tmp;
                erase(tmp1);
            } else {
                tmp1 = tmp;
                ++tmp;
            }
        }
    }

    template<typename T>
    template<typename Operation>
    void ForwardList<T>::removeIf(Operation op) {
        while(op(*_begin)) {
            popFront();
        }
        Iterator tmp = _begin;
        Iterator tmp1;
        while(tmp != _end) {
            if(op(*tmp)) {
                ++tmp;
                erase(tmp1);
            } else {
                tmp1 = tmp;
                ++tmp;
            }
        }
    }

    template<typename T>
    void ForwardList<T>::clear() {
        if(_head != nullptr) {
            Node* tmp = _head->_next;
            delete _head;
            _head = nullptr;
            while(tmp->_next != nullptr) {
                _head = tmp;
                tmp = tmp->_next;
                delete _head;
                _head = nullptr;
            }
        }
        _begin = nullptr;
        _end = nullptr;
    }
    
    template<typename T>
    void ForwardList<T>::resize(int n) {
        resize(n, T());
    }

    template<typename T>
    void ForwardList<T>::resize(int n, const T& value) {
        int size1 = size();
        if(size1 == n) {
            return;
        }
        reverse();
        while(size1 > n) {
            popFront();
            ++n;
        }
        while(size1 < n) {
            pushFront(T(value));
            --n;
        }
        reverse();
    }

    template<typename T>
    void ForwardList<T>::reverse() {
        Iterator iter;
        for(Iterator i = _begin; i != _end; ++i) {
            iter = i;
            ++iter;
            for(;iter != _end; ++iter) {
                std::swap(*i, *iter);
            }
        }
    }
};

#endif
