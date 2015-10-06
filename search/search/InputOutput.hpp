
#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <string>
#include <mutex>
#include <vector>
#include <utility>
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/pool/pool_alloc.hpp"
#include <ostream>

#ifdef _MSC_VER
typedef std::wstring string_std;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>,
    boost::pool_allocator<wchar_t> > string_pool;
typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
    boost::pool_allocator<wchar_t> > sstreampool;
#else
typedef std::string string_std;
typedef std::basic_string<char, std::char_traits<char>,
    boost::pool_allocator<char> > string_pool;
typedef std::basic_stringstream<char, std::char_traits<char>,
    boost::pool_allocator<char> > sstreampool;
#endif

typedef std::basic_string<char, std::char_traits<char>,
   boost::pool_allocator<char> > plain_string_pool;

typedef std::pair<string_pool, int> pair_stringpool_int;

struct Input
{
   boost::filesystem::path root = boost::filesystem::current_path();

   string_std regex;
   bool recursive = false;
   bool content = false;
   bool filename = false;
   bool directoryName = false;

   string_std filterEx;
   bool filterfilename = false;
   bool filterfilecontent = false;
   bool filterdirectoryName = false;
};

struct Output
{
   std::mutex coutLock;
};

template<typename T>
   void
      output(const T& value, std::ostream& stream = std::cout)
   {
      stream << value;
      stream.flush();
   }

   template <typename T>
   void output(const std::basic_string<wchar_t, std::char_traits<wchar_t>, T>& value,
      std::wostream& stream = std::wcout)
   {
      stream << value;
      stream.flush();
   }

   template <typename T>
   void output_line(const T& value, std::ostream& stream = std::cout)
   {
      output(value);
      stream << "\r\n";
   }

   template <typename T>
   void output_line(
       const std::basic_string<wchar_t, std::char_traits<wchar_t>, T>& value,
       std::wostream& stream = std::wcout)
   {
      output(value);
      stream << L"\r\n";
   }

   template <typename T>
   void output(
      const boost::filesystem::path& input, std::basic_ostream<T>& stream)
   {
      output(input.native(), stream);
   }

   template <typename T>
   void output_line(
       const boost::filesystem::path& input, std::basic_ostream<T>& stream)
   {
      output_line(input.native(), stream);
   }

#endif //! INPUT_OUTPUT_HPP
