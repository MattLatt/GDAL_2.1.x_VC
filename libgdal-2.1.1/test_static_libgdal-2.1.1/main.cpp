/* test_static_libgdal-2.1.1.cpp 
 * @author Mathieu LATTES
 * @description a simple command line executable that link GDAL 2.1.1 statically
 * it print all the supported drivers registered with theire capabilities
 ***********/

#include <iostream>

#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogrsf_frmts.h"


int main(int argc, char* argv[])
	{
	std::cout << "GDAL lib Version : " << GDAL_VERSION_NUM  <<  std::endl;
	std::cout << "    ..   Name    : " << GDAL_RELEASE_NAME <<  std::endl;
	std::cout << "    ..   Date    : " << GDAL_RELEASE_DATE <<  std::endl <<std::endl;

	//Registering Raster Drivers
    GDALAllRegister();
	
	//Registering Vector Drivers
	//OGRRegisterAll();

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

