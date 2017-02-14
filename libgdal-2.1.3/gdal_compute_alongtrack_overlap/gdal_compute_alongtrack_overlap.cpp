/* gdal_compute_alongtrack_overlap
 * @author Mathieu LATTES
 * @description a simple command line executable that link GDAL 2.1.2 dynamically
 * it allow to compute overlap between to aerial image footprints
 * V_0.0.1 : Tiff support Only for now
 ***********/

#include <iostream>

#include "gdal_priv.h"
#include "ogrsf_frmts.h"


#include "cpl_error.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"
#include <string>


int main(int argc, char* argv[])
	{
	const char *pszGdalDataPath = ".\\gdal-data" ;
	const char *pszProjLibPath = ".\\projlib" ;
	const char *pszFilePath = "C:\\Users\\Mathieu\\Desktop\\Mayotte\\1609XX_MAYOTTE16_emprises2.shp";
	const char *pszSQLStatement = "SELECT * FROM \"1609XX_MAYOTTE16_emprises2\" ORDER BY ImgNumber";												

    argc = GDALGeneralCmdLineProcessor( argc, &argv, 0 );
    if( argc > 1 )
		{
		for( int i = 0; argv != NULL && argv[i] != NULL; i++ )
			{
			if( EQUAL(argv[i], "--set-gdaldata-path") )
				{ pszGdalDataPath = argv[i+1]; }

			if( EQUAL(argv[i], "--set-projlib-path") )
				{ pszProjLibPath = argv[i+1]; }

			if( EQUAL(argv[i], "--shp-to-process") )
				{ pszFilePath = argv[i+1]; }

			}
		}

	//Setting env vars 

	CPLSetConfigOption( "GDAL_DATA", pszGdalDataPath );
	CPLSetConfigOption( "PROJ_LIB", pszProjLibPath );

    GDALDataset  *poDataset = NULL;
    GDALAllRegister();


	std::string inputDataset = pszFilePath;
	
	std::string filename = ::CPLGetFilename(pszFilePath);
	std::string direname = ::CPLGetDirname(pszFilePath);
	std::string basename = ::CPLGetBasename(pszFilePath);
	//std::string pathname = ::CPLGetPath(pszFilePath);

	std::string outputDataset = direname;

	poDataset = (GDALDataset *) ::GDALOpenEx( inputDataset.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );
	//poDataset = (GDALDataset *) GDALOpen(  "K:\\###DataTest\\Raster\\TIF\\12210_60730.tif", GA_ReadOnly );
   
	if( poDataset != NULL )
		{
		OGRLayer  *poLayer;

		//poLayer = poDataset->GetLayerByName( basename.c_str());
		poLayer = poDataset->ExecuteSQL(pszSQLStatement, NULL, NULL);
		//poLayer->SetAttributeFilter("ORDER BY ImgNumber");

		OGRFeature *poFeatureBack = NULL, *poFeatureCur = NULL;
		OGRGeometry *poGeometryBack = NULL, *poGeometryCur = NULL;
		poLayer->ResetReading();

		poFeatureBack = poLayer->GetNextFeature();
		poGeometryBack = poFeatureBack->GetGeometryRef();

		if ( poFeatureBack != NULL )
			{
			for (poFeatureCur = poLayer->GetNextFeature(); poFeatureCur != NULL ;poFeatureCur = poLayer->GetNextFeature() )
				{
				poGeometryCur = poFeatureCur->GetGeometryRef();
				if( poGeometryBack != NULL && poGeometryCur != NULL 
						&& wkbFlatten(poGeometryBack->getGeometryType()) == wkbPolygon
						&& wkbFlatten(poGeometryCur->getGeometryType()) == wkbPolygon )
					{
					OGRPolygon *poPolygonBack= (OGRPolygon *) poGeometryBack;
					OGRPolygon *poPolygonCur= (OGRPolygon *) poGeometryCur;

					OGRPoint poCenterBack;
					poPolygonBack->Centroid(&poCenterBack);


					OGRGeometry *Overlap = poPolygonBack->Intersection(poPolygonCur);
					if( Overlap != NULL  && wkbFlatten(Overlap->getGeometryType()) == wkbPolygon )
						{
						OGRPolygon *poPolygonOverlap = (OGRPolygon *) Overlap;

						printf( "BackArea(%.9f) / CurArea(%.9f) / IntersectionArea(%.9f) / Overlap (%.3f) \n", poPolygonBack->get_Area(), poPolygonCur->get_Area(), poPolygonOverlap->get_Area(),poPolygonOverlap->get_Area()/poPolygonBack->get_Area()*100.0);
						}
					}
				else
					{ printf( "no point geometry\n" );}


				//Retrieving the attibuts
				/*
				OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
				int iField;
				for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
					{
					OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
					if( poFieldDefn->GetType() == OFTInteger )
						printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
					else if( poFieldDefn->GetType() == OFTInteger64 )
						printf( CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64( iField ) );
					else if( poFieldDefn->GetType() == OFTReal )
						printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
					else if( poFieldDefn->GetType() == OFTString )
						printf( "%s,", poFeature->GetFieldAsString(iField) );
					else
						printf( "%s,", poFeature->GetFieldAsString(iField) );
					}
				*/
					
					OGRFeature::DestroyFeature( poFeatureBack );
					poFeatureBack = poFeatureCur;
					poGeometryBack = poGeometryCur;
					int temp= 1;
				}
			}



		::GDALClose((GDALDatasetH)poDataset);
		int temp = 0;
		}

	return 0;
	}

