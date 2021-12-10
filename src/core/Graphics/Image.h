#pragma once
#include <stdint.h>
#include <algorithm>
#include "alloc.hpp"

namespace gpc {
    template<typename T, typename _Alloc = gpc::alloctor<T> >
    class Image2d {
    public:
        Image2d() = delete;
        Image2d(uint32_t width, uint32_t height)
            : m_width(width)
            , m_height(height)
        {
            _InitImage(width, height, T());
        }
        ~Image2d() {
            m_width = 0;
            m_height = 0;
            m_relWidth = 0;
            if (nullptr != m_data) {
                ///ÊÍ·ÅÊý¾Ý
            }
        }
        Image2d(Image2d&& image) {
            m_width = image.m_width;
            m_height = image.m_height;
            m_relWidth = image.m_relWidth;
            m_data = image.m_data;
            image.m_width = 0;
            image.m_height = 0;
            image.m_relWidth = 0;
            image.m_data = 0;
        }
        Image2d& operator=(Image2d&& image) {
            if (this != &image) {
                m_width = image.m_width;
                m_height = image.m_height;
                m_relWidth = image.m_relWidth;
                m_data = image.m_data;
                image.m_width = 0;
                image.m_height = 0;
                image.m_relWidth = 0;
                image.m_data = 0;

            }
            return *this;
        }

        static Image2d create_image2d(uint32_t width, uint32_t height, T clear = T()) {

            Image2d image;
            image._InitImage(width, height, clear);
            return image;
        }

        inline bool isIn(uint32_t x, uint32_t y) const {
            return (y < m_height&& x < m_width);
        }

        T at(uint32_t x, uint32_t y) const {
            return m_data[y * m_relWidth + x];
        }

        void draw(uint32_t x, uint32_t y, T const& value) {
            m_data[y * m_relWidth + x] = value;
        }

        void clear(T c) {
            for (uint32_t i = 0; i < m_height; ++i) {
                for (uint32_t k = 0; k < m_width; ++k) {
                    m_data[i * m_relWidth + k] = c;
                }
            }
        }

        uint32_t getWidth() const {
            return m_width;
        }

        uint32_t getHeight() const {
            return m_height;
        }

        uint32_t getRelWidth() const {
            return m_relWidth;
        }
        void copyIn(unsigned char* buffer, size_t size) {
            if (nullptr != buffer)
                memcpy(m_data, buffer, std::min(size, size_t(m_relWidth * m_height * sizeof(T))));
        }
    private:
        template<typename U>
        static uint32_t _Allign(uint32_t width) {
            return width;
        }
        void _InitImage(uint32_t width, uint32_t height, T clear) {
            uint32_t relWidth = _Allign<T>(width);
            m_relWidth = relWidth;
            m_data = gpc::alloctor<T>().allocate(relWidth * height);
            this->clear(clear);
        }
    private:
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_relWidth;
        T* m_data;
    };
}

