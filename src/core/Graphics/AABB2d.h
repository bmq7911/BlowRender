#pragma once
#include <vector>

namespace gpc {
	template<typename T>
	class AABB2d {
		struct AABB2d_OR {
			glm::tvec2<T> o;
			glm::tvec2<T> r;
		};
	public:
		AABB2d( T minX, T maxX, T minY, T maxY)
			: m_min( minX, minY )
			, m_max( maxY, maxY )
		{
		}

		static AABB2d makeAABB2d(std::vector<glm::tvec2<T>> const& points) {
			
		}

		T width() const {
			return _Width( std::declval<typename std::conditional<std::is_floating_point<T>::value, int, char>::type>() );
		}

		T height() const {
			return _Height(std::declval< typename std::conditional<std::is_floating_point<T>::value, int, char>::type>());
		}

		T minX() const {
			return m_min.x;
		}

		T maxX() const {
			return m_max.x;
		}

		T minY() const {
			return m_min.y;
		}

		T maxY() const {
			return m_max.y;
		}
	private:
		inline T _Width( char ) {
			return m_max.x - m_min.x + 1;
		}

		inline T _Width( int ) {
			return m_max.x - m_min.x;
		}

		inline T _Height(char) {
			return m_max.y - m_min.y + 1;
		}
		
		inline T _Height(int) {
			return m_max.y - m_min.y;
		}
	private:
		glm::tvec2<T> m_min;
		glm::tvec2<T> m_max;
	};


}