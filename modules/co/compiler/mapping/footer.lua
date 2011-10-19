local function template( writer, c, t )
	writer( "\n" )

	c.utils.closeNamespaces( writer, t.namespace.fullName )

	writer( [[

namespace co {
template<> struct kindOf<]], t.cppName, [[> : public kindOfBase<]], t.kind, [[> {};
template<> struct nameOf<]], t.cppName, [[> { static const char* get() { return "]], t.fullName, [["; } };
} // namespace co

#endif // _]], t.fullNameUpperUnderline, [[_H_
]] )

end

return template
