// Copyright (C) 2007 Id Software, Inc.
//

#include "KeyInputManager_Local.h"

sdKeyInputManagerLocal::sdKeyInputManagerLocal() {
	defaultContexts.SetNum(MAX_KEYS);
}

sdKeyInputManagerLocal::~sdKeyInputManagerLocal() {
	bindContexts.DeleteContents(true);
}

void sdKeyInputManagerLocal::SetBinding( sdBindContext* context, idKey& key, const char* binding, idKey* modifierKey ) {
#if 1
	(void)context;
	(void)modifierKey;
	idKeyInput::SetBinding(key.GetId(), binding);
#else
	if (context)
		context->Bind(key.GetId(), modifierKey ? modifierKey->GetId() : -1, binding);
#endif
}

const char* sdKeyInputManagerLocal::GetBinding( sdBindContext* context, idKey& key, idKey* modifierKey ) {
#if 1
	(void)context;
	return idKeyInput::GetBinding(key.GetId());
#else
	if (context) {
		sdKeyBind *bind = context->GetBind(key.GetId());
		if (bind) {
			if (modifierKey)
				return bind->GetCommand().GetBinding();
			else
				return bind->GetCommand(modifierKey->GetId()).GetBinding();
		}
	}
	return "";
#endif
}

void sdKeyInputManagerLocal::UnbindBinding( sdBindContext* context, const char *bind ) {
#if 1
	(void)context;
	idKeyInput::UnbindBinding(bind);
#else
	if (context)
		context->UnBindBinding(bind);
#endif
}

void sdKeyInputManagerLocal::KeysFromBinding( sdBindContext* context, const char* binding, bool useBindStrWhenEmpty, idWStr& keyName ) {
#if 1
	(void)context;
	const char *name = idKeyInput::KeysFromBinding(binding);
	keyName = StrToWStr(name);
#else
	if (context)
		keyName = StrToWStr(context->GetName());
	else
		keyName = L"";
#endif
}

//karin: _keys maybe null if only for get num keys
void sdKeyInputManagerLocal::KeysFromBinding( sdBindContext* context, const char* binding, int& numKeys, idKey** _keys ) {
#if 1
	(void)context;
	int max = numKeys;
	numKeys = 0;
	if (binding && *binding) {
		for (int i = 0; i < MAX_KEYS; i++) {
			if (keys[i].binding.Icmp(binding) == 0) {
				if(_keys && numKeys < max)
					_keys[numKeys++] = &keys[i];
			}
		}
	}
#else
#endif
}

bool sdKeyInputManagerLocal::IsDown( const idKey& key ) {
	return key.IsDown();
}

bool sdKeyInputManagerLocal::IsDown( keyNum_e key ) {
	return idKeyInput::IsDown( key );
}

idKey* sdKeyInputManagerLocal::GetKey( const char* name ) {
	int id = idKeyInput::StringToKeyNum(name);
	return id != -1 ? &keys[id] : NULL;
}

idKey* sdKeyInputManagerLocal::GetKeyForEvent( const sdSysEvent& evt, bool& down ) {
	if (evt.evType == SE_KEY) {
		if (evt.evValue >= 0 && evt.evValue < MAX_KEYS) {
			idKey *key = &keys[evt.evValue];
			down = key->IsDown();
			return key;
		}
	}
	return NULL;
}

void sdKeyInputManagerLocal::ProcessUserCmdEvent( const sdSysEvent& event ) {
	if (event.evType == SE_KEY)
		idKeyInput::PreliminaryKeyEvent(event.evValue, event.evValue2);
}

sdKeyCommand* sdKeyInputManagerLocal::GetCommand( sdBindContext* context, const idKey& key ) {
#if 1
	(void)context;
	sdKeyCommand* keyCommand = &defaultContexts[key.GetId()];
	keyCommand->Set(idKeyInput::GetBinding(key.GetId()));
	return keyCommand;
#else
	if (context) {
		sdKeyBind *binding = context->GetBind(key.GetId());
		if (binding) {
			return &binding->GetCommand();
		}
	}
	return NULL;
#endif
}

sdBindContext* sdKeyInputManagerLocal::AllocBindContext( const char* context ) {
	for (int i = 0; i < bindContexts.Num(); ++i) {
		if (!idStr::Icmp(bindContexts[i]->GetName(), context))
			return bindContexts[i];
	}
	int index = bindContexts.Append(new sdBindContext(context));
	return bindContexts[index];
}

void sdKeyInputManagerLocal::UnbindKey(  sdBindContext* context, idKey& key, idKey* modifier ) {
#if 1
	(void)context;
	(void)modifier;
	idKeyInput::SetBinding(key.GetId(), "");
#else
	if (modifier)
		context->UnBind(key.GetId(), modifier->GetId());
	else
		context->UnBind(key.GetId(), 0);
#endif
}

bool sdKeyInputManagerLocal::AnyKeysDown( void ) {
	for (int i = 0; i < MAX_KEYS; i++) {
		if (keys[i].IsDown())
			return true;
	}
	return false;
}

void sdKeyInputManagerLocal::Init(void)
{
	for (int i = 0; i < MAX_KEYS; i++) {
		idKey &key = keys[i];
		if (key.binding.IsEmpty())
			continue;
		usercmdbuttonType_t type = game->SetupBinding(key.binding.c_str(), key.usercmdAction);
		key.type = type;
	}
}

const idKey& sdKeyInputManagerLocal::GetKeyByNum( int keynum ) const {
	return keys[keynum];
}



sdKeyCommand::sdKeyCommand( void )
	: action(0),
	type(B_BUTTON)
{
}

void sdKeyCommand::Set( const char* _binding ) {
	binding = _binding;
}

void sdKeyCommand::FixupBind( void ) {
}




void sdKeyBind::ClearCommand( int modifier ) {
	if (modifier >= MAX_MODIFIERS)
		return;
	if (modifier < 0)
		defaultCommand.Set("");
	else
		modifierCommands[modifier].second.Set("");
}

void sdKeyBind::SetCommand( int modifier, const char* command ) {
	if (modifier >= MAX_MODIFIERS)
		return;
	if (modifier < 0)
		defaultCommand.Set(command);
	else
		modifierCommands[modifier].second.Set(command);
}

sdKeyCommand& sdKeyBind::GetCommand( void ) {
	return defaultCommand;
}

sdKeyCommand& sdKeyBind::GetCommand( int modifier ) {
	if (modifier >= MAX_MODIFIERS || modifier < 0)
		return defaultCommand;
	else
		return modifierCommands[modifier].second;
}

void sdKeyBind::Write( idFile* f, const char* context, const char* keyName ) {
}

void sdKeyBind::UnBindBinding( const char* binding ) {
	if (binding && *binding) {
		if (!idStr::Icmp(defaultCommand.GetBinding(), binding))
			defaultCommand.Set("");
		for (int i = 0; i < MAX_MODIFIERS; i++) {
			if (idStr::Icmp(modifierCommands[i].second.GetBinding(), binding) == 0) {
				modifierCommands[i].second.Set("");
			}
		}
	}
}

void sdKeyBind::SetupBinds( void ) {
}



sdKeyBind* sdBindContext::AllocBind( int key ) {
	int index = keyHash.GetFirst(key);
	if (index == idHashIndexInt::NULL_INDEX) {
		index     = keys.Append(pair_t());
		pair_t &p = keys[index];
		p.first   = key;
		p.second  = new sdKeyBind;
		keyHash.Add(key, index);
		return p.second;
	}
	else
		return keys[index].second;
}

sdKeyBind* sdBindContext::GetBind( int key ) {
	int index = keyHash.GetFirst(key);
	if (index == idHashIndexInt::NULL_INDEX)
		return NULL;
	else
		return keys[index].second;
}

sdKeyCommand* sdBindContext::GetCommand( int key ) {
	sdKeyBind* binding = GetBind(key);
	return binding ? &binding->GetCommand() : NULL;
}

void sdBindContext::WriteBindings( idFile* f ) {
}

void sdBindContext::Bind( int key, int modifierKey, const char* binding ) {
	sdKeyBind* b = AllocBind(key);
	b->SetCommand(modifierKey, binding);
}

void sdBindContext::UnBind( int key, int modifierKey ) {
	sdKeyBind* binding = GetBind(key);
	if (!binding)
		return;
	binding->ClearCommand(modifierKey);
}

void sdBindContext::UnBindAll( void ) {
	keyHash.Clear();
	for (int i = 0; i < keys.Num(); i++) {
		delete keys[i].second;
	}
}

void sdBindContext::UnBindBinding( const char* binding ) {
	for (int i = 0; i < keys.Num(); i++) {
		keys[i].second->UnBindBinding(binding);
	}
}

void sdBindContext::SetupBinds( void ) {
}

sdKeyInputManagerLocal inputManagerLocal;

sdKeyInputManager* keyInputManager = &inputManagerLocal;
