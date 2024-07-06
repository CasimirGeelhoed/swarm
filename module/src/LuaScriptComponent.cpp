//
//  LuaScriptComponent.cpp
//  napswarm
//
//  Created by Casimir Geelhoed on 05/07/2024.
//

#include "LuaScriptComponent.h"

#include <entity.h>


// RTTI
RTTI_BEGIN_CLASS(nap::LuaScriptComponent)
    RTTI_PROPERTY("Script", &nap::LuaScriptComponent::mScript, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("ParameterData", &nap::LuaScriptComponent::mParameterData, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("OutputData", &nap::LuaScriptComponent::mOutputData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::LuaScriptComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

