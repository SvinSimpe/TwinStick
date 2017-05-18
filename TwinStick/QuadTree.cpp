#include "QuadTree.h"
using namespace DirectX;

void QuadTree::Split()
{
	float x			= mBounds.mPosition.x;
	float y			= mBounds.mPosition.y;
	float subWidth	= mBounds.mWidth * 0.5f;
	float subHeight	= mBounds.mHeight * 0.5f;

	mChildren[0] = std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x, y ), subWidth, subHeight ) );
	mChildren[1] = std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x + subWidth, y ), subWidth, subHeight ) );
	mChildren[2] = std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x, y - subHeight ), subWidth, subHeight ) );
	mChildren[3] = std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x + subWidth, y - subHeight ), subWidth, subHeight ) );

	mIsLeaf = false;
}

bool QuadTree::Overlap( const CollisionShape& shape ) const
{
	bool overlap = false;

	if( shape.GetCollisionShape() == ECollisionShape::Box )
	{
		BoxCollisionShape& boxShape = BoxCollisionShape( shape.mPosition,
														 shape.GetHalfExtent(),
														 shape.GetHalfExtent() );
		overlap = Intersection::AABBVsAABB( boxShape, mBounds );

	}
	else if( shape.GetCollisionShape() == ECollisionShape::Circle )
	{
		CircleCollisionShape& circleShape = CircleCollisionShape( shape.mPosition,
																  shape.GetHalfExtent() );
		overlap = Intersection::CircleVsAABB( circleShape, mBounds );

	}

	return overlap;
}

bool QuadTree::Overlap( const std::unique_ptr<CollisionComponent>& comp ) const
{
	bool overlap = false;

	if( comp->mCollisionShape->GetCollisionShape() == ECollisionShape::Box )
	{
		BoxCollisionShape& compBox = BoxCollisionShape( comp->mCollisionShape->mPosition,
														comp->mCollisionShape->GetHalfExtent(), comp->mCollisionShape->GetHalfExtent() );
		overlap = Intersection::AABBVsAABB( compBox, mBounds );

	}
	else if( comp->mCollisionShape->GetCollisionShape() == ECollisionShape::Circle )
	{
		CircleCollisionShape& compCircle = CircleCollisionShape( comp->mCollisionShape->mPosition, comp->mCollisionShape->GetHalfExtent() );
		overlap = Intersection::CircleVsAABB( compCircle, mBounds );

	}

	return overlap;
}

QuadTree::QuadTree()
{
	mLevel	= 0;
	mIsLeaf = false;
}

QuadTree::QuadTree( size_t level, BoxCollisionShape bounds )
{
	mBounds = bounds;
	mLevel	= level;
	std::fill( mChildren.begin(), mChildren.end(), nullptr );
	mChildren.reserve( 4 );
	mIsLeaf = true;
}

QuadTree::~QuadTree()
{}

void QuadTree::Clear()
{
	mNodeList.clear();

	for( auto& child : mChildren )
	{
		if( child ) 
		{
			child->Clear();
			child = nullptr;

		}
	}
}

void QuadTree::Insert( CollisionShape& comp )
{
	if( !mIsLeaf )
		mNodeList.push_back( &comp );

	else
	{
		for( auto& child : mChildren )
		{
			if( child->Overlap( comp ) )
				child->Insert( comp );

		}
	}

	if( mNodeList.size() == MAX_NODES_PER_QUAD && mLevel < MAX_LEVEL )
	{
		Split();
		while( !mNodeList.empty() )
		{
			for( auto& child : mChildren )
			{
				if( child->Overlap( *mNodeList.front() ) )
					child->Insert( *mNodeList.front() );

			}

			mNodeList.pop_front();
		}
	}
}

void QuadTree::GetDebugVertices( std::vector<XMFLOAT2>& vertices ) const
{
	if( mIsLeaf )
	{
		XMFLOAT2 upperLeft		= mBounds.mPosition;
		XMFLOAT2 upperRight		= XMFLOAT2( mBounds.mPosition.x + mBounds.mWidth, mBounds.mPosition.y );
		XMFLOAT2 BottomLeft		= XMFLOAT2( mBounds.mPosition.x, mBounds.mPosition.y - mBounds.mHeight );
		XMFLOAT2 BottomRight	= XMFLOAT2( mBounds.mPosition.x + mBounds.mWidth, mBounds.mPosition.y - mBounds.mHeight );

		vertices.push_back( upperLeft );
		vertices.push_back( upperRight );
		vertices.push_back( BottomLeft );
		vertices.push_back( BottomRight );
	}
	else
	{
		for( auto& child : mChildren )
			child->GetDebugVertices( vertices );
	}
}

void QuadTree::GetOverlaps( std::vector<CollisionShape*>& overlaps,
							const CollisionShape& shape ) const
{
	if( mIsLeaf )
	{
		for( auto& node : mNodeList )
		{
			if( node != &shape )
				overlaps.push_back( node );

		}
	}
	else
	{	
		for( auto& child : mChildren )
		{
			if( child->Overlap( shape ) )
				child->GetOverlaps( overlaps, shape );

		}
	}

}