#ifndef CSVREADER_WHICH_YOU_REMEMBER_ARE_A_SPECIES_OF_WHALE
#define CSVREADER_WHICH_YOU_REMEMBER_ARE_A_SPECIES_OF_WHALE

//          Copyright Alex Peer, 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class CSVRow
{
public: 

	CSVRow()	{	;	}
	CSVRow( std::istream& s )
	{	readRowFrom( s );		}

	std::string const & operator[]( unsigned int index ) const
	{
		return data[ index ];
	}

	void readRowFrom( std::istream& s )
	{
		std::string entry;
		char cursor = ' ';

		bool shouldEscapeNext = false;

		
		while( cursor != '\n' )
		{
			cursor = s.get();

			// end of line; commit entry and exit loop
			if( cursor == '\n' && !shouldEscapeNext )
			{
				data.push_back( entry );
				break;
			}

			// end if end of file; commit entry and exit loop
			if( s.eof() )
			{
				data.push_back( entry );
				break;
			}

			if( cursor == '\r' )		continue; // ignore windows extra line ending chars

			// things to ignore if escaping
			if( cursor == '\\' && !shouldEscapeNext )
			{	shouldEscapeNext = true;	}
			
			// read everything inside quotes
			// so: subloop here
			if( cursor == '\"' && !shouldEscapeNext )
			{
				while( cursor != '\"')
				{
					cursor = s.get();

					if( s.eof() )
					{
						data.push_back( entry );
						break;
					}
				}
			}

			// if comma, commit entry and start new one
			if( cursor == ',' && !shouldEscapeNext )
			{
				data.push_back( entry );
				entry = "";
			}
			else
			{
			// write to entry string
				entry += cursor;
			}

			shouldEscapeNext = false;
		}

	}

	int numCols()
	{
		return data.size();
	}

private:
	std::vector< std::string > data;

};

class CSVFile
{
public:

	bool openFile( char * filename, bool hasHeaders = false )
	{
		if( file.is_open() )
			file.close();

		file.open( filename );
		if( file.fail() )
			return false;

		if( file.eof() )
		{
			file.close();
			return true;
		}

		if( hasHeaders )
			headers.readRowFrom( file );

		int i = 0;		
		while( ! file.eof() )
		{
			rows.push_back( CSVRow() );
			rows[i].readRowFrom( file );
			i++;
		}
		i--;

		// remove last row if it's empty
		if( rows[i].numCols() == 0 )
			rows.pop_back();

		file.close();
		return true;
	}

	CSVRow const & operator[]( unsigned int index ) const
	{
		return rows[ index ];
	}

	int numRows()
	{
		return rows.size();
	}

	bool hasHeaders()
	{
		return headers.numCols() > 0;
	}

	int getIndexByHeader( char * header )
	{
		for( int i = 0; i < headers.numCols(); i++ )
		{
			if( (headers[i]).compare( header ) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	CSVRow headers;
private:
    std::ifstream file;
	std::vector< CSVRow > rows;

};

#endif