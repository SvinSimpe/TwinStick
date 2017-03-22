#pragma once

#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

struct Vertex32
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;

};

struct CubeMesh
{
	std::vector<Vertex32> vertices;

	CubeMesh()
	{
	/*
		    0-------1
		  / .     / |
		2-------3   |
		|   .   |   |
		|   .   |   |
		|   4...|...5
		| .     | /
		6-------7
		Front  = 236,376
		Back   = 105,054
		Left   = 024,264
		Right  = 317,157
		Top    = 012,132
		Bottom = 547,467
		*/

		float halfWidth		= 0.5f;
		float halfHeight	= 0.5f;
		float halfDepth		= 0.5f;
		XMFLOAT3 origin		= XMFLOAT3( 0.0f, 0.0f, 0.0f );


		// Vertex 0
		Vertex32 v0;
		v0.position.x = origin.x - halfWidth;
		v0.position.y = origin.y + halfHeight;
		v0.position.z = origin.z + halfDepth;

		// Vertex 1
		Vertex32 v1;
		v1.position.x = origin.x + halfWidth;
		v1.position.y = origin.y + halfHeight;
		v1.position.z = origin.z + halfDepth;

		// Vertex 2
		Vertex32 v2;
		v2.position.x = origin.x - halfWidth;
		v2.position.y = origin.y + halfHeight;
		v2.position.z = origin.z - halfDepth;

		// Vertex 3
		Vertex32 v3;
		v3.position.x = origin.x + halfWidth;
		v3.position.y = origin.y + halfHeight;
		v3.position.z = origin.z - halfDepth;

		// Vertex 4
		Vertex32 v4;
		v4.position.x = origin.x - halfWidth;
		v4.position.y = origin.y - halfHeight;
		v4.position.z = origin.z + halfDepth;

		// Vertex 5
		Vertex32 v5;
		v5.position.x = origin.x + halfWidth;
		v5.position.y = origin.y - halfHeight;
		v5.position.z = origin.z + halfDepth;

		// Vertex 6
		Vertex32 v6;
		v6.position.x = origin.x - halfWidth;
		v6.position.y = origin.y - halfHeight;
		v6.position.z = origin.z - halfDepth;

		// Vertex 7
		Vertex32 v7;
		v7.position.x = origin.x + halfWidth;
		v7.position.y = origin.y - halfHeight;
		v7.position.z = origin.z - halfDepth;


		// Front face
		vertices.push_back( v2 );
		vertices.push_back( v3 );
		vertices.push_back( v6 );


		vertices.push_back( v3 );
		vertices.push_back( v7 );
		vertices.push_back( v6 );


		// Back face
		vertices.push_back( v1 );
		vertices.push_back( v0 );
		vertices.push_back( v5 );


		vertices.push_back( v0 );
		vertices.push_back( v4 );
		vertices.push_back( v5 );


		// Left face
		vertices.push_back( v0 );
		vertices.push_back( v2 );
		vertices.push_back( v4 );

		vertices.push_back( v2 );
		vertices.push_back( v6 );
		vertices.push_back( v4 );


		// Right face
		vertices.push_back( v3 );
		vertices.push_back( v1 );
		vertices.push_back( v7 );

		vertices.push_back( v1 );
		vertices.push_back( v5 );
		vertices.push_back( v7 );


		// Top face
		vertices.push_back( v1 );
		vertices.push_back( v3 );
		vertices.push_back( v0 );

		vertices.push_back( v3 );
		vertices.push_back( v2 );
		vertices.push_back( v0 );
		//mVertexFaces[24] = v0;
		//mVertexFaces[25] = v1;
		//mVertexFaces[26] = v2;

		//mVertexFaces[27] = v1;
		//mVertexFaces[28] = v3;
		//mVertexFaces[29] = v2;


		// Bottom face
		vertices.push_back( v5 );
		vertices.push_back( v4 );
		vertices.push_back( v7 );

		vertices.push_back( v4 );
		vertices.push_back( v6 );
		vertices.push_back( v7 );


		// Normals
		//========================================

		// Front face
		vertices[0].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );	
		vertices[1].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );	
		vertices[2].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );	

		vertices[3].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );	
		vertices[4].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );	
		vertices[5].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );	


		// Back face
		vertices[6].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f );	
		vertices[7].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 	
		vertices[8].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 	

		vertices[9].normal  = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
		vertices[10].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 
		vertices[11].normal = XMFLOAT3( 0.0f, 0.0f, 1.0f ); 


		// Left face
		vertices[12].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
		vertices[13].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
		vertices[14].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 

		vertices[15].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
		vertices[16].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 
		vertices[17].normal = XMFLOAT3( -1.0f, 0.0f, 0.0f ); 


		// Right face
		vertices[18].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );	
		vertices[19].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );	
		vertices[20].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );	

		vertices[21].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );	
		vertices[22].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );	
		vertices[23].normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );	


		// Top face
		vertices[24].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );	
		vertices[25].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );	
		vertices[26].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );	

		vertices[27].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );	
		vertices[28].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );	
		vertices[29].normal = XMFLOAT3( 0.0f, 1.0f, 0.0f );	


		// Bottom face
		vertices[30].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f ); 
		vertices[31].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		vertices[32].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );

		vertices[33].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		vertices[34].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		vertices[35].normal = XMFLOAT3( 0.0f, -1.0f, 0.0f );


		// UV-coordinates
		//========================================

		// Front face
		vertices[0].texCoord = XMFLOAT2( 0.0f, 0.0f );	
		vertices[1].texCoord = XMFLOAT2( 0.33f, 0.0f );	
		vertices[2].texCoord = XMFLOAT2( 0.0f, 0.5f );	

		vertices[3].texCoord = XMFLOAT2( 0.33f, 0.0f );		
		vertices[4].texCoord = XMFLOAT2( 0.33f, 0.5f );	
		vertices[5].texCoord = XMFLOAT2( 0.0f, 0.5f );	


		// Back face
		vertices[6].texCoord = XMFLOAT2( 0.33f, 0.0f );	
		vertices[7].texCoord = XMFLOAT2( 0.66f, 0.0f ); 	
		vertices[8].texCoord = XMFLOAT2( 0.33f, 0.5f ); 	

		vertices[9].texCoord  = XMFLOAT2( 0.66f, 0.0f ); 
		vertices[10].texCoord = XMFLOAT2( 0.66f, 0.5f ); 
		vertices[11].texCoord = XMFLOAT2( 0.33f, 0.5f );


		// Left face
		vertices[12].texCoord = XMFLOAT2( 0.66f, 0.0f ); 
		vertices[13].texCoord = XMFLOAT2( 1.0f, 0.0f ); 
		vertices[14].texCoord = XMFLOAT2( 0.66f, 0.5f ); 

		vertices[15].texCoord = XMFLOAT2( 1.0f, 0.0f ); 
		vertices[16].texCoord = XMFLOAT2( 1.0f, 0.5f ); 
		vertices[17].texCoord = XMFLOAT2( 0.66f, 0.5f ); 


		// Right face
		vertices[18].texCoord = XMFLOAT2( 0.0f, 0.5f );	
		vertices[19].texCoord = XMFLOAT2( 0.33f, 0.5f );	
		vertices[20].texCoord = XMFLOAT2( 0.0f, 1.0f );	

		vertices[21].texCoord = XMFLOAT2( 0.33f, 0.5f );	
		vertices[22].texCoord = XMFLOAT2( 0.33f, 1.0f );	
		vertices[23].texCoord = XMFLOAT2( 0.0f, 1.0f );	


		// Top face										 
		vertices[24].texCoord = XMFLOAT2( 0.33f, 0.5f );	
		vertices[25].texCoord = XMFLOAT2( 0.66f, 0.5f );	
		vertices[26].texCoord = XMFLOAT2( 0.33f, 1.0f );	

		vertices[27].texCoord = XMFLOAT2( 0.66f, 0.5f );	
		vertices[28].texCoord = XMFLOAT2( 0.66f, 1.0f );	
		vertices[29].texCoord = XMFLOAT2( 0.33f, 1.0f );


		// Bottom face
		vertices[30].texCoord = XMFLOAT2( 0.66f, 0.5f );
		vertices[31].texCoord = XMFLOAT2( 1.0f, 0.5f );
		vertices[32].texCoord = XMFLOAT2( 0.66f, 1.0f );

		vertices[33].texCoord = XMFLOAT2( 1.0f, 0.5f );
		vertices[34].texCoord = XMFLOAT2( 1.0f, 1.0f );
		vertices[35].texCoord = XMFLOAT2( 0.66f, 1.0f );
	}

};