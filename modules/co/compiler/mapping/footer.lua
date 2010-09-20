local function template( writer, c, t )
	writer( "\n" )

	c.utils.closeNamespaces( writer, t.namespace.fullName )

	writer( [[

namespace co {
template<> struct kindOf<]], t.cppName, [[> : public kindOfBase<]], t.kind, [[> {};
template<> struct nameOf<]], t.cppName, [[> { static const char* get() { return "]], t.fullName, [["; } };
template<> struct typeOf<]], t.cppName, [[> : public typeOfBase<]], t.cppName, [[, ]], t.typeInterfaceName, [[> {};
]] )

	-- if an interface has ambiguous base classes, define some auxiliary operators
	if t.kind == 'TK_INTERFACE' and next( t.ambiguousBases ) then
		writer( "namespace traits {\n" )
		for base, super in pairs( t.ambiguousBases ) do
			writer( [[
	template<> struct hasAmbiguousBase<]], t.cppName, [[, ]], base.cppName, [[> : public TrueType {};
	template<> struct disambiguateBase<]], t.cppName, [[, ]], base.cppName, [[> { typedef ]], super.cppName, [[ Super; };
]] )
		end
		writer( "} // namespace traits\n" )
	end

	writer( [[
} // namespace co

#endif // _]], t.fullNameUpperUnderline, [[_H_
]] )

end

return template
