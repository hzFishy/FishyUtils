// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Delegates/Delegate.h"

namespace FU_Delegates
{
	/**
	 * Useful when facing racing conditions with delegates (ex: delegate was broadcasted before you could bind to it)
	 *  
	 */
	template <typename MulticastDelegate>
	struct FFUDelegateBuffer : protected MulticastDelegate
	{
		FFUDelegateBuffer() {}

		~FFUDelegateBuffer() {}

	protected:
		bool bBufferActive = true;

	public:
		/*template <typename ...Content>
		inline FDelegateHandle AddUObject(Content... ContentData)
		{
			return MulticastDelegate::AddUObject(ContentData...);
		}
		
		template<typename ...ParamTypes>
		void Broadcast(ParamTypes... Params) const
		{
			MulticastDelegate::Broadcast(Params...);
		}*/
	};
}
