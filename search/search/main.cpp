
#include <iostream>

#include "InputOutput.hpp"
#include "search_task.hpp"

enum class error : int
{
   NONE,
   FILTER_NOT_FOUND,
   TARGET_NOT_FOUND,
   SEARCH_VALUE_NOT_FOUND
};

int main(int argc, char** argv)
{
   std::cout.sync_with_stdio(false);
   std::wcout.sync_with_stdio(false);
   Input input;
   input.root = astd::filesystem::current_path();
   string_std regexValue;
   error result = error::NONE;

   for (auto i = std::size_t(1); i < std::size_t(argc); i++)
   {
      std::string arg = argv[i];
      if (arg == "--clean")
      {
         input.clean_output = true;
      } else
      if (arg[0] == '-')
      {
         if (arg[1] == 'f')
         {
            if (arg[2] == 'n')
               input.filterfilename = true;
            if (arg[2] == 'd')
               input.filterdirectoryName = true;
            if (arg[2] == 'c')
               input.filterfilecontent = true;
            i++;
            if (i < std::size_t(argc))
            {
               std::string buffer(argv[i]);
               input.filterEx = string_std(buffer.begin(), buffer.end());
            }
            else
            {
               std::cout << "error : filter not found";
               result = error::FILTER_NOT_FOUND;
            }
         }
         else
         {
            for (char c : arg)
            {
               if (c == 'r')
                  input.recursive = true;
               if (c == 'c')
                  input.content = true;
               if (c == 'd')
                  input.directoryName = true;
               if (c == 'n')
                  input.filename = true;
               if (c == 't')
               {
                  i++;
                  if (i < std::size_t(argc))
                  {
                     input.root = argv[i];
                  }
                  else
                  {
                     std::cout << "error : no target chosen" << std::endl;
                     result = error::TARGET_NOT_FOUND;
                  }
               }
            }
         }
      }
      else
      {
         regexValue = string_std(arg.begin(), arg.end());
      }
   }

   if (!input.filename && !input.directoryName && !input.content)
   {
      input.filename = true;
   }

   if (result == error::NONE && !regexValue.size())
   {
      std::cout << "error: no search value" << std::endl;
      result = error::SEARCH_VALUE_NOT_FOUND;
   }
   input.regex = regexValue;

   if (!astd::filesystem::exists(input.root))
   {
      std::cout << "error: target doesn't exist" << std::endl;
      std::cout << "target : " << input.root << std::endl;
      result = error::TARGET_NOT_FOUND;
   }
   else if (!input.clean_output)
   {
      std::cout << "target: " << input.root << std::endl;
   }

   if (result == error::NONE)
   {
      if (!input.clean_output)
      {
         output("search \"");
         output(input.regex);
         output_line("\"");
         std::cout << "target " << input.root << std::endl;
         std::cout << "recursive " << input.recursive << std::endl;
         std::cout << "filename " << input.filename << std::endl;
         std::cout << "directory name " << input.directoryName << std::endl;
         std::cout << "file content " << input.content << std::endl;
         if (input.filterfilename || input.filterdirectoryName
            || input.filterfilecontent)
         {
            output("filter \"");
            output(input.filterEx);
            output_line("\"");
         }
         std::cout << "filter on filename " << input.filterfilename << std::endl;
         std::cout << "filter on directory name " << input.filterdirectoryName
            << std::endl;
         std::cout << "filter on file content " << input.filterfilecontent
            << std::endl;
         std::cout << std::endl;
      }

      auto start = std::chrono::system_clock::now();
      search_task task(input);
      task.do_search();

      if (!input.clean_output)
      {
         std::cout << "execution time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() - start)
            .count()
            << " ms" << std::endl;
      }
   }
   else
   {
      std::cout
          << "search [--clean] [-rcnd] [-t root_path] [-f[nd] filter_regex] regex\r\n"
             "--clean remove search detail and performance time\r\n"
             "-n search in filename\r\n"
             "-c search in file content\r\n"
             "-d search in directory name\t\n"
             "-r recurse on sub directory\r\n"
             "-t followed by a path, set the search location\r\n"
             "-f [ndc] followed by a pattern: will filter by directory / "
             "filter the file name where the content is searched or the "
             "file content";
      std::cout.flush();
   }
   return static_cast<int>(result);
}
