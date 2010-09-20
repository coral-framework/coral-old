local function template( writer, c, t )
	writer( [[
class ]], t.name, [[ : public co::UserException
{
public:
	]], t.name, [[()
	{;}

	]], t.name, [[( const std::string& message )
		: co::UserException( message )
	{;}
};
]] )
end

return template
