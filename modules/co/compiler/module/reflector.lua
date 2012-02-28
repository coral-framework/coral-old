local function template( writer, c, t )
	c.header( writer, c, "Reflection code generated for type '", t.fullName, "'" )

	writer( "\n" )

	local requiredTypes = t.typesNeededInReflector

	local function includeIfNotIncluded( typeName )
		if t.fullName ~= typeName and not requiredTypes[typeName] then
			writer( "#include <", t.toHeaderName( typeName ), ">\n" )
		end
	end

	if t.kind == 'TK_NATIVECLASS' then
		writer( "#include \"", t.name, "_Adapter.h\"\n" )
	elseif t.kind ~= 'TK_COMPONENT' then
		writer( "#include <", t.headerName, ">\n" )
	end

	if t.kind == 'TK_INTERFACE' then
		includeIfNotIncluded( "co.IDynamicServiceProvider" )
	end

	for fullName, type in pairs( requiredTypes ) do
		writer( "#include <", type.headerName, ">\n" )
	end

	if t.fields then
		if t.methods and not requiredTypes["co.IMethod"] then
			includeIfNotIncluded( "co.IMethod" )
		end
		includeIfNotIncluded( "co.IField" )
		if t.methods then
			includeIfNotIncluded( "co.IllegalCastException" )
		end
		includeIfNotIncluded( "co.MissingInputException" )
		includeIfNotIncluded( "co.IllegalArgumentException" )
	end

	if t.fullName == 'co.IReflector' and not requiredTypes["co.NotSupportedException"] then
		writer( "#include <co/NotSupportedException.h>\n" )
	end

	writer( "#include <co/reserved/ReflectorBase.h>\n" )

	if t.fields then
		writer( "#include <sstream>\n" )
	end

	writer( "#include <cassert>\n\n" )

	c.revisionCheck( writer, c, t )

	c.utils.openNamespaces( writer, c.moduleName )

	writer( "\n" )

	if t.kind == 'TK_COMPONENT' then
		writer( [[
// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __]], t.name, [[_getSize();
co::IObject* __]], t.name, [[_newInstance();

]] )
	end

	if c.moduleName ~= 'co' then
		writer [[
void moduleRetain();
void moduleRelease();

]]
	end

	if t.kind == 'TK_INTERFACE' then
		writer( [[
// ------ Dynamic Service Proxy ------ //

class ]], t.name, [[_Proxy : public ]], t.cppName, "\n", [[
{
public:
	]], t.name, [[_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
]], c.moduleName == 'co' and '' or "\t\tmoduleRetain();\n", [[
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~]], t.name, [[_Proxy()
	{
		]], c.moduleName == 'co' and "// empty" or "moduleRelease();", "\n", [[
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<]], t.cppName, [[>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

]] )

		local function generateProxyMethodsFor( itf )
			writer( "\t// ", itf.fullName, " Methods:\n\n" )

			-- Field Accessors
			for i, a in ipairs( itf.fields ) do
				local inputType = itf.formatInput( a.type )
				writer( "\t", inputType, " ", itf.formatAccessor( "get", a.name ), "()\n", [[
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<]], itf.cppName, [[>( ]], i - 1, [[ ) );
        return res.get< ]], inputType, [[ >();
	}

]] )
				if not a.isReadOnly then
					writer( "\tvoid ", itf.formatAccessor( "set", a.name ), "( ", inputType, " ", a.name, "_ )\n", [[
	{
		co::Any arg;
		arg.set< ]], inputType, [[ >( ]], a.name, [[_ );
		_provider->dynamicSetField( _cookie, getField<]], itf.cppName, [[>( ]], i - 1, [[ ), arg );
	}

]] )
				end
			end

			-- Methods
			for i, m in ipairs( itf.methods ) do
				local formattedReturnType = itf.formatInput( m.returnType )
				writer( "\t", formattedReturnType, " ", m.name, "(" )
				local params = m.parameters
				if #params > 0 then
					writer( " " )
					for i, p in ipairs( params ) do
						local paramType = ( p.isOut and itf.formatOutput or itf.formatInput )( p.type )
						writer( paramType, " ", p.name, ( i < #params and '_, ' or '_' ) )
					end
					writer( " " )
				end
				writer( ")\n\t{\n" )
				if #params > 0 then
					writer( "\t\tco::Any args[", #params, "];\n" )
					for i, p in ipairs( params ) do
						local paramType = ( p.isOut and itf.formatOutput or itf.formatInput )( p.type )
						writer( "\t\targs[", i - 1, "].set< ", paramType, " >( ", p.name, "_ );\n" )
					end
					writer( "\t\tco::Range<co::Any const> range( args, ", #params, " );\n" )
				else
					writer( "\t\tco::Range<co::Any const> range;\n" )
				end
				writer( "\t\t" )
				if m.returnType then
					writer( "const co::Any& res = " )
				end
				writer( "_provider->dynamicInvoke( _cookie, getMethod<", itf.cppName, ">( ", i - 1, " ), range );\n" )
				if m.returnType then
					writer( "\t\treturn res.get< ", formattedReturnType, " >();\n" )
				end
				writer( "\t}\n\n" )
			end
		end

		for i = #t.superTypes - 1, 1, -1 do
			local super = t.superTypes[i]
			generateProxyMethodsFor( super )
		end

		if t.fullName ~= 'co.IService' then
			generateProxyMethodsFor( t )
		end

		if t.fullName == 'co.IReflector' then
			writer [[
	// These co::IReflector methods are not part of the reflection system:

	void createValues( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::createValues() cannot be called through a proxy" );
	}

	void copyValues( const void*, void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::copyValues() cannot be called through a proxy" );
	}

	void destroyValues( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::destroyValues() cannot be called through a proxy" );
	}

]]
		end

		writer( [[
protected:
	template<typename T>
	co::IField* getField( co::uint32 index )
	{
		return co::typeOf<T>::get()->getFields()[index];
	}

	template<typename T>
	co::IMethod* getMethod( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMethods()[index];
	}

private:
	co::IDynamicServiceProvider* _provider;
	co::uint32 _cookie;
};

]] )

	end

	--- End of Dynamic Service Proxy ---

	writer( [[
// ------ Reflector Component ------ //

class ]], t.name, [[_Reflector : public co::ReflectorBase
{
public:
	]], t.name, [[_Reflector()
	{
		]], c.moduleName == 'co' and "// empty" or "moduleRetain();", "\n", [[
	}

	virtual ~]], t.name, [[_Reflector()
	{
		]], c.moduleName == 'co' and "// empty" or "moduleRelease();", "\n", [[
	}

	co::IType* getType()
	{
		return ]] )

	if t.kind == 'TK_COMPONENT' then
		writer( 'co::getType( "', t.fullName, '" );\n' )
	else
		writer( "co::typeOf<", t.cppName, ">::get();\n" )
	end

	writer( "\t}\n\n" )

	if t.kind == 'TK_COMPONENT' then
		writer( [[
	co::uint32 getSize()
	{
		return __]], t.name, [[_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __]], t.name, [[_newInstance();
		assert( instance->getComponent()->getFullName() == "]], t.fullName, [[" );
		return instance;
	}
]] )
	else
		local sizeofType = ( t.kind == 'TK_INTERFACE' and "void*" or t.cppName )
		writer( "\tco::uint32 getSize()\n\t{\n\t\treturn sizeof(", sizeofType, ");\n\t}\n" )
	end

	if t.kind == 'TK_EXCEPTION' then
		writer( [[

	void raise( const std::string& message )
	{
		throw ]], t.cppName, [[( message );
	}
]] )
	elseif t.kind == 'TK_STRUCT' or t.kind == 'TK_NATIVECLASS' then
		writer( [[

	void createValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			new( reinterpret_cast<]], t.cppName, [[*>( ptr ) + i ) ]], t.cppName, [[;
    }

	void copyValues( const void* fromPtr, void* toPtr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			reinterpret_cast<]], t.cppName, [[*>( toPtr )[i] = reinterpret_cast<const ]], t.cppName, [[*>( fromPtr )[i];
    }

	void destroyValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			callDestructor( reinterpret_cast<]], t.cppName, [[*>( ptr ) + i );
	}
]] )
	elseif t.kind == 'TK_INTERFACE' then
		writer( [[

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new ]], c.moduleNS, [[::]], t.name, [[_Proxy( provider );
	}
]] )
	end

	if t.kind == 'TK_STRUCT' then

		writer( [[

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		]], t.cppName, [[* p = co::checkInstance<]], t.cppName, [[>( instance, field );
		switch( field->getIndex() )
		{
]] )
		for i, a in ipairs( t.fields ) do
			local inputType = t.formatInput( a.type )
			local optionalGet = ( a.type.kind == 'TK_INTERFACE' and ".get()" or "" )
			writer( "\t\tcase ", a.index, ":\t\tvalue.set< ", inputType, " >( p->", a.name, optionalGet, " ); break;\n" )
		end

		writer( [[
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		]], t.cppName, [[* p = co::checkInstance<]], t.cppName, [[>( instance, field );
		switch( field->getIndex() )
		{
]] )

		for i, a in ipairs( t.fields ) do
			if a.type.kind == 'TK_ARRAY' then
				writer( "\t\tcase ", a.index, ":\t\tco::assign( value.get< ", t.formatInput( a.type ), " >(), p->", a.name, " ); break;\n" )
			else
				writer( "\t\tcase ", a.index, ":\t\tp->", a.name, " = value.get< ", t.formatInput( a.type ), " >(); break;\n" )
			end
		end

		writer( [[
		default:	raiseUnexpectedMemberIndex();
		}
	}
]] )

	elseif t.kind == 'TK_NATIVECLASS' or t.kind == 'TK_INTERFACE' then

		local callPrefix = ( t.kind == 'TK_NATIVECLASS' and t.cppName .. "_Adapter::" or "p->" )

		writer( "\n\tvoid getField( co::Any instance, co::IField* field, co::AnyValue& value )\n\t{\n" )

		if #t.fields > 0 then
			writer( "\t\t", t.cppName, [[* p = co::checkInstance<]], t.cppName, [[>( instance, field );
		switch( field->getIndex() )
		{
]] )
			for i, a in ipairs( t.fields ) do
				writer( "\t\tcase ", a.index, ":\t\tvalue.set< ", t.formatInput( a.type ), " >( ", callPrefix,
					t.formatAccessor( "get", a.name ), "(", ( t.kind == 'TK_NATIVECLASS' and " *p " or "" ), ") ); break;\n" )
			end

			writer [[
		default:	raiseUnexpectedMemberIndex();
		}
]]
		else
			writer( [[
		co::checkInstance<]], t.cppName, [[>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
]] )
		end

		writer [[
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
]]
		if #t.fields > 0 then
			writer( "\t\t", t.cppName, [[* p = co::checkInstance<]], t.cppName, [[>( instance, field );
		switch( field->getIndex() )
		{
]] )
			for i, a in ipairs( t.fields ) do
				if a.isReadOnly then
					writer( "\t\tcase ", a.index, ":\t\traiseFieldIsReadOnly( field ); break;\n" )
				else
					writer( "\t\tcase ", a.index, ":\t\t", callPrefix, t.formatAccessor( "set", a.name ), "( ",
						( t.kind == 'TK_NATIVECLASS' and "*p, " or "" ), "value.get< ", t.formatInput( a.type ), " >() ); break;\n" )
				end
			end

			writer( [[
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
]] )
		else
			writer( [[
		co::checkInstance<]], t.cppName, [[>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
]] )
		end

		writer [[
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any const> args, co::AnyValue& res )
	{
]]
		if #t.methods > 0 then
			writer( "\t\t", t.cppName, [[* p = co::checkInstance<]], t.cppName, [[>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
]] )
			for i, m in ipairs( t.methods ) do
				writer( "\t\t\tcase ", m.index, ":\n\t\t\t\t{\n" )
				for i, p in ipairs( m.parameters ) do
					local paramType = ( p.isOut and t.formatOutput or t.formatInput )( p.type )
					writer( "\t\t\t\t\t", paramType, " ", p.name, "_ = args[++argIndex].get< ", paramType, " >();\n" )
				end
				if #m.parameters > 0 then
					writer( "\t\t\t\t\targIndex = -1;\n" )
				end
				writer( "\t\t\t\t\t" )
				if m.returnType then
					writer( "res.set< ", t.formatInput( m.returnType ), " >( " )
				end
				writer( callPrefix, m.name )
				if #m.parameters > 0 then
					writer( "( " )
					if t.kind == 'TK_NATIVECLASS' then
						writer( "*p, " )
					end
					for i, p in ipairs( m.parameters ) do
						writer( p.name, i < #m.parameters and "_, " or "_" )
					end
					writer( " )" )
				elseif t.kind == 'TK_NATIVECLASS' then
					writer( "( *p )" )
				else
					writer( "()" )
				end
				if m.returnType then
					writer( " )" )
				end
				writer( ";\n\t\t\t\t}\n\t\t\t\tbreak;\n" )
			end

			writer [[
			default:
				raiseUnexpectedMemberIndex();
			}
		}
		catch( co::IllegalCastException& e )
		{
			if( argIndex == -1 )
				throw; // just re-throw if the exception is not related to 'args'
			raiseArgumentTypeException( method, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
]]
		else
			writer( [[
		co::checkInstance<]], t.cppName, [[>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
]] )
		end

		writer( "\t}\n" )
	end

	writer( [[
};

// ------ Reflector Creation Function ------ //

co::IReflector* __create]], t.name, [[Reflector()
{
    return new ]], t.name, [[_Reflector;
}

]] )

	c.utils.closeNamespaces( writer, c.moduleName )

end

return template
