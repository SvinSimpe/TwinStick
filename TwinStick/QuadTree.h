#pragma once

#include <list>
#include <vector>
#include <DirectXMath.h>
#include "CollisionComponent.h"




class QuadTree
{
	private:
		BoxCollisionShape						mBounds;
		size_t									mLevel;
		std::vector<std::unique_ptr<QuadTree>>	mChildren;	
		std::vector<CollisionComponent*>		mNodeList;

		bool	mIsLeaf;

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


		void GetOverlaps( std::vector<CollisionComponent*>& overlaps,
						  const CollisionComponent& shape ) const;

		inline const std::vector<CollisionComponent*>& GetNodeList() const // redundant?
		{
			return mNodeList; 
		}

};