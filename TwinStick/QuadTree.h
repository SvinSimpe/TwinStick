#pragma once

#include <list>
#include <vector>
#include <DirectXMath.h>
#include "CollisionComponent.h"

class QuadTree
{
	private:
		static const unsigned int MAX_NODES_PER_QUAD	= 64;
		static const unsigned int MAX_LEVEL				= 8;

		BoxCollisionShape						mBounds;
		unsigned int							mLevel;
		std::vector<std::unique_ptr<QuadTree>>	mChildren;	
		std::list<CollisionShape*>				mNodeList;

		bool	mIsLeaf;

	private:
		void Split();
		
		bool Overlap( const CollisionShape& shape ) const;
		bool Overlap( const std::unique_ptr<CollisionComponent>& comp ) const;

	public:
		QuadTree();
		
		QuadTree( size_t level, BoxCollisionShape bounds );
		
		~QuadTree();
		
		void Clear();

		void Insert( CollisionShape& comp );
		
		void GetDebugVertices( std::vector<XMFLOAT2>& vertices ) const;


		void GetOverlaps( std::vector<CollisionShape*>& overlaps,
						  const CollisionShape& shape ) const;

};