{{NEWLINE}}

{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}

{{NEWLINE}}

namespace co {
template<> struct kindOf<{{TYPE_NAME_CPP}}> : public kindOfBase<{{TYPE_KIND}}> {};
template<> struct nameOf<{{TYPE_NAME_CPP}}> { static const char* get() { return "{{TYPE_NAME_FULL}}"; } };
template<> struct typeOf<{{TYPE_NAME_CPP}}> : public typeOfBase<{{TYPE_NAME_CPP}}, {{TYPE_TYPE_CLASS}}> {};
{{#HAS_AMBIGUOUS_BASES}}
namespace traits {
{{#AMBIGUOUS_BASE_LIST}}
	template<> struct hasAmbiguousBase<{{TYPE_NAME_CPP}}, {{BASE}}> : public TrueType {};
	template<> struct disambiguateBase<{{TYPE_NAME_CPP}}, {{BASE}}> { typedef {{SUPER}} Super; };
{{/AMBIGUOUS_BASE_LIST}}
} // namespace traits
{{/HAS_AMBIGUOUS_BASES}}

} // namespace co

{{NEWLINE}}

#endif // _{{TYPE_NAME_FULL_UPPERSCORE}}_H_
