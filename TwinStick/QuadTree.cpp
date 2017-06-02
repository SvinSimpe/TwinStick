#include "QuadTree.h"
#include "QuadTreeConstants.h"
using namespace DirectX;
using namespace QuadTreeConstants;

void QuadTree::Split()
{
	float x			= mBounds.mPosition.x;
	float y			= mBounds.mPosition.y;
	float subWidth	= mBounds.mWidth * 0.5f;
	float subHeight	= mBounds.mHeight * 0.5f;

	mChildren.push_back( std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x, y ), subWidth, subHeight ) ) );
	mChildren.push_back( std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x + subWidth, y ), subWidth, subHeight ) ) );
	mChildren.push_back( std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x, y - subHeight ), subWidth, subHeight ) ) );
	mChildren.push_back( std::make_unique<QuadTree>( mLevel + 1, BoxCollisionShape( XMFLOAT2( x + subWidth, y - subHeight ), subWidth, subHeight ) ) );

	mChildren.reserve( 4 );
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

	mChildren.clear();

	if( mLevel == 1 )
		mIsLeaf = true;
}

void QuadTree::Insert( CollisionComponent& comp )
{
	if( mIsLeaf )
		mNodeList.push_back( &comp );

	else
	{
		for( auto& child : mChildren )
		{
			if( child->Overlap( *comp.mCollisionShape ) )
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
				if( child->Overlap( *mNodeList.back()->mCollisionShape ) )
					child->Insert( *mNodeList.back() );

			}

			mNodeList.pop_back();
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
		vertices.push_back( BottomRight );
		vertices.push_back( BottomLeft );	
		vertices.push_back( upperLeft );

	}
	else
	{
		for( auto& child : mChildren )
			child->GetDebugVertices( vertices );
	}
}

// GetPossibleOverlaps
/*
Returns all the possible overlaps given a component. The component
can reside within multiple quads.
*/
void QuadTree::GetOverlaps( std::vector<CollisionComponent*>& overlaps,
							const CollisionComponent& comp ) const
{
	if( mIsLeaf )
	{
		if( std::find( mNodeList.begin(), mNodeList.end(), &comp ) != mNodeList.end() )
		{ 
			for( auto& node : mNodeList )
			{
				if( node != &comp )
				{
					if( std::find( overlaps.begin(), overlaps.end(), node ) == overlaps.end() )
						overlaps.push_back( node );
				}
			}
		}
	}
	else
	{	
		for( auto& child : mChildren )
		{
			if( child->Overlap( *comp.mCollisionShape ) )
				child->GetOverlaps( overlaps, comp );

		}
	}

}