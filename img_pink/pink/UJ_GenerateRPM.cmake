# -*- sh -*-
# UjoImro, 2011
# This script generates an rpm source and build package

SET(PINK_MAJOR "1")
SET(PINK_MINOR "0")
if (NOT PINK_REVISION)
    SET(PINK_REVISION "0")
endif(NOT PINK_REVISION)

string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} TBZ2  TBZ2PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} ZIP   ZIPPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} TGZ   TGZPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-GENERIC  RPM-GENERICPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} DEB-GENERIC  DEB-GENERICPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} DEB-DEBIAN4  DEB-DEBIAN4PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} DEB-DEBIAN6  DEB-DEBIAN6PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-FEDORA17      RPM-FEDORA17PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-OpenSuSE12.1  RPM-OpenSuSE12.1PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} NSIS-WINDOWS      EXE-NSISPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-CENTOS6       RPM-CENTOS6PACKAGE )

if (TBZ2PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "TBZ2" )
elseif (ZIPPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "ZIP" )
elseif (TGZPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "TGZ" )
elseif (RPM-GENERICPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python, IPython, python-imaging, python-tk, python-matplotlib, python-matplotlib-tk, python-numpy" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
elseif (DEB-GENERICPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib, ipython, python-vtk" )
	SET( CPACK_GENERATOR              "DEB;TBZ2" )
elseif (DEB-DEBIAN4PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0, libncurses5, libboost-dev, liblapack3, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib, ipython, python-vtk" )
	SET( CPACK_GENERATOR              "DEB;TBZ2" )
elseif (DEB-DEBIAN6PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib, ipython, python-vtk" )
	SET( CPACK_GENERATOR              "DEB;TBZ2" )
elseif (RPM-FEDORA17PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python >= 2.6.0, ipython, python-imaging, python-imaging-tk, tkinter, python-matplotlib, python-matplotlib-tk, numpy" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
elseif (RPM-OpenSuSE12.1PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python >= 2.6.0, IPython, python-imaging, python-tk, python-matplotlib, python-matplotlib-tk, python-numpy" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
elseif (EXE-NSISPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "NSIS" )
	if (NOT PACKAGE_INSTALL_DIRECTORY)
	    execute_process ( COMMAND python -c "from distutils.sysconfig import get_python_lib; path=get_python_lib(); print(path)" OUTPUT_VARIABLE PACKAGE_INSTALL_DIRECTORY OUTPUT_STRIP_TRAILING_WHITESPACE )
	endif(NOT PACKAGE_INSTALL_DIRECTORY)
	SET( CPACK_PACKAGE_INSTALL_DIRECTORY ${PACKAGE_INSTALL_DIRECTORY} )
elseif (RPM-CENTOS6PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python >= 2.6.0, python-imaging, python-imaging-tk, tkinter, python-matplotlib, python-matplotlib-tk, numpy" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
else ()
	MESSAGE( FATAL "cmake configuration error: unknown package type" )
endif()
### Setting up CPack for the source package generation --------
### auto set SET(CPACK_GENERATOR                    "TBZ2;DEB" )
SET(CPACK_SOURCE_GENERATOR             "TBZ2;TGZ" )
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY  "Pink Image" )
SET(CPACK_PACKAGE_VENDOR               "ProCarPlan s.r.o." )
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/readme" )
SET(CPACK_RESOURCE_FILE_LICENSE        "${CMAKE_CURRENT_SOURCE_DIR}/CeCill_header.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR        ${PINK_MAJOR} )
SET(CPACK_PACKAGE_VERSION_MINOR        ${PINK_MINOR} )
SET(CPACK_PACKAGE_VERSION_PATCH        ${PINK_REVISION} )

SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES}
"/tutorial/"
"/private/"
"~$"
"\\\\.svn"
"/TODO"
)

# **************************************************************
# *************************** RPM ******************************
# **************************************************************

### Special settings for RPM generation -----------------------
SET(CPACK_RPM_PACKAGE_SUMMARY          "Pink is an image processing library" )
SET(CPACK_RPM_PACKAGE_NAME             "pink-image" )
SET(CPACK_RPM_PACKAGE_VERSION          ${PINK_MAJOR}.${PINK_MINOR}.${PINK_REVISION} )
SET(CPACK_RPM_PACKAGE_ARCHITECTURE     "x86_64" )
SET(CPACK_RPM_PACKAGE_RELEASE 	       1 )
SET(CPACK_RPM_PACKAGE_LICENSE 	       "CeCILL" )
SET(CPACK_RPM_PACKAGE_GROUP 	       "Productivity/Scientific/Other" )
SET(CPACK_RPM_PACKAGE_VENDOR 	       "ProCarPlan s.r.o." )
SET(CPACK_RPM_PACKAGE_DESCRIPTION      "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license." )
### SET(CPACK_RPM_PACKAGE_REQUIRES         "python >= 2.6.0, cmake >= 2.6")
SET(CPACK_RPM_PACKAGE_PROVIDES         "pink-image-1.0" )
#SET(CPACK_RPM_SPEC_INSTALL_POST 	May be used to set an RPM post-install command inside the spec file. For example setting it to "/bin/true" may be used to prevent rpmbuild to strip binaries (see [Bug7435])	-
#SET(CPACK_RPM_SPEC_MORE_DEFINE 	May be used to add any %define lines to the generated spec file.	-
#SET(CPACK_RPM_USER_BINARY_SPECFILE 	May be used to specify a user provided spec file instead of generating one. This is an feature which currently needs a patch see [Bug9679]	-
#SET(CPACK_RPM_GENERATE_USER_BINARY_SPECFILE_TEMPLATE 	May be used to generate a template for a user provided spec file. This is an feature which currently needs a patch see [Bug9679]	-
#SET(CPACK_RPM_<POST/PRE>_<UN>INSTALL_SCRIPT_FILE 	The content of the specified files will be embedded in the RPM spec file in the appropriate sections. This is an feature which currently needs a patch see [Bug8988] 	-
#SET(CPACK_RPM_PACKAGE_DEBUG 	May be set when invoking cpack in order to trace debug informations during CPack RPM run. For example you may launch CPack like this cpack -D CPACK_RPM_PACKAGE_DEBUG=1 -G RPM 	- 

# **************************************************************
# *************************** DEB ******************************
# **************************************************************

### Special settings for DEBIAN package generation -----------------------
SET( CPACK_DEBIAN_PACKAGE_NAME   "pink-image" )
SET( CPACK_DEBIAN_PACKAGE_VERSION ${PINK_MAJOR}.${PINK_MINOR}.${PINK_REVISION} )
SET( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
## autoset SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib" )
SET( CPACK_DEBIAN_PACKAGE_MAINTAINER "Laszlo Marak <ujoimro@gmail.com>" )
SET( CPACK_DEBIAN_PACKAGE_DESCRIPTION "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license." )
SET( CPACK_DEBIAN_PACKAGE_SECTION "Productivity/Scientific/Other" )
SET( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
SET( CPACK_DEBIAN_PACKAGE_RECOMMENDS "ipython" )
SET( CPACK_DEBIAN_PACKAGE_SUGGESTS "python-vtk" )
#SET( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/CMake/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/CMake/debian/prerm;" )

# **************************************************************
# ********************* NSIS Windows(tm) EXE *******************
# **************************************************************

#SET( CPACK_NSIS_MUI_ICON "" )                 # The icon file (.ico) for the generated install program. Both this and CPACK_NSIS_MUI_UNIICON need to set for this to have any effect. 	installer.ico
#SET( CPACK_NSIS_MUI_UNIICON "" )              # The icon file (.ico) for the generated uninstall program. Both this and CPACK_NSIS_MUI_ICON need to set for this to have any effect. 	uninstaller.ico
#SET( CPACK_PACKAGE_ICON "" )                  # A branding image that will be displayed on the top bar inside the installer. 	installer.bmp
#SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "" )   # Extra NSIS commands that will be added to the install Section. 	ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'
#SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "" ) # Extra NSIS commands that will be added to the uninstall Section. 	
#SET( CPACK_NSIS_COMPRESSOR "" )               # The arguments that will be passed to the NSIS SetCompressor command. 	/SOLID lzma
#SET( CPACK_NSIS_MODIFY_PATH "" )              # If this is set to "ON", then an extra page will appear in the installer that will allow the user to choose whether the program directory should be added to the system PATH variable. 	ON
SET( CPACK_NSIS_DISPLAY_NAME "Pink Image Processing Library" ) # Undocumented. "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project"
#SET( CPACK_NSIS_INSTALLED_ICON_NAME "" )      # Set the icon used for the Windows "Add or Remove Programs" tool. 	"bin\\\\MyExecutable.exe"
#SET( CPACK_NSIS_HELP_LINK  "" )               # Adds link to registry. URI. 	"http:\\\\\\\\www.my-project-home-page.org"
#SET( CPACK_NSIS_URL_INFO_ABOUT "" )           # Adds link to registry and the vendor in add/remove programs' "Click here for support information" in program entry links here. 	"http:\\\\\\\\www.my-personal-home-page.com"
SET( CPACK_NSIS_CONTACT "https://www.pinkhq.com" )                  # Adds link to add/remove programs' "Click here for support information" in program entry. 	"me@my-personal-home-page.com"
#SET( CPACK_NSIS_CREATE_ICONS_EXTRA "" )       # Additional NSIS commands for creating start menu shortcuts. 	set(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\${PROJECT_NAME}.exe'")
#SET( CPACK_NSIS_DELETE_ICONS_EXTRA "" )       # Undocumented. Possibly: Additional NSIS commands to uninstall start menu shortcuts. 	
#SET( CPACK_NSIS_MENU_LINKS "" )               # Used to override the Start Menu links. 	"doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/CMakeSetup.html" "CMakeSetup Help"
#SET( CPACK_NSIS_MUI_FINISHPAGE_RUN "" )       # If used, will make it possible for user to choose (on an additional page, displayed at the end of the installation) to run intalled program. Should point to program name to run, seemingly without any sub-directories of the installation directory in case program installed in such sub-directories (but please check generated NSIS script if you can't make it work). 	"MyExecutable.exe" 

### Calling CPack ---------------------------------------------
INCLUDE(CPack)









# LuM end of file

