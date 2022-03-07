#pragma once
namespace gpc {
#define _CENTER_AABB_CONST_FACTOR 1.2599210498948731647672106072782

	template<typename M, typename T = double>
	class KDTree {
	public:
		KDTree( )
			: m_parent(nullptr)
			, m_left(nullptr)
			, m_right(nullptr)
		{
		}
		KDTree(KDTree* parent, AABB<T> const& aabb)
			: m_parent(parent)
			, m_aabb(aabb)
			, m_left(nullptr)
			, m_right(nullptr)
		{
		}
		AABB<T> getAABB() const {
			return m_aabb;
		}
		void setAABB(AABB<T> aabb) {
			m_aabb = aabb;
		}
		KDTree<M, T>* getParent() const {
			return m_parent;
		}
		void addNode(M* node) {
			m_node.push_back(node);
		}
		size_t sizeNode() const {
			return m_node.size();
		}
		M* atNode(size_t i) const {
			return m_node.at(i);
		}

		KDTree<M, T>* leftChild() const {
			return m_left;
		}
		KDTree<M, T>* rightChild() const {
			return m_right;
		}

		//// hit 表
		bool hit(Ray<T>& ray, glm::tvec3<T>& pos, glm::tvec3<T>& normal) const {
			if (m_aabb.hit(ray)) {
				for (size_t i = 0; i < m_node.size(); ++i) {

				}

				for (size_t i = 0; i < 9; ++i) {
					if (nullptr != m_child[i]) {
						m_child[i]->hit(ray, pos, normal);
					}
				}
			}
		}

		/// 这个递归函数一定要写好
		void mountNode(T gate, M* obj) {
			AABB<T> objAABB = obj->getBVH()->toAABB();
			glm::tvec3<T> dd = m_aabb.max() - m_aabb.min();
			if (dd.x <= gate && dd.y <= gate && dd.z <= gate) {
				m_node.push_back(obj);
			}
			else {
				if (nullptr != m_left) {
					AABB<T> aabb = m_left->getAABB();
					if (aabb.in(objAABB)) {
						m_left->mountNode(gate, obj);
						return;
					}
				}

				if (nullptr != m_right) {
					AABB<T> aabb = m_right->getAABB();
					if (aabb.in(objAABB)) {
						m_right->mountNode(gate, obj);
						return;
					}
				}
				/// 
				if (nullptr != m_left && nullptr != m_right) {
					m_node.push_back(obj);
					return;
				}
				else {// nullptr == m_left || nullptr == m_right 

					AABB<T> leftAABB;
					AABB<T> rightAABB;

					T dx = m_aabb.max.x - m_aabb.min.x;
					T dy = m_aabb.max.y - m_aabb.min.y;
					T dz = m_aabb.max.z - m_aabb.min.z;

					if (dx >= dy && dx >= dz) { /// dx 最大
						std::pair<AABB<T>, AABB<T>> pair = m_aabb.cuteAABB(AABB<T>::CuteType::kX, T(0.5));
						leftAABB = pair.frist;
						rightAABB = pair.second;
					}
					else if (dy >= dz) { /// dy 最大
						std::pair<AABB<T>, AABB<T>> pair = m_aabb.cuteAABB(AABB<T>::CuteType::kY, T(0.5));
						leftAABB = pair.frist;
						rightAABB = pair.second;
					}
					else { /// dz
						std::pair<AABB<T>, AABB<T>> pair = m_aabb.cuteAABB(AABB<T>::CuteType::kZ, T(0.5));
						leftAABB = pair.frist;
						rightAABB = pair.second;
					}
					if (nullptr == m_left && leftAABB.in(objAABB)) {
						m_left = new KDTree<M, T>(this, leftAABB);
						m_left->mountNode(gate, obj);
						return;
					}
					else if (nullptr == m_right && rightAABB.in(objAABB)) {
						m_right = new KDTree<M, T>(this, rightAABB);
						m_right->mountNode(gate, obj);
						return;
					}
				}
			}
		}
	private:
		AABB<T>     m_aabb;
		KDTree<M, T>* m_parent;
		KDTree<M, T>* m_left;
		KDTree<M, T>* m_right;
		std::vector<M*> m_node; ////
	};
}
