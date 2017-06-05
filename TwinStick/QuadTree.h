#pragma once

#include <vector>
#include "CollisionComponent.h"

class QuadTree
{
	private:
		bool									mIsLeaf;
		size_t									mLevel;
		BoxCollisionShape						mBounds;
		std::vector<std::unique_ptr<QuadTree>>	mChildren;	
		std::vector<CollisionComponent*>		mNodeList;
		

	private:
		void Split();
		bool Overlap( const CollisionShape& shape ) const;

	public:
		QuadTree();
		QuadTree( size_t level, BoxCollisionShape bounds );
		~QuadTree();	
		void Clear();
		void Insert( CollisionComponent& comp );
		void GetDebugVertices( std::vector<XMFLOAT2>& vertices ) const;
		void GetPossibleOverlaps( std::vector<CollisionComponent*>& overlaps,
								  const CollisionComponent& comp ) const;

};