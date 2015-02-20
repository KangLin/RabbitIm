#
# RPM Finder
# Authors: Rohit Yadav <rohit@scaleninja.com>
#

FIND_PROGRAM(RPMBUILD
    NAMES rpmbuild
    PATHS "/usr/bin")

IF ( RPMBUILD )
    GET_FILENAME_COMPONENT(RPMBUILD_PATH ${RPMBUILD} ABSOLUTE)
    MESSAGE(STATUS "Found rpmbuild : ${RPMBUILD_PATH}")
    SET(RPMBUILD_FOUND "YES")
ELSE ( RPMBUILD ) 
    MESSAGE(STATUS "rpmbuild NOT found. RPM generation will not be available")
    SET(RPMBUILD_FOUND "NO")
ENDIF ( RPMBUILD )
