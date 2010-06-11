{{NEWLINE}}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
{{#MY_LOCAL}}
{{#HAS_ATTRIBUTES_TRUE}}
		{{TYPE_NAME_CPP}}{{#IS_NATIVE_TRUE}}& r{{/IS_NATIVE_TRUE}}{{#IS_NATIVE_FALSE}}* p{{/IS_NATIVE_FALSE}} = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
{{#ATTRIBUTE_LIST}}
{{#GETTER}}
		case {{ATTRIBUTE_INDEX}}:		value.set< {{RETURN_TYPE}} >( {{CALL_PREFIX}}{{METHOD_NAME}}({{#IS_NATIVE_TRUE}} r {{/IS_NATIVE_TRUE}}) ); break;
{{/GETTER}}
{{/ATTRIBUTE_LIST}}
		default:	raiseUnexpectedMemberIndex();
		}
{{/HAS_ATTRIBUTES_TRUE}}
{{#HAS_ATTRIBUTES_FALSE}}
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
{{/HAS_ATTRIBUTES_FALSE}}
{{/MY_LOCAL}}
	}

{{NEWLINE}}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
{{#MY_LOCAL}}
{{#HAS_ATTRIBUTES_TRUE}}
		{{TYPE_NAME_CPP}}{{#IS_NATIVE_TRUE}}& r{{/IS_NATIVE_TRUE}}{{#IS_NATIVE_FALSE}}* p{{/IS_NATIVE_FALSE}} = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
{{#ATTRIBUTE_LIST}}
{{#SETTER}}
		case {{ATTRIBUTE_INDEX}}:		{{CALL_PREFIX}}{{METHOD_NAME}}( {{#IS_NATIVE_TRUE}}r, {{/IS_NATIVE_TRUE}}value.get< {{PARAMETER_TYPE}} >() ); break;
{{/SETTER}}
{{#IS_READ_ONLY}}
		case {{ATTRIBUTE_INDEX}}:		raiseAttributeIsReadOnly( ai ); break;
{{/IS_READ_ONLY}}
{{/ATTRIBUTE_LIST}}
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( {{#IS_NATIVE_TRUE}}r{{/IS_NATIVE_TRUE}}{{#IS_NATIVE_FALSE}}p{{/IS_NATIVE_FALSE}} );
		CORAL_UNUSED( value );
{{/HAS_ATTRIBUTES_TRUE}}
{{#HAS_ATTRIBUTES_FALSE}}
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
{{/HAS_ATTRIBUTES_FALSE}}
{{/MY_LOCAL}}
	}

{{NEWLINE}}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
{{#MY_LOCAL}}
{{#HAS_METHODS_TRUE}}
		{{TYPE_NAME_CPP}}{{#IS_NATIVE_TRUE}}& r{{/IS_NATIVE_TRUE}}{{#IS_NATIVE_FALSE}}* p{{/IS_NATIVE_FALSE}} = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
{{#METHOD_LIST}}
			case {{METHOD_INDEX}}:
				{
{{#PARAMETER_LIST}}
					{{PARAMETER_TYPE}} {{PARAMETER_NAME}} = args[++argIndex].get< {{PARAMETER_TYPE}} >();
{{/PARAMETER_LIST}}
{{#HAS_PARAMETERS_TRUE}}
					argIndex = -1;
{{/HAS_PARAMETERS_TRUE}}
					{{#HAS_RETURN}}__res.set< {{RETURN_TYPE}} >( {{/HAS_RETURN}}{{CALL_PREFIX}}{{METHOD_NAME}}({{#HAS_PARAMETERS_FALSE}}{{#IS_NATIVE_TRUE}} r {{/IS_NATIVE_TRUE}}{{/HAS_PARAMETERS_FALSE}}{{#HAS_PARAMETERS_TRUE}} {{#IS_NATIVE_TRUE}}r, {{/IS_NATIVE_TRUE}}{{#PARAMETER_LIST}}{{PARAMETER_NAME}}{{#PARAMETER_LIST_separator}}, {{/PARAMETER_LIST_separator}}{{/PARAMETER_LIST}} {{/HAS_PARAMETERS_TRUE}}){{#HAS_RETURN}} ){{/HAS_RETURN}};
				}
				break;
{{/METHOD_LIST}}
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
		CORAL_UNUSED( __res );
{{/HAS_METHODS_TRUE}}
{{#HAS_METHODS_FALSE}}
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( __res );
{{/HAS_METHODS_FALSE}}
{{/MY_LOCAL}}
	}
