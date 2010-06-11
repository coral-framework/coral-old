{{NEWLINE}}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		{{TYPE_NAME_CPP}}* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
{{#ATTRIBUTE_LIST}}
		case {{INDEX}}:		value.set< {{TYPE}} >( p->{{NAME}}{{#NEED_GET}}.get(){{/NEED_GET}} ); break;
{{/ATTRIBUTE_LIST}}
		default:	raiseUnexpectedMemberIndex();
		}
	}

{{NEWLINE}}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		{{TYPE_NAME_CPP}}* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
{{#ATTRIBUTE_LIST}}
{{#IS_ARRAY_TRUE}}
		case {{INDEX}}:		setArray<{{ELEMENT_TYPE}}>( p->{{NAME}}, value ); break;
{{/IS_ARRAY_TRUE}}
{{#IS_ARRAY_FALSE}}
		case {{INDEX}}:		p->{{NAME}} = value.get< {{TYPE}} >(); break;
{{/IS_ARRAY_FALSE}}
{{/ATTRIBUTE_LIST}}
		default:	raiseUnexpectedMemberIndex();
		}
	}
