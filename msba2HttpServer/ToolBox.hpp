/*
 * ToolBox.hpp
 *
 *  Created on: Oct 23, 2013
 *      Author: developer
 */

#ifndef TOOLBOX_HPP_
#define TOOLBOX_HPP_
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>

#include <fstream>
#include <vector>
#include <string>

class toolbox {
public:
    bool writeBase64ToFile( boost::filesystem::path& r_pthFile, std::string& r_strdata, size_t nOffsetData );
	std::vector<uint8_t> readFileBinary( boost::filesystem::path& r_pthFile );
	std::string readFileString( boost::filesystem::path& r_pthFile );


	std::vector<uint8_t> compressGZ( std::vector<char> & r_vIn, size_t nCompLvl );
	std::vector<uint8_t> decompressGZ( std::vector<uint8_t> & r_vIn );

private:


};



#endif /* TOOLBOX_HPP_ */
