// Copyright (C) 2007 Id Software, Inc.
//


class sdKeyInputManagerLocal : public sdKeyInputManager
{
public:
	sdKeyInputManagerLocal();
	virtual ~sdKeyInputManagerLocal();

	virtual void			SetBinding( sdBindContext* context, idKey& key, const char* binding, idKey* modifierKey );
	virtual const char*		GetBinding( sdBindContext* context, idKey& key, idKey* modifierKey );

	virtual void			UnbindBinding( sdBindContext* context, const char *bind );
	virtual void			KeysFromBinding( sdBindContext* context, const char* binding, bool useBindStrWhenEmpty, idWStr& keyName );

	// pass NULL for keys to find the number of keys to allocate
	virtual void			KeysFromBinding( sdBindContext* context, const char* binding, int& numKeys, idKey** keys );

	virtual bool			IsDown( const idKey& key );
	virtual bool			IsDown( keyNum_e key );
	virtual idKey*			GetKey( const char* name );
	virtual idKey*			GetKeyForEvent( const sdSysEvent& evt, bool& down );

	virtual void			ProcessUserCmdEvent( const sdSysEvent& event );

	virtual sdKeyCommand*	GetCommand( sdBindContext* context, const idKey& key );

	virtual sdBindContext*	AllocBindContext( const char* context );

	virtual void			UnbindKey(  sdBindContext* context, idKey& key, idKey* modifier = NULL );

	virtual bool			AnyKeysDown( void );

private:
	idList<sdBindContext *>	bindContexts;
	idList<sdKeyCommand>	defaultContexts; //karin: compat for DOOM3
};



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

void sdKeyInputManagerLocal::KeysFromBinding( sdBindContext* context, const char* binding, int& numKeys, idKey** keys ) {
#if 1
	(void)context;
	numKeys = 0;
	if (binding && *binding) {
		for (int i = 0; i < MAX_KEYS; i++) {
			if (::keys[i].binding.Icmp(binding) == 0) {
				numKeys = 1;
				keys[i] = &::keys[i];
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
