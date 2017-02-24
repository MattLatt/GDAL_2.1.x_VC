# GDAL_2.1.x_VC
==============================================================================
This repository contain Visual C++ 2010 workspace of the GDAL 2.1.x library (http://gdal.org)

    1. The Folder gdal-2.1.3 contain the 01/20/2017 gdal 2.1.3 source release
	(http://download.osgeo.org/gdal/2.1.3/gdal213.zip). The file ./gdal-2.1.3/port/cpl_config.h.vc has been 
	renamed into ./gdal-2.1.3/port/cpl_config.h in order to compile with MS visual C++ and the file 
	./gdal-2.1.3/gcore/Version.rc has been slightly modified (including gdal_version.h instead of gdal.h)
	to avoid the RC1015 error compilation I have with VS 2010. I kept a copy of the original file provided
	by OSgeo (./gdal-2.1.3/gcore/Version.rc.orig).

	2. The folder libgdal-2.1.3 contain a MS Visual Studio Solution with all the workspaces and a "binaries"
	sub-folder with each architecture and configuration binaries copied by VC post-build steps.
	There is both dynamic (dll) and static workspaces (lib) of GDAL 2.1.3 with 5 test workspaces:
	
		2.1. test_dynamic_gdalbuildvrt workspace:
		this is the gdalbuildvrt code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace dynamically linked to GDAL 2.1.3

		2.2. test_dynamic_ogr2ogr workspace:
		this is the ogr2ogr code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace dynamically linked to GDAL 2.1.3
	
		2.3. test_static_libgdal-2.1.3 workspace:
		this is a simple console program that dump all the supported drivers (GDAL/OGR) with theire 
		capabilities (i.e. Read/Write)
		
		2.4. test_static_gdalinfo workspace:
		this is the gdalinfo code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace statically linked to GDAL 2.1.3
		
		2.5. test_dynamic_gdal_envvars_internals workspace:
		this is a simple console program dynamically linked to GDAL 2.1.3 that show how to set env vars 
		with C++, so you may not use the GDAL Cmd console.

/!\ Due to GitHub (free version) limitation I've deleted the static version of gdal-2.1.3.lib (more than 650 MB),
so you will have to rebuild the static lib version to build the "test_static_XX workspaces".

## 2017-02-24:	
	1. Compiled with the 01/20/2017 GDAL 2.1.3 Release but without linking the 3rd part libraries
	
## 2016-11-19:	
	1. Compiled with the 10/27/2016 GDAL 2.1.2 Release but without linking the 3rd part libraries
	
