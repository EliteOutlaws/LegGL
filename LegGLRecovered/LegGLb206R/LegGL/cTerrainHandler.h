#ifndef __CTERRAINHANDLER_H__
#define __CTERRAINHANDLER_H__
#include "LegEngine.h"
#include "LegVector.h"

struct VertexData	
{
	Vector	pos;	
	byte	col[4];	
	float	s, t;	
	float	u, v;
};

struct Vector2D
{
	DWORD x, y;
};

class Terrain
{
public:
	Terrain(char *pHeightMapName);
	~Terrain();
	
	void SetParams(	Vector &origin, 
					Vector &dimen, 
					float density, 
					float waterHeight);
	void SetTextures(TexHandlerClass *pTexTerrain, TexHandlerClass *pTexDetail,
						TexHandlerClass *pTexSkyBox,
						TexHandlerClass *pTexCaustics);
	void BuildTerrain();

	void Render();


private:

	void PutTerrainVertex(DWORD i, DWORD j);
	void RenderTerrain();
	void RenderSkyBox();
	void RenderWater();


	ImageHandlerClass		 imgHeightMap;			// our height map
	DWORD		 mapXRes, mapYRes;		// height map dimensions

	Vector		 origin, dimen;			// terrain origin in space, and its dimensions
	DWORD		 vertXRes, vertYRes;	// vertex buffer size
	float		 waterHeight;			// height of water (0..1) 

	VertexData	*pVertexData;			// the vertex data for the terrain

	WORD		*pIndexArray;

	Vector		 skyBoxVerts[12];		// vertices for skybox

	// textures
	TexHandlerClass	*pTexTerrain, *pTexDetail;	// ground textures
	TexHandlerClass	*pTexSkyBox;				// sky-box textures
	TexHandlerClass	*pTexCaustics;				// caustics texture
};


#endif 