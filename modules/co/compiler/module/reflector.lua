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
		writer( "#include <", t.name, "_Adapter.h>\n" )
	elseif t.kind ~= 'TK_COMPONENT' then
		writer( "#include <", t.headerName, ">\n" )
	end

	if t.kind == 'TK_INTERFACE' then
		includeIfNotIncluded( "co.IDynamicProxyHandler" )
	end

	for fullName, type in pairs( requiredTypes ) do
		writer( "#include <", type.headerName, ">\n" )
	end

	if t.memberAttributes then
		if t.memberMethods and not requiredTypes["co.IMethodInfo"] then
			includeIfNotIncluded( "co.IMethodInfo" )
		end
		includeIfNotIncluded( "co.IAttributeInfo" )
		if t.memberMethods then
			includeIfNotIncluded( "co.IllegalCastException" )
		end
		includeIfNotIncluded( "co.MissingInputException" )
		includeIfNotIncluded( "co.IllegalArgumentException" )
	end

	if t.fullName == 'co.IReflector' and not requiredTypes["co.NotSupportedException"] then
		writer( "#include <co/NotSupportedException.h>\n" )
	end

	writer( "#include <co/reserved/ReflectorBase.h>\n" )

	if t.memberAttributes then
		writer( "#include <sstream>\n" )
	end

	writer( "#include <cassert>\n\n" )

	c.revisionCheck( writer, c, t )

	c.utils.openNamespaces( writer, c.moduleName )

	writer( "\n" )

	if t.kind == 'TK_COMPONENT' then
		writer( [[
// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __]], t.name, [[_getSize();
co::IComponent* __]], t.name, [[_newInstance();

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
// ------ Proxy Interface ------ //

class ]], t.name, [[_Proxy : public ]], t.cppName, "\n", [[
{
public:
	]], t.name, [[_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
]], c.moduleName == 'co' and '' or "\t\tmoduleRetain();\n", [[
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, ]], t.cppName, [[>( this ) );
	}

	virtual ~]], t.name, [[_Proxy()
	{
		]], c.moduleName == 'co' and "// empty" or "moduleRelease();", "\n", [[
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<]], t.cppName, [[>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

]] )

		local function generateProxyMethodsFor( itf )
			writer( "\t// ", itf.fullName, " Methods:\n\n" )

			-- Attribute Accessors
			for i, a in ipairs( itf.memberAttributes ) do
				local inputType = itf.formatInput( a.type )
				writer( "\t", inputType, " ", itf.formatAccessor( "get", a.name ), "()\n", [[
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<]], itf.cppName, [[>( ]], i - 1, [[ ) );
        return res.get< ]], inputType, [[ >();
	}

]] )
				if not a.isReadOnly then
					writer( "\tvoid ", itf.formatAccessor( "set", a.name ), "( ", inputType, " ", a.name, "_ )\n", [[
	{
		co::Any arg;
		arg.set< ]], inputType, [[ >( ]], a.name, [[_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<]], itf.cppName, [[>( ]], i - 1, [[ ), arg );
	}

]] )
				end
			end

			-- Methods
			for i, m in ipairs( itf.memberMethods ) do
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
					writer( "\t\tco::ArrayRange<co::Any const> range( args, ", #params, " );\n" )
				else
					writer( "\t\tco::ArrayRange<co::Any const> range;\n" )
				end
				writer( "\t\t" )
				if m.returnType then
					writer( "const co::Any& res = " )
				end
				writer( "_handler->handleMethodInvocation( _cookie, getMethodInfo<", itf.cppName, ">( ", i - 1, " ), range );\n" )
				if m.returnType then
					writer( "\t\treturn res.get< ", formattedReturnType, " >();\n" )
				end
				writer( "\t}\n\n" )
			end
		end

		for i, ancestor in ipairs( t.interfaceAncestors ) do
			if ancestor.fullName ~= 'co.Interface' then
				generateProxyMethodsFor( ancestor )
			end
		end

		if t.fullName ~= 'co.Interface' then
			generateProxyMethodsFor( t )
		end

		if t.fullName == 'co.IReflector' then
			writer [[
	// These co::IReflector methods are not part of the reflection system:

	void createValue( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::createValue() cannot be called through a proxy interface." );
	}

	void copyValue( const void*, void* )
	{
		throw co::NotSupportedException( "co::IReflector::copyValue() cannot be called through a proxy interface." );
	}

	void destroyValue( void* )
	{
		throw co::NotSupportedException( "co::IReflector::destroyValue() cannot be called through a proxy interface." );
	}

]]
		end

		writer( [[
protected:
	template<typename T>
	co::IAttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::IMethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::IDynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

]] )

	end

	--- End of Interface Proxy ---

	writer( [[
// ------ IReflector ------ //

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
	co::int32 getSize()
	{
		return __]], t.name, [[_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __]], t.name, [[_newInstance();
		assert( component->getComponentType()->getFullName() == "]], t.fullName, [[" );
		return component;
	}
]] )
	else
		writer( "\tco::int32 getSize()\n\t{\n\t\treturn sizeof(", t.cppName, ");\n\t}\n" )
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

	void createValue( void* address, size_t length )
	{
		checkValidSize( sizeof(]], t.cppName, [[), length );
		new( address ) ]], t.cppName, [[;
    }

	void copyValue( const void* fromAddress, void* toAddress )
	{
		*reinterpret_cast<]], t.cppName, [[*>( toAddress ) = *reinterpret_cast<const ]], t.cppName, [[*>( fromAddress );
    }

	void destroyValue( void* address )
	{
		callDestructor( reinterpret_cast<]], t.cppName, [[*>( address ) );
	}
]] )
	elseif t.kind == 'TK_INTERFACE' then
		writer( [[

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, ]], t.cppName, [[>( new ]], c.moduleNS, [[::]], t.name, [[_Proxy( handler ) );
	}
]] )
	end

	if t.kind == 'TK_STRUCT' then

		writer( [[

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		]], t.cppName, [[* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
]] )
		for i, a in ipairs( t.memberAttributes ) do
			local inputType = t.formatInput( a.type )
			local optionalGet = ( a.type.kind == 'TK_INTERFACE' and ".get()" or "" )
			writer( "\t\tcase ", a.index, ":\t\tvalue.set< ", inputType, " >( p->", a.name, optionalGet, " ); break;\n" )
		end

		writer( [[
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		]], t.cppName, [[* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
]] )

		for i, a in ipairs( t.memberAttributes ) do
			if a.type.kind == 'TK_ARRAY' then
				writer( "\t\tcase ", a.index, ":\t\tvalue.get< ", t.formatInput( a.type ), " >().assignTo( p->", a.name, " ); break;\n" )
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

		writer( "\n\tvoid getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )\n\t{\n" )

		if #t.memberAttributes > 0 then
			writer( "\t\t", t.cppName, ( t.kind == 'TK_NATIVECLASS' and "& r" or "* p" ), [[ = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
]] )
			for i, a in ipairs( t.memberAttributes ) do
				writer( "\t\tcase ", a.index, ":\t\tvalue.set< ", t.formatInput( a.type ), " >( ", callPrefix,
					t.formatAccessor( "get", a.name ), "(", ( t.kind == 'TK_NATIVECLASS' and " r " or "" ), ") ); break;\n" )
			end

			writer [[
		default:	raiseUnexpectedMemberIndex();
		}
]]
		else
			writer [[
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
]]
		end

		writer [[
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
]]
		if #t.memberAttributes > 0 then
			writer( "\t\t", t.cppName, ( t.kind == 'TK_NATIVECLASS' and "& r" or "* p" ), [[ = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
]] )
			for i, a in ipairs( t.memberAttributes ) do
				if a.isReadOnly then
					writer( "\t\tcase ", a.index, ":\t\traiseAttributeIsReadOnly( ai ); break;\n" )
				else
					writer( "\t\tcase ", a.index, ":\t\t", callPrefix, t.formatAccessor( "set", a.name ), "( ",
						( t.kind == 'TK_NATIVECLASS' and "r, " or "" ), "value.get< ", t.formatInput( a.type ), " >() ); break;\n" )
				end
			end

			writer( [[
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( ]], t.kind == 'TK_NATIVECLASS' and "r" or "p", [[ );
		CORAL_UNUSED( value );
]] )
		else
			writer [[
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
]]
		end

		writer [[
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
]]
		if #t.memberMethods > 0 then
			writer( "\t\t", t.cppName, ( t.kind == 'TK_NATIVECLASS' and "& r" or "* p" ), [[ = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
]] )
			for i, m in ipairs( t.memberMethods ) do
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
						writer( "r, " )
					end
					for i, p in ipairs( m.parameters ) do
						writer( p.name, i < #m.parameters and "_, " or "_" )
					end
					writer( " )" )
				elseif t.kind == 'TK_NATIVECLASS' then
					writer( "( r )" )
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
			raiseArgumentTypeException( mi, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
]]
		else
			writer [[
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
]]
		end

		writer( "\t}\n" )
	end

	if t.kind ~= 'TK_COMPONENT' then
		if t.memberAttributes then
			writer( "\nprivate:\n" )
			writer( "\t", t.cppName, ( t.kind == 'TK_NATIVECLASS' and "&" or "*" ),
				[[ checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::]], t.typeInterfaceName, [[* myType = co::typeOf<]], t.cppName, [[>::get();

		]] )

			if t.kind == 'TK_INTERFACE' then
				writer( t.cppName, "* res;\n\t\t" )
				writer( "if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<", t.cppName, "*>( any.getState().data.itf ) ) )\n" )
			else
				writer( "if( any.getKind() != co::", t.kind, " || any.getType() != myType || any.getState().data.ptr == NULL )\n" )
			end

			writer( [[
			CORAL_THROW( co::IllegalArgumentException, "expected a valid ]], t.cppName, [[*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to ]], t.fullName, [[" );

		]] )

			if t.kind == 'TK_INTERFACE' then
				writer( "return res;\n" )
			else
				writer( "return ", t.kind == 'TK_NATIVECLASS' and "*" or "", "reinterpret_cast<",
					t.cppName, "*>( any.getState().data.ptr );\n" )
			end

			writer( "\t}\n" )
		end
	end

	writer( [[
};

// ------ IReflector Creation Function ------ //

co::IReflector* __create]], t.name, [[IReflector()
{
    return new ]], t.name, [[_Reflector;
}

]] )

	c.utils.closeNamespaces( writer, c.moduleName )

end

return template
