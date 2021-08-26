#include <iostream>
#include <fstream>
#include <vector>
#include <map>
/*******************************************************************************
 * PHPBuilder
 *
 * Class takes a name, fields and table name to build a PHP file with common 
 * CRUD functions. 
 * 
 *******************************************************************************/
class PHPBuilder
{
    private: 
        std::string className; /**< Name of PHP class */
        std::vector<std::string> Fields; /**< Vector of class properties */ 
        std::string tableName; /**< Name of table or view interacted with */ 
        std::string fileName; /**< Name of PHP output file */
        std::ofstream out; /**< Output File Handle */

    public: 
        /**
        * Default constructor
        */
        PHPBuilder(){}

        /**
        * Constructor that takes a class Name,
        *  vector of fields and name of table.
        * Sets the properties and creates the out
        * output fille handler. 
        * @param className a string argument.
        * @param Fields a vector of strings.
        * @param tableName a string argument
        */
        PHPBuilder(std::string className, std::vector<std::string> Fields, std::string tableName)
        {
            this->className = className; 
            this->Fields = Fields;
            this->tableName = tableName; 
            this->fileName = className + ".php"; 
            this->out.open(fileName, std::ios::out);
        }

        /**
         * Getter
         * Retrieves the name of the class 
         * @return string
         */
        std::string getClassName(){return this->className;}

        /**
         * Getter
         * Retrieves the fields 
         * @return vector of strings
         */
        std::vector<std::string>  getFields(){return this->Fields;}

        /**
         * Getter
         * Retrieves the name of the table 
         * @return string
         */
        std::string getTableName(){return this->tableName;}

        /**
         * Getter
         * Retrieves the name of the PHP output file
         * @return string
         */
        std::string getFileName(){return this->fileName;}

       /**
        * Function that breaks a string with more than
        * 70 characters into a vector of lines. If character
        * 70 is not a space character, it is split at the first
        * character before 70 that is a space. 
        * @return void
        */
        std::vector<std::string> checkIfLineIsTooLong(std::string line)
        {
            std::vector<std::string> lines;
            int i = 0;
            int current = 0; 
            int limit = 70;
            if(line.size() > limit)
            {
                while(line.substr(i, -1).size() > limit)
                { 
                    current = i; 
                    i = i + limit;
                    while(line.at(i) != ' ')
                    { 
                        i = i - 1;
                    }

                    //std::cout << "i is " << i << "/n";
                    lines.push_back(line.substr(current, i));
                }
                //skip the space by starting a character late
                lines.push_back(line.substr(i+1, -1));
            }
            else 
            {
                lines.push_back(line.substr(i, -1));
            }
            return lines;
        }

        /**
        * Function that writes a comment for a method
        * Writes a comment to be inserted above a class
        * method with the description, detailed description, 
        * parameters and return type. 
        * @return void
        */
        void createMethodComment(std::string Description, std::string DetailedDescription, 
                        std::vector<std::pair<std::string, std::string>> Params,
                        std::string returnType)
        {
            std::vector<std::string> DescriptionLines = checkIfLineIsTooLong(Description);
            std::vector<std::string> DetailedDescriptionLines = checkIfLineIsTooLong(DetailedDescription);

            this->out << "\t/**\n";
            for(std::string l : DescriptionLines)
            {
                this->out << "\t * " << l << "\n";
            }
            this->out << "\t *\n";
            if(Params.size() > 0)
            {
                for(std::pair<std::string, std::string> param : Params)
                {
                    this->out << "\t * @param " << param.first << " " << param.second << "\n";
                }
            }
            
            for(std::string d : DetailedDescriptionLines)
            {
                this->out << "\t * " << d << "\n";
            }
            this->out << "\t * @return " << returnType << "\n";
            this->out << "\t*/\n";
        }

        /**
        * Function that writes a comment for a class
        * Writes a comment to be inserted above a class
        * @return void
        */
        void createClassComment()
        {
            this->out << "\t/**\n";
            this->out << "\t * Class used to handle CRUD operations related to " 
                     << this->stringToLower(this->className) << "s. \n";
            this->out << "\t */\n";
        } 

        /**
        * Writes onstructor for the PHP file
        * Writes comments and constructor function for
        * PHP file 
        * @return void
        */
        void createConstructor()
        {
            std::string Description = "Constructor to create an " 
                                    + this->stringToLower(this->className) 
                                    + " object.";
            std::vector<std::pair<std::string, std::string>> Params = { 
                                    std::make_pair("PDO", "$db a configured pdo connection obj")
                                    };
            std::string DetailedDescription = "Creates "
                                             + this->className 
                                             + " object by setting the $conn to a PDO object";
            std::string returnType = "Article";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function __construct($db)\n";
            this->out << "\t\t{\n";
            this->out <<"\t\t\t$this->conn = $db;\n";
            this->out << "\t\t}\n";
            this->out << "\n";
        }

        /**
        * Writes a PHP function to check if a record
        * exists
        * Writes comments and CheckIfExists function 
        * for the output PHP file 
        * @return void
        */
        void createCheckIfExists()
        {
            std::string Description = "Checks if an " 
                                    + this->stringToLower(this->className) 
                                    + " object exists in the database.";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Prepares and executes a simple Select count statement"
                                             "to see if there are multiple"
                                            + this->stringToLower(this->className) 
                                            + "records with the same fields";
            std::string returnType = "bool";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function checkIfExists() : bool\n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$query = \"SELECT Count(*) as count\n"; 
            this->out << "\t\t\t\t FROM \" . $this->table_name . \"\n";
            bool first = true; 
            int i = 0; 
            for (std::string f : this->Fields)
            {
                if(first){
                    this->out << "\t\t\t\tWHERE " << f << " = \" . $this->" << f << " . \"";
                }else
                {
                    this->out << "\t\t\t\tAND " << f << " = \" . $this->" << f << " . \"";
                }
                i++;
                if(i == this->Fields.size())
                {
                    this->out << "\";\n";
                }
                else
                {
                    this->out << "\n";
                }
                
                first = false; 
            }
            this->out << "\n";
            this->out << "\t\t\t//prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\t\t\t$stmt->execute();\n";
            this->out << "\t\t\t$row = $stmt->fetch(PDO::FETCH_ASSOC);\n";
            this->out << "\t\t\tif($row['count'] > 0)\n";
            this->out << "\t\t\t{\n";
            this->out << "\t\t\t\treturn true;\n";
            this->out << "\t\t\t}\n";
            this->out << "\t\t\treturn false;\n";
            this->out << "\t\t}\n";
            this->out << "\n";
        }

        /**
        * Writes a PHP function to check if a record
        * exists and retrieves the fields
        * Writes comments and GetIfExists function 
        * for the output PHP file 
        * @return void
        */
        void createGetIfExists()
        {
            std::string Description = "Checks if an " 
                                    + this->stringToLower(this->className) 
                                    + " object exists in the database using currently set properties.";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Prepares and executes a simple Select statement that"
                                              " retrieves an "
                                            + this->stringToLower(this->className) 
                                            + " object from the database with currently set properties";
            std::string returnType = "bool";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function checkIfExists() : bool\n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$query = \"SELECT *\n"; 
            this->out << "\t\t\t\t FROM \" . $this->table_name . \"\n";

            bool first = true; 
            int i = 0; 
            for (std::string f : this->Fields)
            {
                if(first){
                    this->out << "\t\t\t\tWHERE " << f << " = \" . $this->" << f << " . \"";
                }else
                {
                    this->out << "\t\t\t\tAND " << f << " = \" . $this->" << f << " . \"";
                }
                i++;
                if(i == this->Fields.size())
                {
                    this->out << "\";\n";
                }
                else
                {
                    this->out << "\n";
                }
                
                first = false; 
            }
            this->out << "\n";
            this->out << "\t\t\t//prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\t\t\t$stmt->execute();\n";
            this->out << "\t\t\t$row = $stmt->fetch(PDO::FETCH_ASSOC);\n";
            this->out << "\t\t\tif($row['count'] > 0)\n";
            this->out << "\t\t\t{\n";
            this->out << "\t\t\t\treturn true;\n";
            this->out << "\t\t\t}\n";
            this->out << "\t\t\treturn false;\n";
            this->out << "\t\t}\n";
            this->out << "\n";
        }

        /**
        * Function that writes bindings for parameters
        * in prepared PHP statments
        * Function that loops through fields and writes
        *  bindings for parameters in prepared PHP statments
        * @return void
        */
        void createBindings()
        {
            for(std::string f: this->Fields)
            {
                this->out << "\t\t\t$stmt->bindParam(\":"<< f << "\", $this->" << f << ");\n"; 
            }
        }

        /**
        * Function that assigns properties from query
        * results
        * Loops through fields and sets the class properties
        * to the row results of a sql squery
        * @return void
        */
        void setObjectPropertiesFromQuery()
        {
            for(std::string f: this->Fields)
            {
                this->out << "\t\t\t$this->" << f << " = $row['" << f << "'];\n";
            }
        }

        /**
        * Function that writes functions to sanitize fields
        * used in prepared sql queries
        * Loops through fields and assigns each field the 
        * fields value after sanitizing functions are used.
        * @return void
        */
        void sanitizeFields()
        {
            this->out << "\t\t\t//Santize\n";
            for(std::string f: this->Fields)
            {
                this->out << "\t\t\t$this->" << f << "=htmlspecialchars(strip_tags($" << f << "));\n";
            }
        }

        /**
        * Function that writes a read function to 
        * retrieve all records
        * Writes comments and function to read all 
        * records from the class's table
        * @return void
        */
        void createRead()
        {
            std::string Description = "Retrieves all records from " 
                                    + this->stringToLower(this->className) 
                                    + "s view.";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Prepares and executes a simple Select statement to"
                                            " retrieve all records in the " 
                                            + this->stringToLower(this->className) 
                                            + " view.";
            std::string returnType = "PDOStatement";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\t//Selects all records\n";
            this->out << "\t\tpublic function read()\n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$query = \"SELECT * \n";
            this->out << "\t\t\t\tFROM \" . $this->table_name . \"\n"; 
            this->out << "\t\t\t\t ORDER BY ID ASC\";\n"; 
            this->out << "\n";
            this->out << "\t\t\t//prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\n";
            this->out << "\t\t\t//execute\n";
            this->out << "\t\t\t$stmt->execute();\n";
            this->out << "\n";
            this->out << "\t\t\treturn $stmt;\n";
            this->out << "\t\t}";
            this->out << "\n";
            this->out << "\n";
        }

        /**
        * Function that writes a read one function to 
        * retrieve a record by an ID
        * Writes comments and function to read one 
        * record from the class's table with the
        * class's id. 
        * @return void
        */
        void createReadOne()
        {
            std::string Description = "Retrieves one " 
                                    + this->stringToLower(this->className) 
                                    + " record by ID";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Prepares and executes a simple Select statement "
                                              "retrieving all values for one record and updates the "
                                            +  this->stringToLower(this->className) + " object.";
            std::string returnType = "void";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function readOne() : void\n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t//query to read single record\n";
            this->out << "\t\t\t$query = \"SELECT * \n";
            this->out << "\t\t\t\tFROM \" . $this->table_name . \"\n"; 
            this->out << "\t\t\t\t WHERE Id = ?\n"; 
            this->out << "\t\t\t\t WHERE LIMIT 0,1\";\n"; 
            this->out << "\n";
            this->out << "\t\t\t//prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\n";
            this->out << "\t\t\t//bind id of product to be updated\n";
            this->out << "\t\t\t$stmt->bindParam(1, $this->Id);\n"; 
            this->out << "\n";
            this->out << "\t\t\t//execute query\n";
            this->out << "\t\t\t$stmt->execute();\n";
            this->out << "\n";
            this->out << "\t\t\t//get retrieved row\n";
            this->out << "\t\t\t$row = $stmt->fetch(PDO::FETCH_ASSOC);\n";
            this->out << "\n";
            this->setObjectPropertiesFromQuery();
            this->out << "\t\t}";
            this->out << "\n";
            this->out << "\n";
        }

        /**
        * Function that writes a read paging function to 
        * retrieve records in certain amounts
        * Writes comments and function to read specific
        * amounts of retrieved records from the class's
        * table  
        * @return void
        */
        void createReadPaging()
        {
            std::string Description = "Retrieves a certain number of records from " 
                                    + this->stringToLower(this->className) 
                                    + " view.";
            std::vector<std::pair<std::string, std::string>> Params = { 
                std::make_pair("int", "$from_record_num first record to display"),
                std::make_pair("int", "$records_per_page total number of records to retrieve")
                };
            std::string DetailedDescription = "Prepares and executes a simple Select statement"
                                            " to retrieve records starting from an id to a "
                                            "certain id in the " 
                                            + this->stringToLower(this->className) + " view.";
            std::string returnType = "PDOStatement";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function readPaging($from_record_num, $records_per_page)\n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t// select query\n";
            this->out << "\t\t\t$query = \"SELECT * \n";
            this->out << "\t\t\t\tFROM \" . $this->table_name . \"\n";
            this->out << "\t\t\t\tORDER BY ID ASC\n";
            this->out << "\t\t\t\tLIMIT ?, ?\";";
            this->out << "\n";
            this->out << "\t\t\t// prepare query statement\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare( $query );\n;";
            this->out << "\n";
            this->out << "\t\t\t// bind variable values\n";
            this->out << "\t\t\t$stmt->bindParam(1, $from_record_num, PDO::PARAM_INT);\n";
            this->out << "\t\t\t$stmt->bindParam(2, $records_per_page, PDO::PARAM_INT);\n";
            this->out << "\n";
            this->out << "\t\t\t// execute query\n";
            this->out << "\t\t\t$stmt->execute();\n";
            this->out << "\n";
            this->out << "\t\t\t// return values from database\n";
            this->out << "\t\t\treturn $stmt;\n";
            this->out << "\t\t}\n";
            this->out << "\n";
        }

        /**
        * Function that writes a count function to 
        * retrieve the total number of records in the
        * class's table
        * Writes comments and function to retrieves the
        * total number of records from the class's
        * table  
        * @return void
        */
        void createCount()
        {
            std::string Description = "Retrieves the total number of records in " 
                                    + this->stringToLower(this->className) 
                                    + " view.";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Prepares and executes a simple Select statement "
                                            "retreiving the count for all rows in the " 
                                            + this->stringToLower(this->className) 
                                            + " view.";
            std::string returnType = "int";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\t//Retrieves total number of rows in table\n";
            this->out << "\t\tpublic function count(): int\n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$query = \"SELECT COUNT(*) as count FROM \" . $this->table_name . \"\";\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\t\t\t$stmt->execute();\n";
            this->out << "\t\t\t$row = $stmt->fetch(PDO::FETCH_ASSOC);\n";
            this->out << "\t\t\treturn $row['count'];\n";
            this->out << "\t\t}\n";
            this->out << "\n";
        }

        /**
        * Function that writes an insert function to 
        * insert a record into the class's table
        * Writes comments and function to insert the
        * a record into the class's table using the properties
        * set in the class. 
        * @return void
        */
        void createInsert()
        {
            std::string Description = "Inserts a record into the " 
                                    + this->stringToLower(this->className) 
                                    + " table.";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Inserts a record into the " 
                                            + this->stringToLower(this->className) 
                                            + " table using currently set properties in " 
                                            + this->stringToLower(this->className) 
                                            + " object. If successful returns true.";
            std::string returnType = "bool";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function Insert(): bool \n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$query = \"INSERT INTO \n"; 
            this->out << "\t\t\t\" . $this->table_name . \"\n"; 
            this->out << "\t\t\tSET \n"; 
            int i = 1; 
            for(std::string f : this->Fields)
            {
                if(i % 2 != 0)
                {
                    this->out << "\t\t\t\t";
                }
                this->out << f << "=:" << f;
                if(i < this->Fields.size())
                {
                    if(i %2 == 0)
                    {
                        this->out << ",\n"; 
                    }
                    else
                    {
                        this->out << ", ";
                    }
                }
                else
                {
                    this->out << "\";\n";
                }
                
                i++;
            }
            this->out << "\n";
            this->out << "\t\t\t//prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\n";
            this->sanitizeFields();
            this->out << "\n";
            this->out << "\t\t\t//Bind parameters\n";
            this->createBindings();
            this->out << "\t\t\tif($stmt->execute())\n";
            this->out << "\t\t\t{\n";
            this->out << "\t\t\t\t$this->ID = $this->getLastInsertedId();\n";
            this->out << "\t\t\t\treturn true;\n";
            this->out << "\t\t\t}\n";
            this->out << "\t\t\treturn false;\n";
            this->out << "\t\t}";
            this->out << "\n";
        }

        /**
        * Function that writes an update function to 
        * update a record in the class's table
        * Writes comments and function to update the
        * a record at a certain ID into the class's
        * table using the properties set in the class. 
        * @return void
        */
        void createUpdate()
        {
            std::string Description = "Updates a record in sources " 
                                    + this->stringToLower(this->className) 
                                    + " table using currently  set properties in " 
                                    + this->stringToLower(this->className) 
                                    + " object.";
            std::vector<std::pair<std::string, std::string>> Params = {
                std::make_pair("int",  "ID Integer for ID to update")};
            std::string DetailedDescription = "Updates a record in the " 
                                            + this->stringToLower(this->className) 
                                            + " table using currently set properties in " 
                                            + this->stringToLower(this->className) 
                                            + " object for a given id. If successful returns true.";
            std::string returnType = "bool";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function update($ID): bool \n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$this->ID = $ID;\n";
            this->out << "\t\t\t$properties = get_object_vars($this);\n";
            this->out << "\t\t\t$keys = array_keys($properties);\n";
            this->out << "\t\t\t$propertiesToExclude = [\"ID\", \"conn\", \"table_name\"];\n";
            this->out << "\t\t\t$first = true;\n";
            this->out << "\t\t\t$query = \"UPDATE \" . $this->table_name . \"\n";
            this->out << "\t\t\t\tSET\";\n";
            this->out << "\t\t\tforeach($keys as $k)\n";
            this->out << "\t\t\t{\n";
            this->out << "\t\t\t\tif(in_array($k, $propertiesToExclude) == false)\n";
            this->out << "\t\t\t\t{\n";
            this->out << "\t\t\t\t\tif($first == true)\n";
            this->out << "\t\t\t\t\t{\n";
            this->out << "\t\t\t\t\t\t$query .= \" \" . $k . \"='\" . $properties[$k] . \"'\";\n";
            this->out << "\t\t\t\t\t\t$first=false;\n";
            this->out << "\t\t\t\t\t}\n";
            this->out << "\t\t\t\t\telse\n";
            this->out << "\t\t\t\t\t{\n";
            this->out << "\t\t\t\t\t\t$query .= \", \" . $k . \"='\" . $properties[$k] . \"'\";\n";
            this->out << "\t\t\t\t\t}\n";
            this->out << "\t\t\t\t}\n";
            this->out << "\t\t\t};\n";
            this->out << "\t\t\t$query .= \" WHERE ID = '\" . $ID . \"'\";\n";
            this->out << "\t\t\t//Prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\t\t\t//Execute Query\n";
            this->out << "\t\t\tif($stmt->execute())\n";
            this->out << "\t\t\t{\n"; 
            this->out << "\t\t\t\treturn true;\n";
            this->out << "\t\t\t}\n"; 
            this->out << "\t\t\telse\n"; 
            this->out << "\t\t\t{\n"; 
            this->out << "\t\t\t\treturn false;\n";
            this->out << "\t\t\t}\n"; 
            this->out << "\t\t}\n";
        }

        /**
        * Function that writes a remove function to 
        * remove a record in the class's table.
        * Writes comments and function to remove the
        * a record at a certain ID in the class's table.
        * @return void
        */
        void createRemove()
        {
            std::string Description = "Removes a record in " + this->stringToLower(this->className) 
                                    + " by ID";
            std::vector<std::pair<std::string, std::string>> Params = {};
            std::string DetailedDescription = "Prepares and executes a query to delete a record in the " 
                                            + this->stringToLower(this->className) 
                                            + "table. If successful, returns true.";
            std::string returnType = "bool";
            this->createMethodComment(Description, DetailedDescription, Params, returnType);
            this->out << "\t\tpublic function remove(): bool \n";
            this->out << "\t\t{\n";
            this->out << "\t\t\t$query = \"DELETE * FROM\n";
            this->out << "\t\t\t\t\" . $this->table_name . \"\n";
            this->out << "\t\t\t\tWHERE Id = ?\";\n";
            this->out << "\t\t\t//Prepare query\n";
            this->out << "\t\t\t$stmt = $this->conn->prepare($query);\n";
            this->out << "\t\t\t$this->id=htmlspecialchars(strip_tags($this->id));\n";
            this->out << "\n";
            this->out << "\t\t\t//bind id of reocord to delete\n";
            this->out << "\t\t\t$stmt->bindParam(1, $this->id);\n";
            this->out << "\t\t\t//Execute Query\n";
            this->out << "\t\t\tif($stmt->execute())\n";
            this->out << "\t\t\t{\n"; 
            this->out << "\t\t\t\treturn true;\n";
            this->out << "\t\t\t}\n"; 
            this->out << "\t\t\telse\n"; 
            this->out << "\t\t\t{\n"; 
            this->out << "\t\t\t\treturn false;\n";
            this->out << "\t\t\t}\n"; 
            this->out << "\t\t}\n";
        }

        /**
         * Function that converts a string to lower case
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
        * Function that writes the closing php bracket
        * Writes the closing class bracket and php bracket.
        * @return void
        */
        void closing()
        {
            this->out << "\t}\n";
            this->out << "?>";
        }

        /**
        * Function that creates a PHP class
        * with the built in CRUD methods
        * Function creates a php class file
        * with the class name, properties, constructor,
        * read, readOne, readPaging, count, 
        * checkIfExists, getIfExists, insert,
        * update, and remove functions. 
        * @return void
        */
        void createClassFile()
        {
            std::cout << "Creating opening.\n"; 
            this->opening();
            std::cout << "Creating Constructor.\n"; 
            this->createConstructor();
            std::cout << "Creating Read Method.\n"; 
            this->createRead();
            std::cout << "Creating ReadOne Method.\n"; 
            this->createReadOne();
            std::cout << "Creating ReadPaging Method.\n"; 
            this->createReadPaging();
            std::cout << "Creating Count Method.\n"; 
            this->createCount();
            std::cout << "Creating CheckIfExists Method.\n"; 
            this->createCheckIfExists();
            std::cout << "Creating Insert Method.\n"; 
            this->createInsert();
            std::cout << "Creating Update Method.\n"; 
            this->createUpdate();
            std::cout << "Creating Remove Method.\n"; 
            this->createRemove();
            std::cout << "Creating Closing Method.\n"; 
            this->closing();
            this->out.close();
        }

        /**
        * Function that creates the class name and properties
        * portion of a class.
        * Writes the opening php brackets, class name and loops
        * through the fields vector to generate properties in 
        * the class. 
        * @return void
        */
        void opening()
        {
            this->out << "<\?php \n";
            this->createClassComment();
            this->out << "\tclass " << this->className << "\n";
            this->out << "\t{\n";
            for(std::string f : this->Fields)
            {
                this->out << "\t\tpublic $" << f << ";\n";
            }
            this->out << "\t\tprivate $conn;\n";
            this->out << "\t\tprivate $table_name = \"" << this->tableName << "\";\n";
            this->out << "\n";
        }
};