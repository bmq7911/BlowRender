

template<>
inline _tvec2<float>::_tvec2() {

}
template<>
inline _tvec2<float>::_tvec2(float x_, float y_)
    : x( x_)
    , y( y_)

{
    data[2](0.0f);
    data[3](0.0f);
}
template<>
inline _tvec2<float>::_tvec2(_tvec2<float> const& v) {
    x = v.x;
    y = v.y;
}
template<>
inline _tvec2<float>& _tvec2<float>::operator=( _tvec2<float> const& v) {
    if (this != &v) {
        x = v.x;
        y = v.y;
    }
    return *this;
}

template<>
inline _tvec2<float> _tvec2<float>::operator+(_tvec2<float> const& v) {
    _tvec2<float> t;
    t = _mm_add_ps(data, v.data);
    return t;
}

template<>
inline _tvec2<float> _tvec2<float>::operator += (_tvec2<float> const& v) {
    
}
