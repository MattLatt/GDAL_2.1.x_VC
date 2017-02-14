//##################################################################################//
//									gdal_rename										//
//**********************************************************************************//
// Desc: Command line executable to rename a dataset (including compagnions files)	//
//		 using it's SRS coordinates (TIF only supported for now !!!). It was		//
//		 designed initialy to rename tiled dataset in metric CRS					//
//		 																			//
//		 																			//
// Author: 	Mathieu Lattes (mathieu@lattes.me)								//
//																					//
//		 																			//
// Version:	v0.0.2_20170106-01 : using std lib instead of char * as str container	//
//		 																			//
// ToDo: support for other drivers and double coordinates							//
//																					//
//**********************************************************************************//
// History:	v0.0.1 : initial release without std lib								//
//																					//
//##################################################################################//

#include <iostream>

#include "gdal_priv.h"

#include "cpl_error.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"

#include "commonutils.h"

#include <string>
#include <math.h>

//***********************************************************************//
//!return number of digits
//***********************************************************************//
int nDigits (double dValue)
	{
	/*
	int Count =1;

	while ( dValue > 10.) 
		{
		dValue /= 10. ;
		++Count;
		}
	return Count;
	*/
	return int(ceil(log10(dValue)));
	}

//***********************************************************************//
//!get integer coordinate from GDALDataset
//***********************************************************************//
int getCoord (char refPointPart, double *adfGeoTransform, GDALDataset *poDataset)
	{
	int Coord = 0;
	switch ( refPointPart )
		{
		case 'W' : Coord = (int)adfGeoTransform[0]; break;
		case 'E' : Coord = (int)(adfGeoTransform[0]+(poDataset->GetRasterXSize()*adfGeoTransform[1])); break;
		case 'S' : Coord = (int)(adfGeoTransform[3]+(poDataset->GetRasterYSize()*adfGeoTransform[5])); break;
		case 'N' : Coord = (int)adfGeoTransform[3]; break;
		default	 : Coord = (int)adfGeoTransform[0]; 
		}
	return Coord;
	}

//***********************************************************************//
//! Call gdal rename function and rename old file if already existing
//***********************************************************************//
bool renameFileNoOverWrite (std::string sourcePath, std::string newPath)
	{
	int Success =  ::VSIRename( sourcePath.c_str(), newPath.c_str() );
		
	//already exist ?
	if (Success == -1)
		{
		std::string sNewFilePathTmp = newPath.substr(0,newPath.length()-4);
		sNewFilePathTmp += "_OldName" + newPath.substr(newPath.length()-4, 4);

		//trying to rename already existing file  
		Success = ::VSIRename( newPath.c_str(), sNewFilePathTmp.c_str() );
			
		if ( Success == 0 )
			//do the inital rename if success
			{ Success =  ::VSIRename( sourcePath.c_str(), newPath.c_str() ); }
		else
			//exit program
			{
			std::cout << "Error while renaming \"" << sourcePath << "\" to \"" << newPath << "\"" << std::endl;
			std::cout << "Check if file is not already opened or check file access rights...." << std::endl ;
			return 1;
			}
		}

	}

//***********************************************************************//
//! get world file extension of a GDALDataset
//***********************************************************************//
std::string getWorldFileExt (std::string inputFile)
	{	
	std::vector<std::string> compFilesExt;

	std::string inputExt = ::CPLGetExtension(inputFile.c_str());

	std::string wfExt = inputExt.substr(0,1) + inputExt.substr(2,1) + 'w';

	return wfExt;
	}



//***********************************************************************//
//! get an array of compagnon files extensions of a dataset (ie: tfw, prj
//! for a tif file)
//***********************************************************************//
std::vector<std::string> getCompFilesExt (std::string inputFile)
	{	
	std::vector<std::string> compFilesExt;
	std::string inputExt = ::CPLGetExtension(inputFile.c_str());
	
	compFilesExt.push_back(getWorldFileExt(inputFile));

	if ( inputExt == ".tif")
		{
		compFilesExt.push_back("prj");
		}

	return compFilesExt;
	}

/************************************************************************/
/*                               Usage()                                */
/************************************************************************/
static void Usage(const char* pszErrorMsg = NULL)
	{
    printf( "Usage: gdal_rename [--help|-h] [--input-file-to-rename|-i] [--refpoint|-r]\n"
			"                   { [--coord-zero-padding|-z] [--coord-length|-l]\n"
			"                     [--coord-type|-t] [--coord-sep|-s] [--prefix|-p]\n"
			"                     [--suffix|-s] } | { [--printf-syntax|-f] }\n" 
			"					dataset_to_rename\n\n\n" );

    printf( " --help|-h: print this message\n"
			" --refpoint|-r: Ref point to use in the name. Any pair of 'W', 'E' and 'N',\n"
			"                'S' (ie: WN, NW, EN, NE, WS, SW, ES, SE).\n"
			" --prefix|-p: Prefix to use for the new file name format (ie: \n"
			"              Prefix_XXXXXXXXXXX.EXT)\n"
			" --coord-length|-l: number of digits to use in coordinates value starting with\n"
			"                    high wheight digits (ie: using 4 with easting value of \n"
			"                    621000.0 will print 06210 with zero padding and \n"
			"                    6210 without)\n"
			" --coord-zero-padding|-z: wether or not padding coordinate with 0 if inequal \n"
			"                          number of digitsbetween easting and northing\n \n"
			" --coord-type|-t: 'int' or 'real' used to format coordinates\n"
			" --coord-sep|-s: separator of coordinates components when formating\n"
			" --suffix|-x: suffix to use for the new file name format (ie: \n"
			"              XXXXXXXXXXX_suffix.EXT)\n"
			" --printf-syntax|-f: a string with printf syntax instead of previous params\n"
			"                     (ie: Tiles_%.4d-%.4d_Lambert93) /!\ no syntaxic check\n"
			"                     and only 2 substitution allowed for easting/northin\n");

    if( pszErrorMsg != NULL )
        fprintf(stderr, "\nFAILURE: %s\n", pszErrorMsg);

    exit( 1 );
	}


//***********************************************************************//
//!Main
//***********************************************************************//
int main(int argc, char* argv[])
	{
    ::EarlySetConfigOptions(argc, argv);

    ::GDALAllRegister();
				
	//const char *pszGdalDataPath = ".\\gdal-data" ;
	//const char *pszProjLibPath = ".\\projlib" ;
	const char *pszFilePath = "Test.tif";
	const char *pszCoordRefPoint = "WN"; // any Easting/Northing pair combination in the following list N, E, S, W 
	bool		pszCoordPadding = true; 
	bool		printUsage = false;
	const char *pszCoordLenght = "7"; 
	const char *pszCoordType = ""; //"reel"
	//const char *pszCoordSign = "yes"; //"no"
	const char *pszCoordSep = "_"; 
	const char *pszPrefix = ""; 
	const char *pszSuffix = "";
	const char *pszPrintf= ""; // printf syntax for the whole renaming string ex: "Tile_%.4d_%.4d_SRS" /!\ no syntax check
					

    argc = ::GDALGeneralCmdLineProcessor( argc, &argv, 0 );

    if( argc > 1 )
		{
		for( int i = 0; argv != NULL && argv[i] != NULL; i++ )
			{
			if( EQUAL(argv[i], "--help") || EQUAL(argv[i], "-h") )
				{ printUsage = true;}
/*
			if( EQUAL(argv[i], "--input-file-to-rename") || EQUAL(argv[i], "-i") )
				{ pszFilePath = argv[i+1]; }*/

			if( EQUAL(argv[i], "--refpoint") || EQUAL(argv[i], "-r") ) //WN, WS, EN, ES
				{ pszCoordRefPoint = argv[i+1]; }

			if( EQUAL(argv[i], "--coord-zero-padding") || EQUAL(argv[i], "-z") ) //default '', but can be any char supported by os for file name
				{ pszCoordPadding = true; }

			if( EQUAL(argv[i], "--coord-length") || EQUAL(argv[i], "-l") ) // size of number of coord to use for renaming /!\ if less than number removed by right
				{ pszCoordLenght = argv[i+1]; }

			if( EQUAL(argv[i], "--coord-type") || EQUAL(argv[i], "-t") ) // default int 
				{ pszCoordType = argv[i+1]; }

/*			if( EQUAL(argv[i], "--coord-sign") ) // +/- or  N/S - E/W
				{ pszFilePath = argv[i+1]; }
*/
			if( EQUAL(argv[i], "--coord-sep") || EQUAL(argv[i], "-s") ) //any char
				{ pszCoordSep = argv[i+1]; }

			if( EQUAL(argv[i], "--prefix") || EQUAL(argv[i], "-p") ) //any string
				{ pszPrefix = argv[i+1]; }

			if( EQUAL(argv[i], "--suffix") || EQUAL(argv[i], "-x") ) //any string
				{ pszSuffix = argv[i+1]; }

			if( EQUAL(argv[i], "--printf-syntax") || EQUAL(argv[i], "-f") ) // use instead of --coord-sign, --coord-padding, --coord-type, --coord-lenght, --prefix, --suffix
				{ pszPrintf = argv[i+1]; }

			/*
			if( EQUAL(argv[i], "--generate-batch") ) //any string
				{ pszSuffix = argv[i+1]; }*/

			}
		pszFilePath = argv [argc - 1];
		}
	if (argc <= 1 || printUsage) 
		{
		Usage();
		}

	//Setting env vars 

	//::CPLSetConfigOption( "GDAL_DATA", pszGdalDataPath );
	//::CPLSetConfigOption( "PROJ_LIB", pszProjLibPath );

    ::GDALDataset  *poDataset = NULL;
    //::GDALAllRegister();

	std::string inputDataset = pszFilePath;
	
	std::string sFilename	= ::CPLGetFilename(pszFilePath);
	std::string sDirName	= ::CPLGetDirname(pszFilePath);
	std::string sExt		= ::CPLGetExtension(pszFilePath);
	std::string sBasename	= ::CPLGetBasename(pszFilePath);
	
	std::string sNewFileName;
	std::string sCompFileName = sDirName + "\\" + sBasename + "." ;
	std::string sMainPrintfStx ;


	poDataset = (GDALDataset *) ::GDALOpen( pszFilePath, GA_ReadOnly );
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
			std::string sCoordPrintfStx ;

			if (pszPrintf == "")
				{
				sCoordPrintfStx += "%";
				if ( pszCoordPadding )
					{ sCoordPrintfStx +=  "."; }
				if ( pszCoordLenght != "" )
					{ sCoordPrintfStx += pszCoordLenght ; }
				if ( EQUALN(pszCoordType , "real", 4) )
					{ sCoordPrintfStx += "f" ; }
				else
					{ sCoordPrintfStx += "d"; }
				
				sMainPrintfStx = pszPrefix + sCoordPrintfStx + pszCoordSep + sCoordPrintfStx + pszSuffix;
				}
			else
				{ sMainPrintfStx = pszPrintf;}

			int Coord0 = 0, Coord1 = 0;

			if ( strlen(pszCoordRefPoint)>1)
				{
				Coord0 = getCoord(pszCoordRefPoint[0], adfGeoTransform, poDataset);
				Coord1 = getCoord(pszCoordRefPoint[1], adfGeoTransform, poDataset);
				}
			else
				{
				Coord0 = getCoord('W', adfGeoTransform, poDataset);
				Coord1 = getCoord('N', adfGeoTransform, poDataset);
				}

			int baseDigits = std::max(nDigits(Coord0), nDigits(Coord1));
			int formatDigits = atoi(pszCoordLenght);

			if (baseDigits > formatDigits )
				{
				int exp = baseDigits-formatDigits;
				Coord0 = Coord0 / pow(10.,exp);
				Coord1 = Coord1 / pow(10.,exp);
				}

			char NewFileNameBuff [256] = "\0";
			char* pszNewFileName = (char*)&NewFileNameBuff;

			//Attention rajouter la conv en entier sur n char
			::CPLsnprintf(pszNewFileName, 256, sMainPrintfStx.c_str(), Coord0 , Coord1);

			::GDALClose((GDALDatasetH)poDataset);

			std::string sNewFilePath = sDirName + "\\" + pszNewFileName + "." + sExt ;

			bool Success =  renameFileNoOverWrite(pszFilePath, sNewFilePath) ;

			std::vector<std::string> CompFilesExts = getCompFilesExt(pszFilePath);
			std::vector<std::string>::iterator it = CompFilesExts.begin();

			while ( it != CompFilesExts.end() )
				{
				sCompFileName += (*it).c_str();
				std::string sNewCompFilePath = sDirName + "\\" + pszNewFileName + "." + (*it).c_str() ;
			
				Success =  renameFileNoOverWrite(sCompFileName, sNewCompFilePath) ;
				++it;
				}

			}
		else
			{
			::CPLprintf( "/!\ No GeoRef information found by GDAL in the file \"%s\", Exiting...\n", pszFilePath);
			::GDALClose((GDALDatasetH)poDataset);
			return 1;
			}

		}

	return 0;
	}

