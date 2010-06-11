class {{TYPE_NAME}} : public co::UserException
{
public:
	{{TYPE_NAME}}()
	{;}

	{{NEWLINE}}

	{{TYPE_NAME}}( const std::string& message )
		: co::UserException( message )
	{;}
};
