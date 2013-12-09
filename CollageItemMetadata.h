///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        CollageItemMetadata.h
//
// Author:      Mike Conway
//
// Description: Metadata associated with an image or item in collage, used for sorting, 
//				'tooltips', and other uses
//
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef COLLAGEITEMMETADATA_H
#define COLLAGEITEMMETADATA_H

#include <string>
#include <time.h>

struct CollageItemMetadata {
	std::string fileName;				// file name after the slash and before the .
	std::string fileNameExtension;		// extension after the .
	std::string path;					// file path up to the last slash
	std::string title;					// free form title of the image (not yet implemented)
	time_t itemTimestamp;				// file timestamp
	unsigned int itemLoadOrder;			// order this image was loaded into collage
	unsigned int itemSetOrder;			// order of the image after taking a snapshot (not yet implemented)
};

// enumeration gives various sorting options, used to sort images based on their

enum MetadataSortOption {
	metadataSortFilename, metadataSortExtension, metadataSortPath, metadataSortTimestamp, metadataSortLoadOrder, metadataSortTitle
};

#endif

