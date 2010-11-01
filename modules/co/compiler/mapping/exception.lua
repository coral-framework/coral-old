local function template( writer, c, t )
	writer( [[
class ]], t.name, [[ : public co::Exception
{
public:
	]], t.name, [[()
	{;}

	]], t.name, [[( const std::string& message )
		: co::Exception( message )
	{;}
};
]] )
end

return template
