/* test_dynamic_gdal_envvars_internals
 * @author Mathieu LATTES
 * @description a simple command line executable that link GDAL 2.1.2 statically
 * it print all the supported drivers registered with theire capabilities
 ***********/

#include <iostream>

#include "gdal_priv.h"

#include "cpl_error.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"
#include <string>


int main(int argc, char* argv[])
	{
	const char *pszGdalDataPath = ".\\gdal-data" ;
	const char *pszProjLibPath = ".\\projlib" ;
	const char *pszFilePath = "Test.tif";

    argc = GDALGeneralCmdLineProcessor( argc, &argv, 0 );
    if( argc > 1 )
		{
		for( int i = 0; argv != NULL && argv[i] != NULL; i++ )
			{
			if( EQUAL(argv[i], "--set-gdaldata-path") )
				{ pszGdalDataPath = argv[i+1]; }

			if( EQUAL(argv[i], "--set-projlib-path") )
				{ pszProjLibPath = argv[i+1]; }

			if( EQUAL(argv[i], "--tif-to-rename") )
				{ pszFilePath = argv[i+1]; }
			}
		}

	//Setting env vars 

	//Using GDAL for registering GDAL_DATA Env Var 
	//CPLSetConfigOption( "SDK_ROOT", ".\\" );
	CPLSetConfigOption( "GDAL_DATA", pszGdalDataPath );
	CPLSetConfigOption( "PROJ_LIB", pszProjLibPath );

    GDALDataset  *poDataset;
    GDALAllRegister();

	std::string inputDataset = pszFilePath;
	std::string filename = inputDataset.substr(inputDataset.rfind("\\")+1);
	char ** Tree = ::CSLTokenizeString2(inputDataset.c_str(),"\\",CSLT_HONOURSTRINGS);

	std::string outputDataset;
	int nTok = ::CSLFindString(Tree,filename.c_str() );
	int iTok = 0;

	while ( iTok < nTok )
		{
		outputDataset += Tree[iTok] ;
		outputDataset += "\\";
		++iTok;
		}

	poDataset = (GDALDataset *) ::GDALOpen( inputDataset.c_str(), GA_ReadOnly );
	//poDataset = (GDALDataset *) GDALOpen(  "K:\\###DataTest\\Raster\\TIF\\12210_60730.tif", GA_ReadOnly );
   
	if( poDataset != NULL )
		{
		/*int Width = poDataset->GetRasterXSize();
		int Height = poDataset->GetRasterXSize();
		const char * folder = poDataset->GetDescription();*/
		double        adfGeoTransform[6];
		/*printf( "Driver: %s/%s\n", poDataset->GetDriver()->GetDescription(), poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
		printf( "Size is %dx%dx%d\n", poDataset->GetRasterXSize(), poDataset->GetRasterYSize(), poDataset->GetRasterCount() );
		if( poDataset->GetProjectionRef()  != NULL )
			{printf( "Projection is '%s'\n", poDataset->GetProjectionRef() );}
		*/
		if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
			{
			char buffer [256];
			int n = sprintf (buffer, "%.7d_%.7d.tif\0", (int)adfGeoTransform[0], (int)adfGeoTransform[3]);
			outputDataset += buffer;
			}

		::GDALClose((GDALDatasetH)poDataset);
		int Success =  ::VSIRename( inputDataset.c_str(), outputDataset.c_str() );
		inputDataset.replace(inputDataset.end()-3,inputDataset.end(),std::string("tfw"));     
		outputDataset.replace(outputDataset.end()-3,outputDataset.end(),std::string("tfw"));  
		Success =  ::VSIRename( inputDataset.c_str(), outputDataset.c_str() );
		int temp = 0;
		}
/*
	//When linking statically GDAL, use C++ SetEnv for PROJ_LIB as CPLSetConfigOption is not working ??
#ifdef _WIN32	
	_putenv_s( "PROJ_LIB", ".\\projlib");
#else
	std::setenv( "PROJ_LIB", ".\\projlib");
#endif	
*/

	/*
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



*/
	return 0;
	}

