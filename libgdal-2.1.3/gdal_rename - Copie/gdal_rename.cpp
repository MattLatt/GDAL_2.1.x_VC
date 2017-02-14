/* test_dynamic_gdal_envvars_internals
 * @author Mathieu LATTES
 * @description a simple command line executable that link GDAL 2.1.2 dynamically
 * it allow to rename dataset according the geospatial informations
 * V_0.0.1 : Tiff support Only for now
 ***********/

#include <iostream>

#include <boost/any.hpp>
//#include <boost/variant.hpp>
#include <boost/format.hpp>

#include "gdal_priv.h"

#include "cpl_error.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"
#include <string>


//typedef boost::variant<int, double, float, char *, std::string> PrintableVal;



bool parseRenamePattern(const char *pszGivenPattern,  char **papszAcceptedTaggs , char ***pszPrintfPattern, char ***papszOrderedTaggs)
	{
	bool Success = true;

    char **papszTokens = ::CSLTokenizeStringComplex( pszGivenPattern, "<>", true, true );
	//char **pszPrintfPattern = NULL;

    for( int i = 0; papszTokens != NULL && papszTokens[i] != NULL; ++i )
        {
		int ixTagg = ::CSLFindString(papszAcceptedTaggs, papszTokens[i]);
		if ( ixTagg >=0 )
			{ *papszOrderedTaggs = ::CSLAddString(*papszOrderedTaggs, papszAcceptedTaggs[ixTagg]); }
		else
			{ *pszPrintfPattern = ::CSLAddString(*pszPrintfPattern, papszTokens[i]); }
			//{ ::CPLStrlcat(*pszPrintfPattern,papszTokens[i],0); }

		//printf( "split %d: '%s'\n", i,  papszTokens[i] );
		}
	
	::CPLFree(papszTokens);

	return Success;
	}
/*
char *formatName (char *pszPrintfPatt, print)
	{

	char buffer [256];
	int n = ::CPLsnprintf((char *)&buffer, 256, pszPrintfPatt, (int)adfGeoTransform[0], (int)adfGeoTransform[3]);
	outputDataset += buffer;
	}

bool initTaggsValues()
	{
	bool Success = true;

	::CSLAddNameValue("X0", "6000");

	return Success;
	}
*/

int main(int argc, char* argv[])
	{
	const char *pszGdalDataPath = ".\\gdal-data" ;
	const char *pszProjLibPath = ".\\projlib" ;
	const char *pszFilePath = "Test.tif";
	char *pszTags []= { "X0", "Y0", "X1", "Y1", "Width", "Height" };
	char **papszFormatTaggs = (char **)&pszTags;
	char **papszOrderedUsedTaggs = NULL;

	const char *pszRenamePattern = "Tiles_%.7d<X0>_%.7d<Y0>_%.5d<Width>x%.5d<Height>pxl" ; //ie for a 1000x1000m Tile (GSD 0.5cm 2000x2000 pixels) in French Lambert 93 SRS 
												   // with Top Left Corner 652000.0 6854000.0 
												   //		"%.7X0_%.7Y1" -> 0652000_6854000"
												   //		"%.4X0-%.4Y1" -> 0652-6854"
												   //		"%.4X0_%.4Y0" -> 0652_6852"	
												   //		"%4X0_%4Y0" -> 652_6852"	

												 
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

			if( EQUAL(argv[i], "--rename-pattern") ) //as printf syntax but using X0, Y0, X1, Y1 instead
				{ pszRenamePattern = argv[i+1]; }
			}
		}

	//Setting env vars 

	::CPLSetConfigOption( "GDAL_DATA", pszGdalDataPath );
	::CPLSetConfigOption( "PROJ_LIB", pszProjLibPath );

    ::GDALDataset  *poDataset = NULL;
    ::GDALAllRegister();


	std::string inputDataset = pszFilePath;
	
	std::string filename = ::CPLGetFilename(pszFilePath);
	std::string direname = ::CPLGetDirname(pszFilePath);
	std::string basename = ::CPLGetBasename(pszFilePath);
	std::string pathname = ::CPLGetPath(pszFilePath);

	std::string outputDataset = pathname;

	std::string strPattern = pszRenamePattern;
/*
	bool isX0 = false;
	bool isY0 = false;

	int posFnd = strPattern.find("X0");
	if ( posFnd != std::string::npos)
		{
		isX0 = true;
		strPattern.replace(posFnd,posFnd+2,"d");
		}
	else
		{
		posFnd = strPattern.find("X1");
		if ( posFnd != std::string::npos)
			{
			isX0 = false;
			strPattern.replace(posFnd,posFnd+2,"d");
			}
		}
*/



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
			char **papszPrintfPatt = NULL;
			char Buffer[256];
			char *pszNewName = (char*)&Buffer;
			bool Success = ::parseRenamePattern(pszRenamePattern, papszFormatTaggs, &papszPrintfPatt, &papszOrderedUsedTaggs);
			
			int nTags = 4;

			for (int IndTag ; IndTag <=nTags ; ++IndTag)
				{
				char curBuffer[256];
				int nChars = 0;

				if (papszOrderedUsedTaggs[IndTag] == "X0" )
					{ nChars = ::CPLsnprintf((char *)&curBuffer, 256, papszPrintfPatt[IndTag], (int)adfGeoTransform[0]); }
				if (papszOrderedUsedTaggs[IndTag] == "X1" )
					{ nChars = ::CPLsnprintf((char *)&curBuffer, 256, papszPrintfPatt[IndTag], (int)(adfGeoTransform[0]+(poDataset->GetRasterXSize()*adfGeoTransform[1]))); }
				if (papszOrderedUsedTaggs[IndTag] == "Y0" )
					{ nChars = ::CPLsnprintf((char *)&curBuffer, 256, papszPrintfPatt[IndTag], (int)(adfGeoTransform[3]+(poDataset->GetRasterYSize()*adfGeoTransform[5]))); }		
				if (papszOrderedUsedTaggs[IndTag] == "Y1" )
					{ nChars = ::CPLsnprintf((char *)&curBuffer, 256, papszPrintfPatt[IndTag], (int)adfGeoTransform[3]); }
				if (papszOrderedUsedTaggs[IndTag] == "Width" )
					{ nChars = ::CPLsnprintf((char *)&curBuffer, 256, papszPrintfPatt[IndTag], poDataset->GetRasterXSize()); }		
				if (papszOrderedUsedTaggs[IndTag] == "Height" )
					{ nChars = ::CPLsnprintf((char *)&curBuffer, 256, papszPrintfPatt[IndTag], poDataset->GetRasterYSize()); }
				
				::CPLStrlcat(pszNewName,(char*)&curBuffer, 256);
				}
			


/*			std::vector<PrintableVal> Values;
			if ( papszOrderedUsedTaggs [0] == "X0")
				{
				}

			//::CSLAddNameValue(& "X0", "6000");
*/
			//std::string newName = boost::format(pszRenamePattern) % Values[0] % Values[1] % Values[2] % Values[3];

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

