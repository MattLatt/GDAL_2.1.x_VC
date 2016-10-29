# GDAL_2.1.x_VC
==============================================================================
This repository contain Visual C++ 2010 workspace of the GDAL 2.1.x library (http://gdal.org)

    1. The Folder gdal-2.1.2 contain the 27/10/2016 gdal 2.1.2 source release
	(http://download.osgeo.org/gdal/2.1.2/gdal212.zip). The file ./gdal-2.1.2/port/cpl_config.h.vc has been 
	renamed into ./gdal-2.1.2/port/cpl_config.h in order to compile with MS visual C++ and the file 
	./gdal-2.1.2/gcore/Version.rc has been slightly modified (including gdal_version.h instead of gdal.h)
	to avoid the RC1015 error compilation I have with VS 2010. I kept a copy of the original file provided
	by OSgeo (./gdal-2.1.2/gcore/Version.rc.orig).

	2. The folder libgdal-2.1.2 contain a MS Visual Studio Solution with all the workspaces and a "binaries"
	sub-folder with each architecture and configuration binaries copied by VC post-build steps.
	There is both dynamic (dll) and static workspaces (lib) of GDAL 2.1.2 with 5 test workspaces:
	
		2.1. test_dynamic_gdalbuildvrt workspace:
		this is the gdalbuildvrt code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace dynamically linked to GDAL 2.1.2

		2.2. test_dynamic_ogr2ogr workspace:
		this is the ogr2ogr code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace dynamically linked to GDAL 2.1.2
	
		2.3. test_static_libgdal-2.1.2 workspace:
		this is a simple console program that dump all the supported drivers (GDAL/OGR) with theire 
		capabilities (i.e. Read/Write)
		
		2.4. test_static_gdalinfo workspace:
		this is the gdalinfo code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace statically linked to GDAL 2.1.2
		
		2.5. test_dynamic_gdal_envvars_internals workspace:
		this is a simple console program dynamically linked to GDAL 2.1.2 that show how to set env vars 
		with C++, so you may not use the GDAL Cmd console.
	
	3. The 3rdpart folder contain the libraries not provided internally by GDAL and necessary for some format
	I choosed to add (OSM, ECW...). For now it contain the libexpat 2.4.0 (linked dynamically), 
	Proj.4 lib V4.9.2 (linked dynamically), ECW 4.3 SDK (linked dynamically), libcurl 7.49.1 (linked dynamically),
	lib geos 3.5.0 (linked dynamically) and libsqlite 3.12 amalgamation included in the GDAL workspaces.
	
It contain too a dump (made by test_static_libgdal-2.1.2.exe) that print out all the supported drivers with
theire capabilites in ods and txt format : SupportedDrivers.ods and SupportedDrivers.txt

/!\ Due to GitHub (free version) limitation I've deleted the static version of gdal-2.1.2.lib (more than 650 MB),
so you will have to rebuild the static lib version to build the "test_static_XX workspaces".

## 2016-08-12:	
	1. Compiled with the 27/10/2016 GDAL 2.1.2 Release
	
	2. Added a new test workspace to show how to load GDAL_DATA and PROJ_LIB env vars form C++.

## 2016-08-12:	

	1. Compiled with the HDF5 drivers (dynamically linked to the v1.8.17 of the HDF5 lib, binaries provided in the 3rdpart folder) 

What still need to be done :
	
	1. add iconv library
	   
    1. test if it's possible to use Unicode charset (instead of Multi-Byte)
		
## 2016-08-10:	

	1. Surrounded file URI with quote in the post-build events to fix issue when working in folder with space in the name
	as reported by Jan Heckman on the GDAL-dev mailing list.
	  
	1. Fixed some configuration with hard coded target name of binaries also in the post-build task. Replaced by macro $(TargetName)
	
	1. Deactivated optimisation for X64 Release on the gdalexif.cpp file of the gcore folder as a workaround
	to the C1001 error of the Visual Studio 2015 community edition, thanks again to Jan.
	
	1. Tested full solution import and generation with VS 2015 community edition.
	
	1. Fixed CleanVisualStudioTempFiles.bat so it work too when hour is lesser than 10.


## 2016-07-20:	

	1. Compiled with the july release of gdal 2.1.2
	
	1. Compiled with lib geos 3.5.0 linked dynamically (MS VC Workspace provided in the 3rdpart folder)
	
	1. Corrected the test_static_libgdal-2.1.2 main.cpp source (removed OGRRegisterAll as its already called by GDALRegisterAll)
	
	1. Renamed the repository GDAL_2.1.x_VC
	
	
## 2016-07-13:		
	
	1. Compiled with the following gdal drivers: ARG, CALS, WMTS, WMS, MRF, SENTINEL2, SAFE and MBTILES
	
	1. Compiled with the following ogr drivers: AMIGOCLOUD, VDV, WFS, SQLITE, CSW, VFK, XLSX, GFT

	1. Compiled with the libcurl 7.49.1, provided in the 3rdpart folder. I used the libcurl compiled
	for windows by Stefan Kanthak (home.arcor.de/skanthak/curl.html)

## 2016-07-06: 

    1. Compiled with the ECW and JP2ECW driver support (in Read Only mode), ECW 4.3 RO SDK provided in the 3rdpart folder.
	The SDK is provided as downloaded except I've renamed all the "win32" folders to "x86".
	
	1. Compiled with the ERS driver support.
	
	1. Corrected bad Preprocessor value in the Release (in Dynamic and static workspace for both architectures).
	
## 2016-07-04:	

    1. Added proj lib (4.9.2) dll workspace for both architecture in the 3rdpart folder
	
	1. Added a new test project "test_dynamic_ogr2ogr"

 	
## 2016-06-29:

    1. Added x64 version of all the workspaces (dll, lib and tests).
	I've compiled the 3rd part library libexpat for x64 architecture excluding in UNICODE mode for now
	
## 2016-06-28: 
What is compiling :

    1. x86 version of all the workspaces (dll, lib and tests) 


