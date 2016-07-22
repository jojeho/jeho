#pragma once
#include <atlbase.h>
#include <atlcom.h>
#include <vector>
#include <comdef.h>
#include <atlsafe.h>

namespace jeho {
	namespace window {

		template<typename T>
		struct array_type
		{};

		template<>
		struct array_type<int>
		{
			enum { vt = VT_ARRAY | VT_INT };
		};

		template<>
		struct array_type<short>
		{
			enum { vt = VT_ARRAY | VT_I2 };
		};


		template<>
		struct array_type<std::string>
		{
			enum { vt = VT_ARRAY | VT_BSTR };
		};


		template<>
		struct array_type<float>
		{
			enum { vt = VT_ARRAY | VT_R4 };
		};

		template<>
		struct array_type<double>
		{
			enum { vt = VT_ARRAY | VT_R8 };
		};

		template<>
		struct array_type<long>
		{
			enum { vt = VT_ARRAY | VT_I4 };
		};


		template<>
		struct array_type<_variant_t>
		{
			enum { vt = VT_ARRAY | VT_VARIANT };
		};

		template<typename T>
		struct t_cast
		{
			typedef T type;
		};

		template<>
		struct t_cast<std::string>
		{
			typedef BSTR type;
		};

		std::string to_string(BSTR const& t)
		{
			_bstr_t bt(t);
			return std::string(bt);
		}

		std::string to_string(_bstr_t const& t)
		{
			return std::string(t);
		}

		_bstr_t to_bstr_t(std::string const&t)
		{
			return _bstr_t(t.c_str());
		}

		template<typename T>
		struct to_cast
		{
			const T& operator()(T const& t)
			{
				return t;
			}
		};

		template<>
		struct to_cast<std::string>
		{
			_bstr_t operator()(std::string const& t)
			{
				return to_bstr_t(t);
			}
		};


		template<typename T>
		struct from_cast
		{
			const T& operator()(T const&t)
			{
				return t;
			}
		};

		template<>
		struct from_cast<std::string>
		{
			std::string operator()(BSTR const&bstr)
			{
				return to_string(bstr);
			}
		};


		template<typename T>
		_variant_t to_variant(std::vector<T> const data)
		{
			typedef typename t_cast<T>::type type;
			CComSafeArray<type> safeArray;
			for (std::vector<T>::const_iterator it = data.begin(); it != data.end(); it++) {
				safeArray.Add(to_cast<T>()(*it));
			}

			variant_t vt;
			vt.vt = array_type<T>::vt;
			vt.parray = safeArray.Detach();
			return vt;
		}

		template<typename T>
		std::vector<T> from_variant(_variant_t const& data)
		{
			typedef typename t_cast<T>::type type;

			CComSafeArray<type> Array;
			SAFEARRAY * p = V_ARRAY(&data);
			Array.Attach(p);
			unsigned int Count = Array.GetCount();
			std::vector<T> result(Count);
			for (unsigned int i = 0; i < Count; i++)
			{
				result[i] = from_cast<T>()(Array.GetAt(i));
			}

			return result;
		}

        }
}
