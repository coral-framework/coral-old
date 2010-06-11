namespace {{NAMESPACE}} {

{{#CLASS_LIST}}
	class {{CLASS_NAME}};
{{/CLASS_LIST}}

{{#STRUCT_LIST}}
	struct {{STRUCT_NAME}};
{{/STRUCT_LIST}}

	{{>FORWARD_DECLARATIONS}}

} // namespace {{NAMESPACE}}