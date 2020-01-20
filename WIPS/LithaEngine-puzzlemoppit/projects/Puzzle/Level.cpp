
#include "Level.h"
#include "Map.h"
#include "Events.h"
//#include "MonsterCharacterController.h"
#include "MainState.h"
#include "CameraCollider.h"
#include "EndLevelScreen.h"
#include "GridBasedCharacterController.h"
#include "RotateToAnimator.h"

#include <algorithm>

std::map<core::stringc, f32> lowestPointCache;

// really defines the width of the bounding sphere used for collisions
#define ACTOR_PHYSICAL_HEIGHT 0.5

// Distance fall below level before restart
#define FALL_DIST 10.0

extern ISound *bgAmbientSound;
extern ISound *bgMusic;
extern bool globalIsInEditor;
extern core::stringc globalLogLastLevel;

inline u32 time_in_ms(f32 timeSeconds)
{
	return (u32)(timeSeconds * 1000.0);
}

class PlayerInteractionPreparator : public IInteractionPreparator
{
public:
	void Prepare(CollisionMaterialInteraction &interaction, const Collision &collision)
	{
		// Collisions with walls have zero friction.
		if (fabs(collision.normal.Y) < 0.1)
			interaction.friction = 0.f;
	}
};

void Level::ExpandBB(core::vector3di coord)
{
	core::aabbox3df box( GetPosFromCoord(coord) );
	box.MinEdge -= core::vector3df(0.5);
	box.MaxEdge += core::vector3df(0.5);
	
	if (isBoundingBoxEmpty)
	{
		boundingBox = box; // otherwise BB will use a default value for init
		
		lowestPoint = GetPosFromCoord(coord).Y;
		
		isBoundingBoxEmpty = false;
	}
	else
	{
		boundingBox.addInternalBox( box );
		
		if (GetPosFromCoord(coord).Y < lowestPoint)
			lowestPoint = GetPosFromCoord(coord).Y;
	}
}

core::vector3di Level::GetCoord(core::vector3df pos)
{
	if (pos.X < -0.5)
		pos.X -= 0.5;
	else
		pos.X += 0.5;
	
	if (pos.Y < -0.5)
		pos.Y -= 0.5;
	else
		pos.Y += 0.5;
	
	if (pos.Z < -0.5)
		pos.Z -= 0.5;
	else
		pos.Z += 0.5;
	
	return core::vector3di( (s32)pos.X, (s32)pos.Y, (s32)pos.Z );
}

core::vector3df Level::GetPosFromCoord(core::vector3di mapCoord)
{
	return core::vector3df(mapCoord.X,mapCoord.Y,mapCoord.Z);
}

bool Level::IsActorTouchingCoord(Actor &actor, core::vector3di mapCoord, f32 scale)
{
	core::vector3df mapPos = GetPosFromCoord(mapCoord);
	
	core::vector3df actorPos = actor.entity->GetPosition();
	
	// Construct ray from Actor position towards centre of the map coordinate
	// BUT, only of Actor radius in length.
	
	//core::vector3df vec = mapPos - actorPos;
	core::vector3df vec = mapPos - GetPosFromCoord( GetActorCoord(actor) );
	vec.normalize();
	vec *= ACTOR_PHYSICAL_HEIGHT/2.0; // Actor radius, half a unit.
	
	vec *= scale;
	
	core::line3df ray(actorPos, actorPos + vec);
	
	// Create map coord box and intersect with ray
	
	core::aabbox3df box(mapPos - core::vector3df(0.5,0.5,0.5), mapPos + core::vector3df(0.5,0.5,0.5));
	
	return box.intersectsWithLine(ray);
}

std::vector<core::vector3di> Level::GetActorIntersectingCoords(Actor &actor)
{
	// Determine all squares that a Actor currently occupies.
	// (as it needs to take control of them so e.g. no falling block may fall into them)
	// [x][x][x]
	// [x][P][x]
	// [x][x][x]
	
	core::vector3di actorMapPos = GetCoord( actor.entity->GetPosition() );
	
	// Find surrounding "shell" of map coordinates
	// Then draw a line from centre of Actor towards the centre of each of these coordinates (as if each is a block).
	// Line is length: player radius.
	// Intersect that line with the coordinate box.
	
	std::vector<core::vector3di> actorMapCoords;
	
	// EXCLUDING actor centre location.
	//actorMapCoords.push_back(actorMapPos); // add actor current coord
	
	for (int x = -1; x <= 1; x ++)
	{
		//for (int y = -1; y <= 1; y ++)
		//{
			for (int z = -1; z <= 1; z ++)
			{
				if ( !(x == 0 && z == 0) ) // excluding actor coord
				{
					core::vector3di mapCoord = actorMapPos + core::vector3di(x,0,z);
					
					if (IsActorTouchingCoord(actor, mapCoord))
						actorMapCoords.push_back(mapCoord);
				}
			}
		//}
	}
	
	return actorMapCoords;
}

void Level::PutDebugCube(core::vector3di mapCoord)
{
	engine->GetIrrlichtDevice()->getSceneManager()->addCubeSceneNode(1.0, NULL, -1,
		GetPosFromCoord(mapCoord)
		);
}

LowLevelShaderRegisterMap get_vertex_shader_register_map()
{
	LowLevelShaderRegisterMap rmap;
	rmap.PushFourRegisters("TransWorldMatrix");
	rmap.PushSingleRegister("timeSeconds", NULL,NULL,NULL);
	return rmap;
}

LowLevelShaderRegisterMap get_pixel_shader_register_map()
{
	LowLevelShaderRegisterMap rmap;
	//rmap.PushSingleRegister("UseDiffuseTexture", "diffuseTexture", "cloudShadowTexture", NULL);
	return rmap;
}

// Shader callback class, used in ApplyDefaultShaders below.
class DefaultShaderCallback : public IShaderCallback
{
	IEngine *engine;
	video::IVideoDriver *driver;
	
public:
	DefaultShaderCallback(IEngine *engine)
	{
		this->engine = engine;
		this->driver = engine->GetIrrlichtDevice()->getVideoDriver();
	}
	
	void ShaderOnSetConstants(IShader *shader)
	{
		core::matrix4 matWorld = driver->getTransform(video::ETS_WORLD);
		matWorld = matWorld.getTransposed();
		shader->SetVertexConstant("TransWorldMatrix", matWorld);
		shader->SetVertexConstant("timeSeconds", engine->GetRenderUpdater().GetVirtualTime());
	}
};

void Level::ApplyDefaultShaders(IMesh *mesh, video::E_MATERIAL_TYPE baseMaterial)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("Hemisphere.vert", "Hemisphere.frag", baseMaterial);
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			// Cloud shadow texture in second layer.
			material.setTexture(1, cloudShadowTexture);
			
			//shader->SetPixelConstant("UseDiffuseTexture", material.TextureLayer[0].Texture ? 1 : 0);
			//shader->SetPixelConstant("diffuseTexture",		0);
			//shader->SetPixelConstant("cloudShadowTexture",	1);
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
	}
}

void Level::ApplyDefaultShadersIrr(scene::IMesh *mesh, video::E_MATERIAL_TYPE baseMaterial)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
	{
		video::SMaterial &material = mesh->getMeshBuffer(i)->getMaterial();
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("Hemisphere.vert", "Hemisphere.frag", baseMaterial);
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			// Cloud shadow texture in second layer.
			material.setTexture(1, cloudShadowTexture);
			
			//shader->SetPixelConstant("UseDiffuseTexture", material.TextureLayer[0].Texture ? 1 : 0);
			//shader->SetPixelConstant("diffuseTexture",		0);
			//shader->SetPixelConstant("cloudShadowTexture",	1);
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			shader->ApplyToIrrMaterial(material);
			combinedLevelMeshShaders.push_back(shader);
			
			// shader dropped in destructor
			// (when the combined mesh is cleared)
		}
	}
}


// hacked in for final cutscene landscape
// same as ApplyDefaultShaders with different hemisphere shader
// (which has clouds larger and moving slower)
void Level::ApplyLandShaders(IMesh *mesh, video::E_MATERIAL_TYPE baseMaterial)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("HemisphereLand.vert", "Hemisphere.frag", baseMaterial);
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			// Cloud shadow texture in second layer.
			material.setTexture(1, cloudShadowTexture);
			
			//shader->SetPixelConstant("UseDiffuseTexture", material.TextureLayer[0].Texture ? 1 : 0);
			//shader->SetPixelConstant("diffuseTexture",		0);
			//shader->SetPixelConstant("cloudShadowTexture",	1);
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
	}
}
/*
// Exactly as land above but with EMT_TRANSPARENT_VERTEX_ALPHA and a different frag shader.
// This shader uses colour values as alpha, since Blender will not let you paint vertex alphas...
void Level::ApplyCliffGrassShaders(IMesh *mesh)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("HemisphereLand.vert", "HemisphereCliffGrass.frag",
					video::EMT_TRANSPARENT_VERTEX_ALPHA);
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			// Cloud shadow texture in second layer.
			material.setTexture(1, cloudShadowTexture);
			
			//shader->SetPixelConstant("UseDiffuseTexture", material.TextureLayer[0].Texture ? 1 : 0);
			//shader->SetPixelConstant("diffuseTexture",		0);
			//shader->SetPixelConstant("cloudShadowTexture",	1);
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
	}
}
*/
void Level::ApplyWoodShaders(IMesh *mesh)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("Hemisphere.vert", "HemisphereWood.frag");
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			//shader->SetPixelConstant("UseDiffuseTexture", material.TextureLayer[0].Texture ? 1 : 0);
			//shader->SetPixelConstant("diffuseTexture", 0);
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
	}
}

void Level::ApplyIceShaders(IMesh *mesh)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("Hemisphere.vert", "HemisphereIce.frag");
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			//shader->SetPixelConstant("UseDiffuseTexture", material.TextureLayer[0].Texture ? 1 : 0);
			//shader->SetPixelConstant("diffuseTexture", 0);
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
	}
}

void Level::ApplyBalloonShaders(IMesh *mesh)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("HemisphereBalloon.vert", "HemisphereBalloon.frag",
					video::EMT_TRANSPARENT_VERTEX_ALPHA);
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			// Callback to set the world matrix.
			DefaultShaderCallback *callback = new DefaultShaderCallback(engine);
			shader->SetCallback( callback );
			callback->drop();
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
	}
}

void Level::ApplyPlainAlphaShaders(IMesh *mesh, u8 alpha)
{
	// Set up mesh materials (with shaders)
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		material.Lighting = false;
		material.AntiAliasing = video::EAAM_OFF;
		
		scene::IMeshManipulator *mm = engine->GetIrrlichtDevice()->getSceneManager()->getMeshManipulator();
		mm->setVertexColorAlpha( mesh->GetIrrlichtNode()->getMesh(), alpha );
		
		if (renderSystem->ShadersAreAvailable())
		{
			IShader *shader = renderSystem->CreateShader("PlainWithAlpha.vert", "PlainWithAlpha.frag",
					video::EMT_TRANSPARENT_VERTEX_ALPHA);
			shader->SetVertexRegisterMap(get_vertex_shader_register_map());
			shader->SetPixelRegisterMap(get_pixel_shader_register_map());
			
			mesh->SetShader(i, shader);
			shader->drop();
		}
		else
		{
			material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
		}
	}
}

IMesh *Level::AddObject( core::vector3di mapCoord, const c8 *meshName, bool movable, E_OBJECT_TYPE type, bool solid )
{
	IPhysics *physics = world->GetPhysics();
	
	IMesh *mesh = world->AddMesh(meshName);
	ApplyDefaultShaders(mesh);
	
	// No Body? Then Mesh is the object.
	ITransformable *object = mesh;
	
	if (solid)
	{
		IBody *body = physics->AddStaticBody();
	
		ICollisionGeometry *geom = physics->CreateBoxCollisionGeometry(core::vector3df(1.0,1.0,1.0));
		body->AddCollisionGeometry(geom);
		geom->drop();
		
		body->AddChild(mesh);
		
		// A body? Then body is the object, with mesh as its child.
		object = body;
	}
	
	object->SetPosition( GetPosFromCoord(mapCoord) );
	
	// Position the mesh immediately.
	mesh->ApplyTransformNow();
	
	map->SetObject(mapCoord, object, movable, type);
	
	// OnEnterLocation is called by Load function.
	
	return mesh;
}

void Level::GroundBlockUV_Adjust(IMesh *mesh, core::vector3di mapCoord)
{
	// Adjust UVs to avoid ugly texture repetition
	
	//scene::IAnimatedMeshSceneNode *node = mesh->GetIrrlichtNode();
	
	//NOTE << "MatCount: " << node->getMaterialCount();
	//ASSERT ( node->getMaterialCount() == 6 );
	
	//0-5:
	//mud_front.jpg
	//mud_back.jpg
	//grass1_lighter.jpg
	//mud_bottom.jpg
	//mud_right.jpg
	//mud_left.jpg
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		//NOTE << material.TextureLayer[0].Texture->getName();
		
		core::matrix4 matrix;
		core::vector3df pos = GetPosFromCoord(mapCoord);
		
		const f32 scale = 3.0;
		
		switch (i)
		{
		case 0://front
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( pos.Y/scale, pos.X/scale );
			break;
		case 1://back
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( -pos.Y/scale, pos.X/scale );
			break;
		case 2://top (grass)
		case 6://side (in case of edge tile)
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( pos.X/scale, -pos.Z/scale );
			break;
		case 3://bottom
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( -pos.Z/scale, pos.X/scale );
			break;
		case 4://right
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( pos.Y/scale, pos.Z/scale );
			break;
		case 5://left
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( pos.Y/scale, -pos.Z/scale );
			break;
		}
		
		material.setTextureMatrix(0, matrix);
	}
}

void Level::GroundBlockUV_Adjust2(IMesh *mesh, core::vector3di mapCoord)
{
	// Adjust UVs to avoid ugly texture repetition
	
	//scene::IAnimatedMeshSceneNode *node = mesh->GetIrrlichtNode();
	
	//NOTE << "MatCount: " << node->getMaterialCount();
	//ASSERT ( node->getMaterialCount() == 6 );
	
	//0-1:
	//grass1_lighter.jpg
	//mud_bottom.jpg
	for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	{
		video::SMaterial &material = mesh->GetMaterial(i,0);
		
		core::matrix4 matrix;
		core::vector3df pos = GetPosFromCoord(mapCoord);
		
		const f32 scale = 3.0;
		
		switch (i)
		{
		case 0://top (grass)
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( pos.X/scale, -pos.Z/scale );
			break;
		case 1://bottom
			matrix.setTextureScale( 1.0/scale, 1.0/scale );
			matrix.setTextureTranslate( -pos.Z/scale, pos.X/scale );
			break;
		}
		
		material.setTextureMatrix(0, matrix);
	}
}

void Level::MaybeAddFlower(core::vector3df absPos)
{
	// Flower
	
	/*
	if (rand()%100 < 10)
	{
		// Could in future add flower as child of other mesh.
		IMesh *flowerMesh = world->AddMesh("flower.b3d");
		ApplyDefaultShaders(flowerMesh);
		
		for (u32 i = 0; i < flowerMesh->GetMaterialCount(); i ++)
		{
			flowerMesh->GetMaterial(i).Lighting = false;
			flowerMesh->GetMaterial(i).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		}
		
		flowerMesh->SetPosition( absPos
				+ core::vector3df(0+(rand()%1000*0.001)-0.5, 0.5, 0+(rand()%1000*0.001)-0.5) );
		
		flowerMesh->SetRotation( core::vector3df(0,rand()%360,0) );
		
		flowerMesh->Scale(core::vector3df(1,1,1) * 0.75 + 0.01 * (f32)(rand()%10));
		
		flowerMesh->ApplyTransformNow();
	}
	*/
}

void Level::OptimiseLevel()
{
	// Adjacent 4 map coordinates (in cross shape)
	// [ ][x][ ]
	// [x][P][x]
	// [ ][x][ ]
	std::vector<core::vector3di> cross;
	cross.push_back(core::vector3di(1,0,0));
	cross.push_back(core::vector3di(-1,0,0));
	cross.push_back(core::vector3di(0,0,1));
	cross.push_back(core::vector3di(0,0,-1));
	
	
	// Make a list of all ground blocks which are surrounded completely by other ground blocks.
	
	std::vector<core::vector3di> objectCoords = map->GetAllObjects();
	std::vector<core::vector3di> surrounded_GroundBlocks;
	
	for (u32 i = 0; i < objectCoords.size(); i ++)
	{
		// If a ground block, and surrounded completely by other ground blocks...
		
		if (map->GetObjectType(objectCoords[i]) == EOT_GROUND_BLOCK)
		{
			u8 j;
			
			for (j = 0; j < cross.size(); j ++)
			{
				core::vector3di coord = objectCoords[i]+cross[j];
				
				if (!map->GetObject(coord) || map->GetObjectType(coord) != EOT_GROUND_BLOCK)
					break;
			}
			
			if (j == cross.size())
				surrounded_GroundBlocks.push_back(objectCoords[i]);
		}
	}
	
	// Then replace these with a ground block mesh that has no sides.
	
	for (u32 i = 0; i < surrounded_GroundBlocks.size(); i ++)
	{
		RemoveObject(surrounded_GroundBlocks[i]);
		
		IMesh *mesh = AddObject(surrounded_GroundBlocks[i], "ground_single_sideless.b3d", false, EOT_GROUND_BLOCK);
		GroundBlockUV_Adjust2(mesh, surrounded_GroundBlocks[i]);
	}
	
	
	// NEW!!
	// Combine all ground blocks into a single mesh...
	// With only two mesh buffers, grass and mud.
	// (compare to before, when each individual tile has 6 mesh buffers!)
	if (combineMeshes)
	{
		// Clear old combined mesh.
		// (since may ReplaceWith level)
		
		if (combinedLevelMesh)
		{
			combinedLevelMesh->remove();
			combinedLevelMesh = NULL;
		}
		
		for (u32 i = 0; i < combinedLevelMeshShaders.size(); i ++)
			combinedLevelMeshShaders[i]->drop();
		
		combinedLevelMeshShaders.clear();
		
		NOTE << "Mesh combining...";
		
		scene::ISceneManager *smgr = engine->GetIrrlichtDevice()->getSceneManager();
		scene::IMeshManipulator *meshManipulator = smgr->getMeshManipulator();
		video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
		
		scene::SMesh *mesh = new scene::SMesh();
		
		// We put everything in just two mesh buffers!
		
		scene::IMeshBuffer *mbGrass = new scene::SMeshBuffer();
		scene::IMeshBuffer *mbMud = new scene::SMeshBuffer();
		
		// Set up materials
		
		mbGrass->getMaterial().TextureLayer[0].Texture = driver->getTexture("grass1_lighter.jpg");
		mbMud->getMaterial().TextureLayer[0].Texture = driver->getTexture("mud.jpg");
		
		// First remove all existing ground blocks meshes.
		
		for (u32 i = 0; i < objectCoords.size(); i ++)
		{
			if (map->GetObjectType(objectCoords[i]) == EOT_GROUND_BLOCK)
			{
				core::vector3df pos = GetPosFromCoord(objectCoords[i]);
				
				// All ground blocks are solid.
				// Therefore, we just remove children of this block, and not the block itself.
				// (the block itself is the physical body, and needs to stay)
				
				ITransformable *obj = map->GetObject(objectCoords[i]);
				
				// Find the ground block mesh.
				
				IMesh *objMesh = obj->GetFirstChildOfType<IMesh>();
				
				ASSERT(objMesh);
				
				// Add that mesh's data to the combined mesh...
				// Transform it etc.
				
				scene::IMesh *otherMesh = objMesh->GetIrrlichtNode()->getMesh()->getMesh(0);
				
				for (u32 j = 0; j < otherMesh->getMeshBufferCount(); j ++)
				{
					// We want a completely new, unique mesh buffer, so we can modify it how we like.
					scene::IMeshBuffer *mbTemp = new scene::SMeshBuffer();
					scene::IMeshBuffer *otherMb = otherMesh->getMeshBuffer(j);
					
					// Fill the new mesh buffer
					mbTemp->append( otherMb->getVertices(), otherMb->getVertexCount(),
							otherMb->getIndices(), otherMb->getIndexCount() );
					
					// apply old Litha IMesh transform to this mesh buffer.
					// what about permanently applied position or rotation??
					// --> ignore, ground tiles shouldn't have it??
					
					core::matrix4 mat;
					mat.setTranslation(objMesh->GetAbsolutePosition());
					mat.setRotationDegrees(objMesh->GetAbsoluteRotation());
					meshManipulator->transform(mbTemp, mat);
					
					// Now adjust UVs (do what formerly had to be done via the texture matrix)
					
					ASSERT( mbTemp->getVertexType() == video::EVT_STANDARD );
					
					video::S3DVertex *vertices = (video::S3DVertex *)mbTemp->getVertices();
					
					// Get texture name from otherMb since material has not been set on temp meshbuffer
					io::path textureName = os::path::basename(otherMb->getMaterial().TextureLayer[0].Texture->getName());
					
					if (textureName == "mud_front.jpg") // front
					{
						//matrix.setTextureScale( 1.0/scale, 1.0/scale );
						//matrix.setTextureTranslate( pos.Y/scale, pos.X/scale );
						
						for (u32 k = 0; k < mbTemp->getVertexCount(); k ++)
						{
							const f32 scale = 3.f;
							
							vertices[k].TCoords *= 1.f/scale;
							vertices[k].TCoords.X += pos.Y/scale;
							vertices[k].TCoords.Y += pos.X/scale;
						}
					}
					else if (textureName == "mud_back.jpg") // back
					{
						//matrix.setTextureScale( 1.0/scale, 1.0/scale );
						//matrix.setTextureTranslate( -pos.Y/scale, pos.X/scale );
						
						for (u32 k = 0; k < mbTemp->getVertexCount(); k ++)
						{
							const f32 scale = 3.f;
							
							vertices[k].TCoords *= 1.f/scale;
							vertices[k].TCoords.X += -pos.Y/scale;
							vertices[k].TCoords.Y += pos.X/scale;
						}
					}
					else if (textureName == "grass1_lighter.jpg") // top
					{
						//matrix.setTextureScale( 1.0/scale, 1.0/scale );
						//matrix.setTextureTranslate( pos.X/scale, -pos.Z/scale );
						
						for (u32 k = 0; k < mbTemp->getVertexCount(); k ++)
						{
							const f32 scale = 3.f;
							
							vertices[k].TCoords *= 1.f/scale;
							vertices[k].TCoords.X += pos.X/scale;
							vertices[k].TCoords.Y += -pos.Z/scale;
						}
					}
					else if (textureName == "mud_bottom.jpg") // bottom
					{
						//matrix.setTextureScale( 1.0/scale, 1.0/scale );
						//matrix.setTextureTranslate( -pos.Z/scale, pos.X/scale );
						
						for (u32 k = 0; k < mbTemp->getVertexCount(); k ++)
						{
							const f32 scale = 3.f;
							
							vertices[k].TCoords *= 1.f/scale;
							vertices[k].TCoords.X += -pos.Z/scale;
							vertices[k].TCoords.Y += pos.X/scale;
						}
					}
					else if (textureName == "mud_right.jpg") // right
					{
						//matrix.setTextureScale( 1.0/scale, 1.0/scale );
						//matrix.setTextureTranslate( pos.Y/scale, pos.Z/scale );
						
						for (u32 k = 0; k < mbTemp->getVertexCount(); k ++)
						{
							const f32 scale = 3.f;
							
							vertices[k].TCoords *= 1.f/scale;
							vertices[k].TCoords.X += pos.Y/scale;
							vertices[k].TCoords.Y += pos.Z/scale;
						}
					}
					else if (textureName == "mud_left.jpg") // left
					{
						//matrix.setTextureScale( 1.0/scale, 1.0/scale );
						//matrix.setTextureTranslate( pos.Y/scale, -pos.Z/scale );
						
						for (u32 k = 0; k < mbTemp->getVertexCount(); k ++)
						{
							const f32 scale = 3.f;
							
							vertices[k].TCoords *= 1.f/scale;
							vertices[k].TCoords.X += pos.Y/scale;
							vertices[k].TCoords.Y += -pos.Z/scale;
						}
					}
					else
					{
						FAIL << "texture not handled...";
					}
					
					// NOW, we combine all separate mud textures into a single buffer...
					
					if (textureName == "grass1_lighter.jpg")
					{
						mbGrass->append( mbTemp->getVertices(), mbTemp->getVertexCount(),
								mbTemp->getIndices(), mbTemp->getIndexCount() );
					}
					else // is mud
					{
						mbMud->append( mbTemp->getVertices(), mbTemp->getVertexCount(),
								mbTemp->getIndices(), mbTemp->getIndexCount() );
					}
					
					// Discard the temporary buffer now contents are in the grass or mud buffers.
					mbTemp->drop();
				}
				
				// And remove the old IMesh...
				obj->RemoveAllChildren();
			}
		}
		
		// add new mesh buffers, then drop references
		
		mbGrass->recalculateBoundingBox();
		mbMud->recalculateBoundingBox();
		
		mesh->addMeshBuffer(mbGrass);
		mesh->addMeshBuffer(mbMud);
		
		mbGrass->drop();
		mbMud->drop();
		
		mesh->recalculateBoundingBox();
		
		ApplyDefaultShadersIrr(mesh);
		
		// Debugging
		NOTE << "Final buffer count: " << mesh->getMeshBufferCount();
		
		combinedLevelMesh = smgr->addMeshSceneNode(mesh);
		
		// write it to disk?
		/*
		{
			//EMWT_IRR_MESH
			scene::IMeshWriter *writer = smgr->createMeshWriter(scene::EMWT_OBJ);
			
			io::IFileSystem *fileSys = engine->GetIrrlichtDevice()->getFileSystem();
			io::IWriteFile *writeFile = fileSys->createAndWriteFile("test");
			
			if (writeFile)
			{
				writer->writeMesh(writeFile, mesh);
				writeFile->drop();
			}
			
			writer->drop();
		}
		*/
		mesh->drop();
	}
	
	
	
	// We're also going to add some level effects here
	
	// FAN VORTEX PARTICLE SYSTEM
	
	std::vector<core::vector3di> eventCoords = map->GetAllEvents();
	
	for (u32 i = 0; i < eventCoords.size(); i ++)
	{
		// Fan event?
		if (map->GetEvent(eventCoords[i])->GetType() == EET_FAN_EVENT)
		{
			core::vector3di coordBelow = eventCoords[i] + core::vector3di(0,-1,0);
			
			// Is the event the bottom one?
			// (i.e. there does not exist a fan event below this one)
			if (!map->GetEvent(coordBelow)
					|| map->GetEvent(coordBelow)->GetType() != EET_FAN_EVENT)
			{
				u32 fanEventCount = 1;
				core::vector3di coord = eventCoords[i];
				
				// Now count the fan events stacked above this one, to get the height for the fan particle system
				while (true)
				{
					coord.Y += 1;
					
					if (map->GetEvent(coord) && map->GetEvent(coord)->GetType() == EET_FAN_EVENT)
						fanEventCount ++;
					else
						break;
				}
				
				// We can now add a particle system at location eventCoords[i]
				// and of height fanEventCount.
				
				/*
				// particle system finishes one square below the top square
				// (so it seems objects sit on the top of the vortex, not in it)
				if (fanEventCount > 1)
					AddFanParticleSystem(eventCoords[i], (f32)fanEventCount - 1.0);
				*/
				
				// so I changed my mind about the above
				// it's nice to see objects floating in the particles
				AddFanParticleSystem(eventCoords[i], (f32)fanEventCount);
			}
		}
	}
}

void Level::AddFanParticleSystem(core::vector3di mapCoord, f32 height)
{
	// Test particle system
	
	scene::ISceneManager *smgr = engine->GetIrrlichtDevice()->getSceneManager();
	video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
	
	// in map coordinates
	core::vector3df mapPos = GetPosFromCoord(mapCoord);
	
	
	f32 minParticleSize;
	f32 maxParticleSize;
	f32 speed;
	u32 lifeTime;
	scene::IParticleSystemSceneNode *ps;
	scene::IParticleEmitter *emitter;
	scene::IParticleAffector *affector;
	
	
	
	height += 0.5;
	
	
	// INSIDE, FAST, AND BRIGHT
	
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(mapPos + core::vector3df(0,-1.0,0));//core::vector3df(0,-0.5,0));
	
	minParticleSize = 0.025;
	maxParticleSize = 0.08;
	speed = 0.0005;
	
	lifeTime = (u32)(height / speed);
	
	emitter = ps->createCylinderEmitter(
			core::vector3df(0,0,0),//centre
			0.1,//radius
			core::vector3df(0,1,0),//normal
			0.1,//length
			true,
			core::vector3df(0,speed,0),//velocity
			1,//min per second
			4,// max per second
			video::SColor(0,200,200,0),//colour lower bound
			video::SColor(0,255,255,0),//colour upper bound
			lifeTime,//min lifetime
			lifeTime,//max lifetime
			0,//max angular difference from velocity direction
			core::dimension2df(minParticleSize,minParticleSize),//minimum size
			core::dimension2df(maxParticleSize,maxParticleSize)//maximum size
			);
	
	ps->setEmitter(emitter);
	emitter->drop();
	
	// fade out affector
	affector = ps->createFadeOutParticleAffector();
	ps->addAffector(affector);
	affector->drop();
	
	// rotation affector
	affector = ps->createRotationAffector(
			core::vector3df(0,-100.0,0),
			mapPos
			);
	ps->addAffector(affector);
	affector->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("sparkle.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	//ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
	// add it for removal in destructor
	irrNodes.push_back(ps);
	
	
	
	
	// OUTSIDE, SLOWER, DULLER
	
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(mapPos + core::vector3df(0,-1.0,0));//core::vector3df(0,-0.75,0));
	
	minParticleSize = 1.4;
	maxParticleSize = 1.6;
	speed = 0.0005;
	
	lifeTime = (u32)(height / speed);
	
	emitter = ps->createCylinderEmitter(
			core::vector3df(0,0,0),//centre
			0.25,//radius
			core::vector3df(0,1,0),//normal
			0.1,//length
			false,
			core::vector3df(0,speed,0),//velocity
			3,//min per second
			7,// max per second
			video::SColor(0,5,5,8),//colour lower bound
			video::SColor(0,20,20,25),//colour upper bound
			lifeTime,//min lifetime
			lifeTime,//max lifetime
			0,//max angular difference from velocity direction
			core::dimension2df(minParticleSize,minParticleSize),//minimum size
			core::dimension2df(maxParticleSize,maxParticleSize)//maximum size
			);
	
	ps->setEmitter(emitter);
	emitter->drop();
	
	// fade out affector
	affector = ps->createFadeOutParticleAffector();
	ps->addAffector(affector);
	affector->drop();
	
	// rotation affector
	affector = ps->createRotationAffector(
			core::vector3df(0,-25.0,0),
			mapPos
			);
	ps->addAffector(affector);
	affector->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("fog.png"));
	//ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
	// add it for removal in destructor
	irrNodes.push_back(ps);
	
}

void Level::CreateObject(core::vector3di mapCoord, E_OBJECT_TYPE type)
{
	if (map->GetObject(mapCoord))
	{
		WARN << "An object already exists at the specified location.";
		return;
	}
	
	IMesh *mesh = NULL;
	
	switch (type)
	{
	case EOT_UNKNOWN:
	case EOT_PLAYER_CENTRE:
	case EOT_PLAYER_INTERSECTING:
		// Do nothing, no object.
		return; // must return now so coordinate is not added to level bounding box (done below)
	case EOT_GROUND_BLOCK:
		mesh = AddObject(mapCoord, "ground_single.b3d", false, type);
		GroundBlockUV_Adjust(mesh, mapCoord);
		MaybeAddFlower(mesh->GetAbsolutePosition());
		break;
	case EOT_GROUND_BLOCK_FALL:
		mesh = AddObject(mapCoord, "ground_single_fall.b3d", false, type);
		break;
	case EOT_MOVABLE_BLOCK:
		mesh = AddObject(mapCoord, "box.b3d", true, type);
		ApplyWoodShaders(mesh);
		break;
	case EOT_SLIDING_BLOCK:
		mesh = AddObject(mapCoord, "icybox.irrmesh", true, type);
		//ApplyWoodShaders(mesh);
		ApplyPlainAlphaShaders(mesh, 150);
		{
			IMesh *insideMesh = world->AddMesh("icybox_inner.irrmesh");
			mesh->AddChild(insideMesh);
			ApplyIceShaders(insideMesh);
		}
		break;
	case EOT_BALLOON:
		{
			mesh = AddObject(mapCoord, "balloon.irrmesh", true, type);
			//ApplyPlainAlphaShaders(mesh, 230);
			ApplyBalloonShaders(mesh);
			SetMeshAlpha(mesh, 200);
			
			IBobAnimator *bobAnim = world->CreateBobAnimator(0.04, 1.0 + (rand()%100)*0.002);
			mesh->AddAnimator(bobAnim);
			bobAnim->SetStartPos(mesh->GetPosition());
			bobAnim->drop();
		}
		break;
	case EOT_FAN:
		{
			// *** FAN BASE ***
			// Fan body, the main object
			mesh = AddObject(mapCoord, "magicfan_base.b3d", false, type);
			
			
			// *** FAN BLADES ***
			// And some fan blades as child of mesh.
			
			IMesh *bladeMesh = world->AddMesh("magicfan_blades.b3d");
			mesh->AddChild(bladeMesh);
			
			bladeMesh->SetRotation( core::vector3df(0,rand()%360,0) );
			bladeMesh->ApplyTransformNow();
			
			// brassy sphere map
			
			video::ITexture *sphereMap = engine->GetIrrlichtDevice()->getVideoDriver()->getTexture("brass_spheremap.png");
			bladeMesh->SetShader(0, NULL);
			bladeMesh->GetMaterial(0).MaterialType = video::EMT_SPHERE_MAP;
			bladeMesh->GetMaterial(0).TextureLayer[0].Texture = sphereMap;
			
			// fan blade rotation animator...
			
			IMotionAnimator *anim = world->CreateRotationAnimator( core::vector3df(0,150.0,0) );
			bladeMesh->AddAnimator(anim);
			anim->drop();
			
			
			
			// *** FAN ORB ***
			// Add the glowing orb
			
			IMesh *orbMesh = world->AddMesh("magicfan_orb.irrmesh");
			mesh->AddChild(orbMesh);
			orbMesh->GetMaterial(0).Lighting = false;
			
			
			
			// Fan noise
			ISoundSource *soundSource = world->AddSoundSource();
			mesh->AddChild(soundSource);
			soundSource->GetSound()->SetIsLooped(true);
			soundSource->GetSound()->SetVolume(0.5);
			soundSource->GetSound()->Play("../projects/Puzzle/media/sfx/buttonflutter_micro.ogg");
			soundSource->ApplyTransformNow();
			
			
			// orb bob up and down..
			IBobAnimator *bobAnim = world->CreateBobAnimator(0.035, 1.0 + (rand()%100)*0.001);
			orbMesh->AddAnimator(bobAnim);
			bobAnim->SetStartPos(mesh->GetPosition());
			bobAnim->drop();
		}
		break;
	case EOT_LIFT:
		{
			mesh = AddObject(mapCoord, "lift.b3d", false, type);
			
			//video::SMaterial &mat = mesh->GetMaterial(0,0);
			//mat.TextureLayer[0].Texture = engine->GetIrrlichtDevice()->getVideoDriver()->getTexture("lift_glow.png");
			
			// Plus lift light?
			IMesh *glowMesh = world->AddMesh("lift_glow.irrmesh");
			mesh->AddChild(glowMesh);
			glowMesh->GetMaterial(0).Lighting = false;
			
			IMotionSensor *motionSensor = world->AddSoundMotionSensor(
					// I need a search path manager thing.
					"../projects/Puzzle/media/sfx/hithard.ogg",
					"../projects/Puzzle/media/sfx/hithard.ogg",
					"../projects/Puzzle/media/sfx/liftrun.ogg");
			motionSensor->EnableInitialEvents(false);
			motionSensor->SetAveragingCount(5);
			mesh->AddChild(motionSensor);
			motionSensor->ApplyTransformNow();
		}
		break;
	case EOT_EXIT_PORTAL:
		{
			mesh = AddObject(mapCoord, "nothing.irrmesh", false, type, false);
			
			// Portal teleport effect
			for (u32 i = 0; i < 80; i ++)
			{
				IMesh *teleportMesh = world->AddMesh("teleport.irrmesh");
				//mesh->GetMaterial(0).Lighting = false;
				
				ApplyPlainAlphaShaders(teleportMesh, 40 + rand()%40);
				
				f32 bottom = 0.1;
				f32 top = 0.9;
				
				core::vector3df offPos = core::vector3df(0, bottom + (top-bottom)*(rand()%1000)*0.001, 0);
				
				teleportMesh->SetRotation( core::vector3df(0,rand()%360,0) );
				teleportMesh->SetPosition( core::vector3df(0,-0.5,0) + offPos );
				
				mesh->AddChild(teleportMesh);
				
				//f32 minSize = 0.6;
				//f32 maxSize = 0.9;
				//teleportMesh->Scale( core::vector3df(1,1,1) * minSize + (maxSize-minSize) * (rand()%100)*0.01 );
				teleportMesh->Scale( core::vector3df(1,1,1) * (offPos.Y + (rand()%1000)*0.0001) );
				
				teleportMesh->ApplyTransformNow();
				
				IMotionAnimator *anim = world->CreateRotationAnimator( core::vector3df(0,-150.0,0)
						* ( 1.0 + (rand()%200)*0.01 )
						);
				teleportMesh->AddAnimator(anim);
				anim->drop();
			}
			
		}
		break;
	default:
		WARN << "Invalid object " << type << " could not be loaded.";
	};
	
	// calculate a bounding box for the level.
	ExpandBB(mapCoord);
	
	if (mesh)
		mesh->ApplyTransformNow();
}

void Level::CreateEvent(core::vector3di mapCoord, E_EVENT_TYPE type)
{
	if (map->GetEvent(mapCoord))
	{
		WARN << "An event already exists at the specified location.";
		return;
	}
	
	if (type == EET_UNKNOWN)
		return;
	
	// find the event in the list...
	for (u32 i = 0; i < mapEventInstances.size(); i ++)
	{
		if (mapEventInstances[i]->GetType() == type)
		{
			// Match found, set a pointer to it.
			map->SetEvent(mapCoord, mapEventInstances[i]);
			
			// calculate a bounding box for the level.
			ExpandBB(mapCoord);
			return;
		}
	}
	
	WARN << "Event " << type << " not found.";
}

void Level::RemoveObject(core::vector3di mapCoord)
{
	if (ITransformable *object = map->GetObject(mapCoord))
	{
		// Should not remove player or player intersecting objects.
		// (as references to them are stored elsewhere)
		if (map->GetObjectType(mapCoord) != EOT_PLAYER_CENTRE
				&& map->GetObjectType(mapCoord) != EOT_PLAYER_INTERSECTING)
		{
			world->RemoveTransformable(object);
			map->SetObject(mapCoord, NULL, false, EOT_UNKNOWN);
		}
	}
}

void Level::PlayerPushed(core::vector3di mapCoord, core::vector3di pushVec)
{
	if (!logicEnabled) // hack! so blocks used as decoration in final scene are immovable
		return;
	
	// used below
	//bool isBalloon = map->GetObject(mapCoord) && map->GetObjectType(mapCoord) == EOT_BALLOON;
	
	
	// undo function.
	// hacked in.
	if (map->ObjectMoveWillComplete(mapCoord, mapCoord+pushVec))
	{
		SaveUndoState(CreateUndoState());
	}
	
	/*
	if ( StartMapObjectMove(mapCoord, mapCoord+pushVec) )
	{
		stats.pushes ++;
		
		sound->SetPosition(GetPosFromCoord(mapCoord));
		
		if (isBalloon)
			sound->Play("../projects/Puzzle/media/sfx/balloonpush.ogg");
		else
			sound->Play("../projects/Puzzle/media/sfx/slide.ogg");
		
		core::vector3di below = mapCoord + core::vector3di(0,-1,0);
		
		// We have a pause. With push animation.
		GetPlayer()->SetController(NULL);
		GetPlayer()->StopMoving();
		Event event("PlayerPushedPause");
		TimedEvent(event, 0.5);
		
		// anim
		GetPlayer()->SetAnimations(ANIM_PUSH, -1);
		
		// make player face block that he pushes
		IMotionAnimator *anim = new RotateToAnimator(
				maths::rotation_required_2d(core::vector2df(pushVec.X,pushVec.Z))
				);
		GetPlayer()->AddAnimator(anim);
		anim->drop();
		
		// set false when PlayerPushedPause event is handled.
		isPlayerPushing = true;
	}
	*/
	
	// CHANGED. Actual block move doesn't happen here.
	// Instead, player push is delayed for a fraction of a second.
	// So block move is more in sync with player push animation.
	// (block doesn't start to move until player hands appear to touch block)
	
	if (map->ObjectMoveWillComplete(mapCoord, mapCoord+pushVec))
	{
		{
			// event that triggers the actual block move
			pushStartLoc = mapCoord;
			pushEndLoc = mapCoord+pushVec;
			Event event("PlayerPushedMove");
			TimedEvent(event, 0.2);
		}
		
		{
			// We have a pause. With push animation.
			GetPlayer()->SetController(NULL);
			GetPlayer()->StopMoving();
			Event event("PlayerPushedPause");
			TimedEvent(event, 0.5);
		}
		
		if (localGridBasedMovement)
		{
			static_cast<GridBasedCharacterController *>(playerController)->EndMove();
		}
		
		// anim
		GetPlayer()->SetAnimations(ANIM_PUSH, -1);
		
		// make player face block that he pushes
		IMotionAnimator *anim = new RotateToAnimator(
				maths::rotation_required_2d(core::vector2df(pushVec.X,pushVec.Z))
				);
		GetPlayer()->AddAnimator(anim);
		anim->drop();
		
		// set false when PlayerPushedPause event is handled.
		isPlayerPushing = true;
	}
}

void Level::OnEnterLocation(core::vector3di coord)
{
	locationsEntered.push_back(coord);
	// Actual logic is performed in Update()
}

void Level::OnStartMove(core::vector3di coord)
{
	locationsStartedToEnter.push_back(coord);
}

void Level::OnLeaveLocation(core::vector3di coord, core::vector3di newCoord)
{
	locationsLeft.push_back(coord);
	locationsLeftNew.push_back(newCoord);
	// Actual logic is performed in Update()
}

void Level::PerhapsGravity(core::vector3di coord)
{
	if (!map->GetObject(coord))
		return;
	
	//IMapEventOwner *event = map->GetEvent(coord);
	
	// Assume all movable objects can fall
	// (StartMapObjectMove will fail if object is not movable)
	//if ( !event || (event && event->RequestActionPermission(coord, EAT_FALL)) )
	
	bool fallen = false;
	bool isPlayer = map->GetObjectType(coord) == EOT_PLAYER_CENTRE;
	bool isTraversing = map->IsTraversing(coord);
	
	if (defaultEvent->RequestActionPermission(coord, EAT_FALL))
		fallen = StartMapObjectMove( coord, coord+core::vector3di(0,-1,0) );
	
	
	// Player fall anim.
	if (isPlayer
			// not falling off world AND not pushing?
			&& world->GetCameraController() && !isPlayerPushing)
	{
		if (fallen)
		{
			// And check there's not a lift underneath
			
			core::vector3di below = coord+core::vector3di(0,-2,0);
			
			if (!map->GetObject(below) || map->GetObjectType(below) != EOT_LIFT)
				GetPlayer()->SetAnimations(ANIM_FALL_UPRIGHT, ANIM_FALL_UPRIGHT);
		}
		
		if (!fallen && !isTraversing && GetPlayer()->IsSetAnimations(ANIM_FALL_UPRIGHT, ANIM_FALL_UPRIGHT))
			GetPlayer()->SetAnimations(ANIM_IDLE, ANIM_WALK);
	}
}

void Level::AddActor(core::vector3di mapCoord, ICharacter *entity)
{
	
	IProxyTransformable *proxyTransformable = world->AddProxyTransformable(entity);
	
	proxyTransformable->SetPositionOffset(core::vector3df(0,-(1.0-ACTOR_PHYSICAL_HEIGHT)*0.5,0));
	
	Actor c;
	c.entity = proxyTransformable;
	c.centreLocationSet = false;
	actors.push_back(c);
	
	proxyTransformable->SetPosition(GetPosFromCoord(mapCoord));
}

void Level::CreatePlayer(core::vector3di mapCoord)
{
	/*
	IMesh *mesh = world->AddMesh("player.b3d");
	mesh->Scale(0.15);
	mesh->Rotate(core::vector3df(0,180,0));
	
	//ApplyDefaultShaders(mesh);
	
	// NOTE: We previously set material with transparent vertex alpha here so player
	// can be made transparent while teleporting. However keeping that material all the
	// time caused glitches with the transparent sea on the final scene.
	// So instead we use the regular material here, and only set the transparent one
	// when we touch a teleporter.
	ApplyDefaultShaders(mesh);//, video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	//if (!renderSystem->ShadersAreAvailable())
	//	mesh->GetMaterial(0).MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
	
	mesh->DefineAnimation(ANIM_IDLE, 20,39,	10.0,	1,1);
	mesh->DefineAnimation(ANIM_WALK, 0,19,	30.0,	1,1);
	*/
	
	// FOR NEW CHARACTER MODEL
	
	IMesh *mesh = world->AddMesh("player.b3d");
	
	// some of the params changed here since we upgraded to Irrlicht 1.7.2
	/*
	mesh->Scale(0.18);//0.075);
	mesh->Translate(core::vector3df(0.23,0.19,-0.02));//core::vector3df(0.1,0.19,-0.15));
	*/
	// Oh but then I downgraded to Irrlicht 1.6.1 again... :S
	mesh->Scale(0.075);
	mesh->Translate(core::vector3df(0.1,0.19,-0.15));
	
	mesh->Rotate(core::vector3df(0,180,0));
	
	//ApplyDefaultShaders(mesh);
	
	// NOTE: We previously set material with transparent vertex alpha here so player
	// can be made transparent while teleporting. However keeping that material all the
	// time caused glitches with the transparent sea on the final scene.
	// So instead we use the regular material here, and only set the transparent one
	// when we touch a teleporter.
	//ApplyDefaultShaders(mesh);//, video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	//if (!renderSystem->ShadersAreAvailable())
	//	mesh->GetMaterial(0).MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
	
	mesh->GetMaterial(0).Lighting = false;
	mesh->GetMaterial(0).BackfaceCulling = false;
	
	mesh->DefineAnimation(ANIM_IDLE, 27,45,	5.0,	1,1);
	mesh->DefineAnimation(ANIM_WALK, 0,23,	30.0,	0,1);
	mesh->DefineAnimation(ANIM_FALL, 79,88,	10.0,	1,1, 0.5);
	
	mesh->DefineAnimation(ANIM_PUSH, 93,111, 30.f, 0,1);
	mesh->DefineAnimation(ANIM_FALL_UPRIGHT, 113,128, 30.f, 1,1, 0.5);
	
	
	// Create actor
	
	ICharacter *entity = world->AddCharacter(mesh, ANIM_IDLE, ANIM_WALK, ACTOR_PHYSICAL_HEIGHT);
	
	entity->SetDensity(0.3);
	entity->SetMoveSpeed(0.5);
	
	// player has a special material...
	// (everything else uses default material)
	
	IPhysics *physics = world->GetPhysics();
	
	ICollisionMaterial *material = physics->AddCollisionMaterial();
	entity->SetMaterial(material);
	
	// And let that material interact with the default material
	
	CollisionMaterialInteraction materialInteraction;
	materialInteraction.friction = 100.0;
	materialInteraction.preparator = playerInteractionPreparator;
	
	physics->SetCollisionMaterialInteraction(material,
			&(physics->GetDefaultCollisionMaterial()),
			materialInteraction
			);
	
	
	// Add actor
	AddActor(mapCoord, entity);
	
	
	// This disabled since we want footsteps sound even when walking against a wall.
	// (in which case no motion is detected)
	/*
	IMotionSensor *motionSensor = world->AddSoundMotionSensor(
			// I need a search path manager thing.
			NULL,
			NULL,
			"../projects/Puzzle/media/sfx/step.ogg");
	motionSensor->EnableInitialEvents(false);
	motionSensor->SetAveragingCount(5);
	motionSensor->SetMinTranslateSpeed(0.01);
	motionSensor->EnableTranslateAxes(AXIS_X|AXIS_Z);
	motionSensor->ApplyTransformNow();
	mesh->AddChild(motionSensor);
	*/
	
	// Sound source for footsteps
	entity->AddChild( world->AddSoundSource() );
	
	mesh->ApplyTransformNow();
	
	SetPlayerAlpha(255);
}

void Level::SetPlayerAlpha(u8 alpha)
{
	SetMeshAlpha(GetPlayer()->GetMesh(), alpha);
}

void Level::SetMeshAlpha(IMesh *mesh, u8 alpha)
{
	scene::IMeshManipulator *mm = engine->GetIrrlichtDevice()->getSceneManager()->getMeshManipulator();
	mm->setVertexColorAlpha( mesh->GetIrrlichtNode()->getMesh(), alpha );
}

void Level::SetMeshColour(IMesh *mesh, video::SColor col)
{
	scene::IMeshManipulator *mm = engine->GetIrrlichtDevice()->getSceneManager()->getMeshManipulator();
	mm->setVertexColors( mesh->GetIrrlichtNode()->getMesh(), col );
}

/*
void Level::CreateMonster(core::vector3di mapCoord)
{
	IMesh *mesh = world->AddMesh("monster.b3d");
	mesh->Scale(0.1);
	mesh->Rotate(core::vector3df(0,90,0));
	
	ApplyDefaultShaders(mesh);
	
	mesh->DefineAnimation(ANIM_IDLE, 0,19,	10.0,	1,1);
	mesh->DefineAnimation(ANIM_WALK, 20,39,	25.0,	1,1);
	
	// Create actor
	
	ICharacter *entity = world->AddCharacter(mesh, ANIM_IDLE, ANIM_WALK, ACTOR_PHYSICAL_HEIGHT);
	
	entity->SetDensity(0.4);
	entity->SetMoveSpeed(0.5);
	
	// Custom monster AI controller
	MonsterCharacterController *controller = new MonsterCharacterController(this,GetPlayer());
	entity->SetController(controller);
	controller->drop();
	
	// Add actor
	AddActor(mapCoord, entity);
	
	
	scene::ISceneManager *smgr = engine->GetIrrlichtDevice()->getSceneManager();
	
	
	//mesh->DisableAllMaterials(0);
	
	//for (u32 i = 0; i < mesh->GetMaterialCount(); i ++)
	//{
	//	video::SMaterial &material = mesh->GetMaterial(i);
	//	
	//	material.Lighting = false;
	//	material.AntiAliasing = video::EAAM_OFF;
	//	material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
	//}
	
	//smgr->getMeshManipulator()->setVertexColorAlpha(mesh->GetIrrlichtNode()->getMesh()->getMesh(0), 100);
	
	
	
	// Test. add particle system.
	
	video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
	
	
	scene::IParticleSystemSceneNode* ps =
		smgr->addParticleSystemSceneNode(false, mesh->GetIrrlichtNode());
	
	scene::IParticleEmitter* em = ps->createAnimatedMeshSceneNodeEmitter(
		mesh->GetIrrlichtNode(),
		true,
		core::vector3df(0.0f, 0.0, 0.0f),
		10000.0f,
		-1,
		false,
		500,
		1000,
		video::SColor(0,255,0,0),
		video::SColor(0,255,0,0),
		200,
		400,
		0,
		core::dimension2df(0.05,0.05),
		core::dimension2df(0.1,0.1)
		);
	
	
	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector *paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("flame.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
}
*/

Level::Level(MainState *mainState, core::stringc fileName, std::deque<UndoState> *undoHistory)
{
	this->engine = GetEngine();
	this->world = engine->GetWorld();
	renderSystem = engine->GetRenderSystem();
	this->mainState = mainState;
	this->fileName = fileName;
	
	this->localGridBasedMovement = gridBasedMovement;
	
	timeSinceLastUndoSave = 10000.f; // want to be able do undo immediately. (in case undone to a difficult situation)
	
	NOTE << "Level::Level: " << fileName;
	
	// Ugly hack.
	// In some special cases we don't want grid based movement.
	if (GetShortName() == "titlescreen.lev" || GetShortName() == "intro.lev" || GetShortName() == "finalscene1.lev"
			|| GetShortName() == "finalscene2.lev")
	{
		localGridBasedMovement = false;
	}
	
	globalLogLastLevel = GetShortName();
	
	engine->RegisterEventInterest(this, "ScreenFadeFinished");
	engine->RegisterEventInterest(this, "PlayerPushedPause");
	engine->RegisterEventInterest(this, "PlayerPushedMove");
	engine->RegisterEventInterest(this, "ApplyUndo");
	
	engine->RegisterEventInterest(this, "TutorialShow");
	engine->RegisterEventInterest(this, "TutorialFadeOn");
	engine->RegisterEventInterest(this, "TutorialFadeOff");
	engine->RegisterEventInterest(this, "TutorialDelete");
	
	cloudShadowTexture = engine->GetIrrlichtDevice()->getVideoDriver()->getTexture("cloudshadow.png");
	
	combineMeshes = true;
	combinedLevelMesh = NULL;
	
	if (globalIsInEditor)
		combineMeshes = false;
	
	playerInteractionPreparator = new PlayerInteractionPreparator();
	
	isPlayerPushing = false;
	
	// Player is always first actor in list.
	// Must be added before GetPlayer() can be called...
	CreatePlayer(core::vector3di(0,0,0));
	
	// Camera is created on Start
	thirdPersonCamera = NULL;
	
	// created in constructor..
	playerController = NULL;
	
	// Logic enabled. Not in editor by default.
	logicEnabled = true;
	
	// ...
	alwaysFootsteps = false;
	
	// utility sound
	sound = engine->GetSoundSystem()->CreateSound3D();
	
	// Create cube map
	map = new Map();
	
	
	// Create events
	mapEventInstances.push_back( new FanEvent(this) );
	mapEventInstances.push_back( new LiftEvent(this) );
	mapEventInstances.push_back( new PlayerStartEvent(this) );
	defaultEvent = new DefaultEvent(this); mapEventInstances.push_back( defaultEvent );
	
	isBoundingBoxEmpty = true;
	
	
	tutorialEventQueue = engine->CreateEventQueue();
	GetUpdater().AddUpdatable(tutorialEventQueue);
	tutorialEventQueue->drop();
	
	
	// Attempt to load the specified level!
	// If specified level does not exist, then nothing will be loaded.
	
	if (undoHistory && !undoHistory->empty())
	{
		this->undoHistory = *undoHistory;
		Load(&this->undoHistory.back());
		this->undoHistory.pop_back();
		NOTE << "Popped undo: " << s32(this->undoHistory.size());
		
		// tutorial texts are taken from undo state by ::Load
	}
	else
	{
		Load();
		
		// Then load tutorial texts.
		
#ifdef __APPLE__
		io::path tutorialFileName = "../projects/Puzzle/tutorialtexts_mac/";
#else
		io::path tutorialFileName = "../projects/Puzzle/tutorialtexts/";
#endif
		tutorialFileName += os::path::splitext(GetShortName())[0];
		tutorialFileName += ".ini";
		
		std::vector<core::stringc> lines = file::get_lines(tutorialFileName);
		
		if (lines.size() == 0)
			NOTE << "No tutorial text existed: " << tutorialFileName;
		else
		{
			NOTE << "Found tutorial text: " << tutorialFileName;
			
			std::vector<core::stringc> texts;
			
			const f32 displayTimeDefault = 5.f;
			const f32 delayTimeDefault = 0.f;
			
			f32 displayTime = displayTimeDefault;
			f32 delayTime = delayTimeDefault;
			
			for (u32 i = 0; i < lines.size(); i ++)
			{
				std::vector<core::stringc> parts = str::explode_at_assignment(lines[i]);
				
				if (parts[0].size() && parts[1].size())
				{
					// push a line of text (may be multiple lines)
					if (parts[0] == "line")
						texts.push_back(parts[1]);
					
					if (parts[0] == "display_time")
						displayTime = str::from_f32(parts[1]);
					
					if (parts[0] == "delay_time")
						delayTime = str::from_f32(parts[1]);
					
					// finished pushing text lines, now we have the trigger condition for those lines.
					if (parts[0] == "trigger" || parts[0] == "trigger_x" || parts[0] == "trigger_y"
							|| parts[0] == "trigger_z" || parts[0] == "trigger_surround")
					{
						bool fail = false;
						
						TutorialText tutorialItem;
						tutorialItem.lines = texts;
						tutorialItem.displayTime = displayTime;
						tutorialItem.delayTime = delayTime;
						
						if (parts[0] == "trigger")
						{
							tutorialItem.type = TTT_NORMAL;
							
							// parse trigger
							
							std::vector<core::stringc> triggerParts = str::explode(",", parts[1]);
							
							if (triggerParts.size() == 3)
							{
								core::vector3di triggerPos;
								triggerPos.X = str::from_s32(triggerParts[0]);
								triggerPos.Y = str::from_s32(triggerParts[1]);
								triggerPos.Z = str::from_s32(triggerParts[2]);
								tutorialItem.trigger = triggerPos;
							}
							else
							{
								WARN << "Invalid trigger in tutorial text: " << lines[i];
								fail = true;
							}
						}
						else if (parts[0] == "trigger_surround")
						{
							tutorialItem.type = TTT_SURROUND;
							
							// parse trigger
							
							std::vector<core::stringc> triggerParts = str::explode(",", parts[1]);
							
							if (triggerParts.size() == 3)
							{
								core::vector3di triggerPos;
								triggerPos.X = str::from_s32(triggerParts[0]);
								triggerPos.Y = str::from_s32(triggerParts[1]);
								triggerPos.Z = str::from_s32(triggerParts[2]);
								tutorialItem.trigger = triggerPos;
							}
							else
							{
								WARN << "Invalid trigger in tutorial text: " << lines[i];
								fail = true;
							}
						}
						else if (parts[0] == "trigger_x")
						{
							tutorialItem.type = TTT_X;
							tutorialItem.trigger.X = str::from_s32(parts[1]);
						}
						else if (parts[0] == "trigger_y")
						{
							tutorialItem.type = TTT_Y;
							tutorialItem.trigger.Y = str::from_s32(parts[1]);
						}
						else if (parts[0] == "trigger_z")
						{
							tutorialItem.type = TTT_Z;
							tutorialItem.trigger.Z = str::from_s32(parts[1]);
						}
						
						// Success! Added a tutorial item.
						if (!fail)
							tutorialTexts.push_back(tutorialItem);
						
						// Reset the text lines for the next tutorial text.
						texts.clear();
						
						// Reset stuff
						displayTime = displayTimeDefault;
						delayTime = delayTimeDefault;
					}
				}
			}
			
			// Just for info, output what was read.
			
			for (u32 i = 0; i < tutorialTexts.size(); i ++)
			{
				NOTE << "[tutorial item]";
				
				for (u32 j = 0; j < tutorialTexts[i].lines.size(); j ++)
				{
					NOTE << "Line: " << tutorialTexts[i].lines[j];
				}
				
				NOTE << "Will delay for: " << tutorialTexts[i].delayTime << " seconds";
				NOTE << "Will display for: " << tutorialTexts[i].displayTime << " seconds";
				
				NOTE << "Condition: trigger on map location {"
						<< tutorialTexts[i].trigger.X << ","
						<< tutorialTexts[i].trigger.Y << ","
						<< tutorialTexts[i].trigger.Z << "}";
			}
		}
	}
	
	// orient the player to be looking towards the centre of the bounding box
	
	core::vector3df initVec3d = boundingBox.getCenter() - GetPlayer()->GetPosition();
	core::vector2df initVec2d(initVec3d.X, initVec3d.Z);
	initVec2d.normalize();
	GetPlayer()->SetRotation( core::vector3df(0,initVec2d.getAngle()+90.0,0) );
	
	// Set the camera to a default position overviewing the entire level.
	
	ICamera *camera = world->GetCamera();
	
	// Find bounding box for level (min and max)
	// then use the centre for camera centre, and a corner for camera position.
	// (if I remember correctly this was formerly used for the level preview before Start was called)
	
	f32 radius = boundingBox.getExtent().getLength()/2.0;
	
	f32 thetaHoriz = camera->GetFOV()/2.0;
	
	f32 distHoriz = radius / tan(thetaHoriz);
	f32 distVert = radius / ( tan(thetaHoriz) / camera->GetAspectRatio() );
	
	// use greatest distance to fit sphere completely
	f32 distance = distVert > distHoriz ? distVert : distHoriz;
	
	camera->SetPosition(boundingBox.getCenter()+core::vector3df(1,1,1).normalize()*distance);
	camera->SetOrbitTarget(boundingBox.getCenter());
	
	levelCentrePoint = boundingBox.getCenter();
	
	// Test monster!
	//CreateMonster(core::vector3di(-1,-3,-1));
	
	// Fade on
	renderSystem->ScreenFade(0.0, 0.0, true);
	renderSystem->ScreenFade(1.0, 2, true);
	//renderSystem->ScreenFade(1.0, 0.0, true);
	
	// level not ending yet!
	touchedEndLevelPortal = false;
	
	// for footstep sounds
	lastObjectBelowPlayer = EOT_UNKNOWN;
	
	// ...
	fallCameraMoved = false;
}

Level::~Level()
{
	NOTE << "Level::~Level";
	
	if (combinedLevelMesh)
		combinedLevelMesh->remove();
	
	for (u32 i = 0; i < combinedLevelMeshShaders.size(); i ++)
		combinedLevelMeshShaders[i]->drop();
	
	ClearTutorialTextElements();
	
	engine->UnregisterAllEventInterest(this);
	
	if (thirdPersonCamera) // may not have been created if Start was not called.
	{
		world->SetCameraController(NULL);
		thirdPersonCamera->drop();
	}
	
	if (playerController)
		playerController->drop();
	
	// remove stray irrlicht nodes
	for (u32 i = 0; i < irrNodes.size(); i ++)
		irrNodes[i]->remove();
	
	// need to step through map and remove objects from world.(???)
	std::vector<core::vector3di> mapObjects = map->GetAllObjects();
	
	for (u32 i = 0; i < mapObjects.size(); i ++)
	{
		if (ITransformable *entity = map->GetObject(mapObjects[i]))
		{
			world->RemoveTransformable(entity);
			
			// if this was an actor, it has been removed, so remove from actors list.
			// (so we don't attempt RemoveTransformable on an actor again below)
			for (u32 j = 0; j < actors.size(); j ++)
			{
				if (entity == actors[j].entity)
				{
					actors.erase(actors.begin()+j);
					j --;
				}
			}
		}
	}
	
	// delete events
	for (u32 i = 0; i < mapEventInstances.size(); i ++)
		mapEventInstances[i]->drop();
	
	delete map;
	
	// remove all actors including player
	// may of course have been removed above (when stepping through mapObjects)
	for (u32 i = 0; i < actors.size(); i ++)
		world->RemoveTransformable(actors[i].entity);
	
	// Remove anything else (e.g. flowers)
	world->RemoveAllTransformables();
	
	// No more input
	// (subscribed to if Start has been called)
	world->UnsubscribeFromInput(this);
	
	sound->drop();
	
	delete playerInteractionPreparator;
}

void Level::Start()
{
	// Create and set in-game input profile
	IInputProfile *inputProfile = world->CreateInputProfile(6, 2); // 6 buttons and 2 axis
	
	inputProfile->BindButton(0, KEY_KEY_W);
	inputProfile->BindButton(1, KEY_KEY_S);
	inputProfile->BindButton(2, KEY_KEY_A);
	inputProfile->BindButton(3, KEY_KEY_D);
	
	inputProfile->BindButton(0, KEY_UP);
	inputProfile->BindButton(1, KEY_DOWN);
	inputProfile->BindButton(2, KEY_LEFT);
	inputProfile->BindButton(3, KEY_RIGHT);
	
	// Dvorak!
	inputProfile->BindButton(0, KEY_COMMA);
	inputProfile->BindButton(1, KEY_KEY_O);
	inputProfile->BindButton(2, KEY_KEY_A);
	inputProfile->BindButton(3, KEY_KEY_E);
	// AZERTY
	inputProfile->BindButton(0, KEY_KEY_Z);
	inputProfile->BindButton(1, KEY_KEY_S);
	inputProfile->BindButton(2, KEY_KEY_Q);
	inputProfile->BindButton(3, KEY_KEY_D);
	
	// Not actually used?
	//inputProfile->BindButton(4, KEY_LBUTTON);
	//inputProfile->BindButton(5, KEY_RBUTTON);
	
	// Level restart
#ifndef __APPLE__
	inputProfile->BindButton(6, KEY_DELETE);
#endif
	inputProfile->BindButton(6, KEY_KEY_R);
	
	// Undo?
	inputProfile->BindButton(7, KEY_BACK);
	inputProfile->BindButton(7, KEY_KEY_U);
	
	inputProfile->BindAxis(0, AXIS_MOUSE_X, invertMouseX);
	inputProfile->BindAxis(1, AXIS_MOUSE_Y, invertMouseY);
	inputProfile->BindAxis(2, AXIS_MOUSE_WHEEL);
	
#ifdef __APPLE__
	inputProfile->BindButtonAsAxis(2, 0x3d, 2.0);
	inputProfile->BindButtonAsAxis(2, 0x2d, -2.0);
	inputProfile->BindButtonAsAxis(2, KEY_ADD, 2.0);
	inputProfile->BindButtonAsAxis(2, KEY_SUBTRACT, -2.0);
	inputProfile->BindButtonAsAxis(2, KEY_KEY_1, 2.0);
	inputProfile->BindButtonAsAxis(2, KEY_KEY_2, -2.0);
#else
	inputProfile->BindButtonAsAxis(2, KEY_LBUTTON, 2.0);
	inputProfile->BindButtonAsAxis(2, KEY_RBUTTON, -2.0);
#endif
	
	world->SetInputProfile(inputProfile);
	inputProfile->drop();
	
	// Want input
	// Must unsubscribe in destructor
	world->SubscribeToInput(this);
	
	bool zoomEnabled = !(GetShortName() == "first.lev" || GetShortName() == "shiftblocks.lev"
			|| GetShortName() == "tutorial_lifts.lev");
	
	f32 defaultZoomDistance = 4.f;
	
	if (GetShortName() == "tutorial_lifts.lev")
		defaultZoomDistance = 5.f;
	
	// Create third person camera
	thirdPersonCamera = world->CreateThirdPersonCameraController(defaultZoomDistance, 0.0,
			-85.0,85.0);
	thirdPersonCamera->SetInputAxes(0,1, zoomEnabled ? 2 : -1);
	thirdPersonCamera->SetTurnSmoothing(20.0);
	//thirdPersonCamera->SetZoomLimits(1.5, 10.0);
	thirdPersonCamera->SetZoomLimits(1.5, max(boundingBox.getExtent().getLength(), 10.f));
	thirdPersonCamera->SetZoomSmoothness(3.0);
	
	// Set camera
	world->SetCameraController(thirdPersonCamera);
	
	// Create control system for player
	// This is dropped in destructor.
	
	if (localGridBasedMovement)
	{
		// Custom controler that only allows movement in the 4 compass directions.
		playerController = new GridBasedCharacterController(world, 0,1,2,3, world->GetCamera(),
				this);
	}
	else
	{
		playerController = world->CreateUserCharacterController(0,1,2,3,
				world->GetCamera());
	}
	
	GetPlayer()->SetController(playerController);
	
	// Set third person camera to follow player
	ICameraCollider *collider = new CameraCollider(world->GetPhysics());
	//ICameraCollider *collider = thirdPersonCamera->CreateThirdPersonCameraCollider();
	collider->SetCollisionLayer(2);
	thirdPersonCamera->Follow(GetPlayer(), collider);
	collider->drop();
	
	// Move the camera angle pitch up a little (so is looking down on player)
	
	f32 y_cam = 0.f;
	
	if (GetShortName() != "finalscene1.lev")
		y_cam = maths::extract_y_rotation(GetPlayer()->GetRotation());
	
	thirdPersonCamera->SetAngles( core::vector2df(40, y_cam) );
	
	
	// Everything should now be loaded, so let's init updating...
	engine->InitUpdateTiming();
}

void Level::ReplaceWith(core::stringc fileName)
{
	NOTE << "Replacing level with contents from: " << fileName;
	
	// remove stray irrlicht nodes
	for (u32 i = 0; i < irrNodes.size(); i ++)
		irrNodes[i]->remove();
	
	irrNodes.clear();
	
	// need to step through map and remove objects from world.(???)
	std::vector<core::vector3di> mapObjects = map->GetAllObjects();
	
	for (u32 i = 0; i < mapObjects.size(); i ++)
	{
		// except player
		if (ITransformable *entity = map->GetObject(mapObjects[i]))
		{
			if (map->GetObjectType(mapObjects[i]) != EOT_PLAYER_CENTRE
				&& map->GetObjectType(mapObjects[i]) != EOT_PLAYER_INTERSECTING)
			{
				world->RemoveTransformable(entity);
			}
		}
	}
	
	// recreate map
	map->Clear();
	
	// let's clear some random other stuff
	
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		c.lastLocations.clear();
		c.centreLocationSet = false;
	}
	
	mapObjectMoves.clear();
	locationsLeft.clear();
	locationsLeftNew.clear();
	locationsStartedToEnter.clear();
	locationsEntered.clear();
	
	this->fileName = fileName;
	
	
	ICamera *camera = world->GetCamera();
	
	// get offset from current centre, use that as normalised vector
	// but otherwise calculate completely new position.
	core::vector3df oldVec = (camera->GetPosition()-boundingBox.getCenter()).normalize();
	
	// clear bb
	boundingBox.reset(0,0,0);
	isBoundingBoxEmpty = true;
	
	Load();
	
	
	
	// THIS DISTANCE CALCULATION COPIED FROM CONSTRUCTOR.
	f32 radius = boundingBox.getExtent().getLength()/2.0;
	
	f32 thetaHoriz = camera->GetFOV()/2.0;
	
	f32 distHoriz = radius / tan(thetaHoriz);
	f32 distVert = radius / ( tan(thetaHoriz) / camera->GetAspectRatio() );
	
	// use greatest distance to fit sphere completely
	f32 distance = distVert > distHoriz ? distVert : distHoriz;
	
	
	// Now have new BB so can calc new cam pos
	camera->SetPosition(boundingBox.getCenter()+oldVec*distance);
	camera->SetOrbitTarget(boundingBox.getCenter());
	
	levelCentrePoint = boundingBox.getCenter();
	
	NOTE << "...finished replace.";
}

void Level::OnButtonDown(s32 id)
{
	if (mainState)
	{
		if (id == 6 && !IsEnding())
			mainState->RestartLevel();
	}
	
	if (id == 7 && !IsEnding())
		ApplyUndo();
}

void Level::OnEvent(const Event &event)
{
	if (mainState)
	{
		if (event.IsType("ScreenFadeFinished") && IsEnding())
		{
			// start next level
			mainState->NextLevel(true);
		}
	}
	
	if (event.IsType("ApplyUndo") && !IsEnding())
		ApplyUndo(true);
	
	
	if (event.IsType("PlayerPushedMove"))
	{
		playerPushEvent.push_back(event);
		
		// FIX: We don't do the actual push until a particular place in Update()
		// (needed for stack sliding to work...)
		
		/*
		// A short delay since player push was triggered has passed.
		// Now perform the actual block move.
		
		bool isBalloon = map->GetObject(pushStartLoc) && map->GetObjectType(pushStartLoc) == EOT_BALLOON;
		
		if ( StartMapObjectMove(pushStartLoc, pushEndLoc) )
		{
			stats.pushes ++;
			
			sound->SetPosition(GetPosFromCoord(pushStartLoc));
			
			if (isBalloon)
				sound->Play("../projects/Puzzle/media/sfx/balloonpush.ogg");
			else
				sound->Play("../projects/Puzzle/media/sfx/slide.ogg");
		}
		else
		{
			WARN << "PlayerPushedMove: move failed.";
		}
		*/
	}
	else if (event.IsType("PlayerPushedPause"))
	{
		// Pause has finished, so enable player control again.
		GetPlayer()->SetController(playerController);
		
		// Reset anims too
		GetPlayer()->SetAnimations(ANIM_IDLE, ANIM_WALK);
		
		// Remove rotate animator...
		GetPlayer()->RemoveAllAnimators();
		
		isPlayerPushing = false;
	}
	
	
	HandleTutorialEvents(event);
}

void Level::ShowEndLevelScreen()
{
	EndLevelScreen *es = new EndLevelScreen(mainState, this);
	world->GetUpdater().AddUpdatable(es);
	es->drop();
	
	ClearTutorialTextElements();
}

void Level::ClearEndLevelTeleportEffects()
{
	// Remove player teleport effects.
	for (u32 i = 0; i < endLevelPlayerEffects.size(); i ++)
		world->RemoveTransformable(endLevelPlayerEffects[i]);
	
	endLevelPlayerEffects.clear();
}

void Level::SaveUndoState(const UndoState &state)
{
	// max save every 10 seconds
	if (timeSinceLastUndoSave > 10.f)
	{
		NOTE << "Saved Undo State: " << s32(undoHistory.size());
		
		timeSinceLastUndoSave = 0.f;
		
		undoHistory.push_back(state);
	}
}

UndoState Level::CreateUndoState()
{
	UndoState state;
	
	// Much like is done by ::Save()
	
	std::vector<core::vector3di> mapLocations = map->GetAllMapLocations();
	
	for (u32 i = 0; i < mapLocations.size(); i ++)
	{
		UndoState::MapLoc mapLoc;
		
		core::vector3di &coord = mapLocations[i];
		
		mapLoc.coord = coord;
		mapLoc.objectType = map->GetObject(coord) ? map->GetObjectType(coord) : EOT_UNKNOWN;
		mapLoc.eventType = map->GetEvent(coord) ? map->GetEvent(coord)->GetType() : EET_UNKNOWN;
		
		if (map->GetObject(coord))
		{
			mapLoc.hasPreviousCoord = map->ObjectHasPreviousCoord(coord);
			
			if (map->ObjectHasPreviousCoord(coord))
				mapLoc.previousCoord = map->GetObjectPreviousCoord(coord);
			
			mapLoc.is_sliding = map->GetIsObjectSliding(coord);
		}
		else
		{
			mapLoc.hasPreviousCoord = false;
			mapLoc.is_sliding = false;
		}
		
		state.mapContents.push_back(mapLoc);
	}
	
	state.tutorialTexts = tutorialTexts;
	state.stats = stats;
	
	state.cameraAngle = GetCamera()->GetAngles();
	state.cameraZoom = GetCamera()->GetZoom();
	state.playerAngle = maths::extract_y_rotation(GetPlayer()->GetRotation());
	
	return state;
}

// Undo function!
void Level::ApplyUndo(bool died)
{
	NOTE << "*** undo selected ***";
	
	if (mainState)
	{
		if (ExistsUndo())
		{
			// Through all past states, incrementing undo and death counters as necessary.
			// Since those things remain even after undoing.
			for (u32 i = 0; i < undoHistory.size(); i ++)
			{
				if (died)
					undoHistory[i].stats.deaths ++;
				else
					undoHistory[i].stats.undos ++;
			}
			
			// Also replace all undo texts with the current undo texts.
			// So tutorial texts are not re-shown after undoing.
			for (u32 i = 0; i < undoHistory.size(); i ++)
			{
				undoHistory[i].tutorialTexts = tutorialTexts;
			}
			
			mainState->RestartLevel(&undoHistory);
		}
		else
		{
			NOTE << "(no undo existed, so will restart from beginning)";
			mainState->RestartLevel();
		}
	}
	else
	{
		WARN << "no mainstate for undo";
	}
}

void Level::Save()
{
	std::vector<core::vector3di> mapLocations = map->GetAllMapLocations();
	
	FILE *fp = fopen(fileName.c_str(), "wb");
	
	if (fp)
	{
		for (u32 i = 0; i < mapLocations.size(); i ++)
		{
			core::vector3di &coord = mapLocations[i];
			
			// position
			// objectType
			// eventType
			// NOTE: UNKNOWN types of object or event indicate no object/event.
			fprintf(fp, "%i,%i,%i\t%i\t%i\n",
					coord.X,coord.Y,coord.Z,
					map->GetObject(coord) ? map->GetObjectType(coord) : EOT_UNKNOWN,
					map->GetEvent(coord) ? map->GetEvent(coord)->GetType() : EET_UNKNOWN
					);
		}
		
		fclose(fp);
	}
}

void Level::Load(UndoState *undoState)
{
	std::vector<core::vector3di> startPositions;
	
	if (!undoState) // default, loads from filename
	{
		FILE *fp = fopen(fileName.c_str(), "rb");
		
		if (fp)
		{
			while (true)
			{
				core::vector3di coord;
				int objectType;
				int eventType;
				
				// position
				// objectType,eventType
				// NOTE: UNKNOWN types of object or event indicate no object/event.
				int ret = fscanf(fp, "%i,%i,%i\t%i\t%i\n",
						&coord.X,&coord.Y,&coord.Z,
						&objectType,&eventType);
				
				if (ret == EOF)
					break;
				
				if (ret != 5)
				{
					WARN << "Invalid level file (" << fileName << ")";
					break;
				}
				
				// Successfully read a location, so create stuff!
				CreateObject(coord, (E_OBJECT_TYPE)objectType);
				CreateEvent(coord, (E_EVENT_TYPE)eventType);
				
				// Special logic for player start events.
				if (eventType == EET_PLAYER_START_EVENT)
					startPositions.push_back(coord);
			}
			
			fclose(fp);
		}
		
		lowestPointCache[fileName] = lowestPoint;
	}
	// HACKED IN.
	// undoState is set, so we read level from there.
	// This is mostly copy and pasted from just above.
	else
	{
		std::vector<UndoState::MapLoc> &mapContents = undoState->mapContents;
		
		for (u32 i = 0; i < mapContents.size(); i ++)
		{
			UndoState::MapLoc &loc = mapContents[i];
			
			// Successfully read a location, so create stuff!
			CreateObject(loc.coord, (E_OBJECT_TYPE)loc.objectType);
			CreateEvent(loc.coord, (E_EVENT_TYPE)loc.eventType);
			
			// for undostate, we ingore all player start events, instead using the player's centre location
			// as the start position
			if ((E_OBJECT_TYPE)loc.objectType == EOT_PLAYER_CENTRE)
				startPositions.push_back(loc.coord);
			
			// was object created?
			// we set previous coord
			if (loc.hasPreviousCoord
					// excluded player since it is not positioned yet...
					// (and we ignore its previous move... maybe that's bad!?)
					&& (E_OBJECT_TYPE)loc.objectType != EOT_PLAYER_CENTRE
					&& (E_OBJECT_TYPE)loc.objectType != EOT_PLAYER_INTERSECTING)
			{
				if (map->GetObject(loc.coord))
					map->SetObjectPreviousCoord(loc.coord, loc.previousCoord);
				else
					WARN << "error with undo: previous coord set but no object was created!";
			}
			
			if (map->GetObject(loc.coord))
				map->SetIsObjectSliding(loc.coord, loc.is_sliding);
		}
		
		// tutorial text state
		tutorialTexts = undoState->tutorialTexts;
		
		// level stats!
		stats = undoState->stats;
		
		// undo state is set
		// we are undoing.
		// we want to use the original BB from when the level was loaded,
		// not the current BB.
		// (since a block may have been pushed off the world, making the BB very big,
		// and player will fall forever)
		if (lowestPointCache.count(fileName))
		{
			lowestPoint = lowestPointCache[fileName];
		}
		else
		{
			WARN << "lowestPoint not found in cache: " << fileName;
		}
	}
	
	// Perform optimisations on level...
	// Some ground blocks may be replaced.......
	// Also adds some effects to the level.
	OptimiseLevel();
	
	// Randomly position the player...
	
	if (startPositions.size())
	{
		GetPlayerActor().entity->SetPosition(GetPosFromCoord( startPositions[rand()%startPositions.size()] ));
		GetPlayerActor().entity->ApplyTransformNow();
		GetPlayer()->ApplyTransformNow();
	}
	else
	{
		WARN << "No player start position given in level file (" << fileName << ")";
	}
	
	// NOW NEED TO CALL OnEnterLocation for each square containing an object.
	std::vector<core::vector3di> mapLocations = map->GetAllMapLocations();
	
	for (u32 i = 0; i < mapLocations.size(); i ++)
	{
		if (map->GetObject(mapLocations[i]))
		{
			// Object has been initially positioned.
			// Need to call any events etc set for that location.
			// Also trigger any object actions (e.g. fall)
			OnEnterLocation(mapLocations[i]);
		}
	}
}

bool Level::StartMapObjectMove(core::vector3di source, core::vector3di dest, bool forceMove)
{
	if (map->StartObjectMove(source, dest, forceMove))
	{
		// Check if a actor is being moved, if so then update the actor's centreLocation.
		for (u32 i = 0; i < actors.size(); i ++)
		{
			Actor &c = actors[i];
			
			if (c.centreLocationSet)
			{
				// Update centreLocation if it is this actor being moved.
				if (source == c.centreLocation)
				{
					ASSERT (map->GetObject(dest) == c.entity);
					
					c.centreLocation = dest;
				}
			}
		}
		
		MapObjectMove move;
		move.startTimeMs = time_in_ms(world->GetVirtualTime());
		move.finishTimeMs = move.startTimeMs + 250;
		move.startMapCoord = source;
		ASSERT( map->GetObject(dest) );
		move.startMapPos = map->GetObject(dest)->GetPosition();
		move.endMapCoord = dest;
		
		mapObjectMoves.push_back(move);
		
		OnStartMove(dest);
		
		return true;
	}
	return false;
}

void Level::OnPause()
{
	IUpdatable::OnPause();
	
	ClearTutorialTextElements();
	
	if (bgAmbientSound)
		bgAmbientSound->Pause();
	
	if (bgMusic)
		bgMusic->Pause();
	
	sound->Pause();
	
	if (mainState)
		mainState->OnPause();
	
	// defaultEvent wants pausing since it has a sound (fallblock explosion)
	defaultEvent->Pause();
}

void Level::OnResume()
{
	IUpdatable::OnResume();
	
	if (bgAmbientSound)
		bgAmbientSound->Resume();
	
	if (bgMusic)
		bgMusic->Resume();
	
	sound->Resume();
	
	if (mainState)
		mainState->OnResume();
	
	defaultEvent->Resume();
}

void Level::StartNextLevelTransition(bool stopMoving)
{
	// This method is hacked together so the next level transition can be called
	// from MainState as part of the ending cutscene.
	
	if (touchedEndLevelPortal)
		return;
	
	touchedEndLevelPortal = true;
	
	world->SetCameraController(NULL);
	//thirdPersonCamera->Follow(portal);
	
	GetPlayer()->SetController(NULL);
	
	// stopMoving flag hacked in.
	// We don't want the player to stop moving when ending in the final scenes.
	// (character should keep walking as the screen fades out)
	if (stopMoving)
		GetPlayer()->StopMoving();
	
	//GetPlayer()->GetMesh()->SetVisible(false);
	
	
	// Do we process the end fade or not?
	
	core::stringc name = GetShortName();
	
	if (name == "intro.lev" || name == "finalscene1.lev" || name == "finalscene2.lev")
	{
		// Fade off
		// Next level is triggered by ScreenFadeFinished event.
		renderSystem->ScreenFade(0.0, 2, true);
	}
	else
	{
		// Don't fade off
		// also we don't want to process any future fade-offs
		// (since they may be generated by EndLevelScreen)
		// EndLevelScreen now has control and will trigger the next level.
		engine->UnregisterEventInterest(this, "ScreenFadeFinished");
		ShowEndLevelScreen();
	}
}

class RemoveAnimator : public IMotionAnimator
{
	IUpdatable *localUpdatable;
	f32 removeTime;
	
public:
	RemoveAnimator(IUpdatable *localUpdatable, f32 lifeTime)
		: localUpdatable(localUpdatable)
	{
		removeTime = localUpdatable->GetVirtualTime() + lifeTime;
	}
	
	void Init(ITransformable *target) {}
	
	void Animate(ITransformable *target, f32 dt)
	{
		if (localUpdatable->GetVirtualTime() > removeTime)
		{
			GetEngine()->GetWorld()->QueueForRemoval(target);
			return;
		}
	}
};

void Level::TouchedEndLevelPortal(ITransformable *portal)
{
	portalTouchedTime = world->GetUpdater().GetVirtualTime();
	
	StartNextLevelTransition();
	
	
	// sound
	sound->SetPosition(portal->GetAbsolutePosition());
	sound->Play("../projects/Puzzle/media/sfx/bell.ogg");
	
	// Player teleport effect
	for (u32 i = 0; i < 40; i ++)
	{
		IMesh *mesh = world->AddMesh("teleport.irrmesh");
		//endLevelPlayerEffects.push_back(mesh);
		
		//mesh->GetMaterial(0).Lighting = false;
		
		ApplyPlainAlphaShaders(mesh, 15 + rand()%40);
		
		f32 bottom = 0.3;
		f32 top = 1.0;
		
		mesh->SetRotation( core::vector3df(0,rand()%360,0) );
		mesh->SetPosition( GetPlayer()->GetAbsolutePosition()
				+ core::vector3df(0, -0.5 + bottom + (top-bottom)*(rand()%100)*0.01, 0) );
		
		f32 minSize = 0.45;
		f32 maxSize = 0.65;
		mesh->Scale( core::vector3df(1,1,1) * minSize + (maxSize-minSize) * (rand()%100)*0.01 );
		
		mesh->ApplyTransformNow();
		
		IMotionAnimator *anim = world->CreateRotationAnimator( core::vector3df(0,-150.0,0)
				* ( 1.0 + (rand()%200)*0.01 )
				);
		mesh->AddAnimator(anim);
		anim->drop();
		
		// new: delete anim after random period.
		// (this is because now level doesn't always fade off, we sometimes see the end level screen + scores)
		anim = new RemoveAnimator(this, 1.f + 1.5f * (rand()%100)*0.01);
		mesh->AddAnimator(anim);
		anim->drop();
	}
}

#include "GUIPane.h"

class TutorialWait : public IWaitLogic
{
	Level *level;
	f32 displayTime;
	
	core::vector3df originalPlayerPos;
	
public:
	TutorialWait(Level *level, f32 displayTime)
		: level(level), displayTime(displayTime)
	{
		originalPlayerPos = level->GetPlayer()->GetPosition();
	}
	
	bool IsWaitOver(IEventQueue *eventQueue, f32 waitStartTime)
	{
		// (waitTime has passed AND a certain distance has been moved by the player)
		// OR (a next tutorial item is queued and waiting)
		// OR (mouse left is down)
		return (eventQueue->GetVirtualTime() > waitStartTime + displayTime
				&& level->GetPlayer()->GetPosition().getDistanceFrom(originalPlayerPos) > 1.5f)
				||
				eventQueue->IsEventWaiting("TutorialShow")
				||
				GetEngine()->GetButtonState(KEY_LBUTTON)
				||
				GetEngine()->GetButtonState(KEY_RBUTTON)
				;
	}
};

void Level::HandleTutorialEvents(const Event &event)
{
	if (event.IsType("TutorialShow"))
	{		
		std::vector<core::stringc> lines = event["lines"];
		
		// Disable mouse button zoom!
		// (since user might try pressing mouse button to skip tutorial)
		world->GetInputProfile()->BindButtonAsAxis(2, KEY_LBUTTON, 0.f);
		world->GetInputProfile()->BindButtonAsAxis(2, KEY_RBUTTON, 0.f);
		
		// Stop movement to encourage player to read.
		GetPlayer()->StopMoving();
		IInputProfile *inputProfile = world->GetInputProfile();
		inputProfile->ForceButtonState(0, false);
		inputProfile->ForceButtonState(1, false);
		inputProfile->ForceButtonState(2, false);
		inputProfile->ForceButtonState(3, false);
		
		video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
		
		s32 screenWidth = driver->getScreenSize().Width;
		s32 screenHeight = driver->getScreenSize().Height;
		
		if (lines.size()) // probably not needed
		{
			gui::IGUIStaticText *text;
			s32 bottomY = (s32)(screenHeight * 0.8);
			s32 y = bottomY;
			
			// add bg first so it's rendered below texts
			gui::IGUIElement *bg = new GUIPane(engine->GetIrrlichtDevice()->getGUIEnvironment()->getRootGUIElement(),
					core::recti(0,0,11,11), video::SColor(100, 0,0,0));
			bg->drop();
			
			for (s32 i = lines.size()-1; i >= 0; i --)
			{
				text = add_static_text( core::stringw(lines[i]).c_str() );
				core::rect<s32> rect = text->getRelativePosition();
				rect += core::vector2di( screenWidth/2-rect.getWidth()/2, y - rect.getHeight() );
				text->setRelativePosition(rect);
				tutorialTextElements.push_back(text);
				
				text->setOverrideColor(video::SColor(150, 255,255,255));
				
				y -= rect.getHeight();
			}
			
			const s32 padding = 10;
			
			// now actually size bg
			bg->setRelativePosition( core::recti(-10,y-padding, screenWidth+10,bottomY+padding) );
			
			tutorialTextElements.push_back(bg);
		}
	}
	else if (event.IsType("TutorialFadeOn"))
	{
		for (u32 i = 0; i < tutorialTextElements.size(); i ++)
		{
			GUIElementFade *fade = new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
					tutorialTextElements[i], this, event["fade_time"], event["fade_time"], false);
			fade->drop();
			
			// hack. run once, so fade is initialised to zero.
			fade->OnPostRender(0);
		}
	}
	else if (event.IsType("TutorialFadeOff"))
	{
		for (u32 i = 0; i < tutorialTextElements.size(); i ++)
		{
			GUIElementFade *fade = new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
					tutorialTextElements[i], this, event["fade_time"], event["fade_time"], true);
			fade->drop();
		}
	}
	else if (event.IsType("TutorialDelete"))
	{
		// Re-enable mouse button zoom.
		world->GetInputProfile()->BindButtonAsAxis(2, KEY_LBUTTON, 2.0);
		world->GetInputProfile()->BindButtonAsAxis(2, KEY_RBUTTON, -2.0);
		
		ClearTutorialTextElements();
	}
}

const LevelStats &Level::GetLevelStats()
{
	return stats;
}

void Level::Update(f32 dt)
{
	IUpdatable::Update(dt);
	
	if (!IsPaused())
		timeSinceLastUndoSave += dt;
	
	core::vector3df playerPos = GetPlayer()->GetPosition();
	core::vector3di playerMapPos = GetCoord( GetPlayer()->GetPosition() );
	
	
	if (logicEnabled)
	{
		
		// Tutorial text trigger processing!
		if (tutorialTexts.size())
		{
			for (u32 i = 0; i < tutorialTexts.size(); i ++)
			{
				bool isHit = false;
				const TutorialText &tt = tutorialTexts[i];
				
				switch (tt.type)
				{
				case TTT_NORMAL:
					if (playerMapPos == tt.trigger)
						isHit = true;
					break;
				case TTT_X:
					if (playerMapPos.X == tt.trigger.X)
						isHit = true;
					break;
				case TTT_Y:
					if (playerMapPos.Y == tt.trigger.Y)
						isHit = true;
					break;
				case TTT_Z:
					if (playerMapPos.Z == tt.trigger.Z)
						isHit = true;
					break;
				case TTT_SURROUND:
					{
						std::vector<core::vector3di> surrounding;
						surrounding.push_back(tt.trigger + core::vector3di(1,0,1));
						surrounding.push_back(tt.trigger + core::vector3di(1,0,0));
						surrounding.push_back(tt.trigger + core::vector3di(1,0,-1));
						surrounding.push_back(tt.trigger + core::vector3di(0,0,-1));
						surrounding.push_back(tt.trigger + core::vector3di(-1,0,-1));
						surrounding.push_back(tt.trigger + core::vector3di(-1,0,0));
						surrounding.push_back(tt.trigger + core::vector3di(-1,0,1));
						surrounding.push_back(tt.trigger + core::vector3di(0,0,1));
						
						for (u32 i = 0; i < surrounding.size(); i ++)
						{
							if (surrounding[i] == playerMapPos)
							{
								isHit = true;
								break;
							}
						}
					}
					break;
				}
				
				// Trigger is hit?
				// Then add the tutorial text to queue!
				// We just queue stuff here, the actual work is done when events are received in OnEvent.
				if (isHit)
				{
					ASSERT(tt.lines.size());
					
					const f32 fadeTime = 0.5f;
					
					// Trigger delay? (before text is displayed)
					if (tt.delayTime > 0.f)
						tutorialEventQueue->AddTimeWait(tt.delayTime);
					
					// Show tutorial
					{
						Event event("TutorialShow");
						event["lines"] = tt.lines;
						tutorialEventQueue->AddEvent(event);
					}
					
					// Fade text on
					// (maybe this should be combined with the above event??)
					{
						Event event("TutorialFadeOn");
						event["fade_time"] = fadeTime;
						tutorialEventQueue->AddEvent(event);
					}
					
					// Fade on time
					tutorialEventQueue->AddTimeWait(fadeTime);
					
					// Logic wait. More complex logic that decides when the tutorial
					// text should be removed. (based on time and distance moved by player)
					IWaitLogic *waitLogic = new TutorialWait(this, tt.displayTime);
					tutorialEventQueue->AddLogicWait(waitLogic);
					waitLogic->drop();
					
					// Fade text off
					{
						Event event("TutorialFadeOff");
						event["fade_time"] = fadeTime;
						tutorialEventQueue->AddEvent(event);
					}
					
					// Fade off time
					tutorialEventQueue->AddTimeWait(fadeTime);
					
					// And delete the tutorial text from the screen.
					// More tutorial texts can then follow on from this in the queue.
					{
						Event event("TutorialDelete");
						tutorialEventQueue->AddEvent(event);
					}
					
					// We've created all necessary events, so remove tutorial item from list.
					tutorialTexts.erase(tutorialTexts.begin()+i);
					i --;
				}
			}
		}
		
		
		// Check for end of level
		
		if (map->GetObject(playerMapPos) && map->GetObjectType(playerMapPos) == EOT_EXIT_PORTAL)
		{
			if (!touchedEndLevelPortal)
			{
				TouchedEndLevelPortal(map->GetObject(playerMapPos));
				
				// Change player to a transparent vertex alpha material so it can
				// be faded out.
				// (we don't use this material all the time as it does not go well with semi
				// transparent sea on the final scene)
				ApplyDefaultShaders(GetPlayer()->GetMesh(), video::EMT_TRANSPARENT_VERTEX_ALPHA);
			}
		}
		
		if (touchedEndLevelPortal)
		{
			// this ought to be interpolated...!?
			
			f32 alpha = (world->GetUpdater().GetVirtualTime() - portalTouchedTime)/2.0;
			
			alpha *= 4.f;
			
			if (alpha < 0.f)
				alpha = 0.f;
			
			if (alpha > 1.f)
			{
				alpha = 1.f;
				GetPlayer()->GetMesh()->SetVisible(false);
			}
			
			SetPlayerAlpha( 255 - u8(alpha * 255) );
		}
		
		// Check for fallen too far
		
		if (playerPos.Y < lowestPoint)
		{
			GetPlayer()->SetController(NULL);
			GetPlayer()->StopMoving();
			
			
			// This stuff done only once
			if (world->GetCameraController())
			{
				//sound->SetPosition(playerPos);
				//sound->Play("../projects/Puzzle/media/sfx/fall.ogg");
				
				//GetPlayer()->SetRotation(core::vector3df(-90,180,0));
				GetPlayer()->SetAnimations(ANIM_FALL, -1);
			}
			
			world->SetCameraController(NULL);
			
			// HACK.
			// Rotate to watch character as it falls.
			// Probably should have proper camera->Follow method?
			// with movement in graphics task...
			
			core::vector3df camToPlayer = (GetPlayer()->GetPosition()-world->GetCamera()->GetPosition()).normalize();
			
			// only rotate if not too close to vertical
			// (there are camera glitches at vertical)
			if ( camToPlayer.Y < 0.99 && camToPlayer.Y > -0.99 )
				world->GetCamera()->SetTarget( GetPlayer()->GetPosition() );
			
			// Draw ray from player position to camera position.
			// If it collides, move camera.
			
			if (!fallCameraMoved)
			{
				IPhysics *physics = world->GetPhysics();
				
				core::line3df ray( GetPlayer()->GetPosition(), world->GetCamera()->GetPosition() );
				
				// exclude player from collision
				Set<ICollisionGeometry *> excludedGeometries = GetPlayer()->GetBody()->GetCollisionGeometries();
				
				RayCollision collisionResult;
				
				if (physics->RayCastExcluding(ray, excludedGeometries, &collisionResult))
				{
					world->GetCamera()->SetPosition( collisionResult.collision.pos
							- ray.getVector().normalize()*0.5 );
					
					// move only occurs once to avoid jumping around.
					fallCameraMoved = true;
				}
			}
		}
		
		if (playerPos.Y < lowestPoint - FALL_DIST)
		{
			if (mainState)
			{
				//mainState->RestartLevel();
				ApplyUndo(true);
				return;
			}
		}
	}
	
	
	
	// fall animation!? for falling short distances when will land on another platform.
	// (not fallen into space)
	/*
	{
		// Analyse space below player.
		
		core::vector3di belowPlayer = playerMapPos;
		
		bool hitObject = false;
		
		for (u32 i = 0; i < 4; i ++)
		{
			belowPlayer.Y --;
			
			if (map->GetObject(belowPlayer))
			{
				E_OBJECT_TYPE objectType = map->GetObjectType(belowPlayer);
				
				if (objectType == EOT_PLAYER_CENTRE || objectType == EOT_PLAYER_INTERSECTING
						|| objectType == EOT_EXIT_PORTAL)
				{
					// ignore these types
				}
				else
				{
					hitObject = true;
				}
			}
		}
		
		// But before switching animation back from fall anim we would want a minimum fall anim time...
		// Could we just generate an event here?
		// And set the animation back when the event is received?
		// (i.e. a set time in the future)
		// would need to ensure it didn't interfere with other fall animations...
		
		if (hitObject)
			GetPlayer()->SetAnimations(ANIM_IDLE, ANIM_WALK);
		else
			GetPlayer()->SetAnimations(ANIM_FALL_UPRIGHT, ANIM_FALL_UPRIGHT);
	}
	*/
	
	/*
	if (world->GetCameraController() && !isPlayerPushing // not falling off world AND not pushing?
			&& (!map->GetEvent(playerMapPos) || map->GetEvent(playerMapPos)->GetType() != EET_FAN_EVENT)
			)
	{
		// Analyse space below player.
		
		core::vector3di belowPlayer = playerMapPos;
		
		bool hitObject = false;
		
		for (u32 i = 0; i < 3; i ++)
		{
			if (map->GetObject(belowPlayer))
			{
				E_OBJECT_TYPE objectType = map->GetObjectType(belowPlayer);
				
				if (objectType == EOT_PLAYER_CENTRE || objectType == EOT_PLAYER_INTERSECTING
						|| objectType == EOT_EXIT_PORTAL)
				{
					// ignore these types
				}
				else
				{
					hitObject = true;
				}
			}
			
			belowPlayer.Y --;
		}
		
		// But before switching animation back from fall anim we would want a minimum fall anim time...
		// Could we just generate an event here?
		// And set the animation back when the event is received?
		// (i.e. a set time in the future)
		// would need to ensure it didn't interfere with other fall animations...
		
		if (hitObject)
			GetPlayer()->SetAnimations(ANIM_IDLE, ANIM_WALK);
		else
			GetPlayer()->SetAnimations(ANIM_FALL_UPRIGHT, ANIM_FALL_UPRIGHT);
	}
	*/
	
	
	// for footstep sound stuff
	// we want a different footstep sound when walking on balloons.
	// all other object types use the normal footstep noise.
	
	E_OBJECT_TYPE objectBelowPlayer = EOT_UNKNOWN;
	{
		core::vector3di belowPlayer = playerMapPos + core::vector3di(0,-1,0);
		
		if (map->GetObject(belowPlayer))
			objectBelowPlayer = map->GetObjectType(belowPlayer);
	}
	
	
	// is either a balloon and was not previously, or is no longer a balloon and was previously
	bool changedFootStepSoundType =
			(objectBelowPlayer == EOT_BALLOON && lastObjectBelowPlayer != EOT_BALLOON)
			||
			(objectBelowPlayer != EOT_BALLOON && lastObjectBelowPlayer == EOT_BALLOON)
			;
	
	
	// Player footsteps sound effect.
	// When *attempted* move is nonzero. (attempted, so could be walking against a wall and not moving
	// but will still sound)
	
	ISoundSource *footStepSoundSource = GetPlayer()->GetFirstChildOfType<ISoundSource>();
	ASSERT(footStepSoundSource);
	
	ISound3D *footStepSound = footStepSoundSource->GetSound();
	
	if (GetPlayer()->GetAttemptedMove().getLengthSQ() > 0.001
			// and no footstep noise when falling...
			&& (alwaysFootsteps || (objectBelowPlayer != EOT_UNKNOWN
			&& objectBelowPlayer != EOT_PLAYER_INTERSECTING
			&& objectBelowPlayer != EOT_PLAYER_CENTRE))
			)
	{
		footStepSound->SetIsLooped(true);
		
		if (!footStepSound->IsPlaying() || changedFootStepSoundType)
		{
			if (objectBelowPlayer == EOT_BALLOON)
				footStepSound->Play("../projects/Puzzle/media/sfx/stepballoon.ogg");
			else
				footStepSound->Play("../projects/Puzzle/media/sfx/step.ogg");
		}
	}
	else
	{
		// Finish the current loop then stop.
		footStepSound->SetIsLooped(false);
	}
	
	if (localGridBasedMovement)
	{
		// If moving, interpolate to centre of move axis.
		// i.e. if moving in Z, interpolate X.
		
		if (GetPlayer()->GetAttemptedMove().getLengthSQ() > 0.001)
		{
			core::vector3df playerAdjustedPos = GetPosFromCoord(playerMapPos);
			
			if ( fabs(GetPlayer()->GetAttemptedMove().X) > fabs(GetPlayer()->GetAttemptedMove().Z) )
			{
				// Moving in X, interpolate Z to centre.
				f32 x = playerPos.X;
				f32 y = playerPos.Y;
				playerPos = playerPos.getInterpolated(playerAdjustedPos, 0.90);
				playerPos.Y = y;
				playerPos.X = x;
				GetPlayer()->SetPosition(playerPos);
			}
			else
			{
				// Moving in Z, interpoate X to centre.
				f32 z = playerPos.Z;
				f32 y = playerPos.Y;
				playerPos = playerPos.getInterpolated(playerAdjustedPos, 0.90);
				playerPos.Y = y;
				playerPos.Z = z;
				GetPlayer()->SetPosition(playerPos);
			}
		}
	}
	
	
	
	// if we've touched the end level portal we don't want to interfere with the visibility state
	// (since touching portal may make character invisible)
	if (!touchedEndLevelPortal)
	{
		// hide player if camera is closer than a certain distance
		GetPlayer()->GetMesh()->SetVisible( world->GetCamera()->GetTargetDistance() > 0.5
				// Always visible if no player controller is set...
				// (so is visible in intro)
				|| !GetPlayer()->GetController() );
	}
	
	
	// MOVEABLE BLOCK LOGIC
	// get player's current grid location
	// get the four adjacent squares in a cross pattern
	// does player touch any of these? (or is very close?)
	// if so, and if there is a moveable block in that square, movable block should move.
	
	// Adjacent 4 map coordinates (in cross shape)
	// [ ][x][ ]
	// [x][P][x]
	// [ ][x][ ]
	std::vector<core::vector3di> adjacentCross;
	adjacentCross.push_back(playerMapPos + core::vector3di(1,0,0));
	adjacentCross.push_back(playerMapPos + core::vector3di(-1,0,0));
	adjacentCross.push_back(playerMapPos + core::vector3di(0,0,1));
	adjacentCross.push_back(playerMapPos + core::vector3di(0,0,-1));
	
	// Does player touch any?
	// Probably want to first check if any of these actually have a movable block in them.
	
	for (u32 i = 0; i < adjacentCross.size(); i ++)
	{
		core::vector3di above = adjacentCross[i];
		above.Y ++;
		
		if ( map->GetObject(adjacentCross[i])
				// Is actually touching the coordinate...
				&& IsActorTouchingCoord(GetPlayerActor(), adjacentCross[i], 1.05)//1.8)//1.3)// was 1.05
				// Also mustn't push if this is the player's own location!?
				&& (!GetPlayerActor().centreLocationSet || adjacentCross[i] != GetPlayerActor().centreLocation)
				// ...and mustn't push other actors (enemies)
				&& !(map->GetObject(adjacentCross[i]) && map->GetObjectType(adjacentCross[i]) == EOT_PLAYER_CENTRE)
				// And mustn't push if there is a movable object above this one!
				// UPDATE: commented out movable check, now cannot move if any object is above
				//&& !(map->GetObject(above) /*&& map->IsObjectMovable(above)*/)
				// UPDATE2: Now using defaultEvent for this.
				&& defaultEvent->RequestActionPermission(adjacentCross[i], EAT_MOVE_BY_PUSH)
				)
		{
			// BOOYA. Touching.
			// So maybe move said block.
			// If movable.
			
			// But also, need to check that the player has moved (in XZ plane) towards this object (from
			// the last update) rather than away from it. (don't push if moving away or no move).
			
			// current pos + attempted move must be closer than current pos
			// attempted move moving closer to coord
			
			core::vector3df attemptedMove = GetPlayer()->GetAttemptedMove()*0.1;
			core::vector2df currentPlayerPos2D(playerPos.X,playerPos.Z);
			core::vector2df attemptedMove2D(attemptedMove.X,attemptedMove.Z);
			core::vector2df desiredPlayerPos2D = currentPlayerPos2D + attemptedMove2D;
			core::vector2df coordPos( GetPosFromCoord(adjacentCross[i]).X, GetPosFromCoord(adjacentCross[i]).Z );
			
			if (coordPos.getDistanceFromSQ(desiredPlayerPos2D)+0.001 < coordPos.getDistanceFromSQ(currentPlayerPos2D))
			{
				// ALSO, NEW UPDATE: Only push if the player is actually *facing* the block to be moved.
				// We don't want grazing contact to cause a move (even if the player is actually touching and closer)
				// touchDist in the direction of attemptedMove should be within some distance of the block's centre.
				attemptedMove2D.normalize();
				
				f32 touchDist = 0.5 + ACTOR_PHYSICAL_HEIGHT/2.0;
				
				// This was chosen by trial and error...
				f32 someDistance = 0.4;
				
				if ((currentPlayerPos2D + attemptedMove2D*touchDist).getDistanceFrom(coordPos) < someDistance)
				{
					// Vector to move the movable block (if possible and next map coord is unoccupied)
					core::vector3di moveVec = adjacentCross[i] - playerMapPos;
					
					PlayerPushed(adjacentCross[i], moveVec);
				}
			}
		}
	}
	
	
	// to fix a glitch
	bool wasPlayerPush = false;
	
	
	// actually handle the player push.
	// required to be at this particular location.
	if (playerPushEvent.size())
	{
		// A short delay since player push was triggered has passed.
		// Now perform the actual block move.
		
		bool isBalloon = map->GetObject(pushStartLoc) && map->GetObjectType(pushStartLoc) == EOT_BALLOON;
		
		if ( StartMapObjectMove(pushStartLoc, pushEndLoc) )
		{
			stats.pushes ++;
			
			sound->SetPosition(GetPosFromCoord(pushStartLoc));
			
			if (isBalloon)
				sound->Play("../projects/Puzzle/media/sfx/balloonpush.ogg");
			else
				sound->Play("../projects/Puzzle/media/sfx/slide.ogg");
			
			
			// To fix a glitch.
			wasPlayerPush = true;
		}
		else
		{
			WARN << "PlayerPushedMove: move failed.";
		}
		
		
		playerPushEvent.clear();
	}
	
	
	
	// ACTOR OCCUPATION LOGIC
	// Determine and take ownership (if possible) of any squares an actor intersects.
	// Similar to above, but using the surrounding eight squares.
	// Also attempt to set or move the actor centre location, which is a movable block.
	
	// Remove all previous occupations
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		for (u32 j = 0; j < c.lastLocations.size(); j ++)
		{
			// ASSERT that actor currently occupies this square
			// (only way it wouldn't is if SetObject called elsewhere, or moveobject called with
			// forceMove)
			ASSERT( map->GetObject(c.lastLocations[j]) == c.entity );
			
			// Now remove reference to this actor's entity
			map->SetObject( c.lastLocations[j], NULL, false, EOT_UNKNOWN );
		}
		
		c.lastLocations.clear();
	}
	
	
	// Create the actor centre locations that haven't been created yet.
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		if (!c.centreLocationSet)
		{
			core::vector3di actorMapPos = GetCoord( c.entity->GetPosition() );
			
			if (!map->GetObject(actorMapPos))
			{
				map->SetObject(actorMapPos, c.entity, true, EOT_PLAYER_CENTRE);
				c.centreLocation = actorMapPos;
				c.centreLocationSet = true;
			}
		}
	}
	
	
	// Attempt to move any actor location that the actor has moved to a different location.
	// If the move fails then it will stay where it is...
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		core::vector3di actorMapPos = GetCoord( c.entity->GetPosition() );
		
		if (c.centreLocationSet && actorMapPos != c.centreLocation)
		{
			// Player is just about to move onto actorMapPos.
			// (its grid position is going to change)
			// So maybe cache an undo state, this will only be saved if something bad happens as
			// a result of the following move.
			if (map->ObjectMoveWillComplete(c.centreLocation, actorMapPos, false))
			{
				// Move has not actually been done yet, so we can save here.
				// We want to see if the new actorMapPos is dangerous!
				
				// only ever save if is on solid ground
				// (using old centre pos, not actorMapPos)
				
				bool willSaveUndoState = false;
				bool wasOnSolidGround = false;
				
				// go down until we find something that cannot fall
				// (since we might be standing on a stack)
				
				core::vector3di checkGround = c.centreLocation;
				
				while (true)
				{
					checkGround.Y --;
					
					if (!map->GetObject(checkGround))
						break;
					
					// Is solid?
					E_OBJECT_TYPE checkType = map->GetObjectType(checkGround);
					
					if (checkType == EOT_GROUND_BLOCK || checkType == EOT_LIFT
						|| checkType == EOT_BALLOON || checkType == EOT_GROUND_BLOCK_FALL)
					{
						wasOnSolidGround = true;
						break;
					}
				}
				
				if (wasOnSolidGround)
				{
					core::vector3di below = actorMapPos + core::vector3di(0,-1,0);
					core::vector3di belowLast = c.centreLocation + core::vector3di(0,-1,0);
					
					// Save for Undo if just fallen!
					// But only once, when first fall is detected.
					if (objectBelowPlayer == EOT_UNKNOWN && lastObjectBelowPlayer != EOT_UNKNOWN)
					{
						willSaveUndoState = true;
						
						// force save
						timeSinceLastUndoSave = 10000.f;
					}
					// or moving into a fan event
					// but was not previously on a fan event!!
					else if (map->GetEvent(actorMapPos) && map->GetEvent(actorMapPos)->GetType() == EET_FAN_EVENT
							&& (!map->GetEvent(c.centreLocation) || map->GetEvent(c.centreLocation)->GetType() != EET_FAN_EVENT)
							)
					{
						willSaveUndoState = true;
						
						// force save
						timeSinceLastUndoSave = 10000.f;
					}
					// or we are on a lift, and weren't previously on the *SAME* lift
					// (but we *do* save before stepping from one lift to a new one)
					// if X and Y are not the same as before, we've moved in the ground plane
					// so are on a different lift.
					else if (map->GetObject(below) && map->GetObjectType(below) == EOT_LIFT
							&& !(actorMapPos.X == c.centreLocation.X && actorMapPos.Z == c.centreLocation.Z)
							)
					{
						willSaveUndoState = true;
					}
					// or we are just moved on to an exploding block
					else if (map->GetObject(below) && map->GetObjectType(below) == EOT_GROUND_BLOCK_FALL)
					{
						willSaveUndoState = true;
					}
				}
				
				if (willSaveUndoState)
					SaveUndoState(CreateUndoState());
			}
			
			// If the destination contains an event, and that event DENIES LOCOMOTION,
			// this is a special case and we want to do an interpolated move into that square.
			
			//if (map->GetEvent(actorMapPos)
			//		&& !map->GetEvent(actorMapPos)->RequestActionPermission(actorMapPos, EAT_LOCOMOTE))
			if (!defaultEvent->RequestActionPermission(actorMapPos, EAT_LOCOMOTE))
			{
				StartMapObjectMove(c.centreLocation, actorMapPos);
			}
			else
			{
				// Normal behaviour.
				// Actor takes control of square without interpolated movement into the centre.
				
				if (map->StartObjectMove(c.centreLocation, actorMapPos))
				{
					// Move success!
					map->FinishObjectMove(actorMapPos);
					
					OnStartMove(actorMapPos);
					OnEnterLocation(actorMapPos);
					OnLeaveLocation(c.centreLocation, actorMapPos);
					
					c.centreLocation = actorMapPos;
				}
				else
				{
					// Move failed.
				}
			}
		}
	}
	
	
	// Occupy new actor intersecting locations...
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		// Get possible candidate locations for occupation
		std::vector<core::vector3di> possibleActorLocations = GetActorIntersectingCoords(c);
		
		for (u32 j = 0; j < possibleActorLocations.size(); j ++)
		{
			core::vector3di &loc = possibleActorLocations[j];
			
			// Occupy provided there is not already an object there!
			if ( !map->GetObject(loc)
					
					// FIX:
					// Also don't occupy this location if we just pushed an object out from there.
					// (since there may be a stack above that ought to fall into this location)
					&& (!wasPlayerPush || pushStartLoc != loc)
					)
			{
				map->SetObject(loc, c.entity, false, EOT_PLAYER_INTERSECTING);
				
				// Remember so can be cleared next update
				c.lastLocations.push_back(loc);
			}
		}
	}
	
	
	// If actor is on an event square and that event denies gravity... then disable gravity!
	// Otherwise enabled it.
	// Maybe should also zero downwards velocity...
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		core::vector3di actorMapPos = GetCoord( c.entity->GetPosition() );
		
		c.GetCharacter()->SetGravityEnabled( defaultEvent->RequestActionPermission(actorMapPos, EAT_FALL) );
			
		// Also check for locomotion here. if locomotion is not allowed, then reposition actor
		// on square centre.
		
		if ( !defaultEvent->RequestActionPermission(actorMapPos, EAT_LOCOMOTE)
				// Only deny locomotion if player is actually exactly on a location.
				// (i.e. they have finished traversing into it)
				&& !map->IsTraversing(actorMapPos)
				//&& c.entity->GetPosition() == GetPosFromCoord(actorMapPos)
				)
		{
			c.entity->SetPosition( GetPosFromCoord(actorMapPos) );
		}
		
		if (!defaultEvent->RequestActionPermission(actorMapPos, EAT_LOCOMOTE))
		{
			// we can't locomote. We're stuck?
			// bob animation for fan is set in FanEvent
			GetPlayer()->SetController(NULL);
			GetPlayer()->ClearMotion();
		}
		
		
		/*
		if (IMapEventOwner *event = map->GetEvent(actorMapPos))
		{
			c.GetCharacter()->SetGravityEnabled( event->RequestActionPermission(actorMapPos, EAT_FALL) );
			
			// Also check for locomotion here. if locomotion is not allowed, then reposition actor
			// on square centre.
			
			if ( !event->RequestActionPermission(actorMapPos, EAT_LOCOMOTE)
					// Only deny locomotion if player is actually exactly on a location.
					// (i.e. they have finished traversing into it)
					&& !map->IsTraversing(actorMapPos)
					//&& c.entity->GetPosition() == GetPosFromCoord(actorMapPos)
					)
			{
				c.entity->SetPosition( GetPosFromCoord(actorMapPos) );
			}
			
			if (!event->RequestActionPermission(actorMapPos, EAT_LOCOMOTE))
			{
				// we can't locomote. We're stuck?
				// bob animation for fan is set in FanEvent
				GetPlayer()->SetController(NULL);
				GetPlayer()->ClearMotion();
			}
		}
		else
		{
			c.GetCharacter()->SetGravityEnabled(true);
		}
		*/
	}
	
	
	
	// Update move animations
	// if any are finished, call map->FinishObjectMove
	
	for (u32 i = 0; i < mapObjectMoves.size(); i ++)
	{
		MapObjectMove &move = mapObjectMoves[i];
		
		ITransformable *object = map->GetObject(move.endMapCoord);
		
		ASSERT( object );
		
		if (time_in_ms(world->GetVirtualTime()) > move.finishTimeMs)
		{
			// position exactly
			object->SetPosition( GetPosFromCoord(move.endMapCoord) );
			
			// finish move (traversal over)
			map->FinishObjectMove(move.endMapCoord);
			
			// Object has entered a new square!
			// So call that square's event if one is set.
			OnEnterLocation(move.endMapCoord);
			
			// And has completely left the previous location.
			OnLeaveLocation(move.startMapCoord, move.endMapCoord);
			
			// remove from list
			mapObjectMoves.erase(mapObjectMoves.begin() + i);
			i --;
		}
		else
		{
			// Move still in progress.
			// Perform an interpolation from startMapCoord to destMapCoord.
			
			core::vector3df startPos = move.startMapPos;//GetPosFromCoord(move.startMapCoord);
			core::vector3df endPos = GetPosFromCoord(move.endMapCoord);
			
			u32 timeSoFar = time_in_ms(world->GetVirtualTime()) - move.startTimeMs;
			u32 totalMoveTime = move.finishTimeMs - move.startTimeMs;
			
			f32 interpolation = 1.0 - ((f32)timeSoFar / (f32)totalMoveTime);
			
			object->SetPosition( startPos.getInterpolated(endPos, interpolation) );
		}
	}
	
	
	// Logic for all objects that have entered new locations.
	// Location entry occurs once on creation of each object (AddObject)
	// and also every time an object moves to a new location.
	
	if (logicEnabled) // But don't do this if in editor.
	{
		ASSERT( locationsLeft.size() == locationsLeftNew.size() );
		
		// NEW: Leave a location.
		for (u32 i = 0; i < locationsLeft.size(); i ++)
		{
			core::vector3di &coord = locationsLeft[i];
			
			IMapEventOwner *event = map->GetEvent(coord);
			
			if (event)
				event->OnMapLeaveEvent( coord, locationsLeftNew[i] );
				
			// Default event too.
			defaultEvent->OnMapLeaveEvent( coord, locationsLeftNew[i] );
		}
		
		// NEW: Just started to enter a location.
		// I.e. just leaving a previous location.
		// (previous location cached in map)
		for (u32 i = 0; i < locationsStartedToEnter.size(); i ++)
		{
			core::vector3di &coord = locationsStartedToEnter[i];
			
			IMapEventOwner *event = map->GetEvent(coord);
			
			if (event)
				event->OnMapMoveEvent(coord);
			
			defaultEvent->OnMapMoveEvent(coord);
		}
		
		
		// Originally this was the only event we had.
		// Called when an object has completely finished moving in to a new position.
		for (u32 i = 0; i < locationsEntered.size(); i ++)
		{
			core::vector3di &coord = locationsEntered[i];
			
			IMapEventOwner *event = map->GetEvent(coord);
			
			// SHOULD INITIATE ANY NEW ACTIONS HERE.
			// e.g. falling.
			// MUST ALSO ASK EVENT OWNER FOR PERMISSION TO PERFORM THE ACTION, IF EVENT IS SET.
			// event->RequestActionPermission(...)
			
			
			ITransformable *object = map->GetObject(coord);
			
			// Call event if set
			// Called before gravity etc, as gravity may move the object, skipping event.
			
			if (event)
				event->OnMapEvent(coord);
			
			// Only fall if the object has not moved/changed as a result of the event
			
			if (map->GetObject(coord) == object)
				PerhapsGravity(coord);
			
			// NEW: A default event for all squares.
			// This is kind of hacked in.
			defaultEvent->OnMapEvent(coord);
		}
		
		// Update: Gravity is also done by brute force here. This is so gravity is applied when no
		// location is entered (i.e. the object underneath this one disappears).
		// Gravity application in locationsEntered above is probably no longer needed, but has been left just in case.
		
		std::vector<core::vector3di> allObjects = map->GetAllObjects();
		
		for (u32 i = 0; i < allObjects.size(); i ++)
			PerhapsGravity(allObjects[i]);
	}
	
	locationsEntered.clear();
	locationsStartedToEnter.clear();
	locationsLeft.clear();
	locationsLeftNew.clear();
	
	lastObjectBelowPlayer = objectBelowPlayer;
}

// this function is an ugly hack.
void Level::ClearPlayerMapObjectMove()
{
	// see if player is moving.
	
	for (u32 i = 0; i < mapObjectMoves.size(); i ++)
	{
		MapObjectMove &move = mapObjectMoves[i];
		
		ITransformable *object = map->GetObject(move.endMapCoord);
		
		ASSERT( object );
		
		// this is the player entity?
		// (is wrapped in a proxy)
		if (object == GetPlayerActor().entity)
		{
			NOTE << "Clearing player map object move.";
			
			// following stuff is copied from mapObjectMove processing
			// in Update.
			// We finish the move.
			
			
			// position exactly
			object->SetPosition( GetPosFromCoord(move.endMapCoord) );
			
			// finish move (traversal over)
			map->FinishObjectMove(move.endMapCoord);
			
			// Object has entered a new square!
			// So call that square's event if one is set.
			OnEnterLocation(move.endMapCoord);
			
			// And has completely left the previous location.
			OnLeaveLocation(move.startMapCoord, move.endMapCoord);
			
			// remove from list
			mapObjectMoves.erase(mapObjectMoves.begin() + i);
			
			// And finish.
			return;
		}
	}
}

void Level::ShovePlayerOutOfTheWay(core::vector3di coord)
{
	// Actually, this will work on any actor, not just the player.
	
	// no object? return
	if (!map->GetObject(coord))
		return;
	
	// Not a character intersected square? return
	if (map->GetObjectType(coord) != EOT_PLAYER_INTERSECTING)
		return;
	
	// So find the actor that is intersecting this square.
	// (if any...)
	for (u32 i = 0; i < actors.size(); i ++)
	{
		Actor &c = actors[i];
		
		if (c.entity == map->GetObject(coord))
		{
			// Found the actor, now find its record of this coord (in lastLocations).
			for (u32 j = 0; j < c.lastLocations.size(); j ++)
			{
				// Found it. The location is indeed occupied by the player.
				if (c.lastLocations[j] == coord)
				{
					// Remove reference to this actor's entity
					// (same as when refreshing actor occupied locations in Update)
					map->SetObject( c.lastLocations[j], NULL, false, EOT_UNKNOWN );
					
					// and remove the record of it
					c.lastLocations.erase(c.lastLocations.begin()+j);
					
					return;
				}
			}
			
			WARN << "shouldn't get here...";
		}
	}
}







