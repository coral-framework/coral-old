{{#IS_COMPONENT_FALSE}}
{{#IS_COMPOUND_TYPE}}
{{NEWLINE}}
private:
	{{TYPE_NAME_CPP}}{{#IS_NATIVE_TRUE}}&{{/IS_NATIVE_TRUE}}{{#IS_NATIVE_FALSE}}*{{/IS_NATIVE_FALSE}} checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );
{{NEWLINE}}
		co::{{TYPE_TYPE_CLASS}}* myType = co::typeOf<{{TYPE_NAME_CPP}}>::get();
{{NEWLINE}}
		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::{{TYPE_KIND}} ||
{{#IS_INTERFACE_FALSE}}
			instance.getType() != myType ||
{{/IS_INTERFACE_FALSE}}
{{#IS_INTERFACE_TRUE}}
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
{{/IS_INTERFACE_TRUE}}
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid {{TYPE_NAME_CPP}}*, but got " << instance );
{{NEWLINE}}
		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );
{{NEWLINE}}
{{#IS_INTERFACE_TRUE}}
		return dynamic_cast<{{TYPE_NAME_CPP}}*>( instance.getState().data.itf );
{{/IS_INTERFACE_TRUE}}
{{#IS_INTERFACE_FALSE}}
		return {{#IS_NATIVE_TRUE}}*{{/IS_NATIVE_TRUE}}reinterpret_cast<{{TYPE_NAME_CPP}}*>( instance.getState().data.ptr );
{{/IS_INTERFACE_FALSE}}
	}
{{/IS_COMPOUND_TYPE}}
{{/IS_COMPONENT_FALSE}}
};

{{NEWLINE}}

// ------ Reflector Creation Function ------ //

{{NEWLINE}}

co::Reflector* __create{{TYPE_NAME}}Reflector()
{
    return new {{TYPE_NAME}}_Reflector;
}

{{NEWLINE}}

{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}
