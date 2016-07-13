# GDAL_2.1.0_VC
This repository contain Visual C++ 2010 workspace of the GDAL 2.1.0 library (http://gdal.org)

    * The Folder gdal-2.1.0 contain the gdal 2.1.0 source release 
	(http://download.osgeo.org/gdal/2.1.0/gdal210.zip). The file ./gdal-2.1.0/port/cpl_config.h.vc has been 
	renamed into ./gdal-2.1.0/port/cpl_config.h in order to compile with MS visual C++ and the file 
	./gdal-2.1.0/gcore/Version.rc has been slightly modified (including gdal_version.h instead of gdal.h)
	to avoid the RC1015 error compilation I have with VS 2010. I kept a copy of the original file provided
	by OSgeo (./gdal-2.1.0/gcore/Version.rc.orig).

	* The folder libgdal-2.1.0 contain a MS Visual Studio Solution with all the workspaces and a "binaries"
	sub-folder with each architecture and configuration binaries copied by VC post-build steps.
	There is both dynamic (dll) and static workspaces (lib) of GDAL 2.1.0 with 4 test workspaces:
	
		+ test_dynamic_gdalbuildvrt workspace:
		this is the gdalbuildvrt code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace dynamically linked to GDAL 2.1.0

		+ test_dynamic_ogr2ogr workspace:
		this is the ogr2ogr code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace dynamically linked to GDAL 2.1.0
	
		+ test_static_libgdal-2.1.0 workspace:
		this is a simple console program that dump all the supported drivers (GDAL/OGR) with theire 
		capabilities (i.e. Read/Write)
		
		+ test_static_gdalinfo workspace:
		this is the gdalinfo code provided by gdal (in the app folder of the source package) in a 
		"windows console" workspace statically linked to GDAL 2.1.0
	
	* The 3rdpart folder contain the libraries not provided internally by GDAL and necessary for some format
	I choosed to add (OSM, ECW...). For now it contain the libexpat (linlked dynamically), 
	Proj.4 lib V4.9.2 (linked dynamically), ECW 4.3 SDK (linked dynamically), libcurl 7.49.1 and 
	libsqlite 3.12 amalgamation included in the GDAL workspaces.
	
It contain too a dump (made by test_static_libgdal-2.1.0.exe) that print out all the supported drivers with
theire capabilites in ods and txt format : SupportedDrivers.ods and SupportedDrivers.txt

/!\ Due to GitHub (free version) limitation I've deleted the static version of gdal-2.1.0.lib (more than 650 MB),
so you will have to rebuild the static lib version to build the "test_static_XX workspaces".

## 2016-07-13: Warning this is still a work in progress		
	
	- Compiled with the following gdal drivers: ARG, CALS, WMTS, WMS, MRF, SENTINEL2, SAFE and MBTILES
	
	- Compiled with the following ogr drivers: AMIGOCLOUD, VDV, WFS, SQLITE, CSW, VFK, XLSX, GFT

	- Compiled with the libcurl 7.49.1, provided in the 3rdpart folder. I used the libcurl compiled
	for windows by Stefan Kanthak (home.arcor.de/skanthak/curl.html)

What still need to be done :

	- add GEOS geometry library
	
	- add iconv library
	   
    - test if it's possible to use Unicode charset (instead of Multi-Byte)
		

## 2016-07-06: 

    - Compiled with the ECW and JP2ECW driver support (in Read Only mode), ECW 4.3 RO SDK provided in the 3rdpart folder.
	The SDK is provided as downloaded except I've renamed all the "win32" folders to "x86".
	
	- Compiled with the ERS driver support.
	
	- Corrected bad Preprocessor value in the Release (in Dynamic and static workspace for both architectures).
	
## 2016-07-04:	

    - Added proj lib (4.9.2) dll workspace for both architecture in the 3rdpart folder
	
	- Added a new test project "test_dynamic_ogr2ogr"

 	
## 2016-06-29:

    - Added x64 version of all the workspaces (dll, lib and tests).
	I've compiled the 3rd part library libexpat for x64 architecture excluding in UNICODE mode for now
	
## 2016-06-28: 
What is compiling :

    - x86 version of all the workspaces (dll, lib and tests) 


