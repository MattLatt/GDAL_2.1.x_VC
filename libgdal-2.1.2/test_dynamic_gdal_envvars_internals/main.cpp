/* test_dynamic_gdal_envvars_internals
 * @author Mathieu LATTES
 * @description a simple command line executable that link GDAL 2.1.2 statically
 * it print all the supported drivers registered with theire capabilities
 ***********/

#include <iostream>

#include "gdal_priv.h"
#include "cpl_conv.h"


int main(int argc, char* argv[])
	{
	const char *pszGdalDataPath = ".\\gdal-data" ;
	const char *pszProjLibPath = ".\\projlib" ;

    argc = GDALGeneralCmdLineProcessor( argc, &argv, 0 );
    if( argc > 1 )
		{
		for( int i = 0; argv != NULL && argv[i] != NULL; i++ )
			{
			if( EQUAL(argv[i], "--set-gdaldata-path") )
				{ pszGdalDataPath = argv[i+1]; }

			if( EQUAL(argv[i], "--set-projlib-path") )
				{ pszProjLibPath = argv[i+1]; }
			}
		}

	//Setting env vars 

	//Using GDAL for registering GDAL_DATA Env Var 
	//CPLSetConfigOption( "SDK_ROOT", ".\\" );
	CPLSetConfigOption( "GDAL_DATA", pszGdalDataPath );
	CPLSetConfigOption( "PROJ_LIB", pszProjLibPath );


/*
	//When linking statically GDAL, use C++ SetEnv for PROJ_LIB as CPLSetConfigOption is not working ??
#ifdef _WIN32	
	_putenv_s( "PROJ_LIB", ".\\projlib");
#else
	std::setenv( "PROJ_LIB", ".\\projlib");
#endif	
*/

	std::cout << "GDAL lib Version : " << GDAL_VERSION_NUM  <<  std::endl;
	std::cout << "    ..   Name    : " << GDAL_RELEASE_NAME <<  std::endl;
	std::cout << "    ..   Date    : " << GDAL_RELEASE_DATE <<  std::endl;

	//std::cout << " EnvVar SDK_ROOT : " << CPLGetConfigOption( "SDK_ROOT",NULL) <<  std::endl;
	std::cout << " EnvVar GDAL_DATA: " << CPLGetConfigOption( "GDAL_DATA",NULL) <<  std::endl;
	std::cout << " EnvVar PROJ_LIB : " << CPLGetConfigOption( "PROJ_LIB",NULL) <<  std::endl << std::endl ;

	//Registering Raster Drivers
    GDALAllRegister();
	
	std::cout << "GDAL Drivers: Rast. | Vect. | Read | Wri. | Driver Code | Driver Desc." << std::endl;
	for (int i = 0 ; i < GDALGetDriverCount() ; ++i)
		{
		GDALDataset  *poGDrv  = (GDALDataset *) GDALGetDriver(i);
		bool Raster = poGDrv->GetMetadataItem( GDAL_DCAP_RASTER ) != NULL ;
		bool Vector = poGDrv->GetMetadataItem( GDAL_DCAP_VECTOR ) != NULL ;
		bool Read	= poGDrv->GetMetadataItem( GDAL_DCAP_OPEN ) != NULL ;
		bool Write	= poGDrv->GetMetadataItem( GDAL_DCAP_CREATE ) != NULL ;
		std::cout << "            :  "<< (Raster ? "Yes" : "No ") << "  |  " << (Vector ? "Yes" : "No ") << "  |  " << (Read ? "Yes" : "No ")<< " |  " << (Write ? "Yes" : "No ") << " | " << poGDrv->GetDescription() << "\t| "  << poGDrv->GetMetadataItem( GDAL_DMD_LONGNAME )  << std::endl ;
		}

	return 0;
	}

