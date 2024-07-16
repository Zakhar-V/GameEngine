#pragma once

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

#include "Graphics.hpp"

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


//---------------------------------------------------------------------------//
//  
//---------------------------------------------------------------------------//

struct _Model
{
	struct Bone
	{
		
	};

	struct Mesh
	{

	};

	struct Anim
	{

	};
};

struct KeyFrame
{
	float frame;
	union
	{
		Vector3 pos;
		Quaternion rot;
		Vector3 scl;
	};
};

struct Animation
{

};

//---------------------------------------------------------------------------//
// Material 
//---------------------------------------------------------------------------//

//!
class Material : public Resource
{
public:
	RTTI(Material);

protected:
	SharedPtr<Texture> m_texture;
	Color m_color;
};

//---------------------------------------------------------------------------//
// SubMesh 
//---------------------------------------------------------------------------//

class SubMesh : public Object
{
public:
	RTTI(SubMesh);

	void Draw(void);

protected:
	friend class Mesh;

	Mesh* m_parent = nullptr;
	std::string m_name;
	uint m_start = 0;
	uint m_size = 0;
	SharedPtr<Material> m_material;

	//SharedPtr<Buffer> m_sharedVertexBuffer;
	//SharedPtr<Buffer> m_sharedIndexBuffer;
};

//---------------------------------------------------------------------------//
// Mesh 
//---------------------------------------------------------------------------//

//!
class Mesh : public Resource
{
public:
	RTTI(Mesh);

	/*struct Part
	{
		//std::string name;
		uint start = 0;
		uint count = 0;
		uint material = 0;
	};*/

	//!
	bool Load(File* _src) override;


	SubMesh* AddSubMesh(const std::string& _name, uint _start, uint _size, Material* _material);


	void _Bind(void);

	void _Draw(uint _part);

	void _DrawAll();

protected:

	std::vector <SharedPtr<SubMesh>> m_submeshes;
	//!
	//std::vector<Part> m_parts;
	//!
	std::vector<SharedPtr<Material>> m_materials;
	//!
	SharedPtr<Buffer> m_vertices;
	//!
	SharedPtr<Buffer> m_indices;
};


//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
