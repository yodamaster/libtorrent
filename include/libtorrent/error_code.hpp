/*

Copyright (c) 2008, Arvid Norberg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef TORRENT_ERROR_CODE_HPP_INCLUDED
#define TORRENT_ERROR_CODE_HPP_INCLUDED

#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>

#if BOOST_VERSION < 103500
#include <asio/error_code.hpp>
#else
#include <boost/system/error_code.hpp>
#endif

#include "libtorrent/config.hpp"

namespace libtorrent
{

	namespace errors
	{
		enum error_code_enum
		{
			no_error = 0,
			file_collision,
			failed_hash_check,
			torrent_is_no_dict,
			torrent_missing_info,
			torrent_info_no_dict,
			torrent_missing_piece_length,
			torrent_missing_name,
			torrent_invalid_name,
			torrent_invalid_length,
			torrent_file_parse_failed,
			torrent_missing_pieces,
			torrent_invalid_hashes,
			too_many_pieces_in_torrent,
			invalid_swarm_metadata,
			invalid_bencoding,
			no_files_in_torrent,
			invalid_escaped_string,
			session_is_closing,
			duplicate_torrent,
			invalid_torrent_handle,
			invalid_entry_type,
			missing_info_hash_in_uri,
			file_too_short,
		};
	}

#if BOOST_VERSION < 103500
	typedef asio::error_code error_code;
	inline asio::error::error_category get_posix_category() { return asio::error::system_category; }
	inline asio::error::error_category get_system_category() { return asio::error::system_category; }

	extern asio::error::error_category libtorrent_category;
#else

	struct TORRENT_EXPORT libtorrent_error_category : boost::system::error_category
	{
		virtual const char* name() const;
		virtual std::string message(int ev) const;
		virtual boost::system::error_condition default_error_condition(int ev) const
		{ return boost::system::error_condition(ev, *this); }
	};

	extern TORRENT_EXPORT libtorrent_error_category libtorrent_category;

	using boost::system::error_code;
	inline boost::system::error_category const& get_system_category()
	{ return boost::system::get_system_category(); }
	inline boost::system::error_category const& get_posix_category()
#if BOOST_VERSION < 103600
	{ return boost::system::get_posix_category(); }
#else
	{ return boost::system::get_generic_category(); }
#endif
#endif

#ifndef BOOST_NO_EXCEPTIONS
	struct TORRENT_EXPORT libtorrent_exception: std::exception
	{
		libtorrent_exception(error_code const& s): m_error(s) {}
		virtual const char* what() const throw()
		{
			if (!m_msg) m_msg.reset(new std::string(m_error.message()));
			return m_msg->c_str();
		}
		virtual ~libtorrent_exception() throw() {}
		error_code error() const { return m_error; }
	private:
		error_code m_error;
		mutable boost::shared_ptr<std::string> m_msg;
	};
#endif
}

#endif

