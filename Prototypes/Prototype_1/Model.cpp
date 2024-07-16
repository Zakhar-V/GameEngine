#include "Model.hpp"
#include "Time.hpp"

//---------------------------------------------------------------------------//
// OBJs 
//---------------------------------------------------------------------------//

struct OBJ
{
	struct FaceVertex
	{
		int v, tc, n;
		int idx = -1;

		bool operator == (const FaceVertex& _rhs) const { return v == _rhs.v && tc == _rhs.tc && n == _rhs.n; }
	};


	struct Vertex
	{
		Vector3 xyz;
		Vector4 rgb; // a = 1
	};
};


namespace std
{
	template <> struct hash<OBJ::FaceVertex>
	{
		size_t operator() (const OBJ::FaceVertex& _val)	const noexcept
		{
			size_t _hash = 0;

			// SDBM hash
			for (int i = 0; i < sizeof(OBJ::FaceVertex::v) * 3; ++i)
				_hash = reinterpret_cast<const char*>(&_val)[i] + (_hash << 6) + (_hash << 16) - _hash;

			return _hash;
		}
	};
}

//---------------------------------------------------------------------------//
// Mesh
//---------------------------------------------------------------------------//


//---------------------------------------------------------------------------//
void Mesh::_Bind(void)
{
	gGraphics->SetVertexBuffer(m_vertices);
	gGraphics->SetIndexBuffer(m_indices);
}
//---------------------------------------------------------------------------//
void Mesh::_DrawAll()
{
	for(auto s : m_submeshes)
		gGraphics->DrawIndexed(PrimitiveType::Triangle, s->m_start, s->m_size);

}
//---------------------------------------------------------------------------//
void Mesh::_Draw(uint _part)
{
	SubMesh* _s = m_submeshes[_part];
	gGraphics->DrawIndexed(PrimitiveType::Triangle, _s->m_start, _s->m_size);
}
//---------------------------------------------------------------------------//
bool Mesh::Load(File* _src)
{
	ASSERT(_src != nullptr);
	float _time = gTimer->CurrentTime();
	
	std::string _str = _src->ReadString();

	const char* _iter = _str.c_str();
	

	std::vector<OBJ::Vertex> _srcVertex;
	std::vector<Vector3> _srcNormal;
	std::vector<Vector2> _srcTexcoord;
	std::unordered_set<OBJ::FaceVertex> _srcVertexMap;

	//std::unordered_map<std::string, SharedPtr<Material>> _srcMaterialMap;

	std::vector<Vertex> _dstVertex;
	std::vector<uint> _dstIndex;
	//Mesh::Part _dstPart;

	// [TEMP VARIABLES]
	Vertex _vert;
	Vector3 _xyz;
	OBJ::Vertex _xyzrgb;
	OBJ::FaceVertex _fvert;
	std::vector<OBJ::FaceVertex> _face;
	std::string _object, _usemtl, _mtllib, _newmtl;
	std::string _line, _token;
	int _lineIdx = 1;
	std::vector<int> _triangles;


	// []
	
	m_materials.clear();

	while (m_submeshes.size())
	{
		m_submeshes.back()->m_parent = nullptr;
		m_submeshes.pop_back();
	}
	SharedPtr<Material> _currentMaterial;

	std::string _prevToken;

	while (*_iter)
	{
		_prevToken = _token;
		_line.clear();
		_token.clear();

		while (!strchr("\n\r", *_iter)) _line.push_back(*_iter++); // read line
		while (*_iter && strchr("\n\r", *++_iter));	 // skip new line char

		const char* i = _line.c_str();

		if (*i == '#') continue; // comment
			
		while (*i && *i != ' ') _token.push_back(*i++);	// read token

		while (*i == ' ') i++; // skip white space

		//LOG("%s %s", _token.c_str(), i);

		if (_token == "o") // object
		{
			_object = i;
		}
		else if (_token == "v")	// vertex
		{
			if (sscanf(i, "%f %f %f", &_xyzrgb.xyz.x, &_xyzrgb.xyz.y, &_xyzrgb.xyz.z) != 3) // position
				LOG("OBJ parser error at %s:%d", _src->Name().c_str(), _lineIdx);

			for (int j = 0; j < 3; ++j)
			{
				while (*i && strchr("0123456789.-+eE", *i++)); // skip vertex position
				while (*i == ' ') i++; // skip white space
			}

			if (!*i || sscanf(i, "%f %f %f", &_xyzrgb.rgb.x, &_xyzrgb.rgb.y, &_xyzrgb.rgb.z) != 3) // color (optional)
				_xyzrgb.rgb = Vector4::One;

			_srcVertex.push_back(_xyzrgb);

		}
		else if (_token == "vn") // normal
		{
			if (sscanf(i, "%f %f %f", &_xyz.x, &_xyz.y, &_xyz.z) != 3)
				LOG("OBJ parser error at %s:%d", _src->Name().c_str(), _lineIdx);

			_srcNormal.push_back(_xyz);

		}
		else if (_token == "vt") // texcoord
		{
			if (sscanf(i, "%f %f", &_xyz.x, &_xyz.y) != 2)
				LOG("OBJ parser error at %s:%d", _src->Name().c_str(), _lineIdx);

			_srcTexcoord.push_back(_xyz);

		}
		else if (_token == "f") // face
		{
			if (_prevToken == "s")
			{
				m_submeshes.push_back(new SubMesh());
				m_submeshes.back()->m_parent = this;
				m_submeshes.back()->m_name = _object + "_" + _usemtl;
				m_submeshes.back()->m_start = _dstIndex.size();
			}

			_face.clear();

			ASSERT(m_submeshes.size() > 0);	// TODO

			do
			{
				while (*i == ' ') i++; // skip white space

				if (sscanf(i, "%d/%d/%d", &_fvert.v, &_fvert.tc, &_fvert.n) == 3) // vertex with texcoord
				{
					// do nothing
				}
				else if (sscanf(i, "%d//%d", &_fvert.v, &_fvert.n) == 2)  // vertex without texcoord
				{
					_fvert.tc = -1;
				}
				else // uknown format
				{
					LOG("OBJ parser error at %s:%d", _src->Name().c_str(), _lineIdx);
				}

				_face.push_back(_fvert);

				while (*i && strchr("0123456789/", *i++)); // skip vertex info

			} while (*i);

			if (_face.size() > 2)
			{
				if (_face.size() > 3) // simple triangulation 
				{
					_triangles.clear();

					for (size_t j = 0; j + 2 < _face.size(); ++j)
					{
						_triangles.push_back(0);
						_triangles.push_back(_face.size() - j - 1);
						_triangles.push_back(_face.size() - j - 2);
					}
				}
				else
				{
					_triangles = { 0, 1, 2 };
				}

				for (auto j : _triangles)
				{
					auto& v = _face[j];
					auto _vf = _srcVertexMap.find(v);
					if ( _vf == _srcVertexMap.end())
					{
						v.idx = (int)_srcVertexMap.size();

						_vert.pos = _srcVertex[v.v - 1].xyz;
						_vert.color = _srcVertex[v.v - 1].rgb;
						_vert.color.a = 0xff;
						_vert.normal = _srcNormal[v.n - 1];
						if(v.tc > 0) _vert.texcoord = _srcTexcoord[v.tc - 1];

						_dstVertex.push_back(_vert); // add vertex
						_dstIndex.push_back(v.idx); // add index
						_srcVertexMap.insert(v);
						m_submeshes.back()->m_size++;
					}
					else
					{
						_dstIndex.push_back(_vf->idx); // add index
						m_submeshes.back()->m_size++;
					}
				}
			}
		}
		else if (_token == "l") // line
		{
			// ignore
		}
		else if (_token == "s") // ???
		{
			// ignore
		}
		else if (_token == "group") // group
		{
			// ignore
		}
		else if (_token == "mtllib") // mtllib
		{
			_mtllib = i;

			File _mtlFile;
			if (_mtlFile.Open(_mtllib))
			{
				ptrdiff_t _pos = _iter - _str.c_str();
				_str += _mtlFile.ReadString();
				_iter = _str.c_str() + _pos;
				LOG("OBJ material library '%s' loaded", _mtllib.c_str());
			}
			else
			{
				LOG("Error: Unable to load OBJ material library '%s'", _mtllib.c_str());
			}
		}
		else if (_token == "usemtl") // usemtl
		{
			_usemtl = i;
														 
			m_submeshes.push_back(new SubMesh());
			m_submeshes.back()->m_parent = this;
			m_submeshes.back()->m_name = _object + "_" + _usemtl;
			m_submeshes.back()->m_start = _dstIndex.size();

			int _mtlidx = 0;
			for (auto j : m_materials)
			{
				if (j->GetName() == _usemtl)
				{
					m_submeshes.back()->m_material = j; // _mtlidx;
					break;
				}
				++_mtlidx;
			}

			if (_mtlidx == m_materials.size())
			{
				//m_parts.back().material = _mtlidx;
				m_materials.push_back(new Material);
				m_materials.back()->SetName(_usemtl);
				m_submeshes.back()->m_material = m_materials.back();
			}
		}
		else if (_token == "newmtl") // newmtl
		{
			_newmtl = i;

			int _mtlidx = 0;
			for (auto j : m_materials)
			{
				if (j->GetName() == _newmtl)
				{
					_currentMaterial = j;
					break;
				}
				++_mtlidx;
			}

			if (_mtlidx == m_materials.size())
			{
				_currentMaterial = new Material;								
				_currentMaterial->SetName(_newmtl);
				m_materials.push_back(_currentMaterial);
			}
		}
		else 
		if (_currentMaterial)
		{
			if (_token == "Ns") // 
			{

			}
			else if (_token == "Ka") // 
			{

			}
			else if (_token == "Kd") // 
			{

			}
			else if (_token == "Ks") // 
			{

			}
			else if (_token == "Ke") // 
			{

			}
			else if (_token == "Ni") // 
			{

			}
			else if (_token == "d") // 
			{

			}
			else if (_token == "illum") // 
			{

			}
			else if (_token == "map_Kd") // diffuse texture
			{

			}

		}

		++_lineIdx;
	}


	// []

	m_vertices = new Buffer;
	m_vertices->Resize(Buffer::Type::Vertex, _dstVertex.size());
	memcpy(m_vertices->Data(), _dstVertex.data(), _dstVertex.size() * sizeof(Vertex));


	m_indices = new Buffer;
	m_indices->Resize(Buffer::Type::Index, _dstIndex.size());
	memcpy(m_indices->Data(), _dstIndex.data(), _dstIndex.size() * sizeof(uint));



	/*int i = 0;
	for (const auto& v : _dstVertex)
	{
		printf("v #%d %.2f %.2f %.2f\n", i, v.pos.x, v.pos.y, v.pos.z);
		i++;
	}

	i = 0;
	for (auto f : _dstIndex)
	{
		if (i % 3 == 0)
			printf("\ni #%d", i/3);

			printf(" %d", f);

			++i;
	}
	printf("\n");

	i = 0;
	for (auto p : m_parts)
	{
		printf("p #%d %d %d with material %d\n", i, p.start, p.count, p.material);
		++i;
	}


	i = 0;
	for (auto m : m_materials)
	{
		printf("m #%d %s\n", i, m->GetName().c_str());
		++i;
	}*/

	// TODO: optimize by material

	/*int i = 0;
	for (auto j : m_submeshes)
	{
		LOG("SUBMESH #%d %s %d %d", i, j->m_name.c_str(), j->m_start, j->m_size);
		++i;
	}*/


	LOG("Mesh \'%s\' loaded, %d vertices, %d faces, %d materials, %d submeshes (%.3f seconds)", _src->Name().c_str(), _dstVertex.size(), _dstIndex.size() / 3, m_materials.size(), m_submeshes.size(), gTimer->CurrentTime() - _time);
	return true;
}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//
