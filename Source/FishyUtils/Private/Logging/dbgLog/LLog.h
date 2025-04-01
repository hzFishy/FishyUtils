// Copyright © Laura Andelare.
//
// LLog is free software. It comes without any warranty, to the extent permitted
// by applicable law. You can redistribute it and/or modify it under the terms
// of the Do What The Fuck You Want To Public License, Version 2, as published
// by Sam Hocevar:
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.



// --------------------------------------------------------------------------------------
// This is a slightly modified version of the original, located at https://github.com/landelare/llog
// Appreciate you, Laura.
// --------------------------------------------------------------------------------------


#pragma once
#include <concepts>
#include <format>
#include <source_location>
#include <string>
#include <type_traits>
#include <utility>
#include <chrono>

#include "Containers/StringConv.h"
#include "Engine/Engine.h"
#include "HAL/IConsoleManager.h"

#include "Logging/StructuredLog.h"

#include "Misc/CoreMiscDefines.h"





// Some versions of the MSSTL don't implement P2508R1, so here it goes:
#if __cpp_lib_format <= 202110L && defined(_MSVC_STL_VERSION)
namespace std
{
	template<typename C, typename... A>
	using basic_format_string = _Basic_format_string<C, A...>;
	template<typename... A> using format_string = _Fmt_string<A...>;
	template<typename... A> using wformat_string = _Fmt_wstring<A...>;
}
#endif



namespace DBG::Log
{
	template<typename T>
	concept TInitValue = std::same_as<std::decay_t<T>, EForceInit> ||
						 std::same_as<std::decay_t<T>, EInPlace> ||
						 std::same_as<std::decay_t<T>, ENoInit>;
	
	template<typename T>
	concept THasStdFormat = requires(std::formatter<std::decay_t<T>, wchar_t> fmt,
									 std::basic_format_parse_context<wchar_t> pctx,
									 std::wformat_context fctx)
	{
		fmt.parse(pctx); fmt.format(std::declval<T>(), fctx);
	};

	template<typename T>
	concept TFalse = std::false_type::value;
	
	template<typename T>
	concept TIterable = requires(T t) { t.begin().operator++(); t.end(); t.begin() != t.end(); };

	template<typename T>
	auto FormatArgument(T&& Value)
	{
		using UnderlyingType = std::remove_pointer_t<std::remove_reference_t<T>>;
		constexpr bool bIsPointer = std::is_pointer_v<std::remove_cvref_t<decltype(Value)>>;

		
		// Block FName(ENoInit), etc. constructors before they'd match something
		if constexpr (TInitValue<T>)
		{
			static_assert(TFalse<T>, "Invalid type");
		}
		// Everything that std::format can handle goes straight through
		else if constexpr (THasStdFormat<T>)
		{
			return Value;
		}
		else
		{
			// Get type information before the Value is potentially consumed
			UClass* UType = nullptr;
			using V = std::decay_t<T>;
			if constexpr (std::is_null_pointer_v<T>)
			{
				UType = nullptr;
			}
			else if constexpr (std::is_convertible_v<T, const UObject*>)
			{
				UType = Value ? Value->GetClass()
							 : std::remove_pointer_t<V>::StaticClass();
			}
			else if constexpr (std::is_convertible_v<T, const UObject&>)
			{
				UType = Value.GetClass();
			}
	
			// Try to coerce the Value into a FString somehow, respecting && overloads
			FString Str( TEXT("Error") );

			// If the value is a pointer, check if it's valid before we try get out a value, for UObjects we can get a little more information.
			if constexpr ( bIsPointer )
			{
				if constexpr (std::is_convertible_v<T, const UObject*>)
				{
					if ( ::IsValid(Value) == false )
					{
						if ( UType )
						{
							Str = FString::Format( TEXT("({0}) Null Pointer"), { UType->GetName() } );
						}
						else
						{
							 Str = TEXT("Null Pointer");
						}
						return std::wstring( StringCast<wchar_t>(*Str).Get() );
					}
				}
				else
				{
					if (Value == nullptr)
					{
						return std::wstring( TEXT("Null Pointer") );
					}
				}
			}

			
			if constexpr (requires { Str = std::forward<T>(Value)->GetName(); })
			{
				Str = std::forward<T>(Value)->GetName();
			}
			else if constexpr (requires { Str = std::forward<T>(Value).GetName(); })
			{
				Str = std::forward<T>(Value).GetName();
			}
			else if constexpr (requires { Str = std::forward<T>(Value)->ToString(); })
			{
				Str = std::forward<T>(Value)->ToString();
			}
			else if constexpr (requires { Str = std::forward<T>(Value).ToString(); })
			{
				Str = std::forward<T>(Value).ToString();
			}
			else if constexpr (requires { Str = std::forward<T>(Value)->c_str(); })
			{
				Str = std::forward<T>(Value)->c_str();
			}
			else if constexpr (requires { Str = std::forward<T>(Value).c_str(); })
			{
				Str = std::forward<T>(Value).c_str();
			}
			else if constexpr (std::is_same_v<V, FString>)
			{
				Str = Value;
			}
			else if constexpr ( requires { { UnderlyingType::StaticStruct() } -> std::same_as<UScriptStruct*>; } )
			{
				// Populate from ExportText if we don't provide a ToString or any of the other similar functions.
				Str.Reset();
				if constexpr (bIsPointer)
				{
					UnderlyingType::StaticStruct()->ExportText( Str, Value, Value, nullptr, PPF_None, nullptr );
				}
				else
				{
					UnderlyingType::StaticStruct()->ExportText( Str, &Value, &Value, nullptr, PPF_None, nullptr );
				}
			}
			else if constexpr (TIsUEnumClass<V>::Value) // won't work for legacy UENUMs
			{
				Str = StaticEnum<V>()->GetNameByValue(static_cast<int64>(Value)).ToString();
			}
			else if constexpr (requires { typename V::EnumType; })
			{
				if constexpr (std::is_enum_v<typename V::EnumType>) // TEnumAsByte
				{
					Str = LexToString(Value.GetIntValue());
				}
				else
				{
					static_assert(TFalse<T>, "Type wrongly detected as TEnumAsByte");
				}
			}
			else if constexpr (std::is_enum_v<V>) // enums without a compile-time detectable UEnum
			{
				Str = LexToString(static_cast<int64>(Value));
			}
			else if constexpr (std::is_assignable_v<FString, T>)
			{
				Str = std::forward<T>(Value);
			}
			else if constexpr (std::is_convertible_v<T, FName>) // EName literals
			{
				Str = FName(std::forward<T>(Value)).ToString();
			}
			else if constexpr (std::is_array_v<std::remove_reference_t<T>>) // compile time c arrays
			{
				Str = TEXT("[");
				for (auto&& Elem : Value)
				{
					Str += std::format(TEXT("{}, "), FormatArgument(std::forward<decltype(Elem)>(Elem))).c_str();
				}
				Str.LeftChopInline(2);
				Str += TEXT("]");
			}
			else if constexpr (requires { LexToString(std::forward<T>(Value)); })
			{
				Str = LexToString(std::forward<T>(Value));
			}
			else if constexpr (TIterable<T>) // Collections
			{
				Str = TEXT("[");
				for (auto&& i : std::forward<T>(Value))
				{
					Str += std::format(TEXT("{}, "), FormatArgument(std::forward<decltype(i)>(i))).c_str();
				}
				
				if (Str.Len() > 1)
				{
					Str.LeftChopInline(2);
				}
				else
				{
					Str += TEXT(" Empty ");
				}
				
				Str += TEXT("]");
			}
			else if constexpr (TIsTuple_V<V>)
			{
				Str = TEXT("{");
				std::forward<T>(Value).ApplyBefore([&Str]<typename... A>(A&&... Args)
				{
					((Str += std::format(TEXT("{}, "), FormatArgument(std::forward<A>(Args))).c_str()), ...);
				});
				
				if (Str.Len() > 1)
				{
					Str.LeftChopInline(2);
				}
				else
				{
					Str += TEXT(" Empty ");
				}
				
				Str += TEXT("}");
			}
			else if constexpr (requires { std::tuple_size_v<V>; })
			{
				Str = TEXT("{");
				std::apply([&Str]<typename... A>(A&&... args)
				{
					((Str += std::format(L"{}, ", FormatArgument(std::forward<A>(args))).c_str()), ...);
				}, std::forward<T>(Value));

				if (Str.Len() > 1)
				{
					Str.LeftChopInline(2);
				}
				else
				{
					Str += TEXT(" Empty ");
				}
				
				Str += TEXT("}");
			}
			// else if constexpr (your stuff goes here) { }
			else
			{
				static_assert(TFalse<T>, "Unhandled type");
			}
	
			// Attach extra type info to UObjects (UObject*& counts as a pointer)
			if (UType)
			{
				Str = TEXT("(") + UType->GetName() +
						(std::is_pointer_v<V> ? TEXT("*)") : TEXT("&)"))
						+ Str;
			}

			
			return std::wstring( StringCast<wchar_t>(*Str).Get() );
		}
	}


	template<typename T>
	using TFormatted = decltype(FormatArgument(std::declval<T>()));

	
	template<typename... A>
	FString FormatMessage(std::wformat_string<TFormatted<A>...> Format, A&&... Args)
	{
		return FString( std::format(std::move(Format), FormatArgument(std::forward<A>(Args))...).c_str() );
	}
}


