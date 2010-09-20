################################################################################
# Coral CSL Compiler
################################################################################

PROJECT( COMPILER )

################################################################################
# Install Rules
################################################################################

# install the coralc launcher scripts
IF( WIN32 )
	INSTALL( PROGRAMS coralc.bat DESTINATION . COMPONENT Core )
ELSE()
	INSTALL( PROGRAMS coralc DESTINATION . COMPONENT Core )
ENDIF()
