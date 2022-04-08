#pragma once
namespace gpc {
#define _CENTER_AABB_CONST_FACTOR 1.2599210498948731647672106072782

	template<typename M>
	class KDTree {
	public:
		KDTree( )
			: m_parent(nullptr)
			, m_left(nullptr)
			, m_right(nullptr)
		{
		}
		KDTree(KDTree* parent, AABB const& aabb)
			: m_parent(parent)
			, m_aabb(aabb)
			, m_left(nullptr)
			, m_right(nullptr)
		{
		}
		AABB getAABB() const {
			return m_aabb;
		}
		void setAABB(AABB aabb) {
			m_aabb = aabb;
		}
		KDTree<M>* getParent() const {
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

		KDTree<M>* leftChild() const {
			return m_left;
		}
		KDTree<M>* rightChild() const {
			return m_right;
		}

		//// hit 表
		bool hit(Ray& ray, glm::fvec3 & pos, glm::fvec3& normal) const {
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
		void mountNode(Float gate, M* obj) {
			AABB objAABB = obj->getBVH()->toAABB();
			glm::tvec3 dd = m_aabb.max() - m_aabb.min();
			if (dd.x <= gate && dd.y <= gate && dd.z <= gate) {
				m_node.push_back(obj);
			}
			else {
				if (nullptr != m_left) {
					AABB aabb = m_left->getAABB();
					if (aabb.in(objAABB)) {
						m_left->mountNode(gate, obj);
						return;
					}
				}

				if (nullptr != m_right) {
					AABB aabb = m_right->getAABB();
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

					AABB leftAABB;
					AABB rightAABB;

					Float dx = m_aabb.max.x - m_aabb.min.x;
					Float dy = m_aabb.max.y - m_aabb.min.y;
					Float dz = m_aabb.max.z - m_aabb.min.z;

					if (dx >= dy && dx >= dz) { /// dx 最大
						std::pair<AABB, AABB> pair = m_aabb.cuteAABB(AABB::CuteType::kX, Float(0.5));
						leftAABB = pair.frist;
						rightAABB = pair.second;
					}
					else if (dy >= dz) { /// dy 最大
						std::pair<AABB, AABB> pair = m_aabb.cuteAABB(AABB::CuteType::kY, Float(0.5));
						leftAABB = pair.frist;
						rightAABB = pair.second;
					}
					else { /// dz
						std::pair<AABB, AABB> pair = m_aabb.cuteAABB(AABB::CuteType::kZ, Float(0.5));
						leftAABB = pair.frist;
						rightAABB = pair.second;
					}
					if (nullptr == m_left && leftAABB.in(objAABB)) {
						m_left = new KDTree<M>(this, leftAABB);
						m_left->mountNode(gate, obj);
						return;
					}
					else if (nullptr == m_right && rightAABB.in(objAABB)) {
						m_right = new KDTree<M>(this, rightAABB);
						m_right->mountNode(gate, obj);
						return;
					}
				}
			}
		}
	private:
		AABB     m_aabb;
		KDTree<M>* m_parent;
		KDTree<M>* m_left;
		KDTree<M>* m_right;
		std::vector<M*> m_node; ////
	};
}
