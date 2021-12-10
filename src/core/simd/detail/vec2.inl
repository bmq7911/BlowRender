    
    template<typename _T>
    _tvec2<_T>::_tvec2() {
        memset(&data, 0, sizeof(data));
    }

    template<typename _T>
    _tvec2<_T>::_tvec2( _T x_, _T y_)
        : data[0](x_)
        , data[1](y_)
        , data[2](_T())
        , data[3](_T())
    {
    }
    template<typename _T>
    _tvec2<_T>::_tvec2(_tvec2<_T> const& v) {
        data = v.data;
    }

    template<typename _T>
    _tvec2<_T>& _tvec2<_T>::operator=(_tvec2<_T> const& v) {
        if (this != &v) {
            data = v.data;
        }
        return *this;
    }

    template<typename _T>
    _tvec2<_T>  _tvec2<_T>::operator +(_tvec2<_T> const& v) const {
        _tvec2<_T> t;
        t.x = x + v.x;
        t.y = y + v.y;

        return t;
    }
    template<typename _T>
    _tvec2<_T>& _tvec2<_T>::operator +=(_tvec2<_T> const& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    template<typename _T>
    _tvec2<_T> _tvec2<_T>::operator - (_tvec2<_T> const& v) const {
        _tvec2<_T> t;
        t.x = x - v.x;
        t.y = x - v.y;
        return t;
    }
    template<typename _T>
    _tvec2<_T>& _tvec2<_T>::operator -=(_tvec2<_T> const& v) {
        x -= v.x;
        y -= v.y;
        return t;
    }
    template<typename _T>
    _tvec2<_T> _tvec2<_T>::operator *(_T const& k) const {
        _tvec2<_T> t;
        t.x = k * x;
        t.y = k * y;
        return t;
    }
    template<typename _T>
    _tvec2<_T>& _tvec2<_T>::operator *=(_T const& k) {
        x *= k;
        y *= k;
        return *this;
    }

