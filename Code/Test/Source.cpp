#include <Engine.hpp>

#include <stdio.h>

namespace Engine
{
	class Stream;


	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//
		
	class Window
	{

	};

	class Device
	{

	};

	//----------------------------------------------------------------------------//
	// Variant
	//----------------------------------------------------------------------------//














	enum class EngineEvent : uint
	{
		LoadSettings = String::ConstHash("EngineEvent::LoadSettings"),
		SaveSettings = String::ConstHash("EngineEvent::SaveSettings"),
		Startup = String::ConstHash("EngineEvent::Startup"),
		Shutdown = String::ConstHash("EngineEvent::Shutdown"),
	};

}

using namespace Engine;



class Resource : public Object
{
public:
	RTTI(Resource)

	Resource()
	{
		LOG_EVENT("");
	}
	~Resource()
	{
		LOG_EVENT("");
	}
};
class Image : public Resource
{
public:
	RTTI(Image)

	Image()
	{
		LOG_EVENT("");
	}
	~Image()
	{
		LOG_EVENT("");
	}
};

class Model : public Resource
{
public:
	RTTI(Model)

	Model()
	{
		LOG_EVENT("");
	}
	~Model()
	{
		LOG_EVENT("");
	}
};				


class Serializer;

template <class T> void Write(Serializer* _dst, const T& _val)
{
	printf("default writter\n");
}

/*void Write(Serializer* _dst, const IntVector2& _val)
{
	printf("%f %f\n", _val.x, _val.y);
}*/


/*
text serialization -> Variant
text deserialization -> Variant

binary serialization -> file
binary desrialization -> file

*/

class Serializer
{
public:

	void Write(const void* _data, uint _size) { }

	template<class T> void Write(const T& _val)
	{
		::Write(this, _val);

	}
};

class Deserializer
{
public:

	void Read(void) { }
};


class Serializable
{
public:

	void Serialize(Serializer* _dst) { }
	void Deserialize(Deserializer* _src) { }
};


//---------------------------------------------------------------------------//
// 
//---------------------------------------------------------------------------//

class Application : public Engine::Main
{

};

	//---------------------------------------------------------------------------//
	// 
	//---------------------------------------------------------------------------//


#include <clocale>

int main()
{

	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "English");



	Serializer ser;
	IntVector2 val;
	ser.Write(val);

	int arg = 0;
	//ASSERT(arg != 0, "arg must be equal of zero");

	//LOG_FATAL("Fatal Error %s", "abc");
	LOG_ERROR("Error %s", "abc");
	LOG_WARNING("Warning %s", "abc");
	LOG_EVENT("Event %s", "abc");
	LOG_INFO("Info %s", "abc");
	LOG_DEBUG("Debug %s", "abc");

	RegisterObject<Resource>();
	RegisterObject<Image>();
	RegisterObject<Model>();

	{
		SharedPtr<Resource> _t = CreateObject<Resource>("Image");
		LOG_INFO("Class %s", _t->ClassName().CStr());
		LOG_INFO("Is Class resource %d", _t->IsClass<Resource>());
		LOG_INFO("Is Class image %d", _t->IsClass<Image>());
		LOG_INFO("Is Class model %d", _t->IsClass<Model>());
	}

	Variant v;
	v["x"] = 1;
	v["y"] = 12;
	v["z"] = 0xfff00011;
	v["w"] = "str";
	Variant& v2 = v["zzz"];
	v2["xxx"] = false;
	Variant& v3 = v2["zzz"];
	v3.Resize(3);
	v3[0] = nullptr;
	v3[1] = &main;
	v3[2] = &v;
	String s = v.Print();


	struct sss
	{

	};

	sss* ss = reinterpret_cast<sss*>(v3[2].AsPointer());

	printf("%s\n", s.CStr());

	{
		//Logger::SetMask(Logger::Mask::Level | Logger::Mask::Message);

		System _sys;

		if (gMain->Startup())
		{

			if (gMain->ConfigFileNotLoaded())
			{
				LOG_INFO("ConfigFileNotLoaded");
				gFileSystem->AddSearchPath("../");
				gFileSystem->AddSearchPath("../../");
				gFileSystem->AddSearchPath("Data");
				


				
			}
			{
				Variant& _app = gMain->GetConfig("Application");
				if (_app.IsNull())
				{

					_app["DebugMode"] = true;
				}

			}


			/*gFileSystem->AddSearchPath(".");
			gFileSystem->AddSearchPath("../");
			gFileSystem->AddSearchPath("../../");
		

			gFileSystem->AddSearchPath("Data");
			gFileSystem->AddSearchPath("../Data");
			gFileSystem->AddSearchPath("../../Data");


			FileInfo _fi;
			gFileSystem->GetDiskFileInfo(_fi, "../../test2.json");
			LOG_INFO("%s %s %d bytes", _fi.dir.CStr(), _fi.name.CStr(), (uint)_fi.size);

			Array<FileInfo> _fil;
			if (gFileSystem->EnumDiskFiles(_fil, "C:/Users/zacha/Downloads"))
			{
				for (auto i : _fil)	   
				{
					LOG_INFO("%s %s %d bytes", i.dir.CStr(), i.name.CStr(), (uint)i.size);
				}
			} */

			/*gFileSystem->AddSearchPath("./");
			gFileSystem->AddSearchPath("../");
			gFileSystem->AddSearchPath("../../");

			SharedPtr<File> _f = gFileSystem->OpenFile("../Cache/test/test2.json", AccessMode::Read);
			gFileSystem->CreateDirectory("../../Cache/test2.js");

			if (_f)
			{
				LOG_DEBUG("File: '%s' root: '%s'", _f->GetName().CStr(), _f->GetRoot().CStr());
			}


			Variant _json;
			if (gFileSystem->LoadJson(gFileSystem->FindConfigFile(), _json))
			{
				LOG_INFO("\n%s", _json.Print().CStr());
			}

			gFileSystem->SaveJson("../../test4.json", _json);


			LOG_INFO("confing '%s'", gFileSystem->FindConfigFile("").CStr());*/

			while (gMain->Frame())
			{

			}

		}

		gMain->Shutdown();
	}


	LOG_INFO("Hello World!");
	return 0;
}