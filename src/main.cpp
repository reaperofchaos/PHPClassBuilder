/*********************************************************************************
 * PHPClass Builder
 *
 * Reads an incomplete php class file and generates a more complete PHP Class with
 *  CRUD functions.
 *
 *  ******************************************************************************/
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include "PHPBuilder.h"

int handleArguments(int argc, char *argv[], std::string &fileName);
bool validateFileType(std::string &fileName);
void checkForInputFile(std::string &fileName, int &errorCode, std::vector<std::string> &arguments);
bool flagExists(std::string flag, std::vector<std::string> arguments);
std::string getFlagArgument(std::string flag, std::vector<std::string> arguments);
void handleFile(std::string fileName);
std::string stringToLower(std::string str);
PHPBuilder parseFile(std::ifstream &input);
std::string findField(std::string line);
std::string findClass(std::string line);
std::string findTableName(std::string line);

/**
 * A vector of strings with extensions of supported input file types
 */
std::vector<std::string> SupportedFileTypes = { ".php", ".PHP" };

/**
 * Main function to handles input file to create
 * a PHP class file with CRUD methods
 * 
 * Main function that handles provided arguments
 * with the handleArguments function, then creates
 * a php class file with CRUD methods using the
 * handleFile function. 
 * @param argc integer for total number of arguments
 * @param argv a character array of arguments
 * @return int
 */
int main(int argc, char *argv[])
{
    std::string fileName; 
    //parse arguments
    int errorCode = handleArguments(argc, argv, fileName);
    if(errorCode == 0)
    {
        handleFile(fileName);
    }
    return 0;
}

/**
 * Function that handles user provided arguments
 * Checks to make sure required file name, -l flag 
 * and input file name are set by the user and if not
 * errors are generated. Validates fileName with 
 * validateFileType function they are supported and 
 * generates an error if not. 
 * @param argc integer for total number of arguments
 * @param argv a character array of arguments
 * @param fileName string 
 * @return int
 */
int handleArguments(int argc, char *argv[], std::string &fileName)
{
    int errorCode = 0; 
    std::vector<std::string> arguments(argv + 1, argv + argc);
    if(argc > 0)
    {
        checkForInputFile(fileName, errorCode, arguments);
    }
    else
    {
        std::cout << "PHPClassBuilder command requires a -l flag with input file name. \n Program will now exit. \n";
        errorCode = 1;    
    }
    return errorCode; 
}


/**
 * Function that checks if a fileName has an extension
 * in the SupportedFileTypes vector and returns true if
 * sp and false if not. 
 * @param fileName string 
 * @return bool
 */
bool validateFileType(std::string &fileName)
{
    std::string fileNameLC = stringToLower(fileName);
    for(std::string f : SupportedFileTypes)
    {
        if(fileNameLC.find(f, fileNameLC.size()-4) != std::string::npos)
        {
            return true;
        }
    }
    return false; 
}

/**
 * Function that converts a string to lower case
 * 
 * Taken from //http://cplusplus.com/forum/beginner/218745/
 * Loops through characters in string and converts each 
 * character to lower. 
 * @param str string 
 * @return string
 */
std::string stringToLower(std::string str)
{
    for (size_t i=0; i<str.size(); ++i) 
    {
        if (isalpha(str[i])) 
        {
            str[i] = tolower(str[i]);
        }
    }
    return str; 
}

/**
 * Builds a PHP class file by parsing an input file.
 * 
 * This function parses the file line by line and retrieves
 * the class name with the findClass function, and the tableName
 * using the findTablename function and the fields using the 
 * findField function. The className, fields, and tableName were 
 * used to create a PHPBuilder object. 
 * 
 * @param input ifstream 
 * @return PHPBuilder
 */
PHPBuilder parseFile(std::ifstream &input)
{
    std::vector<std::string> fields;
    std::string className; 
    std::string tableName;
    std::string line; 
    std::string f;
    int i = 1; 
    for(std::string line; std::getline(input, line); )
    {
        if(findClass(line) != ""){
            className = findClass(line);
        }; 
        f = findField(line);
        if(f != ""){fields.push_back(f);}
        if(findTableName(line) != ""){
            tableName = findTableName(line);
        }; 
        i++; 
    }
    std::cout << "Class Name: "<< className << "\n";
    std::cout << "Table name: " << tableName << "\n";
    bool first = true; 
    for(std::string field : fields) 
    {
        if(!first){std::cout << ", ";}
        std::cout << field;
        first = false; 
    }
    std::cout << "\n"; 
    PHPBuilder php = PHPBuilder(className, fields, tableName);
    return php;
}

/**
 * Finds fields in each line of input file
 * 
 * Uses a regular expression to retrieve all
 * public properties in a PHP class 
 * 
 * @param line string 
 * @return string
 */
std::string findField(std::string line)
{
    std::regex rgx("public \\$(.*);");
    std::smatch matches;
    if(std::regex_search(line, matches, rgx))
    {
        return matches[1]; 
    }
    return ""; 
}

/**
 * Finds the class name in an input php file
 * 
 * Uses a regular expression to retrieve the
 * name of the class
 * 
 * @param line string 
 * @return string
 */
std::string findClass(std::string line)
{
    std::regex rgx("Class (.*)\\{");
    std::smatch matches;
    if(std::regex_search(line, matches, rgx))
    {
        return matches[1]; 
    }
    return ""; 
}

/**
 * Finds the name of the table an input php file
 * 
 * Uses a regular expression to retrieve the
 * name of the table identified with the table_name
 * property
 * 
 * @param line string 
 * @return string
 */
std::string findTableName(std::string line)
{
    std::regex rgx("private \\$table_name = \"(.*)\";");
    std::smatch matches;
    if(std::regex_search(line, matches, rgx))
    {
        return matches[1]; 
    }
    return ""; 
}

/**
 * Loads the input file, parses the file and builds
 * the new PHP Class File
 * 
 * Creates an input file handle with the fileName. A
 * PHPBuilder object is created using parseFile and then
 * a PHP Class File is built using the createClassFile
 * method. 
 * 
 * @param fileName string 
 * @return void
 */
void handleFile(std::string fileName)
{
    //load file
    std::ifstream input(fileName);
    //read file
    PHPBuilder phpbuilder = parseFile(input);
    std::cout << "\n\n";
    std::cout << "Creating PHP File \n";
    std::cout << "\n";
    phpbuilder.createClassFile();
    input.close(); 
    std::cout << phpbuilder.getFileName() << " has been created.\n";
}

/**
 * Function that checks if a flag exists in a
 * set of vector of provided arguments. Returns 
 * true if so and false if not. 
 * @param flag string 
 * @param arguments vector of strings 
 * @return bool
 */
bool flagExists(std::string flag, std::vector<std::string> arguments)
{
    for(std::string f : arguments)
    {
        if(f == flag)
        {
            return true; 
        }
    }
    return false;
}

/**
 * Function that checks if a flag exists in a
 * vector of provided arguments and returns
 * the argument immediately after it. Returns 
 * "-1" if not found.  
 * @param flag string 
 * @param arguments vector of strings 
 * @return string
 */
std::string getFlagArgument(std::string flag, std::vector<std::string> arguments)
{
    std::string arg; 
    int i = 0;
    for(std::string f : arguments)
    {
        if(f == flag)
        {
            return arguments[i+1];
        }
        i++;
    }
    return "-1";
}

/**
 * Function that checks if -l flag exists
 * and if an input fileName is provided 
 * using the getFlagArgument. Then it 
 * valideates the outputFileType using the
 * validateOutputType function. If an error 
 * is found the error code is set to 1. 
 * @param outputFileName string
 * @param outputFileType FileCreator::OutputTypes Enum Value for output type
 * @param errorCode int 
 * @param arguments vector of strings 
 * @return void
 */
void checkForInputFile(std::string &fileName, int &errorCode, std::vector<std::string> &arguments)
{
    if(flagExists("-l", arguments))
    {
        fileName = getFlagArgument("-l", arguments);
        //check if parameter provided
        if(fileName != "-1")
        {
            if(!validateFileType(fileName))
            {
                std::cout << "Error: Invalid File Type (Only ";
                bool first = true; 
                for(std::string f : SupportedFileTypes)
                {
                    if(!first){std::cout << ", ";}
                    std::cout << f;
                    first = false; 
                }
                std::cout << " outputs can be generated.)";
                errorCode = 1; 
            }
        }
        else
        {
            std::cout << "The -l parameter requires an input filename. Program will now exit. \n";
            errorCode = 1; 
        }
    }
    else
    {
        std::cout << "The -l parameter with an input file name is required. Program will now exit. \n";
        errorCode = 1; 
    }
}