/*
 * ToolBox.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: developer
 */
#include "ToolBox.hpp"

std::vector<uint8_t> toolbox::readFileBinary(
		boost::filesystem::path& r_pthFile) {

	if(boost::filesystem::exists(r_pthFile) && boost::filesystem::is_regular(r_pthFile))
	{
		// open the file:
		   std::ifstream file(r_pthFile.string(), std::ios::binary);

		// read the data:
		return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
		                              std::istreambuf_iterator<char>());
	}
	return std::vector<uint8_t>();
}

std::string toolbox::readFileString(boost::filesystem::path& r_pthFile) {
	if(boost::filesystem::exists(r_pthFile) && boost::filesystem::is_regular(r_pthFile))
	{
		std::ifstream t(r_pthFile.string());
		std::string str;

		t.seekg(0, std::ios::end);
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
			            std::istreambuf_iterator<char>());

		return str;
	}
	return std::string();
}

std::vector<uint8_t> toolbox::compressGZ( std::vector<char>& r_vIn, size_t nCompLvl) {

	if(!r_vIn.empty())
	{
		std::vector<uint8_t> vCompressed;
		//boost::iostreams::filtering_istream gzip_stream;
		//boost::iostreams::basic_array_source<char> input( &r_vIn[0], r_vIn.size() );
		/*
		boost::iostreams::stream<boost::iostreams::basic_array_source<char>> input_array(input) ;
		gzip_stream.push(boost::iostreams::gzip_compressor());
		gzip_stream.push(input_array);
		boost::iostreams::copy(gzip_stream, boost::iostreams::back_inserter(vCompressed));
		*/
		return vCompressed;
	}
	return std::vector<uint8_t>();
}

std::vector<uint8_t> toolbox::decompressGZ(std::vector<uint8_t>& r_vIn)
{
	if(!r_vIn.empty())
	{
		std::vector<uint8_t> vDecompressed;
		/*
		boost::iostreams::filtering_ostream gzip_stream;
		gzip_stream.push(boost::iostreams::gzip_compressor());
		gzip_stream.push(boost::iostreams::back_inserter(vDecompressed));
		gzip_stream.write( (char*)(&r_vIn[0]), r_vIn.size());
		boost::iostreams::close(gzip_stream);
		*/
		return vDecompressed;
	}
	return std::vector<uint8_t>();
}
