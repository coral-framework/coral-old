local function template( writer, c, t )
	writer( "\n" )

	c.utils.closeNamespaces( writer, t.namespace.fullName )

	writer( [[

namespace co {
template<> struct kindOf<]], t.cppName, [[> : public kindOfBase<]], t.kind, [[> {};
template<> struct nameOf<]], t.cppName, [[> { static const char* get() { return "]], t.fullName, [["; } };
template<> struct typeOf<]], t.cppName, [[> : public typeOfBase<]], t.cppName, [[, ]], t.typeInterfaceName, [[> {};
} // namespace co

#endif // _]], t.fullNameUpperUnderline, [[_H_
]] )

end

return template
