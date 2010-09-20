################################################################################
# Coral CSL Compiler
################################################################################

PROJECT( COMPILER )

################################################################################
# Install Rules
################################################################################

# install the coralc launcher scripts
IF( WIN32 )
	INSTALL( PROGRAMS coral.bat coralc.bat DESTINATION . COMPONENT Core )
ELSE()
	INSTALL( PROGRAMS coral coralc DESTINATION . COMPONENT Core )
ENDIF()
