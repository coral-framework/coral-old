struct {{TYPE_NAME}}
{

{{#ATTRIBUTE_LIST}}
	{{TYPE}} {{NAME}}; // estimated size of {{SIZE}} bytes
{{/ATTRIBUTE_LIST}}

{{NEWLINE}}

	//! Default constructor.
	{{TYPE_NAME}}() :
{{#ATTRIBUTE_LIST}}
		{{NAME}}(){{#ATTRIBUTE_LIST_separator}},{{/ATTRIBUTE_LIST_separator}}
{{/ATTRIBUTE_LIST}}
	{;}

{{NEWLINE}}

	//! Default equality test operator.
	inline bool operator==( const {{TYPE_NAME}}& o ) const
	{
		return (
{{#ATTRIBUTE_LIST}}
			{{NAME}} == o.{{NAME}}{{#ATTRIBUTE_LIST_separator}} &&{{/ATTRIBUTE_LIST_separator}}
{{/ATTRIBUTE_LIST}}
		);
	}

{{NEWLINE}}

	//! Default inequality test operator.
	inline bool operator!=( const {{TYPE_NAME}}& o ) const { return !( *this == o ); }
};
