#pragma once
#include "Container.hpp"
#include "String.hpp"

namespace Engine
{
	//----------------------------------------------------------------------------//
	// Variant
	//----------------------------------------------------------------------------//

	//!
	class ENGINE_API Variant
	{
	public:
		//!
		enum class Type
		{
			Null,
			Bool,
			Int,
			Float,
			Pointer,
			String,
			Array,
			Object,
		};

		//!
		typedef Pair<String, Variant> KeyValue;
		//!
		typedef Array<KeyValue> Node;
		//!
		typedef Node::Iterator Iterator;
		//!
		typedef Node::ConstIterator ConstIterator;

		//!
		Variant(void) = default;
		//!
		~Variant(void) { SetType(Type::Null); }
		//!
		Variant(const Variant& _other);
		//!
		Variant(Variant&& _temp) noexcept;
		//!
		Variant(Type _type) { SetType(_type); }
		//!
		Variant(nullptr_t) { }
		//!
		Variant(bool _value);
		//!
		Variant(int _value);
		//!
		Variant(uint _value);
		//!
		Variant(float _value);
		//!
		Variant(void* _value);
		//!
		template <class T> Variant(T* _value) : Variant(reinterpret_cast<void*>(_value)) { }
		//!
		Variant(const char* _value);
		//!
		Variant(const String& _value);
		//!
		Variant(String&& _value);

		//!
		Variant& operator = (const Variant& _rhs);
		//!
		Variant& operator = (Variant&& _rhs) noexcept;

		//!
		bool IsNull(void) const { return m_type == Type::Null; }
		//!
		bool IsBool(void) const { return m_type == Type::Bool; }
		//!
		bool IsInt(void) const { return m_type == Type::Int; }
		//!
		bool IsFloat(void) const { return m_type == Type::Int; }
		//!
		bool IsPointer(void) const { return m_type == Type::Pointer; }
		//!
		bool IsNumeric(void) const { return m_type == Type::Int || m_type == Type::Float; }
		//!
		bool IsString(void) const { return m_type == Type::String; }
		//!
		bool IsArray(void) const { return m_type == Type::Array; }
		//!
		bool IsObject(void) const { return m_type == Type::Object; }
		//!
		bool IsNode(void) const { return m_type == Type::Array || m_type == Type::Object; }

		//!
		Variant& SetType(Type _type);
		//!
		Variant& SetNull(void) { return SetType(Type::Null); }
		//!
		Variant& SetBool(bool _value) { SetType(Type::Bool).m_bool = _value; return *this; }
		//!
		Variant& SetInt(int _value) { SetType(Type::Int).m_int = _value; return *this; }
		//!
		Variant& SetFloat(float _value) { SetType(Type::Float).m_flt = _value; return *this; }
		//!
		Variant& SetPointer(void* _value) { SetType(Type::Pointer).m_ptr = _value; return *this; }
		//!
		Variant& SetString(const String& _value) { SetType(Type::String)._String() = _value; return *this; }
		//!
		Variant& SetString(String&& _value) { SetType(Type::String)._String() = Move(_value); return *this; }
		//!
		Variant& SetArray(InitializerList<Variant> _value);
		//!
		Variant& SetObject(InitializerList<KeyValue> _value);

		//!
		bool AsBool(void) const;
		//!
		int AsInt(void) const;
		//!
		float AsFloat(void) const;
		//!
		void* AsPointer(void) const;
		//!
		String AsString(void) const;

		//!
		operator bool(void) const { return AsBool(); }
		//!
		operator int(void) const { return AsInt(); }
		//!
		operator uint(void) const { return AsInt(); }
		//!
		operator float(void) const { return AsFloat(); }
		//!
		operator void*(void) const { return AsPointer(); }
		//!
		operator String(void) const { return AsString(); }

		// [ARRAY OR OBJECT]

		//!
		uint Size(void) const;
		//!
		Variant& Clear(void);

		// [ARRAY ONLY]

		//!	
		Variant& Resize(uint _size);

		//!
		Variant& operator [] (uint _index) { return Get(_index); }
		//!
		const Variant& operator [] (uint _index) const { return Get(_index); }
		//
		Variant& operator [] (int _index) { return Get(_index); }
		//!
		const Variant& operator [] (int _index) const { return Get(_index); }
		//!
		Variant& Get(uint _index);
		//!
		const Variant& Get(uint _index) const;

		//! Insert element to array. \return this
		Variant& Insert(uint _pos, const Variant& _value);
		//! Insert element to array. \return this
		Variant& Insert(uint _pos, Variant&& _value);
		//! Add new item to end of array. \return this
		Variant& Push(const Variant& _value);
		//! Add new item to end of array. \return this
		Variant& Push(Variant&& _value);
		//! Add new item to end of array and return it. \return new item
		Variant& Append(void);

		//!	Remove last element of array. \return this
		Variant& Pop(void);
		//! Remove range of elements from array. \return this
		Variant& Erase(uint _start, uint _num);

		// [OBJECT ONLY]

		//!
		Variant& operator [] (const String& _key) { return GetOrAdd(_key); }
		//!
		const Variant& operator [] (const String& _key) const { return Get(_key); }
		//!
		Variant& operator [] (const char* _key) { return GetOrAdd(_key); }
		//!
		const Variant& operator [] (const char* _key) const { return Get(_key); }

		//! Get or add value of key.
		Variant& GetOrAdd(const String& _key);
		//! Get value of key
		const Variant& Get(const String& _key) const;

		//!Find value of key
		Variant* Find(const String& _key);
		//!Find value of key
		const Variant* Find(const String& _key) const;

		//!	Add key with value to object. \return this
		Variant& Set(const String& _key, const Variant& _value);
		//! Remove key from object
		bool Erase(const String& _key);

		//!
		Node& Container(void);
		//!
		const Node& Container(void) const;
		//!
		Iterator Begin(void) { return Container().Begin(); }
		//!
		ConstIterator Begin(void) const { return Container().Begin(); }
		//!
		Iterator End(void) { return Container().End(); }
		//!
		ConstIterator End(void) const { return Container().End(); }

		//!
		bool Parse(const char* _str, String* _error = nullptr);
		//!
		String Print(void) const;

		//!
		static const Variant Null;
		//!
		static const Variant EmptyArray;
		//!
		static const Variant EmptyObject;

	protected:
		//!
		bool _Parse(Tokenizer& _str);
		//!
		void _Print(String& _dst, int _depth) const;
		//!
		static void _PrintString(String& _dst, const String& _src, int _depth);

		//!
		bool& _Bool(void) { return m_bool; }
		//!
		const bool& _Bool(void) const { return m_bool; }
		//!
		int& _Int(void) { return m_int; }
		//!
		const int& _Int(void) const { return m_int; }
		//!
		float& _Float(void) { return m_flt; }
		//!
		const float& _Float(void) const { return m_flt; }
		//!
		void*& _Ptr(void) { return m_ptr; }
		//!
		const void* const& _Ptr(void) const { return m_ptr; }
		//!
		String& _String(void) { return *reinterpret_cast<String*>(m_str); }
		//!
		const String& _String(void) const { return *reinterpret_cast<const String*>(m_str); }
		//!
		Node& _Node(void) { return *reinterpret_cast<Node*>(m_node); }
		//!
		const Node& _Node(void) const { return *reinterpret_cast<const Node*>(m_node); }

		Type m_type = Type::Null;
		union
		{
			bool m_bool;
			int m_int;
			float m_flt;
			void* m_ptr;
			uint8 m_str[sizeof(String)];
			uint8 m_node[sizeof(Node)] = { 0 };
		};
	};

	//!
	inline auto begin(Variant& _v)->decltype(_v.Begin()) { return _v.Begin(); }
	//!
	inline auto begin(const Variant& _v)->decltype(_v.Begin()) { return _v.Begin(); }
	//!
	inline auto end(Variant& _v)->decltype(_v.End()) { return _v.End(); }
	//!
	inline auto end(const Variant& _v)->decltype(_v.End()) { return _v.End(); }


	//----------------------------------------------------------------------------//
	// Serializable
	//----------------------------------------------------------------------------//

	//!
	/*class ENGINE_API Serializable : public Object
	{
	public:
		//!
		virtual bool Serialize(const Variant& _src) { return true; }
		//!
		virtual void Deserialize(Variant& _dst) { }
	};*/

	//----------------------------------------------------------------------------//
	// 
	//----------------------------------------------------------------------------//

}

